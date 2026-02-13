// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_C__
#define __3SATBASECONSOLE_C__

#include "3SATBaseConsole.hpp"

#include <stdio.h>
#include <thread>
#include <condition_variable>
#include <mutex>

#define LVAL 0
#define MVAL 1
#define RVAL 2

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

        //printf_s("%lld: %lld %lld %lld\n", i, lst[i][0], lst[i][1], lst[i][2]);
    }

    //printf_s("\n");

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

    s->cd_sizes_f[0] = 0;
    s->cd_sizes_t[0] = 0;

    s->cd_sizes_f[1] = 0;
    s->cd_sizes_t[1] = 0;

    for (__int64 i = 2; i < n; i++) {

        s->cd_sizes_f[i] = 0;
        s->cd_sizes_t[i] = 0;

        for (__int64 j = 0; j < s->k; j++) {

            __int64 abs_l = s->inopcell_l[j] < 0 ? -s->inopcell_l[j] : s->inopcell_l[j];
            __int64 abs_m = s->inopcell_m[j] < 0 ? -s->inopcell_m[j] : s->inopcell_m[j];
            __int64 abs_r = s->inopcell_r[j] < 0 ? -s->inopcell_r[j] : s->inopcell_r[j];

            __int64 count_f = 0;

            if (s->inopcell_l[j] == FALSE_3SAT)
                count_f++;
            if (s->inopcell_m[j] == FALSE_3SAT)
                count_f++;
            if (s->inopcell_r[j] == FALSE_3SAT)
                count_f++;

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

            __int64 left_val = loc == LVAL ? s->inopcell_m[j] : loc == MVAL ? s->inopcell_l[j] : s->inopcell_l[j];
            __int64 right_val = loc == LVAL ? s->inopcell_r[j] : loc == MVAL ? s->inopcell_r[j] : s->inopcell_m[j];

            if (val < 0) {

                simp_vector_append(&(s->cdopcelll_t[i]), &(s->cdol_vtop_t[i]), &(s->cdol_vcap_t[i]), left_val);
                simp_vector_append(&(s->cdopcellr_t[i]), &(s->cdor_vtop_t[i]), &(s->cdor_vcap_t[i]), right_val);

                if (count_f == 0)
                    s->cd_sizes_t[i]++;
            }
            else {
                
                simp_vector_append(&(s->cdopcelll_f[i]), &(s->cdol_vtop_f[i]), &(s->cdol_vcap_f[i]), left_val);
                simp_vector_append(&(s->cdopcellr_f[i]), &(s->cdor_vtop_f[i]), &(s->cdor_vcap_f[i]), right_val);

                if (count_f == 0)
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


bool two_sat(__int64* lst_l_parm, __int64* lst_r_parm, __int64 k_parm, __int64 n_parm, bool * is_f, bool * is_t) {

    /*
    for (__int64 i = 0; i < k_parm; i++)
        printf_s("%lld: %lld %lld\n", i, lst_l_parm[i], lst_r_parm[i]);
    printf_s("\n");
    //*/

    __int64* encoding = new __int64[n_parm]; // from 2..n_parm to 2..n

    for (__int64 i = 0; i < n_parm; i++)
        encoding[i] = 0;

    bool* used = new bool[n_parm];

    for (__int64 i = 0; i < n_parm; i++)
        used[i] = false;

    for (__int64 i = 2; i < n_parm; i++)
        if (is_f[i] || is_t[i])
            used[i] = true;

    for (__int64 i = 0; i < k_parm; i++) {

        if (lst_l_parm[i] == FALSE_3SAT || lst_r_parm[i] == FALSE_3SAT)
            continue;
        else {

            __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
            __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

            used[l_abs] = true;
            used[r_abs] = true;
        }
    }

    __int64 n = 2;

    for (__int64 i = 2; i < n_parm; i++)
        if (used[i]) {
            encoding[i] = n;
            n++;
        }

    __int64* lst_l = new __int64[k_parm];
    __int64* lst_r = new __int64[k_parm];

    for (__int64 i = 0; i < k_parm; i++) {
        lst_l[i] = 0;
        lst_r[i] = 0;
    }

    __int64 k = 0;

    for (__int64 i = 0; i < k_parm; i++) {

        if (lst_l_parm[i] == FALSE_3SAT || lst_r_parm[i] == FALSE_3SAT)
            continue;
        else {

            __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
            __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

            lst_l[k] = lst_l_parm[i] < 0 ? -encoding[l_abs] : encoding[l_abs];
            lst_r[k] = lst_r_parm[i] < 0 ? -encoding[r_abs] : encoding[r_abs];

            k++;
        }
    }

    /*
    for (__int64 i = 0; i < k_parm; i++)
        printf_s("%lld: %lld %lld\n", i, lst_l[i], lst_r[i]);
    printf_s("\n");
    //*/

    __int64* false_implies_top = new __int64[n];
    __int64* false_implies_cap = new __int64[n];
    __int64** false_implies = new __int64* [n];
    __int64* true_implies_top = new __int64[n];
    __int64* true_implies_cap = new __int64[n];
    __int64** true_implies = new __int64* [n];

    for (__int64 i = 0; i < n; i++) {
        false_implies_top[i] = -1;
        false_implies_cap[i] = 16;
        false_implies[i] = simp_vector_create(false_implies_cap[i]);
        true_implies_top[i] = -1;
        true_implies_cap[i] = 16;
        true_implies[i] = simp_vector_create(true_implies_cap[i]);
    }

    for (__int64 i = 0; i < k; i++) {

        __int64 l_abs = lst_l[i] < 0 ? -lst_l[i] : lst_l[i];
        __int64 r_abs = lst_r[i] < 0 ? -lst_r[i] : lst_r[i];

        if (lst_l[i] < 0)
            simp_vector_append(&(true_implies[l_abs]), &(true_implies_top[l_abs]), &(true_implies_cap[l_abs]), lst_r[i]);
        else
            simp_vector_append(&(false_implies[l_abs]), &(false_implies_top[l_abs]), &(false_implies_cap[l_abs]), lst_r[i]);

        if (lst_r[i] < 0)
            simp_vector_append(&(true_implies[r_abs]), &(true_implies_top[r_abs]), &(true_implies_cap[r_abs]), lst_l[i]);
        else
            simp_vector_append(&(false_implies[r_abs]), &(false_implies_top[r_abs]), &(false_implies_cap[r_abs]), lst_l[i]);
            
    }

    bool is_sat = false;
    __int64 ix = n - 1;
    bool* Z = new bool[n];
    for (__int64 i = 0; i < n; i++)
        Z[i] = false;

    bool* falses = new bool[n];
    bool* trues = new bool[n];

    while (true) {

        for (__int64 i = 0; i < n; i++) {
            falses[i] = false;
            trues[i] = false;
        }

        for (__int64 i = 2; i < n_parm; i++) {
            if (is_f[i])
                falses[encoding[i]] = true;
            if (is_t[i])
                trues[encoding[i]] = true;
        }

        for (__int64 i = ix; i < n; i++)
            if (Z[i])
                trues[i] = true;
            else
                falses[i] = true;

        bool changed;

        do {

            changed = false;

            for (__int64 i = 2; i < n; i++) {
                if (trues[i]) {
                    for (__int64 j = 0; j < true_implies_top[i] + 1; j++) {

                        __int64 val = true_implies[i][j];
                        __int64 val_abs = val < 0 ? -val : val;

                        if (val < 0) {
                            if (!falses[val_abs]) {
                                falses[val_abs] = true;
                                changed = true;
                            }
                        }
                        else {
                            if (!trues[val_abs]) {
                                trues[val_abs] = true;
                                changed = true;
                            }
                        }
                    }
                }
                if (falses[i])
                    for (__int64 j = 0; j < false_implies_top[i] + 1; j++) {

                        __int64 val = false_implies[i][j];
                        __int64 val_abs = val < 0 ? -val : val;

                        if (val < 0) {
                            if (!falses[val_abs]) {
                                falses[val_abs] = true;
                                changed = true;
                            }
                        }
                        else {
                            if (!trues[val_abs]) {
                                trues[val_abs] = true;
                                changed = true;
                            }
                        }
                    }
            }
        } while (changed);
            
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
        else if (contradiction) {

            while (ix < n)
                if (Z[ix]) {
                    Z[ix] = false;
                    ix++;
                }
                else {
                    Z[ix] = true;
                    break;
                }

            for (__int64 i = ix - 1; i >= 0; i--)
                Z[i] = false;

        }

        if (ix >= n)
            break;

    }

    // clean up

    delete[] falses;
    delete[] trues;

    delete[] Z;

    for (__int64 i = 0; i < n; i++)
        delete[] false_implies[i];

    delete[] false_implies_top;
    delete[] false_implies_cap;
    delete[] false_implies;

    for (__int64 i = 0; i < n; i++)
        delete[] true_implies[i];

    delete[] true_implies_top;
    delete[] true_implies_cap;
    delete[] true_implies;

    delete[] encoding;

    delete[] used;

    delete[] lst_l;
    delete[] lst_r;

    return is_sat;
}

bool SATSolver_isSat(SATSolver* s, bool* sln) {

    bool* always_f = new bool[s->n];
    bool* always_t = new bool[s->n];

    for (__int64 i = 0; i < s->n; i++) {
        always_f[i] = false;
        always_t[i] = false;
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
            always_f[val_abs] = true;
        else
            always_t[val_abs] = true;

    }
    __int64 ix = s->n - 1 - s->chops;

    for (__int64 i = s->n - 1; i > ix; i--)
        if ((s->Z[i] && always_f[i]) || (!s->Z[i] && always_t[i])) {
            delete[] always_f;
            delete[] always_t;
            return false;
        }

    for (__int64 i = 2; i < s->n; i++)
        if (always_f[i] && always_t[i]) {
            delete[] always_f;
            delete[] always_t;
            return false;
        }

    bool is_sat = false;

    bool* is_f = new bool[s->n];
    bool* is_t = new bool[s->n];

    while (true) {

        for (__int64 i = 0; i < s->n; i++) {

            is_f[i] = false;
            is_t[i] = false;
        }

        for (__int64 i = 2; i < s->n; i++) {

            if (always_f[i])
                is_f[i] = true;
            if (always_t[i])
                is_t[i] = true;
        }

        for (__int64 i = ix; i < s->n; i++) {

            if (s->Z[i])
                is_t[i] = true;
            else
                is_f[i] = true;
        }

        bool changed;

        do {

            changed = false;

            for (__int64 i = 2; i < s->n; i++) {

                if (is_t[i]) {

                    for (__int64 j = 0; j < s->cdol_vtop_t[i] + 1; j++) {

                        __int64 count_f = 0;

                        if (s->cdopcelll_t[i][j] == FALSE_3SAT)
                            count_f++;
                        if (s->cdopcellr_t[i][j] == FALSE_3SAT)
                            count_f++;

                        if (count_f == 1) {

                            __int64 val = s->cdopcelll_t[i][j] == FALSE_3SAT ? s->cdopcellr_t[i][j] : s->cdopcelll_t[i][j];
                            __int64 val_abs = val < 0 ? -val : val;

                            if (val < 0) {
                                if (!is_f[val_abs]) {
                                    is_f[val_abs] = true;
                                    changed = true;
                                }
                            }
                            else {
                                if (!is_t[val_abs]) {
                                    is_t[val_abs] = true;
                                    changed = true;
                                }
                            }
                        }
                    }
                }
                if (is_f[i]) {

                    for (__int64 j = 0; j < s->cdol_vtop_f[i] + 1; j++) {

                        __int64 count_f = 0;

                        if (s->cdopcelll_f[i][j] == FALSE_3SAT)
                            count_f++;
                        if (s->cdopcellr_f[i][j] == FALSE_3SAT)
                            count_f++;

                        if (count_f == 1) {

                            __int64 val = s->cdopcelll_f[i][j] == FALSE_3SAT ? s->cdopcellr_f[i][j] : s->cdopcelll_f[i][j];
                            __int64 val_abs = val < 0 ? -val : val;

                            if (val < 0) {
                                if (!is_f[val_abs]) {
                                    is_f[val_abs] = true;
                                    changed = true;
                                }
                            }
                            else {
                                if (!is_t[val_abs]) {
                                    is_t[val_abs] = true;
                                    changed = true;
                                }
                            }
                        }
                    }
                }
            }
        } while (changed);

        // printf_s("ix: %lld\n", ix);

        __int64 size_2sat = 0;

        for (__int64 i = 2; i < s->n; i++) {
            if (is_t[i])
                size_2sat += s->cd_sizes_t[i];
            if (is_f[i])
                size_2sat += s->cd_sizes_f[i];
        }

        __int64* cd_2sat_l = new __int64[size_2sat];
        __int64* cd_2sat_r = new __int64[size_2sat];
        __int64 cd_2sat_cur_sz_f = 0;
        __int64 cd_2sat_cur_sz_t = 0;

        for (__int64 i = 2; i < s->n; i++) {

            if (is_t[i]) {

                for (__int64 j = 0; j < s->cdor_vtop_t[i] + 1; j++) {

                    if (s->cdopcelll_t[i][j] == FALSE_3SAT || s->cdopcellr_t[i][j] == FALSE_3SAT)
                        continue;

                    cd_2sat_l[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t] = s->cdopcelll_t[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t] = s->cdopcellr_t[i][j];

                    cd_2sat_cur_sz_t++;
                }
            }
            if (is_f[i]) {

                for (__int64 j = 0; j < s->cdor_vtop_f[i] + 1; j++) {

                    if (s->cdopcelll_f[i][j] == FALSE_3SAT || s->cdopcellr_f[i][j] == FALSE_3SAT)
                        continue;

                    cd_2sat_l[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t] = s->cdopcelll_f[i][j];
                    cd_2sat_r[cd_2sat_cur_sz_f + cd_2sat_cur_sz_t] = s->cdopcellr_f[i][j];

                    cd_2sat_cur_sz_f++;
                }
            }
        }

        bool is_2sat_sat = size_2sat == 0 ? true : two_sat(cd_2sat_l, cd_2sat_r, size_2sat, s->n, is_f, is_t);

        delete[] cd_2sat_l;
        delete[] cd_2sat_r;

        if (is_2sat_sat && ix == 2) {

            for (__int64 i = 2; i < s->n; i++) {
                sln[i] = s->Z[i];
                //printf_s("%lld ", sln[i] ? i : -i);
            }
            //printf_s("\n");

            is_sat = true;

            break;

        }
        else if (is_2sat_sat)
            ix--;
        else {

            while (ix < s->n - s->chops)
                if (s->Z[ix]) {
                    s->Z[ix] = false;
                    ix++;
                }
                else {
                    s->Z[ix] = true;
                    break;
                }

            for (__int64 i = ix - 1; i >= 0; i--)
                s->Z[i] = false;
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

std::mutex m;
std::condition_variable cv;
bool done = false;
bool ready = true;
bool solved = false;
__int64 thread_id = -1;

void thread_3SAT(__int64 tid, bool* arr, __int64** lst, __int64 k_parm, __int64 n_parm, __int64 chops, __int64 chop) {

    SATSolver* s = new SATSolver();
    SATSolver_create(s, lst, k_parm, n_parm, chops, chop);

    bool sat = SATSolver_isSat(s, arr);

    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [] {return ready; });
        ready = false;
        done = sat;
        thread_id = tid;
        cv.notify_all();
    }
}

bool SATSolver_threads(__int64** lst, __int64 k_parm, __int64 n_parm, bool** arr) {

    __int64 num_threads = std::thread::hardware_concurrency() ;
    
    std::thread** threadblock = new std::thread * [num_threads];

    for (__int64 i = 0; i < num_threads; i++)
        threadblock[i] = NULL;

    bool** arrs = new bool* [num_threads];

    for (__int64 i = 0; i < num_threads; i++)
        arrs[i] = new bool[n_parm];

    // get the right number of chops- at least 2^chops

    __int64 count_chops = 1;
    __int64 chops = 2;

    __int64 counter = 0;

    for (counter = 0; count_chops < num_threads; counter++)
        count_chops *= 2;

    if (count_chops == num_threads)
        chops = counter;
    else
        chops = counter + 1;

    chops += 4;


    ///*

    __int64 search_sz = 1;

    for (__int64 i = 0; i < chops; i++)
        search_sz *= 2;


    __int64 pos = 0;
    for (pos = 0; pos < num_threads; pos++)
        threadblock[pos] = new std::thread(thread_3SAT, pos, arrs[pos], lst, k_parm, n_parm, chops, pos);

    do {
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [] {return !ready; });
            threadblock[thread_id]->join();
            delete threadblock[thread_id];
            solved = done;
            if (solved)
                break;
            if (pos < search_sz) {
                threadblock[thread_id] = new std::thread(thread_3SAT, thread_id, arrs[thread_id], lst, k_parm, n_parm, chops, pos);
                ready = true;
                thread_id = -1;
                cv.notify_all();
            }
        }
        pos++;
    } while (pos < search_sz && !solved);

    //*/


    for (__int64 i = 0; i < num_threads; i++)
        if (solved && i != thread_id) {
            threadblock[thread_id]->join();
            delete threadblock[thread_id];
        }

    delete[] threadblock;

    if (solved)
        for (__int64 i = 0; i < n_parm; i++)
            (*arr)[i] = arrs[thread_id][i];

    // free up master resources

    return solved;
}

#endif