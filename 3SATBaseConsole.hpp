// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_H__
#define __3SATBASECONSOLE_H__

#define FALSE_3SAT -1
#define TRUE_3SAT 1

typedef struct SATSolver_tag {

	__int64* in_nand_l;	// instance NAND left
	__int64* in_nand_r;	// instance NAND right

	__int64  innl_vtop;	// vector top
	__int64  innl_vcap;	// vector capacity

	__int64  innr_vtop;	// vector top
	__int64  innr_vcap;	// vector capacity

	__int64* cd_nand_l;	// encoding NAND left
	__int64* cd_nand_r;	// encoding NAND right

	__int64  cdnl_vtop;	// vector top
	__int64  cdnl_vcap;	// vector capacity

	__int64  cdnr_vtop;	// vector top
	__int64  cdnr_vcap;	// vector capacity

	__int64* varref;	// variable reference into encoding NAND

} SATSolver;

__int64* simp_vector_create(__int64 init_sz);
__int64 simp_vector_read(__int64* v, __int64 vtop, __int64 vcap, __int64 loc);
void simp_vector_append(__int64** v, __int64* vtop, __int64* vcap, __int64 data);
void SATSolver_create(SATSolver* s, __int64** lst, __int64 k, __int64 n);
void SATSolver_destroy(SATSolver* s);
bool SATSolver_isSat(SATSolver* s, bool* sln);

#endif