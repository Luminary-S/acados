/* This file was automatically generated by CasADi.
   The CasADi copyright holders make no ownership claim of its contents. */
#ifdef __cplusplus
extern "C" {
#endif

/* How to prefix internal symbols */
#ifdef CODEGEN_PREFIX
  #define NAMESPACE_CONCAT(NS, ID) _NAMESPACE_CONCAT(NS, ID)
  #define _NAMESPACE_CONCAT(NS, ID) NS ## ID
  #define CASADI_PREFIX(ID) NAMESPACE_CONCAT(CODEGEN_PREFIX, ID)
#else
  #define CASADI_PREFIX(ID) pendulum_cost_ext_cost_e_fun_jac_hess_ ## ID
#endif

#include <math.h>

#ifndef casadi_real
#define casadi_real double
#endif

#ifndef casadi_int
#define casadi_int int
#endif

/* Add prefix to internal symbols */
#define casadi_f0 CASADI_PREFIX(f0)
#define casadi_s0 CASADI_PREFIX(s0)
#define casadi_s1 CASADI_PREFIX(s1)
#define casadi_s2 CASADI_PREFIX(s2)
#define casadi_s3 CASADI_PREFIX(s3)

/* Symbol visibility in DLLs */
#ifndef CASADI_SYMBOL_EXPORT
  #if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
    #if defined(STATIC_LINKED)
      #define CASADI_SYMBOL_EXPORT
    #else
      #define CASADI_SYMBOL_EXPORT __declspec(dllexport)
    #endif
  #elif defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
    #define CASADI_SYMBOL_EXPORT __attribute__ ((visibility ("default")))
  #else
    #define CASADI_SYMBOL_EXPORT
  #endif
#endif

static const casadi_int casadi_s0[8] = {4, 1, 0, 4, 0, 1, 2, 3};
static const casadi_int casadi_s1[3] = {0, 0, 0};
static const casadi_int casadi_s2[5] = {1, 1, 0, 1, 0};
static const casadi_int casadi_s3[11] = {4, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3};

/* pendulum_cost_ext_cost_e_fun_jac_hess:(i0[4],i1[])->(o0,o1[4],o2[4x4,4nz]) */
static int casadi_f0(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, void* mem) {
  casadi_real a0, a1, a10, a11, a2, a3, a4, a5, a6, a7, a8, a9;
  a0=1000.;
  a1=arg[0] ? arg[0][0] : 0;
  a2=(a0*a1);
  a3=(a2*a1);
  a4=arg[0] ? arg[0][1] : 0;
  a5=(a0*a4);
  a6=(a5*a4);
  a3=(a3+a6);
  a6=1.0000000000000000e-02;
  a7=arg[0] ? arg[0][2] : 0;
  a8=(a6*a7);
  a9=(a8*a7);
  a3=(a3+a9);
  a9=arg[0] ? arg[0][3] : 0;
  a10=(a6*a9);
  a11=(a10*a9);
  a3=(a3+a11);
  if (res[0]!=0) res[0][0]=a3;
  a1=(a0*a1);
  a2=(a2+a1);
  if (res[1]!=0) res[1][0]=a2;
  a0=(a0*a4);
  a5=(a5+a0);
  if (res[1]!=0) res[1][1]=a5;
  a7=(a6*a7);
  a8=(a8+a7);
  if (res[1]!=0) res[1][2]=a8;
  a6=(a6*a9);
  a10=(a10+a6);
  if (res[1]!=0) res[1][3]=a10;
  a10=2000.;
  if (res[2]!=0) res[2][0]=a10;
  if (res[2]!=0) res[2][1]=a10;
  a10=2.0000000000000000e-02;
  if (res[2]!=0) res[2][2]=a10;
  if (res[2]!=0) res[2][3]=a10;
  return 0;
}

CASADI_SYMBOL_EXPORT int pendulum_cost_ext_cost_e_fun_jac_hess(const casadi_real** arg, casadi_real** res, casadi_int* iw, casadi_real* w, void* mem){
  return casadi_f0(arg, res, iw, w, mem);
}

CASADI_SYMBOL_EXPORT void pendulum_cost_ext_cost_e_fun_jac_hess_incref(void) {
}

CASADI_SYMBOL_EXPORT void pendulum_cost_ext_cost_e_fun_jac_hess_decref(void) {
}

CASADI_SYMBOL_EXPORT casadi_int pendulum_cost_ext_cost_e_fun_jac_hess_n_in(void) { return 2;}

CASADI_SYMBOL_EXPORT casadi_int pendulum_cost_ext_cost_e_fun_jac_hess_n_out(void) { return 3;}

CASADI_SYMBOL_EXPORT const char* pendulum_cost_ext_cost_e_fun_jac_hess_name_in(casadi_int i){
  switch (i) {
    case 0: return "i0";
    case 1: return "i1";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const char* pendulum_cost_ext_cost_e_fun_jac_hess_name_out(casadi_int i){
  switch (i) {
    case 0: return "o0";
    case 1: return "o1";
    case 2: return "o2";
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* pendulum_cost_ext_cost_e_fun_jac_hess_sparsity_in(casadi_int i) {
  switch (i) {
    case 0: return casadi_s0;
    case 1: return casadi_s1;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT const casadi_int* pendulum_cost_ext_cost_e_fun_jac_hess_sparsity_out(casadi_int i) {
  switch (i) {
    case 0: return casadi_s2;
    case 1: return casadi_s0;
    case 2: return casadi_s3;
    default: return 0;
  }
}

CASADI_SYMBOL_EXPORT int pendulum_cost_ext_cost_e_fun_jac_hess_work(casadi_int *sz_arg, casadi_int* sz_res, casadi_int *sz_iw, casadi_int *sz_w) {
  if (sz_arg) *sz_arg = 2;
  if (sz_res) *sz_res = 3;
  if (sz_iw) *sz_iw = 0;
  if (sz_w) *sz_w = 0;
  return 0;
}


#ifdef __cplusplus
} /* extern "C" */
#endif