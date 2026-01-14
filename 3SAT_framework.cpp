// 3SAT_framework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATFRAMEWORK_C__
#define __3SATFRAMEWORK_C__

#include "3SAT_framework.hpp"
#include "3SATBaseConsole.hpp"

#include <iostream>
#include <stdio.h>

// Tseytin transformations being employed here to convert operations to 3-CNF

bit_3sat* create_bit(__int64 * num_para) {

    bit_3sat* ret = new bit_3sat();
    
    ret->id = *num_para;

    if ( *num_para != FALSE_3SAT && *num_para != TRUE_3SAT)
        (*num_para)++;

    return ret;

}

dec_3sat* create_dec(__int64 * num_para, __int64 bd_sz_parm, __int64 ad_sz_parm) {

    dec_3sat* ret = new dec_3sat();
    ret->bd_sz = bd_sz_parm;
    ret->ad_sz = ad_sz_parm;
    ret->sz = bd_sz_parm + ad_sz_parm;
    ret->bits = new bit_3sat * [ret->sz];
    for (__int64 i = 0; i < ret->sz; i++)
        ret->bits[i] = create_bit(num_para);
    return ret;

}

nat_3sat* create_nat(__int64 * num_para, __int64 sz_parm) {

    nat_3sat* ret = new nat_3sat();
    ret->sz = sz_parm;
    ret->bits = new bit_3sat * [ret->sz];
    for (__int64 i = 0; i < ret->sz; i++)
        ret->bits[i] = create_bit(num_para);
    return ret;

}

// c = a and b: (~a or ~b or c) (a or ~c) (b or ~c)

