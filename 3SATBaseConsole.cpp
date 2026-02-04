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

        printf_s("%lld: %lld %lld %lld\n", i, lst[i][0], lst[i][1], lst[i][2]);
    }

    printf_s("\n");

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

// https://github.com/mikolalysenko/strongly-connected-components/blob/master/scc.js

function stronglyConnectedComponents(adjList) {
    var numVertices = adjList.length;
    var index = new Array(numVertices)
        var lowValue = new Array(numVertices)
        var active = new Array(numVertices)
        var child = new Array(numVertices)
        var scc = new Array(numVertices)
        var sccLinks = new Array(numVertices)

        //Initialize tables
        for (var i = 0; i < numVertices; ++i) {
            index[i] = -1
                lowValue[i] = 0
                active[i] = false
                child[i] = 0
                scc[i] = -1
                sccLinks[i] = []
        }

    // The strongConnect function
    var count = 0
        var components = []
        var sccAdjList = []

        function strongConnect(v) {
        // To avoid running out of stack space, this emulates the recursive behaviour of the normal algorithm, effectively using T as the call stack.
        var S = [v], T = [v]
            index[v] = lowValue[v] = count
            active[v] = true
            count += 1
            while (T.length > 0) {
                v = T[T.length - 1]
                    var e = adjList[v]
                    if (child[v] < e.length) { // If we're not done iterating over the children, first try finishing that.
                        for (var i = child[v]; i < e.length; ++i) { // Start where we left off.
                            var u = e[i]
                                if (index[u] < 0) {
                                    index[u] = lowValue[u] = count
                                        active[u] = true
                                        count += 1
                                        S.push(u)
                                        T.push(u)
                                        break // First recurse, then continue here (with the same child!).
                                        // There is a slight change to Tarjan's algorithm here.
                                        // Normally, after having recursed, we set lowValue like we do for an active child (although some variants of the algorithm do it slightly differently).
                                        // Here, we only do so if the child we recursed on is still active.
                                        // The reasoning is that if it is no longer active, it must have had a lowValue equal to its own index, which means that it is necessarily higher than our lowValue.
                                }
                                else if (active[u]) {
                                    lowValue[v] = Math.min(lowValue[v], lowValue[u]) | 0
                                }
                            if (scc[u] >= 0) {
                                // Node v is not yet assigned an scc, but once it is that scc can apparently reach scc[u].
                                sccLinks[v].push(scc[u])
                            }
                        }
                        child[v] = i // Remember where we left off.
                    }
                    else { // If we're done iterating over the children, check whether we have an scc.
                        if (lowValue[v] == = index[v]) { // TODO: It /might/ be true that T is always a prefix of S (at this point!!!), and if so, this could be used here.
                            var component = []
                                var links = [], linkCount = 0
                                for (var i = S.length - 1; i >= 0; --i) {
                                    var w = S[i]
                                        active[w] = false
                                        component.push(w)
                                        links.push(sccLinks[w])
                                        linkCount += sccLinks[w].length
                                        scc[w] = components.length
                                        if (w == = v) {
                                            S.length = i
                                                break
                                        }
                                }
                            components.push(component)
                                var allLinks = new Array(linkCount)
                                for (var i = 0; i < links.length; i++) {
                                    for (var j = 0; j < links[i].length; j++) {
                                        allLinks[--linkCount] = links[i][j]
                                    }
                                }
                            sccAdjList.push(allLinks)
                        }
                        T.pop() // Now we're finished exploring this particular node (normally corresponds to the return statement)
                    }
            }
    }

    //Run strong connect starting from each vertex
    for (var i = 0; i < numVertices; ++i) {
        if (index[i] < 0) {
            strongConnect(i)
        }
    }

    // Compact sccAdjList
    var newE
        for (var i = 0; i < sccAdjList.length; i++) {
            var e = sccAdjList[i]
                if (e.length == = 0) continue
                    e.sort(function(a, b) { return a - b; })
                    newE = [e[0]]
                    for (var j = 1; j < e.length; j++) {
                        if (e[j] != = e[j - 1]) {
                            newE.push(e[j])
                        }
                    }
            sccAdjList[i] = newE
        }

    return { components: components, adjacencyList : sccAdjList }
}

