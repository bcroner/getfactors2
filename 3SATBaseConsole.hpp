// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_H__
#define __3SATBASECONSOLE_H__

#define FALSE_3SAT -1
#define TRUE_3SAT 1

typedef struct SATSolver_tag {

	__int64 n;
	__int64 k;

	__int64* inopcell_l;	// instance operation cell left
	__int64* inopcell_m;	// instance operation cell middle
	__int64* inopcell_r;	// instance operation cell right

	__int64*  inol_vtop;	// vector top
	__int64*  inol_vcap;	// vector capacity

	__int64* inom_vtop;		// vector top
	__int64* inom_vcap;		// vector capacity

	__int64*  inor_vtop;	// vector top
	__int64*  inor_vcap;	// vector capacity

	bool** op;				// operation- true for AND, false for OR

	__int64* op_vtop;		// vector top
	__int64* op_vcap;		// vector capacity

	__int64** cdopcelll;	// encoding operation cell left
	__int64** cdopcellr;	// encoding operation cell right

	__int64*  cdol_vtop;	// vector top
	__int64*  cdol_vcap;	// vector capacity

	__int64*  cdor_vtop;	// vector top
	__int64*  cdor_vcap;	// vector capacity

} SATSolver;

bool* simp_bool_vector_create(__int64 init_sz);
__int64* simp_vector_create(__int64 init_sz);
__int64 simp_vector_read(__int64* v, __int64 vtop, __int64 vcap, __int64 loc);
void simp_vector_append(__int64** v, __int64* vtop, __int64* vcap, __int64 data);
void SATSolver_create(SATSolver* s, __int64** lst, __int64 k, __int64 n);
void SATSolver_destroy(SATSolver* s);
bool SATSolver_isSat(SATSolver* s, bool* sln);

#endif