// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_C__
#define __3SATBASECONSOLE_C__

#include "3SATBaseConsole.hpp"

#include <stdio.h>

bool* simp_bool_vector_create(__int64 init_sz) {

    bool* ret = new bool[init_sz];
    return ret;

}

bool simp_bool_vector_read(bool* v, __int64 vtop, __int64 vcap, __int64 loc) {

    if (loc > vtop)
        return 0;

    return v[loc];
}

void simp_bool_vector_append(bool** v, __int64* vtop, __int64* vcap, bool data) {

    *vtop = *vtop + 1;

    if (*vtop < *vcap)
        *v[*vtop] = data;
    else {
        bool* newv = new bool[*vcap * 2];
        for (__int64 i = 0; i < *vcap * 2; i++)
            newv[i] = 0;
        for (__int64 i = 0; i < *vcap; i++)
            newv[i] = *v[i];
        *vcap *= 2;
        delete[] * v;
        *v = newv;
        *v[*vtop] = data;
    }

}


__int64* simp_vector_create(__int64 init_sz) {

    __int64* ret = new __int64[init_sz];
    return ret;

}

__int64 simp_vector_read(__int64* v, __int64 vtop, __int64 vcap, __int64 loc) {

    if (loc > vtop)
        return 0;

    return v[loc];
}

void simp_vector_append(__int64** v, __int64* vtop, __int64* vcap, __int64 data) {

    *vtop = *vtop + 1;

    if (*vtop < *vcap)
        *v[*vtop] = data;
    else {
        __int64* newv = new __int64[*vcap * 2];
        for (__int64 i = 0; i < *vcap * 2; i++)
            newv[i] = 0;
        for (__int64 i = 0; i < *vcap; i++)
            newv[i] = *v[i];
        *vcap *= 2;
        delete[] * v;
        *v = newv;
        *v[*vtop] = data;
    }

}

void SATSolver_create(SATSolver* s, __int64** lst, __int64 k, __int64 n) {

    s->k = k;
    s->n = n;

    s->inopcell_l = new __int64 [k];
    s->inopcell_m = new __int64 [k];
    s->inopcell_r = new __int64 [k];

    // copy instance into SATSolver vectors

    for (__int64 i = 0; i < k; i++) {

        s->inopcell_l[i] = lst[i][0];
        s->inopcell_m[i] = lst[i][1];
        s->inopcell_r[i] = lst[i][2];
    }

    s->cdopcelll_f = simp_vector_create(16);
    s->cdopcellr_f = simp_vector_create(16);

    s->cdopcelll_t = simp_vector_create(16);
    s->cdopcellr_t = simp_vector_create(16);

    s->cdol_vtop_f = 0;
    s->cdol_vcap_f = 0;

    s->cdol_vtop_t = 0;
    s->cdol_vcap_t = 0;

    s->cdor_vtop_f = 0;
    s->cdor_vcap_f = 0;

    s->cdor_vtop_t = 0;
    s->cdor_vcap_t = 0;

    s->varref_f = new __int64[n];
    s->varref_t = new __int64[n];

    // place instance variables into encoding

    for (__int64 i = 0; i < s->n; i++) {
        for (__int64 j = 0; j < s->k; i++) {

            __int64 abs_l = s->inopcell_l[j] < 0 ? -s->inopcell_l[j] : s->inopcell_l[j];
            __int64 abs_m = s->inopcell_m[j] < 0 ? -s->inopcell_m[j] : s->inopcell_m[j];
            __int64 abs_r = s->inopcell_r[j] < 0 ? -s->inopcell_r[j] : s->inopcell_r[j];

            if (abs_l - 2 == i && s->inopcell_l[j] < 0) {

                simp_vector_append(&(s->cdopcelll_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_m[j]);
                simp_vector_append(&(s->cdopcellr_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_r[j]);

            }
            else if (abs_l - 2 == i) {

                simp_vector_append(&(s->cdopcelll_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_m[j]);
                simp_vector_append(&(s->cdopcellr_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_r[j]);

            }
            else if (abs_m - 2 == i && s->inopcell_m[j] < 0) {

                simp_vector_append(&(s->cdopcelll_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_l[j]);
                simp_vector_append(&(s->cdopcellr_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_r[j]);

            }
            else if (abs_m - 2 == i) {

                simp_vector_append(&(s->cdopcelll_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_l[j]);
                simp_vector_append(&(s->cdopcellr_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_r[j]);

            }
            else if (abs_r - 2 == i && s->inopcell_r[j] < 0) {

                simp_vector_append(&(s->cdopcelll_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_l[j]);
                simp_vector_append(&(s->cdopcellr_t), &(s->cdol_vtop_t), &(s->cdol_vcap_t), s->inopcell_m[j]);

            }
            else if (abs_r - 2 == i) {

                simp_vector_append(&(s->cdopcelll_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_l[j]);
                simp_vector_append(&(s->cdopcellr_f), &(s->cdol_vtop_f), &(s->cdol_vcap_f), s->inopcell_m[j]);

            }

        }
    }

}

void SATSolver_destroy(SATSolver* s) {

    delete[] s->cdopcelll_f;
    delete[] s->cdopcellr_f;

    delete[] s->cdopcelll_t;
    delete[] s->cdopcellr_t;

    delete[] s->inopcell_l;
    delete[] s->inopcell_m;
    delete[] s->inopcell_r;

    delete[] s->cdopcelll_f;
    delete[] s->cdopcellr_f;

    delete[] s->cdopcelll_t;
    delete[] s->cdopcellr_t;

    delete[] s->inopcell_l;
    delete[] s->inopcell_r;

    delete[] s->cdopcelll_f;
    delete[] s->cdopcellr_f;

    delete[] s->cdopcelll_t;
    delete[] s->cdopcellr_t;

    delete[] s->varref_f;
    delete[] s->varref_t;

}

bool SATSolver_isSat(SATSolver* s, bool* sln) {

    return true;
}

#endif