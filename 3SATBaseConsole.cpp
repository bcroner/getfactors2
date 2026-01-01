// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_C__
#define __3SATBASECONSOLE_C__

#include "3SATBaseConsole.hpp"

#include <stdio.h>

#define LVAL 0
#define MVAL 1
#define RVAL 2

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

bool* SATSolver_create_boundary(bool begin, __int64 chop, __int64 offs, __int64 n) {

    bool* ret = new bool[n];

    for (__int64 i = 0; i < chop; i++) {

        __int64 pow2 = 1;
        for (__int64 j = chop - 1 - i; j > 0; j--)
            pow2 *= 2;

        if (offs >= pow2) {
            ret[n - 1 - i] = true;
            offs -= pow2;
        }
        else
            ret[n - 1 - i] = false;
    }

    for (__int64 i = chop; i < n; i++)
        ret[n - 1 - i] = begin ? false : true;

    return ret;

}

void SATSolver_create(SATSolver* s, __int64** lst, __int64 k, __int64 n, __int64 chops, __int64 chop) {

    s->k = k;
    s->n = n;

    s->Z = SATSolver_create_boundary(true, chops, chop, n);
    s->end = SATSolver_create_boundary(false, chops, chop, n);

    s->inopcell_l = new __int64 [k];
    s->inopcell_m = new __int64 [k];
    s->inopcell_r = new __int64 [k];

    // copy instance into SATSolver vectors

    for (__int64 i = 0; i < k; i++) {

        s->inopcell_l[i] = lst[i][0];
        s->inopcell_m[i] = lst[i][1];
        s->inopcell_r[i] = lst[i][2];
    }

    s->cdopcelll_f = new __int64* [n];
    s->cdopcellr_f = new __int64* [n];

    s->cdol_vtop_f = new __int64[n];
    s->cdol_vcap_f = new __int64[n];

    s->cdor_vtop_f = new __int64[n];
    s->cdor_vcap_f = new __int64[n];

    s->cdopcelll_t = new __int64* [n];
    s->cdopcellr_t = new __int64* [n];

    s->cdol_vtop_t = new __int64[n];
    s->cdol_vcap_t = new __int64[n];

    s->cdor_vtop_t = new __int64[n];
    s->cdor_vcap_t = new __int64[n];

    s->cd_sizes_f = new __int64[n];
    s->cd_sizes_t = new __int64[n];



    for (__int64 i = 0; i < n; i++) {

        s->cdopcelll_f[i] = simp_vector_create(16);
        s->cdopcellr_f[i] = simp_vector_create(16);

        s->cdol_vtop_f[i] = -1;
        s->cdol_vcap_f[i] = 16;

        s->cdor_vtop_f[i] = -1;
        s->cdor_vcap_f[i] = 16;

        s->cdopcelll_t[i] = simp_vector_create(16);
        s->cdopcellr_t[i] = simp_vector_create(16);

        s->cdol_vtop_t[i] = -1;
        s->cdol_vcap_t[i] = 16;

        s->cdor_vtop_t[i] = -1;
        s->cdor_vcap_t[i] = 16;
    }

    // place instance variables into encoding

    for (__int64 i = 0; i < n; i++) {

        s->cd_sizes_f[i] = 0;
        s->cd_sizes_t[i] = 0;

        for (__int64 j = 0; j < s->k; i++) {

            __int64 abs_l = s->inopcell_l[j] < 0 ? -s->inopcell_l[j] : s->inopcell_l[j];
            __int64 abs_m = s->inopcell_m[j] < 0 ? -s->inopcell_m[j] : s->inopcell_m[j];
            __int64 abs_r = s->inopcell_r[j] < 0 ? -s->inopcell_r[j] : s->inopcell_r[j];

            __int64 loc = -1;
            __int64 val = 0;

            if (abs_l - 2 == i) {
                loc = LVAL;
                val = s->inopcell_l[j];
            }
            else if (abs_m - 2 == i) {
                loc = MVAL;
                val = s->inopcell_m[j];
            }
            else if (abs_r - 2 == i) {
                loc = RVAL;
                val = s->inopcell_r[j];
            }
            else
                continue;

            if (val < 0) {
                __int64 left_val = loc == LVAL ? s->inopcell_m[j] : loc == MVAL ? s->inopcell_l[j] : s->inopcell_l[j];
                __int64 right_val = loc == LVAL ? s->inopcell_r[j] : loc == MVAL ? s->inopcell_r[j] : s->inopcell_m[j];
                simp_vector_append(&(s->cdopcelll_t[i]), &(s->cdol_vtop_t[i]), &(s->cdol_vcap_t[i]), left_val);
                simp_vector_append(&(s->cdopcellr_t[i]), &(s->cdol_vtop_t[i]), &(s->cdol_vcap_t[i]), right_val);
                s->cd_sizes_t[i]++;
            }
            else {
                __int64 left_val = loc == LVAL ? s->inopcell_m[j] : loc == MVAL ? s->inopcell_l[j] : s->inopcell_l[j];
                __int64 right_val = loc == LVAL ? s->inopcell_r[j] : loc == MVAL ? s->inopcell_r[j] : s->inopcell_m[j];
                simp_vector_append(&(s->cdopcelll_f[i]), &(s->cdol_vtop_f[i]), &(s->cdol_vcap_f[i]), left_val);
                simp_vector_append(&(s->cdopcellr_f[i]), &(s->cdol_vtop_f[i]), &(s->cdol_vcap_f[i]), right_val);
                s->cd_sizes_f[i]++;
            }

        }

    }

}

