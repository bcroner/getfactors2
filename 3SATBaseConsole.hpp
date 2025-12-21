// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_H__
#define __3SATBASECONSOLE_H__

#define FALSE_3SAT -1
#define TRUE_3SAT 1

typedef struct SATSolver_tag {

	__int64* l;
	__int64* r;

} SATSolver;

void SATSolver_create(SATSolver * s, __int64 ** lst, __int64 k, __int64 n);

bool SATSolver_isSat(SATSolver *s, bool * sln);

__int64* simp_vector_create(__int64 init_sz);
__int64 simp_vector_read(__int64* v, __int64 vtop, __int64 vcap, __int64 loc);
void simp_vector_append(__int64** v, __int64* vtop, __int64* vcap, __int64 data);

#endif