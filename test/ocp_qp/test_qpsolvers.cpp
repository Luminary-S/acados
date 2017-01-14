#include <iostream>
#include <string>
#include <vector>
#include "catch/include/catch.hpp"
#include "test/test_utils/read_matrix.h"
#include "acados/ocp_qp/ocp_qp_condensing_qpoases.h"
#include "test/ocp_qp/condensing_test_helper.h"
#include "acados/ocp_qp/ocp_qp_ooqp.h"
#include "OOQP/include/cQpGenSparse.h"

// TODO(dimitris): Add CPP code in blasfeo header for this to work (intead of zeros.h)
// #include "blasfeo/include/blasfeo_d_aux.h"
#include "test/test_utils/zeros.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::Map;

extern real_t COMPARISON_TOLERANCE;

// TODO(dimitris): remove variables below once finished with implementation
int_t MYMAKEFILE = 0;
std::string name_scenario;
int_t TEST_OOQP = 0;

// std::vector<std::string> scenarios = {"LTI", "LTV"};
// std::vector<std::string> constraints = {"UNCONSTRAINED", "ONLY_AFFINE",
// "ONLY_BOUNDS", "CONSTRAINED"};

// TODO(dimitris): Check all cases (above) after fixing everything
std::vector<std::string> scenarios = {"LTI"};
std::vector<std::string> constraints = {"UNCONSTRAINED"};

void readInputDimensionsFromFile(int_t *N, int_t *nx, int_t *nu, std::string folder) {
    *N = (int_t) readMatrix(folder + "/N.dat")(0, 0);
    REQUIRE(*N > 0);
    *nx = (int_t) readMatrix(folder + "/nx.dat")(0, 0);
    REQUIRE(*nx > 0);
    *nu = (int_t) readMatrix(folder + "/nu.dat")(0, 0);
    REQUIRE(*nu > 0);
}

void concatenateSolution(int_t N, int_t nx, int_t nu, const ocp_qp_out *out, VectorXd *acados_W) {
    int ii, jj;
    double *concatenated_W;

    d_zeros(&concatenated_W, (N+1)*nx+N*nu, 1);

    for (ii = 0; ii < N; ii++) {
        for (jj = 0; jj < nx; jj++) concatenated_W[ii*(nx+nu)+jj] = out->x[ii][jj];
        for (int jj = 0; jj < nu; jj++) concatenated_W[ii*(nx+nu)+nx+jj] = out->u[ii][jj];
    }
    for (jj = 0; jj < nx; jj++) concatenated_W[N*(nx+nu)+jj] = *(out->x[N]+jj);
    *acados_W = Eigen::Map<VectorXd>(concatenated_W, (N+1)*nx + N*nu);
}

// TODO(dimitris): Fix problems with bounded and constrained cases (and implement them)
// TODO(dimitris): Clean up octave code with Robin
TEST_CASE("Solve random OCP_QP", "[QP solvers]") {
    ocp_qp_in qp_in;
    ocp_qp_out qp_out;

    VectorXd x0;

    for (std::string constraint : constraints) {
        SECTION(constraint) {
            for (std::string scenario : scenarios) {
                name_scenario = scenario;
                if (MYMAKEFILE == 1) scenario = "../build/test/" + scenario;

                SECTION(name_scenario) {
                    // fill-in qp_in struct with data
                    fillWithUnconstrainedData(&qp_in, &x0, scenario);
                    int_t N  = qp_in.N;
                    int_t nx = qp_in.nx[0];
                    int_t nu = qp_in.nu[0];

                    // TODO(dimitris): fix bug in fillBounds
                    if (constraint == "ONLY_BOUNDS" || constraint == "CONSTRAINED") {
                        fillWithBoundsData(&qp_in, N, nx, nu, scenario);
                    }
                    if (constraint == "ONLY_AFFINE" || constraint == "CONSTRAINED") {
                        fillWithGeneralConstraintsData(&qp_in, N, nx, nu, scenario);
                    }

                    // load optimal solution from quadprog
                    MatrixXd true_W;
                    if (constraint == "UNCONSTRAINED") {
                        true_W = readMatrixFromFile(scenario +
                            "/w_star_ocp_unconstrained.dat", (N+1)*nx + N*nu, 1);
                    } else if (constraint == "ONLY_BOUNDS") {
                        true_W = readMatrixFromFile(scenario +
                            "/w_star_ocp_bounds.dat", (N+1)*nx + N*nu, 1);
                    } else if (constraint == "ONLY_AFFINE") {
                        true_W = readMatrixFromFile(scenario +
                            "/w_star_ocp_no_bounds.dat", (N+1)*nx + N*nu, 1);
                    } else if (constraint == "CONSTRAINED") {
                        true_W = readMatrixFromFile(scenario +
                            "/w_star_ocp_constrained.dat", (N+1)*nx + N*nu, 1);
                    }

                    // initialize qp_out struct
                    double *hx[N + 1];
                    double *hu[N + 1];

                    for (int ii = 0; ii < N; ii++) {
                        d_zeros(&hx[ii], nx, 1);
                        d_zeros(&hu[ii], nu, 1);
                    }
                    d_zeros(&hx[N], nx, 1);

                    qp_out.x = hx;
                    qp_out.u = hu;

                    int return_value;
                    VectorXd acados_W;

                    SECTION("qpOASES") {
                        ocp_qp_condensing_qpoases_args args;
                        args.dummy = 42.0;

                        initialise_qpoases(&qp_in);

                        // TODO(dimitris): Ask why NULL in example (don't we warm-start?)
                        return_value = ocp_qp_condensing_qpoases(&qp_in, &qp_out, &args, NULL);

                        concatenateSolution(N, nx, nu, &qp_out, &acados_W);

                        // printf("\nu = \n");
                        // for (int ii = 0; ii < N; ii++) d_print_mat(1, qp_in.nu[ii], hu[ii], 1);

                        // std::cout << "ACADOS output:\n" << acados_W << std::endl;
                        // printf("-------------------\n");
                        // std::cout << "OCTAVE output:\n" << true_W << std::endl;
                        // printf("-------------------\n");
                        // printf("return value = %d\n", return_value);
                        // printf("-------------------\n");

                        REQUIRE(return_value == 0);
                        // TODO(dimitris): update qpOASES/quadprog settings to reach COMPARISON_TOL.
                        REQUIRE(acados_W.isApprox(true_W, 1e-10));
                    }
                    if (TEST_OOQP) {
                        SECTION("OOQP") {
                            ocp_qp_ooqp_args args;
                            args.dummy = 32.0;
                            ocp_qp_ooqp_workspace work;

                            int init_return_value = ocp_qp_ooqp_create_workspace(&qp_in, &work);
                            REQUIRE(init_return_value == 0);

                            return_value = ocp_qp_ooqp(&qp_in, &qp_out, &args, &work);
                            concatenateSolution(N, nx, nu, &qp_out, &acados_W);

                            ocp_qp_ooqp_free_workspace(&work);

                            REQUIRE(return_value == 0);
                            REQUIRE(acados_W.isApprox(true_W, 1e-10));
                        }
                    }
                }  // END_SECTION_SCENARIOS
            }  // END_FOR_SCENARIOS
        }  // END_SECTION_CONSTRAINTS
    }  // END_FOR_CONSTRAINTS
}  // END_TEST_CASE