void SATSolver_destroy(SATSolver* s) {

    delete[] s->Z;

    for (__int64 i = 0; i < n; i++) {

        delete[] s->cdopcelll_f[i];
        delete[] s->cdopcellr_f[i];

        delete[] s->cdopcelll_t[i];
        delete[] s->cdopcellr_t[i];

    }

    delete[] s->cdopcelll_t;
    delete[] s->cdopcellr_t;

    delete[] s->cdopcelll_t;
    delete[] s->cdopcellr_t;

    delete[] s->cdopcelll_f;
    delete[] s->cdopcellr_f;

    delete[] s->cdol_vtop_f;
    delete[] s->cdol_vcap_f;

    delete[] s->cdor_vtop_f;
    delete[] s->cdor_vcap_f;

    delete[] s->cdol_vtop_t;
    delete[] s->cdol_vcap_t;

    delete[] s->cdor_vtop_t;
    delete[] s->cdor_vcap_t;

    delete[] s->cd_sizes_f;
    delete[] s->cd_sizes_t;
}

bool bool_equals(bool* A, bool* B, __int64 n) {

    for (__int64 i = 0; i < n; i++)
        if (A[i] != B[i])
            return false;

    return true;
}

bool two_sat(__int64* lst_l_parm, __int64* lst_r_parm, __int64 k_parm, __int64 n_parm, bool* is_f_parm, bool* is_t_parm) {

    __int64 counter = 0;

    __int64* encoding = new __int64[n];

    for (__int64 i = 0; i < n_parm; i++) {
        if (is_f_parm[i]) {
            encoding[counter] = i;
            counter++;
        }
        if (is_t_parm[i]) {
            encoding[counter] = i;
            counter++;
        }
    }

    bool* used = new bool[n_parm];

    for (__int64 i = 0; i < n_parm; i++)
        used[i] = false;

    for (__int64 i = 0; i < k_parm; i++) {

        __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
        __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

        used[l_abs] = true;
        used[r_abs] = true;
    }

    __int64 n = 0;

    for (__int64 i = 0; i < n_parm; i++)
        if (used[i])
            n++;

    for (__int64 i = 0; i < n; i++) {
        if (used[i]) {
            encoding[counter] = i;
            counter++;
        }
    }

    __int64* is_f = new __int64[n];
    __int64* is_t = new __int64[n];

    for (__int64 i = 0; i < n; i++) {
        is_f[i] = false;
        is_t[i] = false;
    }

    for (__int64 i = 0; i < n; i++) {
        if (is_f_parm[encoding[i]])
            is_f[i] = true;
        if (is_t_parm[encoding[i]])
            is_t[i] = true;
    }

    __int64 k = 0;

    for (__int64 i = 0; i < k_parm; i++) {
        if (lst_l_parm[i] != FALSE_3SAT && lst_r_parm[i] != FALSE_3SAT) {
            k++;
            continue;
        }

        __int64 val = lst_l_parm[i] == FALSE_3SAT ? lst_r_parm[i] : lst_l_parm[i];
        __int64 val_abs = val < 0 ? -val : val;

        if (val < 0)
            is_f[encoding[val_abs]] = true;
        else
            is_t[encoding[val_abs]] = true;
    }

    __int64* lst_l = new __int64[k];
    __int64* lst_r = new __int64[k];

    __int64 counter_k = 0;

    for (__int64 i = 0; i < k_parm; i++) {
        if (lst_l_parm[i] != FALSE_3SAT && lst_r_parm[i] != FALSE_3SAT) {

            __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
            __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

            lst_l[counter_k] = lst_l_parm[i] < 0 ? -encoding[l_abs] : encoding[l_abs];
            lst_r[counter_k] = lst_r_parm[i] < 0 ? -encoding[r_abs] : encoding[r_abs];

            counter_k++;
        }
    }

}