// https://github.com/mikolalysenko/binary-search-bounds/blob/master/search-bounds.js

function ge(a, y, c, l, h) {
    var i = h + 1;
    while (l <= h) {
        var m = (l + h) >> > 1, x = a[m];
        var p = (c != = undefined) ? c(x, y) : (x - y);
        if (p >= 0) { i = m; h = m - 1 }
        else { l = m + 1 }
    }
    return i;
};

function gt(a, y, c, l, h) {
    var i = h + 1;
    while (l <= h) {
        var m = (l + h) >> > 1, x = a[m];
        var p = (c != = undefined) ? c(x, y) : (x - y);
        if (p > 0) { i = m; h = m - 1 }
        else { l = m + 1 }
    }
    return i;
};

function lt(a, y, c, l, h) {
    var i = l - 1;
    while (l <= h) {
        var m = (l + h) >> > 1, x = a[m];
        var p = (c != = undefined) ? c(x, y) : (x - y);
        if (p < 0) { i = m; l = m + 1 }
        else { h = m - 1 }
    }
    return i;
};

function le(a, y, c, l, h) {
    var i = l - 1;
    while (l <= h) {
        var m = (l + h) >> > 1, x = a[m];
        var p = (c != = undefined) ? c(x, y) : (x - y);
        if (p <= 0) { i = m; l = m + 1 }
        else { h = m - 1 }
    }
    return i;
};

function eq(a, y, c, l, h) {
    while (l <= h) {
        var m = (l + h) >> > 1, x = a[m];
        var p = (c != = undefined) ? c(x, y) : (x - y);
        if (p == = 0) { return m }
        if (p <= 0) { l = m + 1 }
        else { h = m - 1 }
    }
    return -1;
};

function norm(a, y, c, l, h, f) {
    if (typeof c == = 'function') {
        return f(a, y, c, (l == = undefined) ? 0 : l | 0, (h == = undefined) ? a.length - 1 : h | 0);
    }
    return f(a, y, undefined, (c == = undefined) ? 0 : c | 0, (l == = undefined) ? a.length - 1 : l | 0);
}

// https://github.com/mikolalysenko/2-sat/blob/master/2sat.js

function clauseToVariable(x, n) {
    if (x < 0) {
        return (-1 - x) + n
    }
    else {
        return x - 1
    }
}

function negate(x, n) {
    if (x < n) {
        return x + n
    }
    else {
        return x - n
    }
}

function compareInt(a, b) {
    return a - b
}

function contains(cc, v) {
    var b = bounds.le(cc, v)
        if (b >= 0) {
            return cc[b] == = v
        }
    return false
}

function solve2Sat(numVariables, clauses) {
    //Build implication graph
    var adj = new Array(2 * numVariables)
        for (var i = 0; i < 2 * numVariables; ++i) {
            adj[i] = []
        }
    for (var i = 0; i < clauses.length; ++i) {
        var c = clauses[i]
            var a = clauseToVariable(c[0], numVariables)
            var b = clauseToVariable(c[1], numVariables)
            var na = negate(a, numVariables)
            adj[na].push(b)
            var nb = negate(b, numVariables)
            adj[nb].push(a)
    }

    //Extract strongly connected components
    var scc = stronglyConnectedComponents(adj).components

        //Mark cells and check satisfiability
        var solution = new Array(2 * numVariables)
        for (var i = 0; i < solution.length; ++i) {
            solution[i] = -1
        }

    for (var i = 0; i < scc.length; ++i) {
        var cc = scc[i]
            cc.sort(compareInt)

            //Visit all nodes in queue
            var to_visit = []
            var color = 0
            for (var j = 0; j < cc.length; ++j) {
                var v = cc[j]
                    if (v < numVariables && contains(cc, numVariables + v)) {
                        return false
                    }
                var s = solution[v]
                    if (s >= 0) {
                        color = s
                    }
            }

        //Update solution in component
        for (var j = 0; j < cc.length; ++j) {
            var v = cc[j]
                var nv = negate(v, numVariables)
                solution[v] = color
                solution[nv] = color ^ 1
                var e = color ? adj[v] : adj[nv]
                for (var k = 0; k < e.length; ++k) {
                    solution[e[k]] = 1
                }
        }
    }

    solution.length = numVariables
        return solution
}

