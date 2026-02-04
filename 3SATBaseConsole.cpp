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
        (*v)[*vtop] = data;
    else {
        bool* newv = new bool[*vcap * 2];
        for (__int64 i = 0; i < *vcap * 2; i++)
            newv[i] = 0;
        for (__int64 i = 0; i < *vcap; i++)
            newv[i] = *v[i];
        *vcap = *vcap * 2;
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
        (*v)[*vtop] = data;
    else {
        __int64* newv = new __int64[*vcap * 2];
        for (__int64 i = 0; i < *vcap * 2; i++)
            newv[i] = 0;
        for (__int64 i = 0; i < *vcap; i++)
            newv[i] = *v[i];
        *vcap = *vcap * 2;
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

    s->chops = chops;

    s->Z = SATSolver_create_boundary(true, chops, chop, n);

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

    for (__int64 i = 2; i < n; i++) {

        s->cd_sizes_f[i] = 0;
        s->cd_sizes_t[i] = 0;

        for (__int64 j = 0; j < s->k; j++) {

            __int64 abs_l = s->inopcell_l[j] < 0 ? -s->inopcell_l[j] : s->inopcell_l[j];
            __int64 abs_m = s->inopcell_m[j] < 0 ? -s->inopcell_m[j] : s->inopcell_m[j];
            __int64 abs_r = s->inopcell_r[j] < 0 ? -s->inopcell_r[j] : s->inopcell_r[j];

            __int64 count = 0;

            if (abs_l == 1)
                count++;
            if (abs_m == 1)
                count++;
            if (abs_r == 1)
                count++;

            if (count == 2)
                continue;

            __int64 loc = -1;
            __int64 val = 0;

            if (abs_l == i) {
                loc = LVAL;
                val = s->inopcell_l[j];
            }
            else if (abs_m == i) {
                loc = MVAL;
                val = s->inopcell_m[j];
            }
            else if (abs_r == i) {
                loc = RVAL;
                val = s->inopcell_r[j];
            }
            else
                continue;

            if (val < 0) {
                __int64 left_val = loc == LVAL ? s->inopcell_m[j] : loc == MVAL ? s->inopcell_l[j] : s->inopcell_l[j];
                __int64 right_val = loc == LVAL ? s->inopcell_r[j] : loc == MVAL ? s->inopcell_r[j] : s->inopcell_m[j];
                simp_vector_append(&(s->cdopcelll_t[i]), &(s->cdol_vtop_t[i]), &(s->cdol_vcap_t[i]), left_val);
                simp_vector_append(&(s->cdopcellr_t[i]), &(s->cdor_vtop_t[i]), &(s->cdor_vcap_t[i]), right_val);
                s->cd_sizes_t[i]++;
            }
            else {
                __int64 left_val = loc == LVAL ? s->inopcell_m[j] : loc == MVAL ? s->inopcell_l[j] : s->inopcell_l[j];
                __int64 right_val = loc == LVAL ? s->inopcell_r[j] : loc == MVAL ? s->inopcell_r[j] : s->inopcell_m[j];
                simp_vector_append(&(s->cdopcelll_f[i]), &(s->cdol_vtop_f[i]), &(s->cdol_vcap_f[i]), left_val);
                simp_vector_append(&(s->cdopcellr_f[i]), &(s->cdor_vtop_f[i]), &(s->cdor_vcap_f[i]), right_val);
                s->cd_sizes_f[i]++;
            }

        }
    }

}

void SATSolver_destroy(SATSolver* s) {

    delete[] s->Z;

    for (__int64 i = 0; i < s->n; i++) {

        delete[] s->cdopcelll_f[i];
        delete[] s->cdopcellr_f[i];

        delete[] s->cdopcelll_t[i];
        delete[] s->cdopcellr_t[i];

    }

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

    bool is_sat = false;

    __int64 counter = 2;

    __int64* encoding = new __int64[n_parm]; // from 2..n_parm to 2..n
    __int64* decoding = new __int64[n_parm]; // from 2..n to 2..n_parm

    for (__int64 i = 0; i < n_parm; i++) {
        encoding[i] = 0;
        decoding[i] = 0;
    }

    for (__int64 i = 2; i < n_parm; i++) {
        if (is_f_parm[i]) {
            encoding[i] = counter;
            decoding[counter] = i;
            counter++;
        }
        else if (is_t_parm[i]) {
            encoding[i] = counter;
            decoding[counter] = i;
            counter++;
        }
    }

    bool* used = new bool[n_parm];

    for (__int64 i = 0; i < n_parm; i++)
        used[i] = false;

    for (__int64 i = 0; i < n_parm; i++)
        if (is_f_parm[i] || is_t_parm[i])
            used[i] = true;

    for (__int64 i = 0; i < k_parm; i++) {

        __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
        __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

        used[l_abs] = true;
        used[r_abs] = true;
    }

    __int64 n = 2;

    for (__int64 i = 2; i < n_parm; i++)
        if (used[i])
            n++;

    for (__int64 i = 2; i < n_parm; i++) {
        if (used[i] && ! is_f_parm[i] && ! is_t_parm[i]) {
            encoding[i] = counter;
            decoding[counter] = i;
            counter++;
        }
    }

    __int64* is_f = new __int64[n];
    __int64* is_t = new __int64[n];

    for (__int64 i = 0; i < n; i++) {
        is_f[i] = false;
        is_t[i] = false;
    }

    for (__int64 i = 2; i < n; i++) {
        if (is_f_parm[decoding[i]])
            is_f[i] = true;
        if (is_t_parm[decoding[i]])
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

    __int64** false_implies = new __int64* [n];
    __int64** true_implies = new __int64* [n];

    __int64* false_implies_sz = new __int64[n];
    __int64* true_implies_sz = new __int64[n];

    for (__int64 i = 0; i < n; i++) {

        false_implies_sz[i] = 0;
        true_implies_sz[i] = 0;
    }

    __int64* lst_l = new __int64[k];
    __int64* lst_r = new __int64[k];

    for (__int64 i = 0; i < k; i++) {
        lst_l[i] = 0;
        lst_r[i] = 0;
    }

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

    for (__int64 i = 0; i < k; i++) {

        __int64 l_abs = lst_l[i] < 0 ? -lst_l[i] : lst_l[i];
        __int64 r_abs = lst_r[i] < 0 ? -lst_r[i] : lst_r[i];

        if (lst_l[i] < 0)
            true_implies_sz[l_abs]++;
        else
            false_implies_sz[l_abs]++;

        if (lst_r[i] < 0)
            true_implies_sz[r_abs]++;
        else
            false_implies_sz[r_abs]++;
    }

    for (__int64 i = 0; i < n; i++) {
        true_implies[i] = new __int64[true_implies_sz[i]];
        false_implies[i] = new __int64[false_implies_sz[i]];
    }

    for (__int64 i = 0; i < n; i++) {
        for (__int64 j = 0; j < true_implies_sz[i]; j++)
            true_implies[i][j] = 0;
        for (__int64 j = 0; j < false_implies_sz[i]; j++)
            false_implies[i][j] = 0;
    }

    __int64* true_implies_counter = new __int64[n];
    __int64* false_implies_counter = new __int64[n];

    for (__int64 i = 0; i < n; i++) {
        true_implies_counter[i] = 0;
        false_implies_counter[i] = 0;
    }

    for (__int64 i = 0; i < k; i++) {

        __int64 l_abs = lst_l[i] < 0 ? -lst_l[i] : lst_l[i];
        __int64 r_abs = lst_r[i] < 0 ? -lst_r[i] : lst_r[i];

        if (lst_l[i] < 0) {
            true_implies[l_abs][true_implies_counter[l_abs]] = lst_r[i];
            true_implies_counter[l_abs]++;
        }
        else {
            false_implies[l_abs][false_implies_counter[l_abs]] = lst_r[i];
            false_implies_counter[l_abs]++;
        }
        if (lst_r[i] < 0) {
            true_implies[r_abs][true_implies_counter[r_abs]] = lst_l[i];
            true_implies_counter[r_abs]++;
        }
        else {
            false_implies[r_abs][false_implies_counter[r_abs]] = lst_l[i];
            false_implies_counter[r_abs]++;
        }
    }

    bool* Z = new bool[n];

    for (__int64 i = 0; i < n; i++)
        Z[i] = false;

    bool* falses = new bool[n];
    bool* trues = new bool[n];

    __int64 ix = n - 1;

    while (true) {

        for (__int64 i = 0; i < n; i++) {
            falses[i] = false;
            trues[i] = false;
        }
        
        for (__int64 i = 0; i < n; i++) {

            if (is_f[i])
                falses[i] = true;

            if (is_t[i])
                trues[i] = true;

        }

        for (__int64 i = 2; i < n; i++) {
            if (Z[i])
                for (__int64 j = 0; j < true_implies_sz[i]; j++) {
                    __int64 val_abs = true_implies[i][j] < 0 ? - true_implies[i][j] : true_implies[i][j];
                    if (true_implies[i][j] < 0)
                        falses[val_abs] = true;
                    else
                        trues[val_abs] = true;
                }
            else
                for (__int64 j = 0; j < false_implies_sz[i]; j++) {
                    __int64 val_abs = false_implies[i][j] < 0 ? -false_implies[i][j] : false_implies[i][j];
                    if (false_implies[i][j] < 0)
                        falses[val_abs] = true;
                    else
                        trues[val_abs] = true;
                }
        }

        bool contradiction = false;

        for (__int64 i = 2; i < n; i++)
            if (trues[i] && falses[i]) {
                contradiction = true;
                break;
            }

        if (!contradiction && ix == 2) {

            is_sat = true;
            break;

        }
        else if (!contradiction)
            ix--;
        else {
            for (__int64 i = ix - 1; i >= 0; i--)
                Z[i] = false;
            while (ix < n) {
                if (!Z[ix]) {
                    Z[ix] = true;
                    break;
                }
                else {
                    Z[ix] = false;
                    ix++;
                }
            }
        }

        if (ix == n)
            break;

    }

    // clean up

    delete[] encoding;
    delete[] decoding;

    delete[] used;

    delete[] is_f;
    delete[] is_t;

    for (__int64 i = 0; i < n; i++) {

        delete[] false_implies[i];
        delete[] true_implies[i];
    }

    delete[] false_implies;
    delete[] true_implies;

    delete[] false_implies_sz;
    delete[] true_implies_sz;

    delete[] lst_l;
    delete[] lst_r;

    delete[] true_implies_counter;
    delete[] false_implies_counter;

    delete[] Z;

    delete[] falses;
    delete[] trues;

    return is_sat;
}

bool SATSolver_isSat(SATSolver* s, bool* sln) {

    bool* always_f = new bool[s->n];
    bool* always_t = new bool[s->n];

    bool* is_f = new bool[s->n];
    bool* is_t = new bool[s->n];

    for (__int64 i = 0; i < s->n; i++) {

        always_f[i] = false;
        always_t[i] = false;

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

        if (val < 0) {
            always_f[val_abs] = true;
            is_f[val_abs] = true;
        }
        else {
            always_t[val_abs] = true;
            is_t[val_abs] = true;
        }

    }
    __int64 ix = s->n - 1 - s->chops;

    for (__int64 i = s->n - 1; i > ix; i--)
        if ((s->Z[i] && always_f[i]) || (!s->Z[i] && always_t[i])) {
            delete[] always_f;
            delete[] always_t;
            delete[] is_t;
            delete[] is_f;
            return false;
        }

    bool is_sat = false;

    while (true) {

        for (__int64 i = 0; i < s->n; i++)
            if (always_f[i])
                is_f[i] = true;
            else if (always_t[i])
                is_t[i] = true;

        for (__int64 i = s->n - 1; i > ix; i--)
            if (s->Z[i]) {
                is_t[i] = true;
                if (!always_t[i])
                    is_f[i] = false;
            }
            else if (!s->Z[i]) {
                if (!always_f[i])
                    is_t[i] = false;
                is_f[i] = true;

            }

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
                    cd_2sat_l[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t + j] = s->cdopcelll_f[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t + j] = s->cdopcellr_f[i][j];
                }
                cd_2sat_cur_sz_f += s->cd_sizes_f[i];
            }
            else {
                for (__int64 j = 0; j < s->cd_sizes_t[i]; j++) {
                    cd_2sat_l[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t + j] = s->cdopcelll_t[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t + j] = s->cdopcellr_t[i][j];
                }
                cd_2sat_cur_sz_t += s->cd_sizes_t[i];
            }
        }

        bool is_2sat_sat = two_sat(cd_2sat_l, cd_2sat_r, cd_2sat_cur_sz_f + cd_2sat_cur_sz_t, s->n, is_f, is_t);

        delete[] cd_2sat_l;
        delete[] cd_2sat_r;

        if (is_2sat_sat && ix == 2) {

            for (__int64 i = 0; i < s->n; i++)
                sln[i] = s->Z[i];

            is_sat = true;

            break;

        }
        else if (is_2sat_sat)
            ix--;
        else {

            for (__int64 i = ix - 1; i >= 0; i--) {
                    s->Z[i] = false;
                    is_f[i] = true;
                    if (! always_t [i])
                        is_t[i] = false;
                }

            while (ix < s->n - s->chops) {
                if (!s->Z[ix]) {
                    s->Z[ix] = true;
                    is_t[ix] = true;
                    if (! always_t[ix])
                        is_f[ix] = false;
                    break;
                }
                else if (s->Z[ix]) {
                    s->Z[ix] = false;
                    is_f[ix] = true;
                    if (!always_t[ix])
                        is_t[ix] = false;

                    ix++;
                }
            }
        }

        if (ix >= s->n - s->chops)
            break;
    }

    delete[] always_f;
    delete[] always_t;
    delete[] is_t;
    delete[] is_f;

    return is_sat;
}

#endif