bool SATSolver_isSat(SATSolver* s, __int64 chops, bool* sln) {

    bool* is_f = new bool[s->n];
    bool* is_t = new bool[s->n];

    for (__int64 i = 0; i < s->n; i++) {
        is_f[i] = false;
        is_t[i] = false;
    }

    for (__int64 i = 0; i < s->k; i++) {
        __int64 count_f = 0;
        if (s->inopcell_l[i] == FALSE_3SAT)
            count_f++;
        if (s->inopcell_m[i] == FALSE_3SAT)
            count_f++;
        if (s->inopcell_r[i] == FALSE_3SAT)
            count_f++;

        if (count_f < 2)
            continue;

        __int64 val = 0;

        if (s->inopcell_l[i] != FALSE_3SAT)
            val = s->inopcell_l[i];
        else if (s->inopcell_m[i] != FALSE_3SAT)
            val = s->inopcell_m[i];
        else
            val = s->inopcell_r[i];

        __int64 val_abs = val < 0 ? -val : val;

        if (val < 0)
            is_f[val_abs] = true;
        else
            is_t[val_abs] = true;

    }

    __int64 ix = s->n - 1 - chops;

    while (!bool_equals(s->Z, s->end, s->n)) {

        __int64 size_2sat = 0;

        for (__int64 i = s->n - 1; i >= ix; i--)
            if (s->Z[i])
                size_2sat += s->cd_sizes_f[i];
            else
                size_2sat += s->cd_sizes_t[i];

        __int64* cd_2sat_l = new __int64[size_2sat];
        __int64* cd_2sat_r = new __int64[size_2sat];
        __int64 cd_2sat_cur_sz_f = 0;
        __int64 cd_2sat_cur_sz_t = 0;

        for (__int64 i = s->n - 1; i >= ix; i--) {
            if (s->Z[i]) {
                for (__int64 j = 0; j < s->cd_sizes_f[i]; j++) {
                    cd_2sat_l[cd_2sat_cur_sz_f + j] = s->cdopcelll_f[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_f + j] = s->cdopcellr_f[i][j];
                }
                cd_2sat_cur_sz_f += s->cd_sizes_f[i];
            }
            else {
                for (__int64 j = 0; j < s->cd_sizes_t[i]; j++) {
                    cd_2sat_l[cd_2sat_cur_sz_t + j] = s->cdopcelll_t[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_t + j] = s->cdopcellr_t[i][j];
                }
                cd_2sat_cur_sz_t += s->cd_sizes_t[i];
            }
        }

        bool is_2sat_sat = two_sat(cd_2sat_l, cd_2sat_r, cd_2sat_cur_sz_f + cd_2sat_cur_sz_t, s->n, is_f, is_t);

        if (is_2sat_sat && ix == 0) {

            for (__int64 i = 0; i < s->n; i++)
                sln[i] = s->Z[i];

            return true;

        }
        else if (is_2sat_sat)
            ix--;
        else if (s->Z[ix]) {
            for (__int64 i = ix; i >= 0; i--)
                s->Z[i] = false;
            ix++;
        }
        else {
            s->Z[ix] = true;
        }
    }

    return false;
}

#endif