bool two_sat(__int64* lst_l_parm, __int64* lst_r_parm, __int64 k_parm, __int64 n_parm) {

    bool is_sat = false;

    __int64 counter = 1;

    __int64* encoding = new __int64[n_parm]; // from 2..n_parm to 2..n
    __int64* decoding = new __int64[n_parm]; // from 2..n to 2..n_parm

    for (__int64 i = 0; i < n_parm; i++) {
        encoding[i] = 0;
        decoding[i] = 0;
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

    for (__int64 i = 2; i < n_parm; i++)
        if (used[i])
            n++;

    for (__int64 i = 2; i < n_parm; i++) {
        if (used[i]) {
            encoding[i] = counter;
            decoding[counter] = i;
            counter++;
        }
    }

    __int64* lst_l = new __int64[k_parm];
    __int64* lst_r = new __int64[k_parm];

    for (__int64 i = 0; i < k; i++) {
        lst_l[i] = 0;
        lst_r[i] = 0;
    }

    __int64 counter_k = 0;

    for (__int64 i = 0; i < k_parm; i++) {

        __int64 l_abs = lst_l_parm[i] < 0 ? -lst_l_parm[i] : lst_l_parm[i];
        __int64 r_abs = lst_r_parm[i] < 0 ? -lst_r_parm[i] : lst_r_parm[i];

        lst_l[counter_k] = lst_l_parm[i] < 0 ? -encoding[l_abs] : encoding[l_abs];
        lst_r[counter_k] = lst_r_parm[i] < 0 ? -encoding[r_abs] : encoding[r_abs];

        counter_k++;
    }


    // clean up

    delete[] encoding;
    delete[] decoding;

    delete[] used;

    delete[] lst_l;
    delete[] lst_r;

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

    __int64 count_always_t = 0;
    __int64 count_always_f = 0;

    for (__int64 i = 0; i < s->n; i++)
        if (always_t[i])
            count_always_t++;

    for (__int64 i = 0; i < s->n; i++)
        if (always_f[i])
            count_always_f++;

    bool is_sat = false;

    while (true) {

        __int64 size_2sat = count_always_f + count_always_t;

        for (__int64 i = s->n - 1; i >= ix; i--)
            if (s->Z[i])
                size_2sat += s->cd_sizes_f[i];
            else
                size_2sat += s->cd_sizes_t[i];

        __int64* cd_2sat_l = new __int64[size_2sat];
        __int64* cd_2sat_r = new __int64[size_2sat];
        __int64 cd_2sat_cur_sz_f = 0;
        __int64 cd_2sat_cur_sz_t = 0;

        for (__int64 i = 2; i < s->n; i++) {
            if (always_f[i]) {
                cd_2sat_l[cd_2sat_cur_sz_f] = i;
                cd_2sat_r[cd_2sat_cur_sz_f] = i;
                cd_2sat_cur_sz_f++;
            }
            if (always_t[i]) {
                cd_2sat_l[cd_2sat_cur_sz_t] = i;
                cd_2sat_r[cd_2sat_cur_sz_t] = i;
                cd_2sat_cur_sz_t++;
            }
        }

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