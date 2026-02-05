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

__int64* simp_stack_create(__int64* vtop, __int64 *vcap) {

    *vtop = -1;
    *vcap = 16;
    return simp_vector_create(*vcap);

}

__int64 simp_stack_pop(__int64* s, __int64* vtop, __int64 vcap) {

    if (*vtop == -1)
        return 0;
    else {
        *vtop = *vtop - 1;
        return simp_vector_read(s, *vtop, vcap, *vtop + 1);
    }
}

void simp_stack_push(__int64** s, __int64* vtop, __int64* vcap, __int64 data) {

    simp_vector_append(s, vtop, vcap, data);
    *vtop = *vtop + 1;

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

// https://www.geeksforgeeks.org/c-program-for-iterative-quick-sort/

/* This function is same in both iterative and recursive*/
__int64 partition(__int64 arr_parm[], __int64 low_parm, __int64 high_parm)
{
    __int64 pivot = arr_parm[high_parm];    // pivot
    __int64 i = (low_parm - 1);  // Index of smaller element

    for (__int64 j = low_parm; j <= high_parm - 1; j++)
    {
        // swap if current element is greater than pivot
        if (arr_parm[j] > pivot)
        {
            i++;    // increment index of smaller element
            __int64 t = arr_parm[i];
            arr_parm[i] = arr_parm[j];
            arr_parm[j] = t;
        }
    }
    __int64 t = arr_parm[i + 1];
    arr_parm[i + 1] = arr_parm[high_parm];
    arr_parm[high_parm] = t;
    return (i + 1);
}

/* A[] --> Array to be sorted,
   l  --> Starting index,
   h  --> Ending index */
void MyQSort(__int64 arr[], __int64 l, __int64 h)
{
    // Create an auxiliary stack
    __int64* stack = new __int64[h - l + 1];

    // initialize top of stack
    __int64 top = -1;

    // push initial values of l and h to stack
    stack[++top] = l;
    stack[++top] = h;

    // Keep popping from stack while is not empty
    while (top >= 0) {
        // Pop h and l
        h = stack[top--];
        l = stack[top--];

        // Set pivot element at its correct position
        // in sorted array
        __int64 p = partition(arr, l, h);

        // If there are elements on left side of pivot,
        // then push left side to stack
        if (p - 1 > l) {
            stack[++top] = l;
            stack[++top] = p - 1;
        }

        // If there are elements on right side of pivot,
        // then push right side to stack
        if (p + 1 < h) {
            stack[++top] = p + 1;
            stack[++top] = h;
        }
    }

    delete[] stack;
}


// https://github.com/mikolalysenko/strongly-connected-components/blob/master/scc.js

void stronglyConnectedComponents(__int64** adjList, __int64* adjList_top, __int64 numVertices, __int64 ***components, __int64** components_top, __int64** components_cap, __int64 ***sccAdjList, __int64** sccAdjList_top, __int64** sccAdjList_cap) {

    __int64* index = new __int64[numVertices];
    __int64* lowValue = new __int64[numVertices];
    bool* active = new bool[numVertices];
    __int64* child = new __int64[numVertices];

    __int64* scc = new __int64[numVertices];

    __int64* sccLinks_top = new __int64[numVertices];
    __int64* sccLinks_cap = new __int64[numVertices];
    __int64** sccLinks = new __int64* [numVertices];

    for (__int64 i = 0; i < numVertices; i++) {

        sccLinks_top[i] = -1;
        sccLinks_cap[i] = 16;
        sccLinks[i] = simp_stack_create(&(sccLinks_top[i]), &(sccLinks_cap[i]));
    }

    //Initialize tables
    for (__int64 i = 0; i < numVertices; ++i) {
        index[i] = -1;
        lowValue[i] = 0;
        active[i] = false;
        child[i] = 0;
        scc[i] = -1;
        sccLinks[i] = simp_stack_create(&(sccLinks_top[i]), &(sccLinks_cap[i]));
    }

    // The strongConnect function
    __int64 count = 0;
    *components_cap = new __int64[numVertices];
    *components_top = new __int64[numVertices];

    for (__int64 i = 0; i < numVertices; i++) {

        *(components_cap[i]) = 16;
        *(components_top[i]) = -1;
        *(components[i]) = simp_stack_create(&(*(components_cap[i])), &(*(components_top[i])));
    }

    __int64* temp_sccAdjList_top = new __int64[numVertices];
    __int64* temp_sccAdjList_cap = new __int64[numVertices];
    __int64** temp_sccAdjList = new __int64*[numVertices];

    for (__int64 i = 0; i < numVertices; i++) {
        
        temp_sccAdjList_top[i] = -1;
        temp_sccAdjList_cap[i] = 16;
        
        temp_sccAdjList[i] = simp_stack_create(&(temp_sccAdjList_top[i]), &(temp_sccAdjList_cap[i]));
    }

    for (__int64 i = 0; i < numVertices; i++) {

        *(sccAdjList_cap[i]) = 16;
        *(sccAdjList_top[i]) = -1;
        *(sccAdjList[i]) = simp_stack_create(&(*(sccAdjList_top[i])), &(*(sccAdjList_cap[i])));
    }

    //Run strong connect starting from each vertex
    for (__int64 i = 0; i < numVertices; i++) {

        if (index[i] < 0) {

            __int64 v = i;

            __int64 S_cap = 16;
            __int64 S_top = -1;
            __int64* S = simp_stack_create(&S_top, &S_cap);
            simp_stack_push(&S, &S_top, &S_cap, v);
            __int64 T_cap = 16;
            __int64 T_top = -1;
            __int64* T = simp_stack_create(&T_top, &T_cap);
            simp_stack_push(&T, &T_top, &T_cap, v);

            index[v] = lowValue[v] = count;
            active[v] = true;
            count += 1;

            while (T_top >= 0) {
                v = T[T_top];
                __int64 e_len = adjList_top[v] + 1;
                __int64* e = new __int64[e_len];
                for (__int64 j = 0; j < e_len; j++)
                    e[j] = adjList[v][j];
                if (child[v] < e_len) { // If we're not done iterating over the children, first try finishing that.
                    for (__int64 j = child[v]; j < e_len; j++) { // Start where we left off.
                        __int64 u = e[j];
                        if (index[u] < 0) {
                            index[u] = lowValue[u] = count;
                            active[u] = true;
                            count += 1;
                            simp_stack_push(&S, &S_top, &S_cap, u);
                            simp_stack_push(&T, &T_top, &T_cap, u);
                            break; // First recurse, then continue here (with the same child!).
                            // There is a slight change to Tarjan's algorithm here.
                            // Normally, after having recursed, we set lowValue like we do for an active child (although some variants of the algorithm do it slightly differently).
                            // Here, we only do so if the child we recursed on is still active.
                            // The reasoning is that if it is no longer active, it must have had a lowValue equal to its own index, which means that it is necessarily higher than our lowValue.
                        }
                        else if (active[u])
                            lowValue[v] = (lowValue[v] < lowValue[u] ? lowValue[v] : lowValue[u]) | 0;
                        if (scc[u] >= 0)
                            simp_stack_push(&(sccLinks[v]), &(sccLinks_top[v]), &(sccLinks_cap[v]), scc[u]);
                        // Node v is not yet assigned an scc, but once it is that scc can apparently reach scc[u]
                        child[v] = j; // Remember where we left off.
                    }
                }
                else { // If we're done iterating over the children, check whether we have an scc.
                    if (lowValue[v] == index[v]) { // TODO: It /might/ be true that T is always a prefix of S (at this point!!!), and if so, this could be used here.

                        __int64 component_cap = 16;
                        __int64 component_top = -1;
                        __int64* component = simp_stack_create(&component_top, &component_cap);

                        __int64* links_cap = new __int64[S_top + 1];
                        __int64* links_top = new __int64[S_top + 1];
                        __int64** links = new __int64* [S_top + 1];

                        for (_int64 j = 0; j < S_top + 1; j++) {
                            links_cap[j] = 16;
                            links_top[j] = -1;
                            links[i] = simp_stack_create(&(links_top[j]), &(links_cap[j]));
                        }

                        __int64 linkCount = 0;
                        for (__int64 j = S_top; j >= 0; j--) {
                            __int64 w = S[i];
                            active[w] = false;
                            simp_stack_push(&component, &component_top, &component_cap, w);

                            for (__int64 k = 0; k < sccLinks_top[w] + 1; k++)
                                simp_stack_push(&(links[k]), &(links_top[v]), &(links_cap[v]), sccLinks[w][k]);

                            linkCount += sccLinks_top[w] + 1;
                            scc[w] = *(components_top[v]) + 1;
                            if (w == v) {
                                S_top = j - 1;
                                break;
                            }
                        }
                        for (__int64 j = 0; j < component_top + 1; j++)
                            simp_stack_push(&(*(components[v])), &(*(components_top[v])), &(*(components_cap[v])), component[j]);

                        __int64* allLinks = new __int64[linkCount];

                        __int64 tempLinkCount = linkCount;

                        for (__int64 j = 0; j < S_top + 1; j++) {
                            for (__int64 k = 0; k < links_top[i] + 1; k++) {
                                tempLinkCount--;
                                allLinks[tempLinkCount] = links[j][k];
                            }
                        }

                        for (__int64 j = 0; j < linkCount; j++)
                            simp_stack_push(&(temp_sccAdjList[v]), &(temp_sccAdjList_top[v]), &(temp_sccAdjList_cap[v]), allLinks[j]);

                        simp_stack_pop(T, &T_top, T_cap); // Now we're finished exploring this particular node (normally corresponds to the return statement)

                        // clean up

                        for (__int64 j = 0; j < S_top + 1; j++)
                            delete[] links[j];

                        delete[] links_cap;
                        delete[] links_top;
                        delete[] links;
                    }

                }
            }
        }
    }

    // Compact sccAdjList

    for (__int64 i = 0; i < numVertices; i++) {

        if (temp_sccAdjList_top[i] + 1 == 0)
            continue;

        __int64 newE_top = -1;
        __int64 newE_cap = 16;
        __int64* newE = simp_stack_create(&newE_top, &newE_cap);

        __int64* e = new __int64[temp_sccAdjList_top[i] + 1];

        for (__int64 j = 0; j < temp_sccAdjList_top[i] + 1; j++)
            e[j] = temp_sccAdjList[i][j];

        MyQSort(e, 0, temp_sccAdjList_top[i]);

        simp_stack_push(&newE, &newE_top, &newE_cap, e[0]);
                
        for (__int64 j = 1; j < temp_sccAdjList_top[i] + 1; j++)
            if (e[j] != e[j - 1])
                simp_stack_push(&newE, &newE_top, &newE_cap, e[j]);

        *(sccAdjList[i]) = newE;
        *(sccAdjList_top[i]) = newE_top;
        *(sccAdjList_cap[i]) = newE_cap;
    }

    // clean up

    for (__int64 i = 0; i < numVertices; i++)
        delete[] sccLinks[i];

    delete[] sccLinks_top;
    delete[] sccLinks_cap;
    delete[] sccLinks;

    for (__int64 i = 0; i < numVertices; i++)
        delete[] temp_sccAdjList[i];

    delete[] temp_sccAdjList_top;
    delete[] temp_sccAdjList_cap;
    delete[] temp_sccAdjList;

}

// https://github.com/mikolalysenko/2-sat/blob/master/2sat.js

__int64 clauseToVariable (__int64 x, __int64 n) {

    if (x < 0)
        return (-1 - x) + n;
    else
        return x - 1;
}

__int64 negate (__int64 x, __int64 n) {

    if (x < n)
        return x + n;
    else
        return x - n;
}

bool contains (__int64* cc, __int64 cc_top, __int64 v) {

    for (__int64 i = 0; i < cc_top + 1; i++)
        if (cc[i] == v)
            return true;

    return false;
}

bool solve2Sat(__int64 numVariables, __int64* clauses_l, __int64* clauses_r, __int64 k) {

    //Build implication graph
    __int64* adj_top = new __int64[2 * numVariables];
    __int64* adj_cap = new __int64[2 * numVariables];
    __int64** adj = new __int64* [2 * numVariables];

    for (__int64 i = 0; i < 2 * numVariables; ++i) {

        adj_top[i] = -1;
        adj_cap[i] = 16;
        adj[i] = simp_stack_create(&(adj_top[i]), &(adj_cap[i]));
    }

    for (__int64 i = 0; i < k; i++) {

        __int64 a = clauseToVariable(clauses_l[i], numVariables);
        __int64 b = clauseToVariable(clauses_r[i], numVariables);
        __int64 na = negate(a, numVariables);
        simp_stack_push(&(adj[na]), &(adj_top[na]), &(adj_cap[na]), b);
        __int64 nb = negate(b, numVariables);
        simp_stack_push(&(adj[nb]), &(adj_top[nb]), &(adj_cap[nb]), a);
    }

    //Extract strongly connected components

    __int64* scc_top;
    __int64* scc_cap;
    __int64** scc;

    __int64* sccAdjList_top;
    __int64* sccAdjList_cap;
    __int64** sccAdjList;

    stronglyConnectedComponents(adj, adj_top, numVariables, &scc, &scc_top, &scc_cap, &sccAdjList, &sccAdjList_top, &sccAdjList_cap);
    
        //Mark cells and check satisfiability
    __int64* solution = new __int64[2 * numVariables];

    for (__int64 i = 0; i < 2 * numVariables; i++)
        solution[i] = -1;

    for (__int64 i = 0; i < numVariables; i++) {

        __int64* cc = new __int64[scc_top[i] + 1];

        for (__int64 j = 0; j < scc_top[i] + 1; j++)
            cc[j] = scc[i][j];
            
        MyQSort(cc, 0, scc_top[i]);

            //Visit all nodes in queue
        __int64 color = 0;
       
        for (__int64 j = 0; j < scc_top[i] + 1; j++) {

            __int64 v = cc[j];
                    
            if (v < numVariables && contains(cc, scc_top[i], numVariables + v))
                return false;

            __int64 s = solution[v];

            if (s >= 0)
                color = s;
        }

        //Update solution in component
        for (__int64 j = 0; j < scc_top[i] + 1; j++) {

            __int64 v = cc[j];
            __int64 nv = negate(v, numVariables);
            solution[v] = color;
            solution[nv] = color ^ 1;

            __int64 e_len;
            __int64* e;
            if (color) {
                e_len = adj_top[v] + 1;
                e = new __int64[e_len];
                for (__int64 k = 0; k < e_len; k++)
                    e[k] = adj[v][k];
            }
            else {
                e_len = adj_top[nv] + 1;
                e = new __int64[e_len];
                for (__int64 k = 0; k < e_len; k++)
                    e[k] = adj[nv][k];
            }

            for (__int64 k = 0; k < e_len; ++k)
                solution[e[k]] = 1;
        }
    }

    return true;
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

    for (__int64 i = 0; i < k_parm; i++) {
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

    bool is_sat = solve2Sat(n, lst_l, lst_r, k_parm);

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