char* and_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id == TRUE_3SAT && b->id == TRUE_3SAT)
            (*c)->id = TRUE_3SAT;
        else
            (*c)->id = FALSE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [3];
    for (__int64 i = 0; i < 3; i++)
        lst[i] = new char[64];

    /*
    sprintf_s(lst[0], "-%lld -%lld %lld", a->id, b->id, c->id);
    sprintf_s(lst[1], "%lld -%lld", a->id, c->id);
    sprintf_s(lst[2], "%lld -%lld", b->id, c->id);
    */

    sprintf_s(lst[0], 64, "%lld %lld %lld", -a->id, -b->id, (*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", a->id, -(*c)->id, (__int64)FALSE_3SAT);
    sprintf_s(lst[2], 64, "%lld %lld %lld", b->id, -(*c)->id, (__int64)FALSE_3SAT);

    __int64 ret_len = (__int64)strnlen_s(lst[0], 64) + (__int64)strnlen_s(lst[1], 64) + (__int64)strnlen_s(lst[2], 64) + 3 * (__int64)strnlen_s("\n", 2) + 1;
    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 3; i++) {
        sprintf_s(&(ret[len]), ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = a nand b: (~a or ~b or ~c) (a or c) (b or c)

char* nand_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id == TRUE_3SAT && b->id == TRUE_3SAT)
            (*c)->id = FALSE_3SAT;
        else
            (*c)->id = TRUE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [3];
    for (__int64 i = 0; i < 3; i++)
        lst[i] = new char[64];

    /*
    sprintf_s(lst[0], "-%lld -%lld -%lld", a->id, b->id, c->id);
    sprintf_s(lst[1], "%lld %lld", a->id, c->id);
    sprintf_s(lst[2], "%lld %lld", b->id, c->id);
    */

    sprintf_s(lst[0], 64, "%lld %lld %lld", -a->id, -b->id, -(*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", a->id, (*c)->id, (__int64)FALSE_3SAT);
    sprintf_s(lst[2], 64, "%lld %lld %lld", b->id, (*c)->id, (__int64)FALSE_3SAT);

    __int64 ret_len = (__int64)strnlen_s(lst[0], 64) + (__int64)strnlen_s(lst[1], 64) + (__int64)strnlen_s(lst[2], 64) + 3 * (__int64)strnlen_s("\n", 2) + (__int64)1;
    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 3; i++) {
        sprintf_s(&ret[len], ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = a or b: (a or b or ~c) (~a or c) (~b or c)

char* or_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id == FALSE_3SAT && b->id == FALSE_3SAT)
            (*c)->id = FALSE_3SAT;
        else
            (*c)->id = TRUE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [3];
    for (__int64 i = 0; i < 3; i++)
        lst[i] = new char[64];

    /*
    sprintf_s(lst[0], "%lld %lld -%lld", a->id, b->id, c->id);
    sprintf_s(lst[1], "-%lld %lld", a->id, c->id);
    sprintf_s(lst[2], "-%lld %lld", b->id, c->id);
    */

    sprintf_s(lst[0], 64, "%lld %lld %lld", a->id, b->id, -(*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", -a->id, (*c)->id, (__int64)FALSE_3SAT);
    sprintf_s(lst[2], 64, "%lld %lld %lld", -b->id, (*c)->id, (__int64)FALSE_3SAT);

    __int64 ret_len = (__int64)strnlen_s(lst[0], 64) + (__int64)strnlen_s(lst[1], 64) + (__int64)strnlen_s(lst[2], 64) + 3 * (__int64)strnlen_s("\n", 2) + (__int64)1;
    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 3; i++) {
        sprintf_s(&ret[len], ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = a nor b: (a or b or c) (~a or ~c) (~b or ~c)

char* nor_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id == FALSE_3SAT && b->id == FALSE_3SAT)
            (*c)->id = TRUE_3SAT;
        else
            (*c)->id = FALSE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [3];
    for (__int64 i = 0; i < 3; i++)
        lst[i] = new char[64];

    /*
    sprintf_s(lst[0], "%lld %lld %lld", a->id, b->id, c->id);
    sprintf_s(lst[1], "-%lld -%lld", a->id, c->id);
    sprintf_s(lst[2], "-%lld -%lld", b->id, c->id);
    */

    sprintf_s(lst[0], 64, "%lld %lld %lld", a->id, b->id, (*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", -a->id, -(*c)->id, (__int64)FALSE_3SAT);
    sprintf_s(lst[2], 64, "%lld %lld %lld", -b->id, -(*c)->id, (__int64)FALSE_3SAT);

    __int64 ret_len = (__int64)strnlen_s(lst[0], 64) + (__int64)strnlen_s(lst[1], 64) + (__int64)strnlen_s(lst[2], 64) + 3 * (__int64)strnlen_s("\n", 2) + (__int64)1;
    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 3; i++) {
        sprintf_s(&(ret[len]), ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = not a: (~a or ~c) (a or c)

char* not_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, __int64* len_para) {

    /*
    if (a->id == FALSE_3SAT || a->id == TRUE_3SAT) {
        *c = new bit_3sat();
        (*c)->id = -(*c)->id;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [4];
    for (__int64 i = 0; i < 2; i++)
        lst[i] = new char[64];

    /*
    sprintf_s(lst[0], "-%lld -%lld", a->id, c->id);
    sprintf_s(lst[1], "%lld %lld", a->id, c->id);
    */

    sprintf_s(lst[0], 64, "%lld %lld %lld", -a->id, -(*c)->id, (__int64)FALSE_3SAT);
    sprintf_s(lst[1], 64, "%lld %lld %lld", a->id, (*c)->id, (__int64)FALSE_3SAT);

    __int64 ret_len = (__int64)strnlen_s(lst[0], 64) + (__int64)strnlen_s(lst[1], 64) + 2 * (__int64)strnlen_s("\n", 2) + (__int64)1;

    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 2; i++) {
        sprintf_s(&ret[len], ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = a xor b: (~a or ~b or ~c) (a or b or ~c) (a or ~b or c) (~a or b or c)

char* xor_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id != b->id)
            (*c)->id = TRUE_3SAT;
        else
            (*c)->id = FALSE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);
    char** lst = new char* [4];
    for (__int64 i = 0; i < 4; i++)
        lst[i] = new char[64];

    sprintf_s(lst[0], 64, "%lld %lld %lld", -a->id, -b->id, -(*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", a->id, b->id, -(*c)->id);
    sprintf_s(lst[2], 64, "%lld %lld %lld", a->id, -b->id, (*c)->id);
    sprintf_s(lst[3], 64, "%lld %lld %lld", -a->id, b->id, (*c)->id);

    __int64 ret_len = 1;
    for (__int64 i = 0; i < 4; i++)
        ret_len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);

    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 4; i++) {
        sprintf_s(&(ret[len]), ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete[] lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// c = a xnor b: (~a or ~b or c) (a or b or c) (a or ~b or ~c) (~a or b or ~c)

char* xnor_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((a->id == FALSE_3SAT || a->id == TRUE_3SAT) && (b->id == FALSE_3SAT || b->id == TRUE_3SAT)) {
        *c = new bit_3sat();
        if (a->id == b->id)
            (*c)->id = TRUE_3SAT;
        else
            (*c)->id = FALSE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    *c = create_bit(num_para);

    char** lst = new char* [4];
    for (__int64 i = 0; i < 4; i++)
        lst[i] = new char[64];

    sprintf_s(lst[0], 64, "%lld %lld %lld", -a->id, -b->id, (*c)->id);
    sprintf_s(lst[1], 64, "%lld %lld %lld", a->id, b->id, (*c)->id);
    sprintf_s(lst[2], 64, "%lld %lld %lld", a->id, -b->id, -(*c)->id);
    sprintf_s(lst[3], 64, "%lld %lld %lld", -a->id, b->id, -(*c)->id);

    __int64 ret_len = 1;
    for (__int64 i = 0; i < 4; i++)
        ret_len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);

    char* ret = new char[ret_len];

    __int64 len = 0;
    for (__int64 i = 0; i < 4; i++) {
        sprintf_s(&(ret[len]), ret_len - len, "%s\n", lst[i]);
        len += (__int64)strnlen_s(lst[i], 64) + (__int64)strnlen_s("\n", 2);
        delete[] lst[i];
    }

    *len_para = len;

    delete[] lst;

    return ret;

}

// bitadd = a + b + cin: cin xor (a xor b) = (~a or ~b or ~c) (a or b or ~c) (a or ~b or c) (~a or b or c)

char* bitaddsum_3sat(__int64* num_para, bit_3sat** sum, bit_3sat* c_in, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((c_in->id == TRUE_3SAT || c_in->id == FALSE_3SAT) && (a->id == TRUE_3SAT || a->id == FALSE_3SAT) && (b->id == TRUE_3SAT || b->id == FALSE_3SAT)) {
        int x = c_in->id == TRUE_3SAT ? 1 : 0;
        int y = a->id == TRUE_3SAT ? 1 : 0;
        int z = b->id == TRUE_3SAT ? 1 : 0;
        *sum = new bit_3sat();
        (*sum)->id = (x + y + z) % 2 == 0 ? FALSE_3SAT : TRUE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    bit_3sat* c;

    __int64 x1_len = 0;
    char* x1 = xor_3sat(num_para, &c, a, b, &x1_len);

    __int64 x2_len = 0;
    char* x2 = xor_3sat(num_para, sum, c_in, c, &x2_len);

    __int64 ret_len = x1_len + x2_len + 1;
    char* ret = new char[ret_len];

    __int64 pos = 0;

    if (x1_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, x1);
        pos += x1_len;
    }
    if (x2_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, x2);
        pos += x2_len;
    }

    *len_para = pos;

    delete c;

    delete[] x1;
    delete[] x2;

    return ret;

}

// bitcout = a + b + cin: (a and b) or (b and cin) or (a and cin):

char* bitaddcout_3sat(__int64* num_para, bit_3sat** c_out, bit_3sat* c_in, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    /*
    if ((c_in->id == TRUE_3SAT || c_in->id == FALSE_3SAT) && (a->id == TRUE_3SAT || a->id == FALSE_3SAT) && (b->id == TRUE_3SAT || b->id == FALSE_3SAT)) {
        int x = c_in->id == TRUE_3SAT ? 1 : 0;
        int y = a->id == TRUE_3SAT ? 1 : 0;
        int z = b->id == TRUE_3SAT ? 1 : 0;
        *c_out = new bit_3sat();
        (*c_out)->id = (x + y + z) < 2 ? FALSE_3SAT : TRUE_3SAT;
        *len_para = 0;
        return NULL;
    }
    //*/

    bit_3sat* a1;
    bit_3sat* a2;
    bit_3sat* a3;
    bit_3sat* o1;

    __int64 a1_len = 0;
    char* a1_str = and_3sat(num_para, &a1, a, b, &a1_len);
    __int64 a2_len = 0;
    char* a2_str = and_3sat(num_para, &a2, a, c_in, &a2_len);
    __int64 a3_len = 0;
    char* a3_str = and_3sat(num_para, &a3, b, c_in, &a3_len);
    __int64 o1_len = 0;
    char* o1_str = or_3sat(num_para, &o1, a1, a2, &o1_len);
    __int64 o2_len = 0;
    char* o2_str = or_3sat(num_para, c_out, a3, o1, &o2_len);

    __int64 ret_len = a1_len + a2_len + a3_len + o1_len + o2_len + 1;
    char* ret = new char[ret_len];

    __int64 pos = 0;

    if (a1_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, a1_str);
        pos += a1_len;
    }
    if (a2_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, a2_str);
        pos += a2_len;
    }
    if (a3_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, a3_str);
        pos += a3_len;
    }
    if (o1_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, o1_str);
        pos += o1_len;
    }
    if (o2_len > 0) {
        strcpy_s(&(ret[pos]), ret_len - pos, o2_str);
        pos += o2_len;
    }

    *len_para = pos;

    delete a1;
    delete a2;
    delete a3;
    delete o1;

    delete[] a1_str;
    delete[] a2_str;
    delete[] a3_str;
    delete[] o1_str;
    delete[] o2_str;

    return ret;
}

// implies = a->b: (~a or b)

char* implies_3sat(__int64* num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64* len_para) {

    __int64 pos = 0;

    // invert a

    __int64 not_len = 0;

    bit_3sat* a_inv = NULL;
    char* not_a_str = not_3sat(num_para, &a_inv, a, &not_len);

    // or with b

    __int64 or_len = 0;
    char* or_str = or_3sat(num_para, c, a_inv, b, &or_len);

    __int64 ret_len = not_len + or_len + 1;

    char* ret = new char[ret_len];

    strcpy_s(&(ret[pos]), ret_len - pos, not_a_str);
    pos += not_len;
    strcpy_s(&(ret[pos]), ret_len - pos, or_str);
    pos += or_len;

    *len_para = pos;

    delete not_a_str;
    delete or_str;

    return ret;

}



// dec_add c = a + b

char* dec_add(__int64 * num_para, dec_3sat** c, dec_3sat* a, dec_3sat* b, bool discard_final_c_out, __int64 *len_para) {

    // find the largest number of bits

    __int64 bd_bits = a->bd_sz > b->bd_sz ? a->bd_sz : b->bd_sz ;
    __int64 ad_bits = a->ad_sz > b->ad_sz ? a->ad_sz : b->ad_sz ;
    __int64 tt_bits = bd_bits + ad_bits;

    // create modified a and b

    dec_3sat* a_mod = new dec_3sat();
    dec_3sat* b_mod = new dec_3sat();

    a_mod->ad_sz = ad_bits;
    a_mod->bd_sz = bd_bits;
    a_mod->sz = tt_bits;

    b_mod->ad_sz = ad_bits;
    b_mod->bd_sz = bd_bits;
    b_mod->sz = tt_bits;

    a_mod->bits = new bit_3sat * [tt_bits];
    b_mod->bits = new bit_3sat * [tt_bits];

    // populate a_mod and b_mod

    __int64 a_ad_extra = ad_bits > a->ad_sz ? ad_bits - a->ad_sz : 0;
    __int64 a_bd_extra = bd_bits > a->bd_sz ? bd_bits - a->bd_sz : 0;

    __int64 b_ad_extra = ad_bits > b->ad_sz ? ad_bits - b->ad_sz : 0;
    __int64 b_bd_extra = bd_bits > b->bd_sz ? bd_bits - b->bd_sz : 0;

    // sign-extend for bd

    __int64 a_mod_start = 0;
    __int64 b_mod_start = 0;

    for (a_mod_start = 0; a_mod_start < a_bd_extra; a_mod_start++) {
        a_mod->bits[a_mod_start] = new bit_3sat();
        a_mod->bits[a_mod_start]->id = a->bits[0]->id;
    }

    for (b_mod_start = 0; b_mod_start < b_bd_extra; b_mod_start++) {
        b_mod->bits[b_mod_start] = new bit_3sat();
        b_mod->bits[b_mod_start]->id = b->bits[0]->id;
    }

    // copy over bits

    for (__int64 i = 0; i < a->sz; i++) {
        a_mod->bits[a_mod_start + i] = new bit_3sat();
        a_mod->bits[a_mod_start + i]->id = a->bits[i]->id;
    }

    for (__int64 i = 0; i < b->sz; i++) {
        b_mod->bits[b_mod_start + i] = new bit_3sat();
        b_mod->bits[b_mod_start + i]->id = b->bits[i]->id;
    }

    // rpad with 0s

    __int64 a_rpad = a_mod->ad_sz > a->ad_sz ? a_mod->ad_sz - a->ad_sz : 0;
    __int64 b_rpad = b_mod->ad_sz > b->ad_sz ? b_mod->ad_sz - b->ad_sz : 0;

    for (__int64 i = 0; i < a_rpad; i++) {
        a_mod->bits[a_mod_start + a->sz + i] = new bit_3sat();
        a_mod->bits[a_mod_start + a->sz + i]->id = FALSE_3SAT;
    }

    for (__int64 i = 0; i < b_rpad; i++) {
        b_mod->bits[b_mod_start + b->sz + i] = new bit_3sat();
        b_mod->bits[b_mod_start + b->sz + i]->id = FALSE_3SAT;
    }

    // instantiate c

    *c = new dec_3sat();
    (*c)->bd_sz = bd_bits;
    (*c)->ad_sz = ad_bits;
    (*c)->sz = tt_bits + (discard_final_c_out ? 0 : 1);
    (*c)->bits = new bit_3sat * [(*c)->sz];

    bit_3sat* c_in = new bit_3sat();
    c_in->id = FALSE_3SAT;
    bit_3sat* c_out = NULL;

    // create sum_str and c_out_str buffers

    __int64* sum_str_len = new __int64[tt_bits];
    char** sum_strs = new char* [tt_bits];
    for (__int64 i = 0; i < tt_bits; i++)
        sum_strs[i] = new char[256];

    __int64* cout_str_len = new __int64[tt_bits];
    char** cout_strs = new char* [tt_bits];
    for (__int64 i = 0; i < tt_bits; i++)
        cout_strs[i] = new char[512];

    // perform the first addition with a zero for carry_in

    char* sum_str = bitaddsum_3sat(num_para, &((*c)->bits [(*c)->sz - 1]), c_in, a_mod->bits[a_mod->sz - 1], b_mod->bits[b_mod->sz - 1], &(sum_str_len[tt_bits - 1]));
    if (sum_str_len[tt_bits - 1] > 0)
        strcpy_s(sum_strs[tt_bits - 1], 256, sum_str);

    char* c_out_str = bitaddcout_3sat(num_para, &c_out, c_in, a_mod->bits[a_mod->sz - 1], b_mod->bits[b_mod->sz - 1], &(cout_str_len[tt_bits - 1]));
    if (cout_str_len[tt_bits - 1] > 0)
        strcpy_s(cout_strs[tt_bits - 1], 512, c_out_str);

    delete c_in;
    c_in = c_out;

    delete [] sum_str;
    delete [] c_out_str;

    // perform all the rest of the additions

    for (__int64 i = 1; i < tt_bits; i++) {

        char* sum_str = bitaddsum_3sat(num_para, &((*c)->bits[(*c)->sz - i - 1]), c_in, a_mod->bits[a_mod->sz - i - 1], b_mod->bits[b_mod->sz - i - 1],
            &(sum_str_len[tt_bits - i - 1]));
        if (sum_str_len[tt_bits - i - 1] > 0)
            strcpy_s(sum_strs[tt_bits - i - 1], 256, sum_str);

        char* c_out_str = bitaddcout_3sat(num_para, &c_out, c_in, a_mod->bits[a_mod->sz - i - 1], b_mod->bits[b_mod->sz - i - 1], &(cout_str_len[tt_bits - i - 1]));
        if (cout_str_len[tt_bits - i - 1] > 0)
            strcpy_s(cout_strs[tt_bits - i - 1], 512, c_out_str);

        delete c_in;
        c_in = c_out;

        delete [] sum_str;
        delete [] c_out_str;
    }

    // set the msb of c as the final carry_out value

    if (discard_final_c_out)
        delete c_in;
    else
        (*c)->bits[0] = c_in;

    // create ret string

    __int64 ret_sz = 1;
    for (__int64 i = 0; i < tt_bits; i++) {
        ret_sz += sum_str_len[i];
        ret_sz += cout_str_len[i];
    }

    char* ret = NULL;
    if ( ret_sz > 0 )
        ret = new char[ret_sz];

    // populate ret string

    __int64 pos = 0;

    for (__int64 i = 0; i < tt_bits; i++) {
        if (sum_strs[i] != NULL) {
            strcpy_s(&(ret[pos]), ret_sz - pos, sum_strs[i]);
            pos += sum_str_len[i];
        }
        if (cout_strs[i] != NULL) {
            strcpy_s(&(ret[pos]), ret_sz - pos, cout_strs[i]);
            pos += cout_str_len[i];
        }
    }

    *len_para = pos;

    // clean up

    for (__int64 i = 0; i < tt_bits; i++) {
        if ( sum_strs [i] != NULL)
            delete [] sum_strs[i];
        if ( cout_strs [i] != NULL)
            delete [] cout_strs[i];
    }
    delete [] sum_strs;
    delete [] cout_strs;
    delete [] sum_str_len;
    delete [] cout_str_len;

    // return the buffer of 3-CNF

    return ret;

}

char* nat_add(__int64 * num_para, nat_3sat** c, nat_3sat* a, nat_3sat* b, bool discard_final_c_out, __int64 * len_para) {

    // find the largest number of bits

    __int64 tt_bits = a->sz > b->sz ? a->sz : b->sz;

    // create modified a and b

    nat_3sat* a_mod = new nat_3sat();
    nat_3sat* b_mod = new nat_3sat();

    a_mod->sz = tt_bits;

    b_mod->sz = tt_bits;

    a_mod->bits = new bit_3sat * [tt_bits];
    b_mod->bits = new bit_3sat * [tt_bits];

    // populate a_mod and b_mod

    __int64 a_extra = tt_bits - a->sz;
    __int64 b_extra = tt_bits - b->sz;

    // sign-extend for bd

    __int64 a_mod_start = 0;
    __int64 b_mod_start = 0;

    for (a_mod_start = 0; a_mod_start < a_extra; a_mod_start++) {
        a_mod->bits[a_mod_start] = new bit_3sat();
        a_mod->bits[a_mod_start]->id = FALSE_3SAT;
    }

    for (b_mod_start = 0; b_mod_start < b_extra; b_mod_start++) {
        b_mod->bits[b_mod_start] = new bit_3sat();
        b_mod->bits[b_mod_start]->id = FALSE_3SAT;
    }

    // copy over bits

    for (__int64 i = 0; i < a->sz; i++) {
        a_mod->bits[a_mod_start + i] = new bit_3sat();
        a_mod->bits[a_mod_start + i]->id = a->bits[i]->id;
    }

    for (__int64 i = 0; i < b->sz; i++) {
        b_mod->bits[b_mod_start + i] = new bit_3sat();
        b_mod->bits[b_mod_start + i]->id = b->bits[i]->id;
    }

    // instantiate c

    *c = new nat_3sat();
    (*c)->sz = tt_bits + (discard_final_c_out ? 0 : 1);
    (*c)->bits = new bit_3sat * [(*c)->sz];

    bit_3sat* c_in = new bit_3sat();
    c_in->id = FALSE_3SAT;
    bit_3sat* c_out = NULL;

    // create sum_str and c_out_str buffers

    __int64* sum_str_len = new __int64[tt_bits];
    char** sum_strs = new char* [tt_bits];
    for (__int64 i = 0; i < tt_bits; i++)
        sum_strs[i] = new char[256];

    __int64* cout_str_len = new __int64[tt_bits];
    char** cout_strs = new char* [tt_bits];
    for (__int64 i = 0; i < tt_bits; i++)
        cout_strs[i] = new char[512];

    // perform the first addition with a zero for carry_in

    char* sum_str = bitaddsum_3sat(num_para, &((*c)->bits[(*c)->sz - 1]), c_in, a_mod->bits[a_mod->sz - 1], b_mod->bits[b_mod->sz - 1], &(sum_str_len[tt_bits - 1]));
    if (sum_str_len[tt_bits - 1] > 0 )
        strcpy_s(sum_strs[tt_bits - 1], 256, sum_str);

    char* c_out_str = bitaddcout_3sat(num_para, &c_out, c_in, a_mod->bits[a_mod->sz - 1], b_mod->bits[b_mod->sz - 1], &(cout_str_len[tt_bits - 1]));
    if ( cout_str_len[tt_bits - 1] > 0 )
        strcpy_s(cout_strs[tt_bits - 1], 512, c_out_str);
    delete c_in;
    c_in = c_out;

    if ( sum_str != NULL)
        delete [] sum_str;
    if ( c_out_str != NULL)
        delete [] c_out_str;

    // perform all the rest of the additions

    for (__int64 i = 1; i < tt_bits; i++) {

        char* sum_str = bitaddsum_3sat(num_para, &((*c)->bits[(*c)->sz - i - 1]), c_in, a_mod->bits[a_mod->sz - i - 1], b_mod->bits[b_mod->sz - i - 1],
            &(sum_str_len[tt_bits - i - 1]));
        if (sum_str_len[tt_bits - 1 - i] > 0)
            strcpy_s(sum_strs[tt_bits - 1 - i], 256, sum_str);

        char* c_out_str = bitaddcout_3sat(num_para, &c_out, c_in, a_mod->bits[a_mod->sz - i - 1], b_mod->bits[b_mod->sz - i - 1], &(cout_str_len[tt_bits - i - 1]));
        if (cout_str_len[tt_bits - 1 - i] > 0)
            strcpy_s(cout_strs[tt_bits - 1 - i], 512, c_out_str);

        delete c_in;
        c_in = c_out;

        if ( sum_str != NULL )
            delete [] sum_str;
        if ( c_out_str != NULL)
            delete [] c_out_str;
    }

    // set the msb of c as the final carry_out value

    if (discard_final_c_out)
        delete c_in;
    else
        (*c)->bits[0] = c_in;

    // create ret string

    __int64 ret_sz = 1;
    for (__int64 i = 0; i < tt_bits; i++) {
        ret_sz += sum_str_len[i];
        ret_sz += cout_str_len[i];
    }

    char* ret = NULL;
    if ( ret_sz > 0 )
        ret = new char[ret_sz];

    // populate ret string

    __int64 pos = 0;

    for (__int64 i = 0; i < tt_bits; i++) {
        if (sum_str_len[i] > 0) {
            strcpy_s(&(ret[pos]), ret_sz - pos, sum_strs[i]);
            pos += sum_str_len[i];
        }
        if (cout_str_len[i] > 0) {
            strcpy_s(&(ret[pos]), ret_sz - pos, cout_strs[i]);
            pos += cout_str_len[i];
        }
    }

    *len_para = pos;

    // clean up

    for (__int64 i = 0; i < tt_bits; i++) {
        if ( sum_strs [i] != NULL)
            delete [] sum_strs[i];
        if ( cout_strs [i] != NULL)
            delete [] cout_strs[i];
    }
    delete [] sum_strs;
    delete [] cout_strs;
    delete [] sum_str_len;
    delete [] cout_str_len;

    // return the buffer of 3-CNF

    return ret;

}

// twos_complement = -a

char* twos_comp_3sat(__int64 * num_para, dec_3sat** c, dec_3sat* a, __int64 *len_para) {

    // create not_3sat_strs and not_3sat_len

    char** not_3sat_strs = new char *[a->sz];
    for (__int64 i = 0; i < a->sz; i++)
        not_3sat_strs[i] = new char[64];

    __int64* not_3sat_len = new __int64[a->sz];
    for (__int64 i = 0; i < a->sz; i++)
        not_3sat_len[i] = 0;

    // invert a

    dec_3sat* a_inv = new dec_3sat();
    a_inv->sz = a->sz;
    a_inv->ad_sz = a->ad_sz;
    a_inv->bd_sz = a->bd_sz;
    a_inv->bits = new bit_3sat * [a_inv->sz]();
    for (__int64 i = 0; i < a_inv->sz; i++) {
        char * not_str = not_3sat(num_para, &a_inv->bits[i], a->bits[i], &(not_3sat_len[i]));
        strcpy_s(not_3sat_strs[i], 64, not_str);
    }

    // create one, adjusting for any fractions

    dec_3sat* b = new dec_3sat();
    b->sz = 2;
    b->bd_sz = 2;
    b->ad_sz = 0;
    b->bits = new bit_3sat * [2];
    b->bits[0] = new bit_3sat();
    b->bits[0]->id = FALSE_3SAT;
    b->bits[1] = new bit_3sat();
    b->bits[1]->id = TRUE_3SAT;

    // perform the addition discarding the final carry

    __int64 add_str_len = 0;

    char* add_str = dec_add(num_para, c, b, a_inv, true, &add_str_len);

    // calculate size of ret string and copy over all data

    __int64 ret_sz = 0;

    for (__int64 i = 0; i < a->sz; i++)
        ret_sz += not_3sat_len[i];
    ret_sz += add_str_len;

    char* ret = new char[ret_sz];

    __int64 pos = 0;

    for (__int64 i = 0; i < a->sz; i++) {
        strcpy_s(&(ret[pos]), not_3sat_len[i], not_3sat_strs[i]);
        pos += not_3sat_len[i];
    }
    strcpy_s(&(ret[pos]), add_str_len, add_str);
    pos += add_str_len;

    ret[pos] = '\0';

    // clean up

    for (__int64 i = 0; i < a->sz; i++)
        delete not_3sat_strs[i];
    delete [] not_3sat_strs;

    delete add_str;
    delete [] not_3sat_len;

    *len_para = pos;

    // return 3sat source

    return ret;
}

// dec_sub c = a + -b

char* dec_sub(__int64 * num_para, dec_3sat** c, dec_3sat* a, dec_3sat* b, __int64 * len_para) {

    __int64 buf_sz = (64 + 768) * b->sz + 768 * (a->sz > b->sz ? a->sz : b->sz);
    char* ret = new char[buf_sz];
    __int64 pos = 0;

    dec_3sat* twos_b = NULL;

    __int64 twos_str_len = 0;
    char* twos_str = twos_comp_3sat(num_para, &twos_b, b, &twos_str_len);
    strcpy_s(&(ret[pos]), twos_str_len, twos_str);
    pos += twos_str_len;

    __int64 add_str_len = 0;
    char* add_str = dec_add(num_para, c, a, twos_b, false, &add_str_len);
    strcpy_s(&(ret[pos]), add_str_len, add_str);
    pos += add_str_len;

    *len_para = twos_str_len + add_str_len;

    delete twos_str;
    delete add_str;

    return ret;

}

// force equals

char* dec_equals(__int64 * num_para, dec_3sat* a, dec_3sat* b, bool eq, __int64* len_para) {

    // c = a xnor b
    // d = and all c bits together
    // d must be equal to T

    // create modified a and b for comparison purposes

    dec_3sat* a_mod;
    dec_3sat* b_mod;

    __int64 bd_sz = a->bd_sz < b->bd_sz ? a->bd_sz : b->bd_sz;
    __int64 a_start = bd_sz < a->bd_sz ? a->bd_sz - bd_sz : a->bd_sz;
    __int64 b_start = bd_sz < b->bd_sz ? b->bd_sz - bd_sz : b->bd_sz;

    // sign-extend, if necessary

    __int64 ad_sz = a->ad_sz > b->ad_sz ? a->ad_sz : b->bd_sz;
    __int64 a_sign_ext = ad_sz - a->ad_sz;
    __int64 b_sign_ext = ad_sz - b->ad_sz;

    // instantiate a_mod and b_mod
    a_mod = new dec_3sat();
    a_mod->ad_sz = ad_sz;
    a_mod->bd_sz = bd_sz;
    a_mod->sz = ad_sz + bd_sz;
    a_mod->bits = new bit_3sat * [ad_sz + bd_sz];

    b_mod = new dec_3sat();
    b_mod->ad_sz = ad_sz;
    b_mod->bd_sz = bd_sz;
    b_mod->sz = ad_sz + bd_sz;
    b_mod->bits = new bit_3sat * [ad_sz + bd_sz];

    // sign-extend a
    for (__int64 i = 0; i < a_start; i++) {
        a_mod->bits[i] = new bit_3sat();
        a_mod->bits[i]->id = a->bits[0]->id;
    }

    // sign-extend b
    for (__int64 i = 0; i < b_start; i++) {
        b_mod->bits[i] = new bit_3sat();
        b_mod->bits[i]->id = b->bits[0]->id;
    }

    // copy over data for a
    for (__int64 i = a_start; i < a->sz - a_sign_ext; i++) {
        a_mod->bits[i] = new bit_3sat();
        a_mod->bits[i]->id = a->bits[i]->id;
    }

    // copy over data for b
    for (__int64 i = b_start; i < b->sz - b_sign_ext; i++) {
        b_mod->bits[i] = new bit_3sat();
        b_mod->bits[i]->id = b->bits[i]->id;
    }

    // rpad a with 0s out to length
    for (__int64 i = a->sz - a_sign_ext; i < bd_sz + ad_sz; i++) {
        a_mod->bits[i] = new bit_3sat();
        a_mod->bits[i]->id = FALSE_3SAT;
    }

    // rpad b with 0s out to length
    for (__int64 i = b->sz - b_sign_ext; i < bd_sz + ad_sz; i++) {
        b_mod->bits[i] = new bit_3sat();
        b_mod->bits[i]->id = FALSE_3SAT;
    }

    // create return buffer

    __int64 buf_sz = (bd_sz + ad_sz) * (128 + 128) + 128;
    char* ret = new char[buf_sz];
    __int64 pos = 0;

    // xnor them all together

    __int64* xnor_str_len = new __int64[bd_sz + ad_sz];
    for (__int64 i = 0; i < bd_sz + ad_sz; i++)
        xnor_str_len[i] = 0;

    dec_3sat* c = create_dec(num_para, bd_sz, ad_sz);
    for (__int64 i = 0; i < bd_sz + ad_sz; i++) {
        char* xnor_str = xnor_3sat(num_para, &c->bits[i], a_mod->bits[i], b_mod->bits[i], & (xnor_str_len[i]));
        strcpy_s(&(ret[pos]), buf_sz - pos, xnor_str);
        pos += xnor_str_len[i];
    }

    // and all those xnors together

    __int64* and_str_len = new __int64[c->sz];
    for (__int64 i = 0; i < c->sz; i++)
        and_str_len[i] = 0;

    __int64 id = c->bits[0]->id;

    bit_3sat* d = new bit_3sat();
    d->id = id;

    for (__int64 i = 1; i < c->sz; i++) {
        bit_3sat* temp = NULL;
        char* and_str = and_3sat(num_para, &temp, d, c->bits[i], &(and_str_len[i]));
        strcpy_s(&(ret[pos]), buf_sz - pos, and_str);
        pos += and_str_len[i];
        bit_3sat* dump = d;
        d = temp;
        delete dump;
    }

    // verify that d xnors with eq (set to either true or false)

    __int64 final_xnor_str_len = 0;

    bit_3sat* eq_bit = new bit_3sat();
    eq_bit->id = eq ? TRUE_3SAT : FALSE_3SAT;

    bit_3sat* f = NULL;
    char* final_xnor_str = xnor_3sat(num_para, &f, d, eq_bit, &final_xnor_str_len);
    strcpy_s(&(ret[pos]), buf_sz - pos, final_xnor_str);
    pos += final_xnor_str_len;

    *len_para = pos;

    delete final_xnor_str;
    delete [] xnor_str_len;
    delete [] and_str_len;

    return ret;

}

char* nat_equals(__int64 * num_para, nat_3sat* a, nat_3sat* b, bool eq, __int64* len_para) {

    // c = a xnor b
    // d = and all c bits together
    // d must be equal to T
    
    // create modified a and b for comparison purposes

    __int64 num_sz = a->sz > b->sz ? a->sz : b->sz;

    nat_3sat* a_mod = new nat_3sat ();
    nat_3sat* b_mod = new nat_3sat ();
    a_mod->sz = num_sz;
    b_mod->sz = num_sz;
    a_mod->bits = new bit_3sat * [num_sz];
    b_mod->bits = new bit_3sat * [num_sz];

    for (__int64 i = 0; i < num_sz; i++) {
        a_mod->bits[i] = new bit_3sat();
        b_mod->bits[i] = new bit_3sat();
        a_mod->bits[i]->id = FALSE_3SAT;
        b_mod->bits[i]->id = FALSE_3SAT;
    }

    for (int i = 0; i < a->sz; i++)
        a_mod->bits[a_mod->sz - 1 - i]->id = a->bits[a->sz - 1 - i]->id;

    for (int i = 0; i < b->sz; i++)
        b_mod->bits[b_mod->sz - 1 - i]->id = b->bits[b->sz - 1 - i]->id;

    // create return buffer

    __int64 buf_sz = (num_sz) * (128 + 128) + 128;
    char* ret = new char[buf_sz];
    __int64 pos = 0;

    // xnor them all together

    __int64 xnor_str_len = 0;

    nat_3sat* c = new nat_3sat();
    c->sz = num_sz ;
    c->bits = new bit_3sat * [num_sz];

    for (__int64 i = 0; i < num_sz; i++) {
        char* xnor_str = xnor_3sat(num_para, &(c->bits[i]), a_mod->bits[i], b_mod->bits[i], &xnor_str_len);
        if (xnor_str_len > 0) {
            strcpy_s(&(ret[pos]), buf_sz - pos, xnor_str);
            pos += xnor_str_len;
            delete[] xnor_str;
        }
    }

    // and all those xnors together

    __int64 and_str_len = 0;

    bit_3sat* d = new bit_3sat();
    d->id = c->bits[0]->id;

    for (__int64 i = 1; i < c->sz; i++) {
        bit_3sat* temp = NULL;
        char* and_str = and_3sat(num_para, &temp, d, c->bits[i], &and_str_len);
        if (and_str_len > 0) {
            strcpy_s(&(ret[pos]), buf_sz - pos, and_str);
            pos += and_str_len;
            delete[] and_str;
        }
        bit_3sat* dump = d;
        d = temp;
        delete dump;

    }

    // verify that d xnors with eq (set to either true or false)

    __int64 final_xnor_str_len = 0;

    bit_3sat* eq_bit = new bit_3sat();
    eq_bit->id = eq ? TRUE_3SAT : FALSE_3SAT;

    bit_3sat* f = NULL;
    char* final_xnor_str = xnor_3sat(num_para, &f, d, eq_bit, &final_xnor_str_len);

    if (final_xnor_str_len > 0) {
        strcpy_s(&(ret[pos]), buf_sz - pos, final_xnor_str);
        pos += final_xnor_str_len;
    }

    *len_para = pos;

    if ( final_xnor_str != NULL)
        delete final_xnor_str;
    delete eq_bit;
    delete f;

    return ret;

}

// force not equals

char* dec_not_equals(__int64 * num_para, dec_3sat* a, dec_3sat* b, __int64 *len_para) {

    return dec_equals(num_para, a, b, false, len_para);

}

char* nat_not_equals(__int64 * num_para, nat_3sat* a, nat_3sat* b, __int64* len_para) {

    return nat_equals(num_para, a, b, false, len_para);

}

// dec_mul c = a * b

char* dec_mul(__int64 * num_para, dec_3sat** c, dec_3sat* a, dec_3sat* b, __int64 bd_sz, __int64 ad_sz, __int64 *len_para) {

    // create the buffers to collect the initial 3CNF

    char*** and_strs = new char** [b->sz];
    for (__int64 i = 0; i < b->sz; i++)
        and_strs[i] = new char* [a->sz];

    __int64** and_str_itmd_ab = new __int64* [b->sz];
    for (__int64 i = 0; i < b->sz; i++) {
        and_str_itmd_ab[i] = new __int64[a->sz];
        for (__int64 j = 0; j < a->sz; j++)
            and_str_itmd_ab[i][j] = 0;
    }

    // create the first __int64ermediate number

    dec_3sat* itmd_c = new dec_3sat();
    itmd_c->bits = new bit_3sat * [bd_sz + ad_sz];
    itmd_c->sz = bd_sz + ad_sz;
    itmd_c->bd_sz = bd_sz;
    itmd_c->ad_sz = ad_sz;

    for (__int64 j = 0; j < a->sz; j++) {
        char * and_str = and_3sat(num_para, &itmd_c->bits[itmd_c->sz - j - 1], a->bits[a->sz - j - 1], b->bits[b->sz - 1], &(and_str_itmd_ab[0][j]));
        and_strs[0][j] = new char[and_str_itmd_ab[0][j] + 1];
        strcpy_s(and_strs[0][j], and_str_itmd_ab[0][j] + 1, and_str);
        delete[] and_str;
    }

    for (__int64 j = 0; j < itmd_c->sz - a->sz; j++) {
        itmd_c->bits[j] = new bit_3sat();
        itmd_c->bits[j]->id = FALSE_3SAT;
    }

    // create the sum strings and their lengths

    char** sum_strs = new char* [b->sz];

    __int64* sum_str_len = new __int64[b->sz];
    for (__int64 i = 0; i < b->sz; i++)
        sum_str_len[i] = 0;

    sum_strs[0] = new char[2];
    strcpy_s(sum_strs[0], 2, "");
    sum_str_len[0] = (__int64)strnlen_s(sum_strs[0], 2);

    for (__int64 i = 1; i < b->sz; i++) {
        dec_3sat* itmd_b = itmd_c;
        dec_3sat* itmd_a = new dec_3sat();
        itmd_a->bits = new bit_3sat * [ad_sz + bd_sz];
        itmd_a->sz = ad_sz + bd_sz;
        itmd_a->bd_sz = bd_sz;
        itmd_a->ad_sz = ad_sz;
        for (__int64 j = 0; j < i; j++) {
            itmd_a->bits[itmd_a->sz - j - 1] = new bit_3sat();
            itmd_a->bits[itmd_a->sz - j - 1]->id = FALSE_3SAT;
        }
        __int64 limit = a->sz + i; // < bd_sz + ad_sz ? a->sz + i : bd_sz + ad_sz;
        for (__int64 j = i; j < limit; j++) {
            char* and_str = and_3sat(num_para, &itmd_a->bits[itmd_a->sz - j - 1], a->bits[a->sz - 1 - (j - i)],
                b->bits[b->sz - i - 1], &(and_str_itmd_ab[i][j-i]));
            and_strs[i][j - i] = new char[and_str_itmd_ab[i][j - i] + 1];
            strcpy_s(and_strs [i][j-i], and_str_itmd_ab[i][j - i] + 1, and_str);
            delete[] and_str;
        }
        for (__int64 j = 0; j < itmd_a->sz - limit; j++) {
            itmd_a->bits[j] = new bit_3sat();
            itmd_a->bits[j]->id = FALSE_3SAT;
        }
        char* sum_str = dec_add(num_para, &itmd_c, itmd_a, itmd_b, true, &(sum_str_len[i]));
        sum_strs[i] = new char[sum_str_len[i] + 1];
        strcpy_s(sum_strs [i], sum_str_len[i] + 1, sum_str);
        for (__int64 i = 0; i < itmd_a->sz; i++)
            delete itmd_a->bits[i];
        delete itmd_a;
        delete sum_str;
    }

    // copy itmd_c to c truncated according to bd_sz and ad_sz

    *c = create_dec(num_para, bd_sz, ad_sz);

    // if c->bd_sz > itmd_c->bd_sz, sign-extend itmd_c to size of c before decimal

    __int64 c_start = 0;
    __int64 itmd_c_start = 0;

    if ((*c)->bd_sz > itmd_c->bd_sz) {
        for (__int64 i = 0; i < (*c)->bd_sz - itmd_c->bd_sz; i++) {
            (*c)->bits[i] = new bit_3sat();
            (*c)->bits[i]->id = itmd_c->bits[0]->id;
        }
        c_start = (*c)->bd_sz - itmd_c->bd_sz;
    }
    else
        itmd_c_start = itmd_c->bd_sz - (*c)->bd_sz;

    // calculate number of bits to copy over

    __int64 copy_over = (*c)->sz > itmd_c->sz ? itmd_c->sz : (*c)->sz;

    // copy over that many bits

    for (__int64 i = 0; i < copy_over; i++) {
        (*c)->bits[c_start + i] = new bit_3sat();
        (*c)->bits[c_start + i]->id = itmd_c->bits[itmd_c_start + i]->id;
    }

    // calculate rpad

    __int64 c_rpad = (*c)->ad_sz > itmd_c->ad_sz ? (*c)->ad_sz - itmd_c->ad_sz : 0;

    // rpad with false

    for (__int64 i = 0; i < c_rpad; i++) {
        (*c)->bits[c_start + copy_over + i] = new bit_3sat();
        (*c)->bits[c_start + copy_over + i]->id = FALSE_3SAT;
    }
    

    // create the return buffer and populate with 3CNF

    __int64 buf_sz = 1;

    for (__int64 i = 0; i < b->sz; i++) {
        buf_sz += sum_str_len[i];
        for (__int64 j = 0; j < a->sz; j++)
            buf_sz += and_str_itmd_ab[i][j];
    }

    char* ret = new char[buf_sz];
    __int64 pos = 0;

    for (__int64 i = 0; i < b->sz; i++) {
        if (sum_str_len[i] > 0) {
            strcpy_s(&(ret[pos]), buf_sz - pos, sum_strs[i]);
            pos += sum_str_len[i];
        }
        for (__int64 j = 0; j < a->sz; j++) {
            if (and_str_itmd_ab[i][j] > 0) {
                strcpy_s(&(ret[pos]), buf_sz - pos, and_strs[i][j]);
                pos += and_str_itmd_ab[i][j];
            }
        }
    }


    // delete all work buffers

    for (__int64 i = 0; i < b->sz; i++) {
        for (__int64 j = 0; j < a->sz; j++)
            if (and_str_itmd_ab[i][j] > 0)
                delete and_strs[i][j];
            delete and_strs[i];
    }
    for (__int64 i = 1; i < b->sz; i++)
        if ( sum_str_len[i] > 0)
            delete sum_strs[i];
    delete [] sum_strs;
    delete [] and_strs;
    for (__int64 i = 0; i < b->sz; i++)
        delete and_str_itmd_ab[i];
    delete [] and_str_itmd_ab;
    delete [] sum_str_len;

    *len_para = pos;

    return ret;

}

///*
char* nat_mul(__int64 * num_para, nat_3sat** c, nat_3sat* a, nat_3sat* b, __int64 sz, __int64* len_para) {

    // create the buffers to collect the initial 3CNF

    char*** and_strs = new char** [b->sz];
    for (__int64 i = 0; i < b->sz; i++)
        and_strs[i] = new char* [a->sz];

    __int64** and_str_itmd_ab = new __int64* [b->sz];
    for (__int64 i = 0; i < b->sz; i++) {
        and_str_itmd_ab[i] = new __int64[a->sz];
        for (__int64 j = 0; j < a->sz; j++)
            and_str_itmd_ab[i][j] = 0;
    }

    // create the first intermediate number

    nat_3sat* itmd_c = new nat_3sat();
    itmd_c->bits = new bit_3sat * [a->sz + b->sz];
    itmd_c->sz = a->sz + b->sz;

    for (__int64 j = 0; j < a->sz; j++) {
        char* and_str = and_3sat(num_para, &itmd_c->bits[itmd_c->sz - j - 1], a->bits[a->sz - j - 1], b->bits[b->sz - 1], &(and_str_itmd_ab[0][j]));
        and_strs[0][j] = new char[and_str_itmd_ab[0][j] + 1];
        if (and_str_itmd_ab[0][j] > 0) {
            strcpy_s(and_strs[0][j], and_str_itmd_ab[0][j] + 1, and_str);
            delete[] and_str;
        }
    }

    for (__int64 j = 0; j < itmd_c->sz - a->sz; j++) {
        itmd_c->bits[j] = new bit_3sat();
        itmd_c->bits[j]->id = FALSE_3SAT;
    }

    // create the sum strings and their lengths

    char** sum_strs = new char* [b->sz];

    __int64* sum_str_len = new __int64[b->sz];
    for (__int64 i = 0; i < b->sz; i++)
        sum_str_len[i] = 0;

    sum_strs[0] = new char[2];
    strcpy_s(sum_strs[0], 2, "");
    sum_str_len[0] = (__int64)strnlen_s(sum_strs[0], 2);

    //__int64 dummy;
    //printf_s("nat_mul: initial itmd_c: %s\n", nat_to_str(NULL, itmd_c, &dummy));

    for (__int64 i = 1; i < b->sz; i++) {
        nat_3sat* itmd_b = itmd_c;
        nat_3sat* itmd_a = new nat_3sat();
        itmd_a->bits = new bit_3sat * [a->sz + b->sz];
        itmd_a->sz = a->sz + b->sz;
        for (__int64 j = 0; j < i; j++) {
            itmd_a->bits[itmd_a->sz - j - 1] = new bit_3sat();
            itmd_a->bits[itmd_a->sz - j - 1]->id = FALSE_3SAT;
        }
        __int64 limit = a->sz + i;
        for (__int64 j = i; j < limit; j++) {
            char* and_str = and_3sat(num_para, &itmd_a->bits[itmd_a->sz - j - 1], a->bits[a->sz - 1 - (j - i)],
                b->bits[b->sz - i - 1], &(and_str_itmd_ab[i][j - i]));
            if (and_str_itmd_ab[i][j - i] > 0) {
                and_strs[i][j - i] = new char[and_str_itmd_ab[i][j - i] + 1];
                strcpy_s(and_strs[i][j - i], and_str_itmd_ab[i][j - i] + 1, and_str);
                delete[] and_str;
            }
        }
        for (__int64 j = 0; j < itmd_a->sz - limit; j++) {
            itmd_a->bits[j] = new bit_3sat();
            itmd_a->bits[j]->id = FALSE_3SAT;
        }
        char* sum_str = nat_add(num_para, &itmd_c, itmd_a, itmd_b, true, &(sum_str_len[i]));
        //__int64 dummy;
        //printf_s("nat_mul %lld: %s + %s = %s\n", i, nat_to_str ( NULL, itmd_a , &dummy ) , nat_to_str ( NULL, itmd_b, & dummy ) , nat_to_str ( NULL, itmd_c , & dummy) );
        if (sum_str_len[i] > 0) {
            sum_strs[i] = new char[sum_str_len[i] + 1];
            strcpy_s(sum_strs[i], sum_str_len[i] + 1, sum_str);
            delete sum_str;
        }
        for (__int64 i = 0; i < itmd_a->sz; i++)
            delete itmd_a->bits[i];
        delete itmd_a;
    }

    // copy itmd_c to c truncated according to bd_sz and ad_sz

    *c = new nat_3sat();
    (*c)->sz = sz;
    (*c)->bits = new bit_3sat* [sz];

    for (__int64 i = 0; i < sz; i++) {
        (*c)->bits[i] = new bit_3sat();
        (*c)->bits[i]->id = FALSE_3SAT;
    }

    __int64 c_start = (*c)->sz > itmd_c->sz ? (*c)->sz - itmd_c->sz : 0;
    __int64 itmd_c_start = (*c)->sz > itmd_c->sz ? 0 : itmd_c->sz - (*c)->sz;

    // calculate number of bits to copy over

    __int64 copy_over = (*c)->sz > itmd_c->sz ? itmd_c->sz : (*c)->sz;

    // copy over that many bits

    for (__int64 i = 0; i < copy_over; i++)
        (*c)->bits[c_start + i]->id = itmd_c->bits[itmd_c_start + i]->id;

    // create the return buffer and populate with 3CNF

    __int64 buf_sz = 1;

    for (__int64 i = 0; i < b->sz; i++) {
        buf_sz += sum_str_len[i];
        for (__int64 j = 0; j < a->sz; j++)
            buf_sz += and_str_itmd_ab[i][j];
    }

    char* ret = new char[buf_sz];
    __int64 pos = 0;

    for (__int64 i = 0; i < b->sz; i++) {
        if (sum_str_len[i] > 0) {
            strcpy_s(&(ret[pos]), buf_sz - pos, sum_strs[i]);
            pos += sum_str_len[i];
        }
        for (__int64 j = 0; j < a->sz; j++) {
            if (and_str_itmd_ab[i][j] > 0) {
                strcpy_s(&(ret[pos]), buf_sz - pos, and_strs[i][j]);
                pos += and_str_itmd_ab[i][j];
            }
        }
    }


    // delete all work buffers

    for (__int64 i = 0; i < b->sz; i++) {
        for (__int64 j = 0; j < a->sz; j++)
            if (and_str_itmd_ab[i][j] > 0)
                delete and_strs[i][j];
        delete [] and_strs[i];
    }
    for (__int64 i = 1; i < b->sz; i++)
        if (sum_str_len[i] > 0)
            delete [] sum_strs[i];
    delete [] sum_strs;
    delete [] and_strs;
    for (__int64 i = 0; i < b->sz; i++)
        delete and_str_itmd_ab[i];
    delete [] and_str_itmd_ab;
    delete [] sum_str_len;

    *len_para = pos;

    return ret;
}
//*/


// dec_div: b = c/a; a*b = c

char* dec_div(__int64 * num_para, dec_3sat* c, dec_3sat* a, dec_3sat** b, __int64* len_para) {

    // calculate sizes after decimal and before decimal

    __int64 ad_sz = (c->ad_sz > a->ad_sz ? c->ad_sz : a->ad_sz) + 1;
    __int64 bd_sz = c->bd_sz > a->bd_sz ? c->bd_sz - a->bd_sz : a->bd_sz - c->bd_sz;
    __int64 sz = ad_sz + bd_sz;

    *b = create_dec(num_para, bd_sz, ad_sz);

    __int64 dec_mul_str_len = 0;

    dec_3sat* itmd_c = create_dec(num_para, c->bd_sz, c->ad_sz);
    char* dec_mul_str = dec_mul(num_para, &itmd_c, a, *b, bd_sz, ad_sz, & dec_mul_str_len);

    __int64 equals_str_len = 0;

    char * equals_str = dec_equals(num_para, c, itmd_c, true, &equals_str_len);

    char* ret = new char[dec_mul_str_len + equals_str_len];
    strcpy_s(&(ret[0]), dec_mul_str_len, dec_mul_str);
    strcpy_s(&(ret[dec_mul_str_len]), equals_str_len, equals_str);

    *len_para = dec_mul_str_len + equals_str_len;

    delete dec_mul_str;
    delete equals_str;

    return ret;
}

// dec_sqrt: a = sqrt(c); a*a = c

char* dec_sqrt(__int64 * num_para, dec_3sat* c, dec_3sat** a, __int64 *len_para) {
    // calculate sizes after decimal and before decimal

    __int64 ad_sz = c->ad_sz + c->sz;
    __int64 bd_sz = c->bd_sz / 2 + c->bd_sz % 2;
    __int64 sz = ad_sz + bd_sz;

    *a = create_dec(num_para, bd_sz, ad_sz);

    __int64 dec_mul_str_len = 0;

    dec_3sat* itmd_c = create_dec(num_para, c->bd_sz, c->ad_sz);
    char* dec_mul_str = dec_mul(num_para, &itmd_c, *a, *a, bd_sz, ad_sz, &dec_mul_str_len);

    __int64 equals_str_len = 0;

    char * equals_str = dec_equals(num_para, c, itmd_c, true, & equals_str_len);

    char* ret = new char[dec_mul_str_len + equals_str_len];
    strcpy_s(&(ret[0]), dec_mul_str_len, dec_mul_str);
    strcpy_s(&(ret[dec_mul_str_len]), equals_str_len, equals_str);

    *len_para = dec_mul_str_len + equals_str_len;

    delete dec_mul_str;
    delete equals_str;

    return ret;
}

inline __int64 octal_from_char(char c) {

    switch (c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    default: return 0;
    }
}

inline __int64 int_from_hex_char(char c) {

    if (c >= 'A' && c <= 'F')
        c = c + ('a' - 'A');

    switch (c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    default: return 0;
    }
}

inline char hex_char_from_int(__int64 a) {
    switch (a) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'a';
    case 11: return 'b';
    case 12: return 'c';
    case 13: return 'd';
    case 14: return 'e';
    case 15: return 'f';
    default: return '0';
    }
}

inline __int64 decimal_from_char(char c) {

    switch (c) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    default: return 0;
    }
}

bool* hex2bool(char* a, __int64 sz) {

    // get length of a
    __int64 strln = 0;
    for (strln = 0; strln < sz && a[strln] != '\0'; strln++)
        ;

    __int64 inbuffer_sz = strln * 4;

    bool* inbuffer = new bool[inbuffer_sz];

    for (__int64 i = 0; i < strln; i++) {

        // decode the hex __int64o bits

        __int64 hexbits[4];
        hexbits[0] = hexbits[1] = hexbits[2] = hexbits[3] = 0;

        __int64 hexval = int_from_hex_char(a[i]);

        if (hexval >= 8) {
            hexbits[0] = 1;
            hexval = hexval - 8;
        }
        if (hexval >= 4) {
            hexbits[1] = 1;
            hexval = hexval - 4;
        }
        if (hexval >= 2) {
            hexbits[2] = 1;
            hexval = hexval - 2;
        }
        hexbits[3] = hexval;

        for (__int64 j = 0; j < 4; j++)
            inbuffer[i * 4 + j] = hexbits[j] == 1 ? true : false;
    }

    return inbuffer;
}

__int64** input_from_char_buf(char * buf_3sat, __int64 buf_3sat_sz, __int64 * k, bool cnf) {

    if (buf_3sat == NULL)
        return NULL;

    // count the newlines for k where no clause has a TRUE value in it

    __int64 pos = 0;

    while (buf_3sat[pos] != '\0') {

        char temp[128];

        __int64 t_pos = 0;

        while (buf_3sat[pos] != '\n') {
            temp[t_pos] = buf_3sat[pos];
            t_pos++;
            pos++;
        }
        temp[t_pos] = '\0';

        __int64 a;
        __int64 b;
        __int64 c;

        sscanf_s(temp, "%lld %lld %lld", &a, &b, &c);

        if ((cnf || (!cnf && a != TRUE_3SAT && b != TRUE_3SAT && c != TRUE_3SAT)) && 
            a != -b && a != -c && b != -c)
            (*k)++;

        pos++;
    }

    // copy over all the tuples

    __int64** ret = new __int64* [*k];

    pos = 0;

    __int64 iter = 0;

    while (buf_3sat[pos] != '\0') {

        char temp[128];

        __int64 t_pos = 0;

        while (buf_3sat[pos] != '\n') {
            temp[t_pos] = buf_3sat[pos];
            t_pos++;
            pos++;
        }
        temp[t_pos] = '\0';

        ret[iter] = new __int64[3];

        __int64 a;
        __int64 b;
        __int64 c;

        sscanf_s(temp, "%lld %lld %lld", &a, &b, &c);

        if ((cnf || (!cnf && a != TRUE_3SAT && b != TRUE_3SAT && c != TRUE_3SAT)) &&
            a != -b && a != -c && b != -c) {
            ret[iter][0] = a;
            ret[iter][1] = b;
            ret[iter][2] = c;
            iter++;
        }
        pos++;
    }

    return ret;

}

char* dec_to_str(bool * decodable_buf, dec_3sat * a, __int64 * str_sz) {

    // rtrim trailing zeros and calculate rpad out to a multiple of 4
    __int64 last_nonzero_offs = 0;
    while (last_nonzero_offs < a->ad_sz && a->bits[a->sz - last_nonzero_offs - 1]->id == FALSE_3SAT && 
        !decodable_buf [a->bits[a->sz - last_nonzero_offs - 1]->id - 2])
        last_nonzero_offs++;

    __int64 rpad = last_nonzero_offs < a->ad_sz - 1 ? 4 - (a->ad_sz - last_nonzero_offs - 1) % 4 : 0;

    // find first nonzero bit and calculate lpad out to a multiple of 4
    __int64 first_nonzero_offs = 0;
    while (first_nonzero_offs < a->bd_sz && a->bits[first_nonzero_offs]->id == FALSE_3SAT &&
        !decodable_buf[a->bits[first_nonzero_offs]->id - 2])
        first_nonzero_offs++;

    __int64 lpad = first_nonzero_offs < a->bd_sz ? (a->bd_sz - first_nonzero_offs) % 4 : 4;

    // calculate size of the buffer

    __int64 bool_buf_ad_sz = a->ad_sz - last_nonzero_offs + rpad ;
    __int64 bool_buf_bd_sz = a->bd_sz - first_nonzero_offs + lpad ;
    __int64 bool_buf_sz = bool_buf_ad_sz + bool_buf_bd_sz;

    // instantiate boolean buffer

    __int64* bool_buf = new __int64[bool_buf_sz];

    // lpad with zeros for multiple of 4
    for (__int64 i = 0; i < lpad; i++)
        bool_buf[i] = 0;

    // copy over boolean bits

    for (__int64 i = 0; i < a->sz - last_nonzero_offs - first_nonzero_offs; i++) {
        if (a->bits[first_nonzero_offs + i]->id == TRUE_3SAT)
            bool_buf[lpad + i] = 1;
        else if (a->bits[first_nonzero_offs + i]->id == FALSE_3SAT)
            bool_buf[lpad + i] = 0;
        else
            bool_buf[lpad + i] = decodable_buf[a->bits[first_nonzero_offs + i]->id - 2];
    }
    
    // rpad with 0s to make multiple of 4
    for (__int64 i = 0; i < rpad; i++)
        bool_buf[lpad + a->sz - last_nonzero_offs - first_nonzero_offs + i] = 0;

    __int64 num_hex_bd = bool_buf_bd_sz / 4;
    __int64 num_hex_ad = bool_buf_ad_sz / 4;

    * str_sz = num_hex_bd + num_hex_ad + (num_hex_ad == 0 ? 0 : 1) + 1;
    char* ret_str = new char[*str_sz];
    
    __int64 hexbits[4];

    // position in return string
    __int64 ret_str_pos = 0;

    // do the before-the-decimal part

    __int64 bd_hex_pos = 0;

    while (bd_hex_pos < num_hex_bd) {

        hexbits[0] = hexbits[1] = hexbits[2] = hexbits[3] = 0;

        for (__int64 i = 0; i < 4; i++)
            hexbits[3 - i] = bool_buf[bd_hex_pos * 4 + i];
        ret_str[ret_str_pos] = hex_char_from_int(hexbits[3] * 8 + hexbits[2] * 4 + hexbits[1] * 2 + hexbits[0]);
        
        bd_hex_pos++;
        ret_str_pos++;
    }

    // fill in decimal po__int64 if needed

    if (num_hex_ad > 0) {
        ret_str[ret_str_pos] = '.';
        ret_str_pos++;
    }

    // do the after-the-decimal part
    __int64 ad_hex_pos = 0;

    while (ad_hex_pos < num_hex_ad) {

        hexbits[0] = hexbits[1] = hexbits[2] = hexbits[3] = 0;

        for (__int64 i = 0; i < 4; i++)
            hexbits[3 - i] = bool_buf[num_hex_bd * 4 + ad_hex_pos * 4 + i];
        ret_str[ret_str_pos + ad_hex_pos] = hex_char_from_int(hexbits[3] * 8 + hexbits[2] * 4 + hexbits[1] * 2 + hexbits[0]);

        ad_hex_pos++;
        ret_str_pos++;
    }

    // null-terminate return string

    ret_str[ret_str_pos] = '\0';

    return ret_str;
}

char* nat_to_str(bool* decodable_buf, nat_3sat* a, __int64* str_sz) {

    bool * bool_buf = new bool[a->sz];

    for (__int64 i = 0; i < a->sz; i++)
        bool_buf[i] = false;

    for (__int64 i = 0; i < a->sz; i++)
        if (a->bits[i]->id == TRUE_3SAT)
            bool_buf[i] = true;
        else if (a->bits[i]->id == FALSE_3SAT)
            bool_buf[i] = false;
        else
            bool_buf[i] = decodable_buf[a->bits[i]->id];

    __int64 first_nonzero = 0;
    for (first_nonzero = 0; first_nonzero < a->sz && ! bool_buf[first_nonzero]; first_nonzero++)
        ;

    __int64 amod4 = (a->sz - first_nonzero) % 4 == 0 ? 0 : 4 - ((a->sz - first_nonzero) % 4);

    __int64 psd_bool_buf_sz = a->sz - first_nonzero + amod4;

    bool* psd_bool_buf = new bool[psd_bool_buf_sz];

    for (int i = 0; i < psd_bool_buf_sz; i++)
        psd_bool_buf[i] = false;

    for (int i = 0; i < a->sz - first_nonzero; i++)
        psd_bool_buf[amod4 + i] = bool_buf[first_nonzero + i];

    __int64 num_hex = psd_bool_buf_sz / 4;

    if (num_hex == 0) {

        *str_sz = 2;
        char* ret_str = new char[*str_sz];
        ret_str[0] = '0';
        ret_str[1] = '\0';
        return ret_str;

    }

    *str_sz = num_hex + 1;
    char* ret_str = new char[*str_sz];

    __int64 hexbits[4];

    // position in return string
    __int64 ret_str_pos = 0;

    for (__int64 i = 0; i < num_hex; i++) {

        hexbits[0] = hexbits[1] = hexbits[2] = hexbits[3] = 0;

        for (__int64 j = 0; j < 4; j++)
            hexbits[j] = psd_bool_buf[i * 4 + j];

        ret_str[ret_str_pos] = hex_char_from_int(hexbits[0] * 8 + hexbits[1] * 4 + hexbits[2] * 2 + hexbits[3]);
        ret_str_pos++;

    }

    ret_str[ret_str_pos] = '\0';

    return ret_str;
}

char* dec_get_factors(char* c_str, __int64 c_str_buf_sz, __int64 * len_para) {

    if (c_str == NULL or c_str == "")
        return NULL;

    __int64 num_para = 2; // TRUE_3SAT = 1 is reserved for true, while FALSE_3SAT = -1 is false

    // get length of c_str
    __int64 strln = 0;
    for (strln = 0; strln < c_str_buf_sz && c_str[strln] != '\0'; strln++)
        ;

    // transform hex input __int64o bool buffer

    __int64 inbuffer_sz = strln * 4;
    bool* inbuffer = new bool[inbuffer_sz];

    for (__int64 i = 0; i < strln; i++) {

        // decode the hex __int64o bits

        __int64 hexbits[4];
        hexbits[0] = hexbits[1] = hexbits[2] = hexbits[3] = 0;

        __int64 hexval = int_from_hex_char ( c_str[i] );

        if (hexval >= 8) {
            hexbits[3] = 1;
            hexval = hexval - 8;
        }
        if (hexval >= 4) {
            hexbits[2] = 1;
            hexval = hexval - 4;
        }
        if (hexval >= 2) {
            hexbits[1] = 1;
            hexval = hexval - 2;
        }
        hexbits[0] = hexval;

        for (__int64 j = 0; j < 4; j++)
            inbuffer[i * 4 + j] = hexbits[3 - j] == 1 ? true : false;
    }

    // count leading zeros

    __int64 leading_zeros = 0;
    for (leading_zeros = 0; !inbuffer[leading_zeros]; leading_zeros++)
        ;

    __int64 c_bit_count = inbuffer_sz - leading_zeros;

    dec_3sat* c_equals = new dec_3sat();
    c_equals->sz = c_bit_count * 2;
    c_equals->ad_sz = 0;
    c_equals->bd_sz = c_bit_count * 2;
    c_equals->bits = new bit_3sat * [c_bit_count * 2];

    c_equals->bits[0] = new bit_3sat();
    c_equals->bits[0]->id = FALSE_3SAT;

    // copy over leading multiply zeros

    for ( __int64 i = 0 ; i < c_bit_count; i++) {
        c_equals->bits[1 + i] = new bit_3sat();
        c_equals->bits[1 + i]->id = FALSE_3SAT;
    }

    // copy over leading c zeros

    for (__int64 i = 0; i < leading_zeros; i++) {
        c_equals->bits[1 + c_bit_count + i] = new bit_3sat();
        c_equals->bits[1 + c_bit_count + i]->id = FALSE_3SAT;
    }

    // transfer over inbuffer

    for (__int64 i = leading_zeros; i < inbuffer_sz; i++) {
        c_equals->bits[1 + c_bit_count + i] = new bit_3sat();
        c_equals->bits[1 + c_bit_count + i]->id = inbuffer[i] ? TRUE_3SAT : FALSE_3SAT;
    }

    delete[] inbuffer;

    dec_3sat* c = NULL;

    dec_3sat* a = new dec_3sat();
    a->sz = c_bit_count;
    a->ad_sz = 0;
    a->bd_sz = c_bit_count;
    a->bits = new bit_3sat * [c_bit_count - 1];

    dec_3sat* b = new dec_3sat();
    b->sz = c_bit_count;
    b->ad_sz = 0;
    b->bd_sz = c_bit_count;
    b->bits = new bit_3sat * [c_bit_count - 1];

    a->bits[0] = new bit_3sat();
    a->bits[0]->id = FALSE_3SAT;

    for (__int64 i = 0; i < c_bit_count - 1; i++)
        a->bits[i] = create_bit(&num_para);

    b->bits[0] = new bit_3sat();
    b->bits[0]->id = FALSE_3SAT;

    for (__int64 i = 0; i < c_bit_count - 1; i++)
        b->bits[i] = create_bit(&num_para);

    __int64 mul_str_len = 0;

    char* mul_str = dec_mul(&num_para, &c, a, b, (c_bit_count - 1) * 2, (__int64) 0, & mul_str_len);

    __int64 equals_str_len = 0;

    char* equals_str = dec_equals(&num_para, c, c_equals, true, & equals_str_len);
    
    __int64 bd_sz = a->bd_sz < b->bd_sz ? a->bd_sz : b->bd_sz;
    __int64 ad_sz = a->ad_sz > b->ad_sz ? a->ad_sz : b->bd_sz;

    __int64 buf_3sat_sz = mul_str_len + equals_str_len + 1;
    char* buf_3sat = new char[buf_3sat_sz];
    strcpy_s(buf_3sat, buf_3sat_sz, mul_str);
    strcpy_s(&(buf_3sat[mul_str_len]), buf_3sat_sz - mul_str_len, equals_str);

    delete[] mul_str;
    delete[] equals_str;

    __int64 k = 0;
    __int64** input = input_from_char_buf(buf_3sat, buf_3sat_sz, &k, false);
    bool* sln = new bool[num_para-1];

    delete[] buf_3sat;

    char* prime_str = new char[8];
    sprintf_s(prime_str, 8, "prime");

    bool is_sat = false;

    SATSolver* s = new SATSolver();
    SATSolver_create(s, input, k, num_para + 1, 0, 0);

    is_sat = SATSolver_isSat(s, sln);

    SATSolver_destroy(s);

    delete s;

    if (!is_sat)
        return prime_str;

    __int64 a_str_sz = 0;

    char* a_str = dec_to_str(sln, a, &a_str_sz);
    __int64 b_str_sz = 0;
    
    char* b_str = dec_to_str(sln, b, &b_str_sz);

    __int64 ret_buf_sz = a_str_sz + (__int64) strnlen_s("\n\n", 4) + b_str_sz + 1;
    char* ret_buf = new char[ret_buf_sz];
    sprintf_s(ret_buf, ret_buf_sz, "%s\n\n%s", a_str, b_str);

    *len_para = ret_buf_sz - 1;

    delete a_str;
    delete b_str;

    return ret_buf;
}

char* nat_get_factors(char* c_str, __int64 c_str_buf_sz, __int64 * len_para) {

    if (c_str == NULL or c_str == "")
        return NULL;

    __int64 num_para = 2; // TRUE_3SAT = 1 is reserved for true, while FALSE_3SAT = -1 is false

    // get length of c_str
    __int64 strln = 0;
    for (strln = 0; strln < c_str_buf_sz && c_str[strln] != '\0'; strln++)
        ;

    // transform hex input into bool buffer

    __int64 inbuffer_sz = strln * 4;
    bool* inbuffer = hex2bool(c_str, inbuffer_sz);

    // count leading zeros

    __int64 leading_zeros = 0;
    for (leading_zeros = 0; !inbuffer[leading_zeros]; leading_zeros++)
        ;

    __int64 c_bit_count = inbuffer_sz - leading_zeros;

    nat_3sat* c_equals = new nat_3sat();
    c_equals->sz = c_bit_count * 2;
    c_equals->bits = new bit_3sat * [c_bit_count * 2];

    // copy over leading multiply zeros

    for (__int64 i = 0; i < c_bit_count * 2; i++) {
        c_equals->bits[i] = new bit_3sat();
        c_equals->bits[i]->id = FALSE_3SAT;
    }

    // transfer over inbuffer

    for (__int64 i = 0; i < c_bit_count * 2; i++)
        c_equals->bits[i]->id = inbuffer[leading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;

    delete[] inbuffer;

    nat_3sat* c = NULL;

    nat_3sat* a = new nat_3sat();
    a->sz = c_bit_count - 1;
    a->bits = new bit_3sat * [c_bit_count - 1];

    nat_3sat* b = new nat_3sat();
    b->sz = c_bit_count - 1;
    b->bits = new bit_3sat * [c_bit_count - 1];

    for (__int64 i = 0; i < c_bit_count - 1; i++)
        a->bits[i] = create_bit(&num_para);
    for (__int64 i = 0; i < c_bit_count - 1; i++)
        b->bits[i] = create_bit(&num_para);

    __int64 mul_str_len = 0;

    char* mul_str = nat_mul(&num_para, &c, a, b, a->sz + b->sz, &mul_str_len);

    __int64 equals_str_len = 0;

    char* equals_str = nat_equals(&num_para, c, c_equals, true, &equals_str_len);

    __int64 buf_3sat_sz = mul_str_len + equals_str_len + 1;
    char* buf_3sat = new char[buf_3sat_sz];
    strcpy_s(buf_3sat, buf_3sat_sz, mul_str);
    strcpy_s(&(buf_3sat[mul_str_len]), buf_3sat_sz - mul_str_len, equals_str);

    delete[] mul_str;
    delete[] equals_str;

    __int64 k = 0;
    __int64** input = input_from_char_buf(buf_3sat, buf_3sat_sz, &k, false);
    bool* sln = new bool[num_para];

    /*

    printf_s("p cnf %lld %lld\n", num_para - 1, k);

    for (__int64 i = 0; i < k; i++) {
        char a[6];
        char b[6];
        char c[6];
        
        if (input[i][0] == -1)
            sprintf_s(a, 6, "%s", "");
        else if (input[i][0] == 1)
            sprintf_s(a, 6, "%s", "T");
        else
            sprintf_s(a, 6, "%lld", input[i][0] < 0 ? input[i][0] + 1 : input[i][0] - 1);

        if (input[i][1] == -1)
            sprintf_s(b, 6, "%s", "");
        else if (input[i][1] == 1)
            sprintf_s(b, 6, "%s", "T");
        else
            sprintf_s(b, 6, "%lld", input[i][1] < 0 ? input[i][1] + 1 : input[i][1] - 1);

        if (input[i][2] == -1)
            sprintf_s(c, 6, "%s", "");
        else if (input[i][2] == 1)
            sprintf_s(c, 6, "%s", "T");
        else
            sprintf_s(c, 6, "%lld", input[i][2] < 0 ? input[i][2] + 1 : input[i][2] - 1);

        printf_s("%s %s %s 0\n", a, b, c);

    }

    return NULL;

    //*/

    delete[] buf_3sat;

    char* prime_str = new char[8];
    sprintf_s(prime_str, 8, "prime");

    bool is_sat = false;

    SATSolver* s = new SATSolver();
    SATSolver_create(s, input, k, num_para + 1, 0, 0);

    is_sat = SATSolver_isSat(s, sln);
        
    SATSolver_destroy(s);

    delete s;

    if (!is_sat)
        return prime_str;

    __int64 a_str_sz = 0;
    __int64 b_str_sz = 0;

    char* a_str = nat_to_str(sln, a, &a_str_sz);
    char* b_str = nat_to_str(sln, b, &b_str_sz);

    __int64 ret_buf_sz = a_str_sz + (__int64)strnlen_s("\n\n", 4) + b_str_sz + 1;
    char* ret_buf = new char[ret_buf_sz];
    sprintf_s(ret_buf, ret_buf_sz, "%s\n\n%s", a_str, b_str);

    *len_para = ret_buf_sz - 1;

    delete [] a_str;
    delete [] b_str;

    return ret_buf;
}


#endif // !__3SATFRAMEWORK_C__
