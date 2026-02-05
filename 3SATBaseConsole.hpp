// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_H__
#define __3SATBASECONSOLE_H__

#define FALSE_3SAT -1
#define TRUE_3SAT 1

typedef struct SATSolver_tag {

	__int64 n;
	__int64 k;

	__int64 chops;

	bool* Z;				// current state of advancement through search space

	__int64* inopcell_l;	// instance operation cell left
	__int64* inopcell_m;	// instance operation cell middle
	__int64* inopcell_r;	// instance operation cell right

	__int64** cdopcelll_f;	// encoding operation cell left, false
	__int64** cdopcellr_f;	// encoding operation cell right, false

	__int64* cdol_vtop_f;	// vector top, left, false
	__int64* cdol_vcap_f;	// vector capacity, left, false

	__int64* cdor_vtop_f;	// vector top, right, false
	__int64* cdor_vcap_f;	// vector capacity, right, false

	__int64** cdopcelll_t;	// encoding operation cell right, true
	__int64** cdopcellr_t;	// encoding operation cell right, true

	__int64*  cdol_vtop_t;	// vector top, left, true
	__int64*  cdol_vcap_t;	// vector capacity, left, true

	__int64* cdor_vtop_t;	// vector top, right, true
	__int64* cdor_vcap_t;	// vector capacity, right, true

	__int64* cd_sizes_f;	// sizes of encodings, false
	__int64* cd_sizes_t;	// sizes of encodings, true

} SATSolver;

__int64* simp_vector_create(__int64 init_sz);
__int64 simp_vector_read(__int64* v, __int64 vtop, __int64 vcap, __int64 loc);
void simp_vector_append(__int64** v, __int64* vtop, __int64* vcap, __int64 data);
bool* SATSolver_create_boundary(bool begin, __int64 chop, __int64 offs, __int64 n);
void SATSolver_create(SATSolver* s, __int64** lst, __int64 k, __int64 n, __int64 chops, __int64 chop);
void SATSolver_destroy(SATSolver* s);
bool two_sat(__int64* lst_l_parm, __int64* lst_r_parm, __int64 k_parm, __int64 n_parm, bool* is_f, bool* is_t);
bool SATSolver_isSat(SATSolver* s, bool* sln);

#endif