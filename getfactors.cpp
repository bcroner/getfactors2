// get_factors_base.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __GET_FACTORS_BASE_CPP__
#define __GET_FACTORS_BASE_CPP__

#include <iostream>
#include <stdio.h>

#include "3SATBaseConsole.hpp"
#include "3SAT_framework.hpp"

using namespace std;

char* nat_test_add(__int64 * len_para) {

    const char* a_str_c = "4";
    const char* b_str_c = "5";

    char a_str[2000];
    char b_str[2000];

    strcpy_s(a_str, 2000, a_str_c);
    strcpy_s(b_str, 2000, b_str_c);

    __int64 num_para = 2; // TRUE_3SAT = 1 is reserved for true, while FALSE_3SAT = -1 is false

    // get lengths of a_str and b_str
    __int64 astrln = 0;
    for (astrln = 0; a_str[astrln] != '\0'; astrln++)
        ;

    __int64 bstrln = 0;
    for (bstrln = 0; b_str[bstrln] != '\0'; bstrln++)
        ;

    // transform hex input into bool buffer

    bool* ainbuffer = hex2bool(a_str, astrln);
    bool* binbuffer = hex2bool(b_str, bstrln);

    __int64 ainbuffer_sz = astrln * 4;
    __int64 binbuffer_sz = bstrln * 4;

    // count leading zeros

    __int64 aleading_zeros = 0;
    for (aleading_zeros = 0; !ainbuffer[aleading_zeros]; aleading_zeros++)
        ;

    __int64 bleading_zeros = 0;
    for (bleading_zeros = 0; !binbuffer[bleading_zeros]; bleading_zeros++)
        ;

    __int64 a_bit_count = ainbuffer_sz - aleading_zeros;
    __int64 b_bit_count = binbuffer_sz - bleading_zeros;

    nat_3sat* a = new nat_3sat();
    a->sz = a_bit_count;
    a->bits = new bit_3sat * [a_bit_count];

    nat_3sat* b = new nat_3sat();
    b->sz = b_bit_count;
    b->bits = new bit_3sat * [b_bit_count];

    for (__int64 i = 0; i < a_bit_count; i++) {
        a->bits[i] = new bit_3sat();
        a->bits[i]->id = ainbuffer[aleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
    }

    for (__int64 i = 0; i < b_bit_count; i++) {
        b->bits[i] = new bit_3sat();
        b->bits[i]->id = binbuffer[bleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
     }

    //nat_3sat* c = new nat_3sat();
    //c->sz = a_bit_count > b_bit_count ? a_bit_count + 1 : b_bit_count + 1;
    //c->bits = new bit_3sat * [c->sz];

    nat_3sat* c;

    delete[] ainbuffer;
    delete[] binbuffer;

    __int64 add_str_len = 0;

    char* add_str = nat_add(&num_para, &c, a, b, false, &add_str_len);

    //__int64 dummy = 0;
    //char* cst = nat_to_str(NULL, c, &dummy);
    //return cst;

    __int64 buf_3sat_sz = add_str_len + 1;

    char* buf_3sat = NULL;
    bool* sln = NULL;

    if (add_str_len > 0) {
        buf_3sat = new char[buf_3sat_sz];
        strcpy_s(buf_3sat, buf_3sat_sz, add_str);

        delete[] add_str;

        __int64 k = 0;
        __int64** input = input_from_char_buf(buf_3sat, buf_3sat_sz, &k, false);

        /*
        for (__int64 i = 0; i < k; i++) {
            __int64 a = input[i][0] == FALSE_3SAT ? 0 : input[i][0] < 0 ? input[i][0] : input[i][0];
            __int64 b = input[i][1] == FALSE_3SAT ? 0 : input[i][1] < 0 ? input[i][1] : input[i][1];
            __int64 c = input[i][2] == FALSE_3SAT ? 0 : input[i][2] < 0 ? input[i][2] : input[i][2];
            char a_str[16];
            char b_str[16];
            char c_str[16];
            if (a == 0)
                sprintf_s(a_str, 16, "%s", "");
            else
                sprintf_s(a_str, 16, "%lld", a);
            if (b == 0)
                sprintf_s(b_str, 16, "%s", "");
            else
                sprintf_s(b_str, 16, "%lld", b);
            if (c == 0)
                sprintf_s(c_str, 16, "%s", "");
            else
                sprintf_s(c_str, 16, "%lld", c);
            printf_s("%lld: %s %s %s\n", i, a_str, b_str, c_str);
        }
        printf_s("\n");
        //*/

        delete[] buf_3sat;

        sln = new bool[num_para];

        char* prime_str = new char[8];
        sprintf_s(prime_str, 8, "prime");

        bool is_sat = false;

        SATSolver* s = new SATSolver();
        SATSolver_create(s, input, k, num_para, 0, 0);

        is_sat = SATSolver_isSat(s, sln);

        //for (__int64 i = 0; i < num_para; i++)
        //    printf_s("%lld: %lld\n", i, sln[i] ? 1 : 0);
        //printf_s("\n");

        SATSolver_destroy(s);

        delete s;

        if (!is_sat)
            return prime_str;
    }

    __int64 c_str_sz = 0;

    char* c_str = nat_to_str(sln, c, &c_str_sz);

    __int64 ret_buf_sz = c_str_sz + (__int64)strnlen_s("\n\n", 4) + (__int64) 1;
    char* ret_buf = new char[ret_buf_sz];
    sprintf_s(ret_buf, ret_buf_sz, "%s\n\n", c_str);

    *len_para = ret_buf_sz - 1;

    if ( sln != NULL )
        delete[] sln;

    for (int i = 0; i < a->sz; i++)
        delete a->bits[i];
    for (int i = 0; i < b->sz; i++)
        delete b->bits[i];

    delete[] a->bits;
    delete[] b->bits;

    delete a;
    delete b;

    return ret_buf;
}

char* nat_test_mul(__int64 * len_para) {

    const char* a_str_c = "3";
    const char* b_str_c = "5";

    char a_str[2000];
    char b_str[2000];

    strcpy_s(a_str, 2000, a_str_c);
    strcpy_s(b_str, 2000, b_str_c);

    __int64 num_para = 2; // TRUE_3SAT = 1 is reserved for true, while FALSE_3SAT = -1 is false

    // get lengths of a_str and b_str
    int astrln = 0;
    for (astrln = 0; a_str[astrln] != '\0'; astrln++)
        ;

    int bstrln = 0;
    for (bstrln = 0; b_str[bstrln] != '\0'; bstrln++)
        ;

    // transform hex input into bool buffer

    bool* ainbuffer = hex2bool(a_str, astrln);
    bool* binbuffer = hex2bool(b_str, bstrln);

    int ainbuffer_sz = astrln * 4;
    int binbuffer_sz = bstrln * 4;

    // count leading zeros

    int aleading_zeros = 0;
    for (aleading_zeros = 0; !ainbuffer[aleading_zeros]; aleading_zeros++)
        ;

    int bleading_zeros = 0;
    for (bleading_zeros = 0; !binbuffer[bleading_zeros]; bleading_zeros++)
        ;

    int a_bit_count = ainbuffer_sz - aleading_zeros;
    int b_bit_count = binbuffer_sz - bleading_zeros;

    nat_3sat* a = new nat_3sat();
    a->sz = a_bit_count;
    a->bits = new bit_3sat * [a_bit_count];

    nat_3sat* b = new nat_3sat();
    b->sz = b_bit_count;
    b->bits = new bit_3sat * [b_bit_count];

    for (__int64 i = 0; i < a_bit_count; i++) {
        a->bits[i] = new bit_3sat();
        a->bits[i]->id = ainbuffer[aleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
    }

    for (__int64 i = 0; i < b_bit_count; i++) {
        b->bits[i] = new bit_3sat();
        b->bits[i]->id = binbuffer[bleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
    }

    //nat_3sat* c = new nat_3sat();
    //c->sz = a_bit_count + b_bit_count;
    //c->bits = new bit_3sat * [a_bit_count + b_bit_count];

    nat_3sat* c;

    delete[] ainbuffer;
    delete[] binbuffer;

    __int64 mul_str_len = 0;

    char* mul_str = nat_mul(&num_para, &c, a, b, a->sz + b->sz, &mul_str_len);

    __int64 buf_3sat_sz = mul_str_len + 1;

    char* buf_3sat = NULL;
    bool* sln = NULL;

    if (mul_str_len > 0) {
        buf_3sat = new char[buf_3sat_sz];
        strcpy_s(buf_3sat, buf_3sat_sz, mul_str);

        delete[] mul_str;

        __int64 k = 0;
        __int64** input = input_from_char_buf(buf_3sat, buf_3sat_sz, &k, false);

        /*
        for (__int64 i = 0; i < k; i++)
            printf_s("%lld: %lld %lld %lld\n", i, input[i][0], input[i][1], input[i][2]);
        printf_s("\n");
        */

        delete[] buf_3sat;

        sln = new bool[num_para];

        char* prime_str = new char[8];
        sprintf_s(prime_str, 8, "prime");


        bool is_sat = false;

        SATSolver* s = new SATSolver();
        SATSolver_create(s, input, k, num_para, 0, 0);

        is_sat = SATSolver_isSat(s, sln);

        SATSolver_destroy(s);

        delete s;

        if (!is_sat)
            return prime_str;
    }

    __int64 c_str_sz = 0;

    char* c_str = nat_to_str(sln, c, &c_str_sz);

    __int64 ret_buf_sz = c_str_sz + (__int64)strnlen_s("\n\n", 4) + (__int64)1;
    char* ret_buf = new char[ret_buf_sz];
    sprintf_s(ret_buf, ret_buf_sz, "%s\n\n", c_str);

    *len_para = ret_buf_sz - 1;

    if (sln != NULL)
        delete[] sln;

    for (int i = 0; i < a->sz; i++)
        delete a->bits[i];
    for (int i = 0; i < b->sz; i++)
        delete b->bits[i];

    delete[] a->bits;
    delete[] b->bits;

    delete a;
    delete b;

    return ret_buf;
}

char* nat_test_equals(__int64 * len_para) {


    const char* a_str_c = "7";
    const char* b_str_c = "7";

    char a_str[2000];
    char b_str[2000];

    strcpy_s(a_str, 2000, a_str_c);
    strcpy_s(b_str, 2000, b_str_c);

    __int64 num_para = 2; // TRUE_3SAT = 1 is reserved for true, while FALSE_3SAT = -1 is false

    // get lengths of a_str and b_str
    int astrln = 0;
    for (astrln = 0; a_str[astrln] != '\0'; astrln++)
        ;

    int bstrln = 0;
    for (bstrln = 0; b_str[bstrln] != '\0'; bstrln++)
        ;

    // transform hex input into bool buffer

    bool* ainbuffer = hex2bool(a_str, astrln);
    bool* binbuffer = hex2bool(b_str, bstrln);

    int ainbuffer_sz = astrln * 4;
    int binbuffer_sz = bstrln * 4;

    // count leading zeros

    int aleading_zeros = 0;
    for (aleading_zeros = 0; !ainbuffer[aleading_zeros]; aleading_zeros++)
        ;

    if (aleading_zeros == ainbuffer_sz)
        aleading_zeros--;

    int bleading_zeros = 0;
    for (bleading_zeros = 0; !binbuffer[bleading_zeros]; bleading_zeros++)
        ;

    if (bleading_zeros == binbuffer_sz)
        bleading_zeros--;

    int a_bit_count = ainbuffer_sz - aleading_zeros;
    int b_bit_count = binbuffer_sz - bleading_zeros;

    nat_3sat* a = new nat_3sat();
    a->sz = a_bit_count;
    a->bits = new bit_3sat * [a_bit_count];

    nat_3sat* b = new nat_3sat();
    b->sz = b_bit_count;
    b->bits = new bit_3sat * [b_bit_count];

    for (__int64 i = 0; i < a_bit_count; i++) {
        a->bits[i] = new bit_3sat();
        a->bits[i]->id = ainbuffer[aleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
    }

    for (__int64 i = 0; i < b_bit_count; i++) {
        b->bits[i] = new bit_3sat();
        b->bits[i]->id = binbuffer[bleading_zeros + i] ? TRUE_3SAT : FALSE_3SAT;
    }

    //nat_3sat* c = new nat_3sat();
    //c->sz = a_bit_count + b_bit_count;
    //c->bits = new bit_3sat * [a_bit_count + b_bit_count];
    
    __int64 equals_str_len = 0;
    char* equals_str = nat_equals(&num_para, a, b, true, &equals_str_len);

    printf_s("%s\n", equals_str);

    __int64 buf_3sat_sz = equals_str_len + 1;
    char* buf_3sat = new char[buf_3sat_sz];
    strcpy_s(buf_3sat, buf_3sat_sz, equals_str);

    delete[] equals_str;

    __int64 k = 0;
    __int64** input = input_from_char_buf(buf_3sat, buf_3sat_sz, &k, false);
    bool* sln = new bool[num_para];

    //*
    for (__int64 i = 0; i < k; i++) {
        __int64 a = input[i][0] == FALSE_3SAT ? 0 : input[i][0] < 0 ? input[i][0] + 1 : input[i][0] - 1;
        __int64 b = input[i][1] == FALSE_3SAT ? 0 : input[i][1] < 0 ? input[i][1] + 1 : input[i][1] - 1;
        __int64 c = input[i][2] == FALSE_3SAT ? 0 : input[i][2] < 0 ? input[i][2] + 1 : input[i][2] - 1;
        char a_str[16];
        char b_str[16];
        char c_str[16];
        if (a == 0)
            sprintf_s(a_str, 16, "%s", "");
        else
            sprintf_s(a_str, 16, "%lld", a);
        if (b == 0)
            sprintf_s(b_str, 16, "%s", "");
        else
            sprintf_s(b_str, 16, "%lld", b);
        if (c == 0)
            sprintf_s(c_str, 16, "%s", "");
        else
            sprintf_s(c_str, 16, "%lld", c);
        printf_s("%s %s %s 0\n", a_str, b_str, c_str);
    }
    printf_s("\n");
    //*/

    /*
    for (__int64 i = 0; i < k; i++)
        printf_s("%lld: %lld %lld %lld\n", i, input[i][0], input[i][1], input[i][2]);
    printf_s("\n");
    //*/

    delete[] buf_3sat;

    char* prime_str = new char[16];
    sprintf_s(prime_str, 16, "not equal");
    char* comps_str = new char[16];
    sprintf_s(comps_str, 16, "equal");

    bool is_sat = false;

    SATSolver* s = new SATSolver();
    SATSolver_create(s, input, k, num_para, 0, 0);

    is_sat = SATSolver_isSat(s, sln);

    for (__int64 i = 0; i < num_para; i++)
        printf_s("%lld: %lld\n", i, sln[i] ? 1 : 0);
    printf_s("\n");

    SATSolver_destroy(s);

    delete s;

    if (!is_sat)
        return prime_str;
    else
        return comps_str;
}

int main()
{
    int num_para = 2;
    __int64 len_para = 0;

/*

    strcpy_s(buf_3sat, 200000, buf_3sat_src);

    int buf_3sat_sz = strnlen_s(buf_3sat, 200000);

    int k = 0;
    int** input = input_from_char_buf (buf_3sat, buf_3sat_sz, &k, true);


    char* prime_str = new char[8];
    sprintf_s(prime_str, 8, "prime");

    // count n

    /*

    int counted = 0;

    for (int i = 0; i < k; i++) {

        int a = input[i][0] < 0 ? -input[i][0] : input[i][0];
        int b = input[i][0] < 0 ? -input[i][0] : input[i][0];
        int c = input[i][0] < 0 ? -input[i][0] : input[i][0];

        if (a > counted)
            counted = a;
        if (b > counted)
            counted = b;
        if (c > counted)
            counted = c;

    }

    bool* sln = new bool[counted];

    SATSolverMaster* master = new SATSolverMaster();
    SATSolverMaster_create(master, input, k, counted, 2);

    int count_sz = 1;

    for (int i = 0; i < master->chops; i++)
        count_sz *= 2;

    bool is_sat = false;

    for (int i = 0; i < count_sz; i++) {

        SATSolver* s = new SATSolver();
        SATSolver_create(s, master, input, k, counted, i);

        is_sat = SATSolver_isSat(s, i, sln);

        if (is_sat)
            break;
    }

    if (! is_sat)
        printf_s("not satisfiable\n");
    else {
        printf_s("satisfiable: \n");
        for (int i = 0; i < counted; i++)
            printf_s("%d", (int)sln[i]);
        printf_s("\n");

    }


    //if ( ! SATSolver_threads(input, k, counted, &sln))
    //    printf_s("not satisfiable\n");
    //else
    //    printf_s("satisfiable\n");

    //*/

    ///*

    //bit_3sat * a = create_bit(&num_para);
    //bit_3sat * b = create_bit(&num_para);
    //bit_3sat * c = NULL;;
    //printf("%s\n", and_3sat(&num_para, &c, a, b, &len_para));
    //char c_str[5];
    //char c_str[2000];
    /*
    strcpy_s(c_str, 2000,
        "BE88B9A8CA71030100C1798F81149B6F\
        5687B7EB3E218554D6608D4528914571\
        A54AE074E604863A0AA37302F1ED9DA3\
        A7EDCB8BB05ACE317D289A6E62A56888\
        A4BE324087C5A5BA67731FCFCE9EFD52\
        50FAABC5447D6DBDF6FCA97F4F370913\
        384EBC8A814063CA5151B0667394EABB\
        9ECE59C4191EFA3C67BA5022FC169BD6\
        08D1E14262E7D3EB8E61E66B70F720C9\
        4CD16BBEE0AA918992BE202C34F2901B\
        BE971780AC9DC4284C350DACBEFF7A2F\
        5543B9851BFC95A03081496AFC99466F\
        D31393574E5655A9C3AF13A5E98E2A11\
        59D6AD59EC1AAF2A7EF65F40D8C282B3\
        CA90A2C1788723C9B226D84545356A2F\
        4DF3F73A85483E8C08D5855055E7BE4D");
    //*/
    /*
    strcpy_s(c_str, 2000,
        "04898102f980b7d6f2a263692d1dad\
        7355c722bd52bc5615facfe9b7a5b2\
        d615b9d2da021a9b3b86f659909fc1\
        6ffff83445962c809d95dadf4daa8a\
        8304d84267"); // https://gmail.com
    //*/

    /*
    strcpy_s(c_str, 2000,
        "04296895a7038ff658c9531c799321\
        b31ae82d973f5ff59dd93f481a46cd\
        07d27760db4e151236ec5b0db99ead\
        6a94c856fadff9045f6011be6b3f6e\
        cf909a12a7"); // https://gmail.com
    //*/

    //char* nat_add_str = nat_test_add(&len_para);
    //printf_s("%s\n", nat_add_str);

    //char* nat_mul_str = nat_test_mul(&len_para);
    //printf_s("%s\n", nat_mul_str);

    //char* equals_str = nat_test_equals(&len_para);
    //printf_s("%s\n", equals_str);

    //*
    char c_str[2000];
    //strcpy_s(c_str, 2000, "4");
    //strcpy_s(c_str, 2000, "5");
    //strcpy_s(c_str, 2000, "6");
    //strcpy_s(c_str, 2000, "7");
    //strcpy_s(c_str, 2000, "8");
    //strcpy_s(c_str, 2000, "F");
    //strcpy_s(c_str, 2000, "8F");
    strcpy_s(c_str, 2000, "7a9");
    //char * factors = dec_get_factors(c_str, 2000, & len_para);
    char* factors = nat_get_factors(c_str, 2000, &len_para);
    printf_s("%s\n", factors);
    //*/
    /*
    int str_sz = 0;
    bool decodable_buf[] = {true, false, true, true, false, false, true, false , false};
    dec_3sat * numa = new dec_3sat();
    numa->ad_sz = 0;
    numa->bd_sz = 5;
    numa->sz = 5;
    strcpy_s(numa->name, 17, "numa_test");
    numa->bits = new bit * [5];
    int tf = FALSE_3SAT;
    numa->bits[0] = create_bit(&tf);
    numa->bits[1] = create_bit(&num_para);
    numa->bits[2] = create_bit(&num_para);
    numa->bits[3] = create_bit(&num_para);
    numa->bits[4] = create_bit(&num_para);
    char* dec_str = dec_to_str(decodable_buf, numa, &str_sz);
    //*/
    /*
    dec_3sat* c;
    dec_3sat* a = new dec_3sat();
    a->bd_sz = 4;
    a->ad_sz = 0;
    a->sz = 4;
    a->bits = new bit_3sat*[4];
    a->bits[0] = new bit_3sat();
    a->bits[0]->id = FALSE_3SAT;
    a->bits[1] = create_bit(&num_para);
    a->bits[2] = create_bit(&num_para);
    a->bits[3] = create_bit(&num_para);


    dec_3sat* b = new dec_3sat();
    b->bd_sz = 4;
    b->ad_sz = 0;
    b->sz = 4;
    b->bits = new bit_3sat * [4];
    b->bits[0] = new bit_3sat();
    b->bits[0]->id = FALSE_3SAT;
    b->bits[1] = create_bit(&num_para);
    b->bits[2] = create_bit(&num_para);
    b->bits[3] = create_bit(&num_para);

    char* sum_str = dec_add(&num_para, &c, a, b, false, &len_para);

    printf_s("%s", sum_str);

    //*/

    return 0;
}

#endif

/*
*
* mit.edu website security certificate

Modulus (2048 bits):
BE88B9A8CA71030100C1798F81149B6F
5687B7EB3E218554D6608D4528914571
A54AE074E604863A0AA37302F1ED9DA3
A7EDCB8BB05ACE317D289A6E62A56888
A4BE324087C5A5BA67731FCFCE9EFD52
50FAABC5447D6DBDF6FCA97F4F370913
384EBC8A814063CA5151B0667394EABB
9ECE59C4191EFA3C67BA5022FC169BD6
08D1E14262E7D3EB8E61E66B70F720C9
4CD16BBEE0AA918992BE202C34F2901B
BE971780AC9DC4284C350DACBEFF7A2F
5543B9851BFC95A03081496AFC99466F
D31393574E5655A9C3AF13A5E98E2A11
59D6AD59EC1AAF2A7EF65F40D8C282B3
CA90A2C1788723C9B226D84545356A2F
4DF3F73A85483E8C08D5855055E7BE4D

  Public Exponent (17 bits):
  01 00 01

*/

/*
* 
* mail.google.com

04:89:81:02:f9:80:b7:d6:f2:a2:63:69:2d:1d:ad:
73:55:c7:22:bd:52:bc:56:15:fa:cf:e9:b7:a5:b2:
d6:15:b9:d2:da:02:1a:9b:3b:86:f6:59:90:9f:c1:
6f:ff:f8:34:45:96:2c:80:9d:95:da:df:4d:aa:8a:
83:04:d8:42:67

*/

/*

"-48 -49 21\n\
41 37 -42\n\
-25 -22 10\n\
-25 26 -8\n\
-20 27 -4\n\
2 9 36\n\
-44 47 10\n\
-29 28 -33\n\
6 -36 8\n\
3 -49 -38\n\
-43 -27 8\n\
2 -42 9\n\
23 -5 -39\n\
-37 49 36\n\
-5 16 -13\n\
-40 14 47\n\
44 -4 -18\n\
-28 -35 19\n\
19 -26 4\n\
18 4 30\n\
-50 -38 -31\n\
6 -24 46\n\
3 -6 -10\n\
-40 7 12\n\
-11 -37 -4\n\
-8 -45 -16\n\
-31 47 -19\n\
-2 -23 -11\n\
-34 46 9\n\
-37 -1 -13\n\
34 -42 6\n\
-3 -29 20\n\
-31 21 19\n\
-37 48 3\n\
4 -10 18\n\
-6 -21 12\n\
46 17 -33\n\
-5 -35 18\n\
-32 46 -28\n\
30 -13 45\n\
-37 33 12\n\
-25 37 7\n\
-3 -36 -23\n\
29 -30 25\n\
-34 -29 31\n\
-29 32 40\n\
14 -36 -47\n\
-26 -37 33\n\
46 -13 -20\n\
-1 24 -19\n\
-9 49 15\n\
-12 -45 21\n\
8 -16 50\n\
-1 12 21\n\
-30 27 10\n\
46 -17 -16\n\
-49 -13 9\n\
-43 -30 22\n\
-4 6 43\n\
13 -42 -28\n\
40 -34 -47\n\
-32 37 -1\n\
-6 9 -27\n\
-1 10 -41\n\
46 13 -20\n\
8 34 -46\n\
-44 8 34\n\
39 26 -23\n\
15 24 36\n\
47 3 20\n\
15 -27 4\n\
44 32 -28\n\
4 28 -13\n\
-34 -25 -39\n\
33 1 -3\n\
-25 26 19\n\
-30 -47 16\n\
10 -1 39\n\
35 -14 -11\n\
49 45 47\n\
-31 -40 38\n\
50 -18 -4\n\
34 10 -41\n\
45 6 -43\n\
-50 20 -32\n\
-8 2 -38\n\
2 1 -44\n\
-45 49 3\n\
15 16 -28\n\
-45 20 -18\n\
24 -39 -11\n\
19 40 30\n\
18 -4 -25\n\
17 41 25\n\
-18 -31 -41\n\
-18 20 29\n\
-7 47 44\n\
-47 -6 -40\n\
-16 -21 -12\n\
8 12 26\n\
-19 28 -22\n\
11 21 -26\n\
20 -47 33\n\
46 12 34\n\
42 8 -27\n\
-22 28 -13\n\
-22 -36 -49\n\
25 9 -46\n\
19 13 2\n\
-17 11 -44\n\
-50 47 41\n\
15 -7 17\n\
-37 5 11\n\
12 34 3\n\
31 -13 -16\n\
-3 -41 32\n\
-12 49 9\n\
43 -47 -9\n\
4 2 13\n\
-11 46 29\n\
-11 22 -10\n\
45 26 42\n\
8 -46 -49\n\
47 2 40\n\
25 -19 50\n\
-22 -5 41\n\
25 -41 -30\n\
19 2 -28\n\
-6 15 -27\n\
-50 48 -2\n\
18 23 -1\n\
-12 -29 -37\n\
24 -14 -32\n\
27 -36 -14\n\
-37 -30 -35\n\
-35 46 8\n\
32 16 -40\n\
-49 -31 -48\n\
-30 8 -15\n\
31 -47 25\n\
1 20 50\n\
19 -36 -8\n\
-2 -6 41\n\
13 45 -34\n\
37 -3 4\n\
3 7 -11\n\
-46 28 -37\n\
-1 -9 -19\n\
45 -42 -21\n\
-38 -49 4\n\
42 43 -17\n\
31 -50 4\n\
46 -49 16\n\
2 -27 -3\n\
-23 15 -48\n\
37 -45 -26\n\
-30 -27 -29\n\
40 25 5\n\
-6 33 18\n\
-1 -24 -13\n\
13 22 -17\n\
-45 30 29\n\
-28 -31 -23\n\
37 -20 5\n\
-15 -3 -34\n\
4 2 21\n\
14 -32 45\n\
30 18 35\n\
48 25 -27\n\
26 -7 31\n\
-21 38 12\n\
7 21 -46\n\
-14 3 1\n\
-20 11 -30\n\
44 48 -41\n\
-35 14 47\n\
-32 41 -6\n\
-47 -17 -39\n\
14 42 24\n\
-4 -11 48\n\
13 -11 38\n\
20 18 49\n\
14 -47 29\n\
46 5 50\n\
27 -19 -24\n\
48 -9 -35\n\
38 12 40\n\
-26 -9 -30\n\
-23 4 -38\n\
-19 -38 24\n\
40 43 31\n\
15 48 -36\n\
6 35 36\n\
-35 -9 11\n\
-46 -15 34\n\
-27 41 36\n\
15 49 -47\n\
26 44 50\n\
-34 40 -14\n\
-5 -12 6\n\
-44 25 -45\n\
40 44 -37\n\
-34 -45 -30\n\
28 39 -48\n\
27 16 -44\n\
-8 39 -42\n\
27 3 -30\n\
43 -44 -27\n\
-8 15 -5\n\
-13 -36 -37\n\
35 39 26\n\
48 5 42\n\
-4 -9 49\n\
-33 -11 39\n\
32 -10 -44\n\
-3 -22 -33\n\
-39 42 40\n\
3 -5 14\n";// not satisfiable

*/

/*

"-35 -36 -10\n\
-31 34 37\n\
8 38 5\n\
14 3 -33\n\
-14 6 33\n\
22 37 27\n\
36 6 -12\n\
-25 -20 -14\n\
38 -47 20\n\
32 1 46\n\
-40 37 39\n\
-30 -14 25\n\
-22 -46 -16\n\
-38 -25 7\n\
-25 -33 -14\n\
-33 -21 -16\n\
7 42 4\n\
25 -14 21\n\
12 17 -8\n\
37 -34 23\n\
-27 9 42\n\
-42 48 -24\n\
-20 29 50\n\
7 -36 27\n\
12 29 -39\n\
-39 -13 -22\n\
1 -40 -25\n\
4 -5 -18\n\
-22 -6 23\n\
15 41 47\n\
49 41 14\n\
48 -21 -1\n\
-12 49 -40\n\
-17 23 44\n\
7 -36 14\n\
4 39 -6\n\
-29 -7 -47\n\
15 40 -41\n\
-43 36 -14\n\
8 -32 20\n\
-37 39 48\n\
18 35 -13\n\
-6 -30 -31\n\
38 26 35\n\
35 -19 41\n\
14 26 13\n\
-19 -35 4\n\
23 5 -48\n\
-3 -38 7\n\
-47 -23 -37\n\
12 41 3\n\
-43 21 5\n\
15 -25 8\n\
-44 32 -47\n\
18 15 -8\n\
-31 -13 -26\n\
3 -30 50\n\
11 41 27\n\
27 -46 36\n\
9 13 20\n\
-2 -47 34\n\
6 -33 49\n\
-7 50 14\n\
38 45 12\n\
-28 -19 48\n\
46 33 1\n\
-29 -45 -19\n\
40 34 7\n\
-11 47 -34\n\
12 -43 -9\n\
27 -33 -28\n\
34 33 16\n\
-8 1 -14\n\
-42 49 20\n\
-38 -7 18\n\
-23 -26 -39\n\
47 -15 -11\n\
-20 40 -28\n\
-19 -11 -29\n\
48 23 -49\n\
28 -42 -18\n\
44 38 -13\n\
31 20 34\n\
38 23 -15\n\
9 8 -23\n\
49 1 34\n\
-14 10 13\n\
21 44 -33\n\
9 39 8\n\
-23 13 -25\n\
37 -18 49\n\
45 47 -25\n\
19 -44 -31\n\
-37 -7 -21\n\
-11 -1 39\n\
-34 33 26\n\
46 48 -7\n\
-8 23 24\n\
-21 10 28\n\
18 -14 8\n\
-14 33 -37\n\
49 -10 -37\n\
34 -37 -14\n\
29 -46 -47\n\
13 4 14\n\
24 45 -10\n\
12 -18 -35\n\
50 34 -39\n\
-36 -5 23\n\
5 46 -36\n\
48 -28 44\n\
-5 2 19\n\
15 18 50\n\
29 9 13\n\
23 -5 50\n\
-6 -26 32\n\
1 -38 -24\n\
-35 -42 -9\n\
43 23 -41\n\
10 45 46\n\
-13 9 -49\n\
-17 -8 -19\n\
-32 -12 -37\n\
22 -39 -5\n\
4 22 49\n\
26 -33 21\n\
-38 49 21\n\
-10 -37 -3\n\
-22 21 39\n\
-39 -19 37\n\
-9 -7 -44\n\
-32 14 3\n\
2 -43 21\n\
24 47 44\n\
-46 31 45\n\
42 -19 -49\n\
4 9 14\n\
-24 3 -13\n\
-1 -27 -35\n\
-3 31 -26\n\
3 -6 -5\n\
-33 -31 9\n\
-16 11 12\n\
-16 -6 -43\n\
46 -1 8\n\
9 1 27\n\
13 -34 35\n\
-32 46 -31\n\
9 -39 -41\n\
-29 35 -40\n\
-8 -14 -38\n\
39 40 24\n\
44 -43 49\n\
-9 -44 -11\n\
-39 24 -33\n\
16 18 -2\n\
43 41 -18\n\
20 -8 21\n\
-40 -50 45\n\
32 6 -28\n\
-21 26 -48\n\
-16 35 2\n\
15 21 -40\n\
21 10 -19\n\
33 40 2\n\
5 -30 49\n\
-40 -45 1\n\
-1 4 -25\n\
21 45 -40\n\
-42 38 2\n\
29 -10 -9\n\
-2 -27 -41\n\
43 -5 32\n\
11 -34 -42\n\
12 -27 -16\n\
-20 6 36\n\
28 14 38\n\
-9 46 23\n\
-43 -22 -46\n\
-12 2 -8\n\
-4 35 40\n\
-1 41 16\n\
-6 18 13\n\
-40 -1 -46\n\
16 48 38\n\
-7 28 37\n\
48 17 -41\n\
-31 -11 16\n\
18 28 25\n\
-50 28 -7\n\
40 8 -27\n\
2 -41 18\n\
-46 -7 32\n\
7 -23 -29\n\
20 12 -45\n\
47 19 33\n\
1 -24 -49\n\
34 33 -1\n\
-21 -29 17\n\
-38 18 13\n\
-7 -21 43\n\
-47 45 -38\n\
-39 46 -9\n\
34 -31 9\n\
2 -17 -24\n\
-37 -6 18\n\
-43 5 31\n\
26 12 -15\n\
49 -3 -33\n\
-33 -13 48\n\
47 29 -49\n\
-13 2 28\n\
42 23 19\n\
-17 -21 16\n\
-49 -12 -29\n\
-50 -38 -6\n\
39 -8 -30\n\
-20 -28 -10\n"; //unsatisfiable

*/

/*

"14 35 3\n\
-30 -7 -26\n\
-41 50 23\n\
-41 -38 -31\n\
-49 33 38\n\
2 4 9\n\
-39 -15 -41\n\
22 49 11\n\
-2 -45 43\n\
33 15 -8\n\
-26 5 -31\n\
-50 -4 3\n\
-30 6 1\n\
2 -20 45\n\
35 50 27\n\
31 8 33\n\
16 26 -24\n\
36 -16 -27\n\
27 3 36\n\
-40 21 16\n\
-25 32 2\n\
6 -29 47\n\
48 -5 -49\n\
49 -40 -19\n\
-10 -34 -9\n\
-44 -4 14\n\
18 50 -46\n\
-36 -9 -39\n\
-16 30 34\n\
-9 44 32\n\
28 -25 -23\n\
-18 40 -3\n\
37 -12 -22\n\
-47 3 -21\n\
-4 -5 -26\n\
-15 1 17\n\
-26 36 50\n\
-17 1 35\n\
-25 15 -24\n\
39 14 41\n\
3 17 -18\n\
29 -22 28\n\
24 36 35\n\
-36 -19 30\n\
-20 -43 6\n\
36 10 -40\n\
33 -41 -47\n\
-11 -22 18\n\
-50 -19 40\n\
-45 18 10\n\
22 -36 13\n\
-36 -23 16\n\
-14 -44 36\n\
-3 38 -37\n\
10 11 4\n\
-23 -32 -40\n\
-10 29 -3\n\
-40 25 30\n\
-44 9 5\n\
17 -11 -32\n\
-24 3 -14\n\
48 44 -47\n\
-13 -29 -12\n\
32 -43 33\n\
-29 -4 -35\n\
-23 36 50\n\
-31 -22 9\n\
9 -7 -42\n\
-49 -26 19\n\
35 -16 -19\n\
37 35 45\n\
-4 30 -20\n\
39 31 25\n\
-8 -50 -20\n\
-23 3 -47\n\
-38 -4 -30\n\
43 30 -18\n\
44 49 -22\n\
-19 16 -45\n\
19 27 35\n\
-10 -6 -39\n\
-30 12 -44\n\
35 -20 6\n\
21 -43 -47\n\
-27 -24 13\n\
-1 -48 20\n\
16 15 -14\n\
45 -40 -5\n\
-2 13 25\n\
-26 -47 -45\n\
3 -38 34\n\
-23 44 -33\n\
21 -48 27\n\
-8 -50 31\n\
-4 24 -48\n\
35 -15 23\n\
37 -9 -44\n\
-6 -15 -44\n\
-1 -47 -42\n\
-32 13 29\n\
-15 40 -37\n\
-20 -34 -31\n\
-44 -43 21\n\
26 29 -5\n\
38 39 -7\n\
31 34 -21\n\
-5 37 30\n\
-12 -18 -49\n\
-45 18 9\n\
-43 -12 4\n\
31 -10 6\n\
-48 32 -9\n\
-33 28 -50\n\
12 5 -11\n\
-35 47 38\n\
-28 35 -16\n\
27 43 -47\n\
-36 30 -17\n\
1 32 -49\n\
-2 -13 -20\n\
25 -21 -45\n\
-30 -25 29\n\
-18 19 38\n\
13 16 42\n\
44 21 20\n\
-43 28 37\n\
26 34 48\n\
25 -35 44\n\
-23 -14 45\n\
-27 5 -21\n\
32 16 4\n\
-37 47 -35\n\
-20 -47 -34\n\
-1 -9 -31\n\
-10 46 -3\n\
-31 -34 -50\n\
-32 43 21\n\
45 -1 -44\n\
25 -4 -10\n\
19 41 44\n\
-49 -42 7\n\
34 -18 -45\n\
14 38 -50\n\
-37 -36 -49\n\
-38 -12 -35\n\
4 28 34\n\
13 8 -37\n\
7 -26 -17\n\
2 20 14\n\
-45 -4 -19\n\
21 -38 -25\n\
25 11 7\n\
40 -24 -44\n\
-35 -32 46\n\
6 26 -15\n\
-22 -25 -37\n\
46 3 45\n\
21 4 36\n\
-41 -45 -47\n\
48 4 12\n\
-44 32 -47\n\
-36 -14 -25\n\
9 -31 -22\n\
14 -35 33\n\
48 -25 14\n\
19 -40 2\n\
46 15 26\n\
-7 -12 33\n\
-15 -49 -8\n\
8 47 10\n\
-42 -24 46\n\
-6 35 -40\n\
34 -12 -5\n\
-41 4 -38\n\
-33 -8 -43\n\
20 -12 44\n\
-19 -35 9\n\
34 -50 -6\n\
-41 -43 32\n\
-22 -1 14\n\
48 -50 6\n\
-29 34 -33\n\
12 22 -39\n\
-2 -3 -15\n\
-12 46 34\n\
-38 -22 21\n\
-17 -10 4\n\
33 49 47\n\
3 12 9\n\
-49 14 32\n\
27 -44 -24\n\
-36 35 47\n\
-17 -21 18\n\
40 -43 -34\n\
-5 -8 37\n\
39 -23 -3\n\
6 28 -13\n\
-23 29 -6\n\
-15 6 29\n\
35 15 -31\n\
-30 40 -50\n\
9 7 -19\n\
6 32 -10\n\
-47 -36 -31\n\
-49 -18 6\n\
7 -18 -45\n\
48 -7 3\n\
48 -33 43\n\
33 40 -50\n\
4 -14 -1\n\
15 50 7\n\
6 4 -12\n\
20 18 22\n\
25 7 4\n\
7 -23 -33\n\
46 -25 2\n\
11 34 18\n\
-38 -22 -28\n";//satisfiable

*/

/*
"40 -9 17\n\
-30 -45 35\n\
-16 -43 22\n\
18 23 -21\n\
36 38 25\n\
2 -21 -19\n\
-2 -41 -23\n\
1 25 34\n\
40 27 -12\n\
50 7 23\n\
-14 -40 -44\n\
38 26 -3\n\
24 -31 -34\n\
8 -37 -16\n\
-16 48 42\n\
13 23 -1\n\
-36 46 12\n\
21 -42 -50\n\
44 6 -33\n\
-44 -41 -29\n\
-46 -41 10\n\
33 -14 44\n\
-42 24 12\n\
-40 28 41\n\
43 -50 28\n\
13 47 20\n\
41 27 46\n\
-3 32 -19\n\
-39 -32 -16\n\
29 -27 37\n\
-30 10 -19\n\
-2 9 10\n\
4 24 50\n\
49 -11 35\n\
-2 -26 -45\n\
-10 13 3\n\
11 14 30\n\
-39 -14 -24\n\
41 -35 6\n\
-43 -50 28\n\
-30 47 -23\n\
-4 36 48\n\
34 -44 22\n\
2 -22 -43\n\
-9 -39 -42\n\
47 -6 -25\n\
4 42 -36\n\
-9 8 35\n\
-39 16 -37\n\
23 31 -21\n\
43 -13 -1\n\
18 -35 30\n\
-44 45 -13\n\
-44 -14 43\n\
-13 -18 46\n\
-29 -25 26\n\
-18 10 -42\n\
2 -6 -22\n\
-40 27 10\n\
-48 19 21\n\
-7 29 27\n\
-40 25 -29\n\
-9 -23 -46\n\
-27 26 6\n\
40 -7 -30\n\
-13 -7 9\n\
-8 27 42\n\
26 -17 36\n\
1 24 47\n\
-17 49 12\n\
-27 22 18\n\
-2 18 -33\n\
-6 29 7\n\
13 2 -28\n\
-15 24 -36\n\
37 -47 -17\n\
-10 -34 -44\n\
32 -26 -39\n\
-35 -27 19\n\
-8 -41 34\n\
24 -2 -25\n\
39 21 25\n\
46 42 -45\n\
-3 -45 6\n\
-4 36 -1\n\
-8 -37 38\n\
-16 -30 36\n\
-47 40 45\n\
-28 11 -6\n\
-43 -11 -20\n\
30 -5 20\n\
32 -2 -41\n\
19 -39 -24\n\
-25 -8 -43\n\
-15 -33 10\n\
-17 7 -12\n\
-46 5 -50\n\
-21 40 29\n\
26 16 18\n\
21 38 -26\n\
-21 -44 6\n\
49 -40 38\n\
9 10 -3\n\
-37 -33 46\n\
13 -26 37\n\
-38 11 12\n\
-45 3 42\n\
-41 -31 -32\n\
21 27 32\n\
28 -32 -46\n\
-33 -36 48\n\
-47 23 -5\n\
-11 -46 -8\n\
41 9 -25\n\
-43 1 26\n\
25 -48 -9\n\
-24 -39 10\n\
6 -38 -11\n\
-11 -20 -7\n\
29 35 -32\n\
-27 47 -45\n\
-23 42 25\n\
48 -26 40\n\
6 -48 -39\n\
44 12 -16\n\
22 -42 35\n\
-2 -50 45\n\
28 -29 -40\n\
36 42 25\n\
5 48 -19\n\
-7 45 25\n\
-45 -12 -1\n\
-4 -15 -10\n\
-20 -17 -38\n\
2 45 40\n\
-4 21 7\n\
-31 -7 28\n\
13 -48 30\n\
18 10 28\n\
-48 -6 -13\n\
50 30 46\n\
23 39 14\n\
-16 31 -6\n\
-7 21 35\n\
23 -16 -15\n\
10 -21 -36\n\
-40 -18 6\n\
-5 17 20\n\
-6 41 -22\n\
42 -39 -47\n\
46 -9 31\n\
14 -5 -29\n\
-8 13 28\n\
-1 -3 34\n\
29 -1 -21\n\
26 27 4\n\
18 30 -21\n\
-17 35 -36\n\
-12 -13 -2\n\
-17 -24 -46\n\
17 -16 19\n\
38 46 49\n\
37 21 5\n\
22 27 30\n\
18 -38 -48\n\
6 42 -3\n\
-44 50 -26\n\
13 46 1\n\
34 40 42\n\
-37 -30 -1\n\
21 48 -18\n\
-4 16 -30\n\
10 -20 34\n\
26 -20 41\n\
31 19 3\n\
-22 25 -46\n\
9 35 -28\n\
37 6 47\n\
-43 -47 39\n\
-45 30 50\n\
19 24 -32\n\
-12 -26 -21\n\
26 29 48\n\
-44 45 2\n\
1 15 -30\n\
-8 -30 6\n\
-24 -5 -48\n\
-37 19 -35\n\
33 -16 -41\n\
16 50 21\n\
23 -40 -8\n\
-35 -37 27\n\
-16 -48 35\n\
-39 -20 21\n\
-21 49 7\n\
-43 -38 -7\n\
29 33 25\n\
21 -27 -3\n\
-12 -16 -9\n\
-38 16 41\n\
11 -49 -43\n\
-6 -27 -24\n\
-35 3 -12\n\
-26 20 -5\n\
27 -39 15\n\
23 44 19\n\
-6 -16 44\n\
-14 31 -27\n\
14 -44 4\n\
-17 4 -43\n\
2 43 -20\n\
-38 -13 -46\n\
-6 -5 -23\n\
17 44 35\n\
25 28 -17\n\
43 -11 -30\n\
-42 -19 -1\n\
30 -6 46\n";//satisfiable

*/

/*

"43 -96 79\n\
33 -63 68\n\
68 64 85\n\
-40 52 6\n\
62 52 -94\n\
-79 -25 78\n\
-11 59 95\n\
-29 2 4\n\
-96 76 24\n\
42 69 81\n\
-35 83 12\n\
73 90 -34\n\
62 -30 -56\n\
-13 29 98\n\
-34 14 6\n\
3 -1 -51\n\
-54 -35 -72\n\
-40 30 -29\n\
-69 -59 -11\n\
-92 -86 -70\n\
-14 -56 -89\n\
45 24 -86\n\
16 51 82\n\
26 -14 88\n\
-93 -72 -24\n\
50 83 34\n\
-90 91 28\n\
28 -23 89\n\
38 93 48\n\
-64 -21 -99\n\
-65 -33 -50\n\
1 60 -87\n\
-67 -62 -16\n\
89 9 74\n\
-63 -96 23\n\
-28 47 38\n\
-73 -46 68\n\
-53 -40 63\n\
50 84 -42\n\
27 81 -71\n\
-55 69 100\n\
-100 -18 -90\n\
69 -29 -3\n\
65 -47 -81\n\
-81 96 80\n\
4 -100 89\n\
47 -91 53\n\
99 -26 -58\n\
-99 21 -1\n\
-78 -8 67\n\
-77 -61 11\n\
-1 11 92\n\
82 45 75\n\
-93 44 -52\n\
63 -99 86\n\
-40 -32 -4\n\
52 98 78\n\
-93 -32 68\n\
98 -63 -56\n\
28 18 76\n\
-56 87 -63\n\
27 90 58\n\
-27 -26 39\n\
-8 91 71\n\
41 -67 -44\n\
47 -35 78\n\
69 20 4\n\
-34 50 -63\n\
-81 52 -47\n\
78 -56 53\n\
-13 18 -67\n\
6 95 80\n\
46 54 -53\n\
64 -8 -50\n\
97 -37 -20\n\
82 -71 8\n\
-9 -66 28\n\
91 21 2\n\
-41 93 -36\n\
-54 100 -31\n\
-64 -30 60\n\
4 -86 -87\n\
-79 64 82\n\
9 47 4\n\
-38 -89 -6\n\
57 -25 -84\n\
37 100 46\n\
-84 -9 -82\n\
-76 97 -35\n\
-87 78 59\n\
46 2 -29\n\
-55 -40 -35\n\
60 -79 35\n\
-1 64 57\n\
-67 81 85\n\
5 28 -51\n\
-23 -100 -31\n\
10 -36 78\n\
67 61 9\n\
51 -8 94\n\
-41 70 -47\n\
82 -25 -91\n\
-11 92 -3\n\
33 3 -24\n\
54 77 72\n\
67 52 7\n\
-5 24 35\n\
-63 46 9\n\
-27 67 -48\n\
-15 63 -51\n\
24 -76 -15\n\
-41 -94 81\n\
-92 53 -61\n\
-44 -62 -91\n\
-9 32 -15\n\
19 17 91\n\
-90 -100 28\n\
-56 -42 -4\n\
-99 -2 59\n\
-53 -4 60\n\
44 -17 18\n\
81 56 23\n\
-51 76 54\n\
52 29 14\n\
-38 70 12\n\
98 80 -28\n\
-39 95 -50\n\
74 54 -45\n\
-40 -58 4\n\
-61 5 90\n\
83 100 5\n\
-4 53 13\n\
-23 88 -68\n\
42 55 -33\n\
-66 67 5\n\
-80 23 6\n\
3 -37 -73\n\
-30 -12 -75\n\
72 -20 -45\n\
3 -30 -18\n\
14 64 80\n\
39 -52 49\n\
-99 -97 50\n\
64 -65 48\n\
-59 -72 42\n\
33 92 90\n\
-24 -50 -90\n\
-10 61 53\n\
87 99 -11\n\
-46 65 51\n\
12 -73 21\n\
39 -7 82\n\
85 89 4\n\
-84 -35 37\n\
74 15 8\n\
-54 13 20\n\
-6 -53 -10\n\
31 67 78\n\
74 -88 60\n\
-54 -56 43\n\
9 33 -57\n\
32 13 24\n\
-31 -72 8\n\
39 64 93\n\
-2 -1 65\n\
-72 13 65\n\
-31 33 -59\n\
2 64 35\n\
42 68 74\n\
56 -18 -20\n\
-28 -52 13\n\
-28 -82 -65\n\
-49 -56 -60\n\
-94 7 -56\n\
86 97 -89\n\
5 45 -22\n\
50 64 54\n\
-10 -98 63\n\
57 76 -34\n\
-84 29 -72\n\
-42 -38 -33\n\
-36 4 -33\n\
-97 -27 -80\n\
86 -53 -37\n\
-94 48 8\n\
-53 62 79\n\
-54 60 -38\n\
68 45 -82\n\
9 89 -30\n\
25 -2 -42\n\
-74 19 23\n\
86 57 -6\n\
-79 -11 98\n\
-37 68 -90\n\
61 -37 -99\n\
-64 -43 -83\n\
32 80 -42\n\
-91 82 -96\n\
-89 78 22\n\
-74 5 71\n\
37 -70 -82\n\
4 87 -56\n\
30 15 -40\n\
-79 -90 59\n\
34 41 7\n\
43 -49 82\n\
-100 71 24\n\
-14 -45 -62\n\
-35 48 53\n\
-94 -81 -61\n\
-13 -27 -23\n\
-31 51 -60\n\
-2 -20 -19\n\
-25 -7 -36\n\
-97 -93 64\n\
-31 -6 73\n\
42 -30 -74\n\
-76 85 -98\n\
83 72 -78\n\
-1 47 -70\n\
-11 -34 -68\n\
-8 27 -97\n\
-60 -4 65\n\
29 -81 83\n\
-83 -100 -78\n\
-14 -74 -15\n\
48 65 -27\n\
-24 -5 -58\n\
-99 -91 37\n\
-14 78 -35\n\
-73 43 72\n\
86 72 94\n\
67 39 43\n\
81 26 -96\n\
-52 20 -69\n\
-84 -83 -41\n\
-84 -55 -36\n\
-16 -34 90\n\
-64 -39 41\n\
50 -8 -67\n\
-16 78 61\n\
-20 -17 -85\n\
-5 -10 85\n\
-10 -32 81\n\
-98 -39 44\n\
43 72 16\n\
33 -46 -65\n\
-68 -5 67\n\
-95 -9 -66\n\
-12 56 -94\n\
93 -63 22\n\
56 98 -95\n\
78 82 -99\n\
78 28 -7\n\
-99 -14 21\n\
-43 -62 -4\n\
13 26 -66\n\
-1 -22 -61\n\
98 -44 22\n\
-69 -85 39\n\
30 -20 69\n\
-2 97 64\n\
47 4 -30\n\
2 1 38\n\
-48 -73 -9\n\
-94 91 -5\n\
-1 -26 -85\n\
72 40 -92\n\
-96 -49 -34\n\
20 72 -66\n\
-99 23 57\n\
61 -73 31\n\
-96 -5 10\n\
92 -100 -1\n\
83 -91 72\n\
-100 -96 -27\n\
2 18 46\n\
-51 38 -15\n\
-65 27 -66\n\
-34 27 37\n\
82 -19 66\n\
-40 93 -85\n\
-22 72 -11\n\
47 -32 -41\n\
77 -17 4\n\
53 80 46\n\
57 14 42\n\
51 20 47\n\
-79 57 63\n\
-68 -57 -43\n\
-33 -34 15\n\
8 -50 -49\n\
87 17 -32\n\
-97 99 -17\n\
56 95 -98\n\
69 82 81\n\
-27 -40 -66\n\
-61 -62 79\n\
-46 -77 -18\n\
17 -20 -50\n\
-81 24 80\n\
78 71 52\n\
-6 -7 -90\n\
4 44 47\n\
77 23 -31\n\
25 -17 -10\n\
-17 -56 -23\n\
-97 -5 -75\n\
56 92 17\n\
13 90 -45\n\
28 41 -97\n\
90 61 29\n\
-80 26 32\n\
27 -67 12\n\
65 81 14\n\
-89 -52 70\n\
-59 -78 75\n\
15 68 30\n\
-65 -48 -6\n\
-100 -55 14\n\
53 29 13\n\
-96 -94 2\n\
98 -51 -35\n\
-8 20 -81\n\
-84 64 -9\n\
-40 80 14\n\
-78 -13 25\n\
84 93 58\n\
-93 43 -48\n\
27 10 21\n\
20 -46 27\n\
-64 -75 -56\n\
-95 -38 -43\n\
81 48 22\n\
86 -46 -58\n\
-55 100 10\n\
-24 -97 81\n\
-30 79 72\n\
-51 12 -74\n\
-20 27 39\n\
-41 -87 18\n\
57 -92 46\n\
72 -100 9\n\
70 -22 -52\n\
-36 -13 -19\n\
94 -31 38\n\
61 -37 -97\n\
36 -47 67\n\
84 58 -61\n\
27 37 -67\n\
96 5 -86\n\
40 -75 -4\n\
-4 -17 24\n\
-9 -45 -71\n\
-23 7 89\n\
-48 22 -31\n\
-21 -12 -100\n\
61 -10 1\n\
39 -16 -84\n\
30 -2 -61\n\
-62 1 -93\n\
-2 49 -89\n\
19 51 -67\n\
-88 37 63\n\
24 -34 -77\n\
-3 -87 -56\n\
-84 69 -41\n\
38 -45 -32\n\
83 -80 89\n\
62 98 54\n\
95 42 -27\n\
-42 -20 -62\n\
-64 29 33\n\
48 -15 -5\n\
57 -23 77\n\
-50 55 -56\n\
70 47 12\n\
-20 -93 -33\n\
-78 -35 79\n\
61 28 -37\n\
-87 -77 -7\n\
-32 -28 11\n\
83 49 47\n\
-12 71 90\n\
-54 69 90\n\
42 -93 -88\n\
-47 87 -97\n\
72 -33 2\n\
28 -79 8\n\
46 26 18\n\
46 57 -17\n\
-3 52 59\n\
-53 -81 41\n\
-91 27 -86\n\
-48 89 -84\n\
90 69 -56\n\
77 4 13\n\
-43 55 -2\n\
-91 -99 -26\n\
35 45 -58\n\
-17 100 49\n\
33 82 -53\n\
-67 59 10\n\
-69 34 17\n\
63 -45 -59\n\
1 -79 -68\n\
-98 84 -55\n\
5 -95 -30\n\
32 5 -48\n\
88 59 -20\n\
-9 99 56\n\
55 -32 -83\n\
10 55 -83\n\
55 -50 -58\n\
-64 -63 -31\n\
72 79 -49\n\
56 -90 -30\n\
-92 3 -45\n\
-27 -82 -12\n\
-56 48 49\n\
-37 2 98\n\
84 85 22\n\
2 -15 18\n\
9 -15 26\n\
-7 55 -57\n\
-74 -78 89\n\
4 -46 3\n\
-41 61 40\n\
3 15 42\n\
-37 -42 95\n";// not satisfiable


*/


/*

"-97 89 18\n\
37 52 -71\n\
40 -46 -83\n\
-54 -24 -66\n\
-27 -55 84\n\
-71 -72 -27\n\
-23 92 22\n\
-50 77 32\n\
49 54 -72\n\
31 -85 -37\n\
-98 -30 -2\n\
9 76 94\n\
-51 -90 2\n\
-89 33 86\n\
-14 -68 -100\n\
26 -92 -88\n\
-99 -86 -48\n\
17 37 50\n\
49 -7 66\n\
50 82 -71\n\
-7 -58 57\n\
67 -83 20\n\
74 91 -43\n\
56 -57 68\n\
-25 -40 16\n\
59 -96 -5\n\
-49 -57 71\n\
74 -44 36\n\
77 23 -100\n\
-79 -69 31\n\
43 21 15\n\
36 -84 61\n\
-51 25 74\n\
29 -18 63\n\
-20 41 76\n\
-70 78 -59\n\
88 -60 78\n\
-78 16 -54\n\
-91 -30 -11\n\
61 79 13\n\
73 -98 25\n\
25 69 -8\n\
-30 94 -88\n\
-77 -36 -19\n\
-60 -50 -56\n\
19 -58 -82\n\
-92 14 31\n\
-72 45 -41\n\
-48 -4 20\n\
45 50 58\n\
-85 -41 -1\n\
-29 -76 3\n\
7 46 -61\n\
-31 53 -92\n\
59 -64 -32\n\
-59 49 -94\n\
-49 44 52\n\
67 -10 -72\n\
16 87 29\n\
89 95 -12\n\
-73 71 25\n\
-43 78 -84\n\
-20 95 -96\n\
-67 39 -38\n\
-43 -34 -69\n\
-31 -26 -51\n\
-96 35 -58\n\
-66 48 81\n\
13 6 -42\n\
-23 -44 -20\n\
72 -30 -98\n\
-1 -81 -24\n\
45 -25 100\n\
-29 99 44\n\
-56 -71 -89\n\
48 -33 16\n\
-35 55 82\n\
10 -58 -66\n\
-47 -11 -50\n\
36 37 67\n\
11 59 61\n\
28 -38 -69\n\
49 5 39\n\
66 -60 32\n\
-13 86 -48\n\
-8 20 18\n\
82 -74 -58\n\
31 -57 -69\n\
12 -30 21\n\
39 -65 80\n\
-26 94 -56\n\
74 -14 -34\n\
54 -26 43\n\
11 83 42\n\
63 -48 -86\n\
81 -92 -53\n\
12 -58 6\n\
-31 -35 -26\n\
86 -68 -79\n\
49 62 36\n\
93 -84 46\n\
-32 35 44\n\
-8 51 -70\n\
-67 -75 46\n\
-43 35 -69\n\
69 22 63\n\
-52 72 -74\n\
38 56 40\n\
74 -3 -22\n\
52 39 -30\n\
-90 -83 89\n\
-92 47 2\n\
-54 -79 6\n\
-39 -60 45\n\
-55 29 -81\n\
52 -39 -77\n\
-71 82 -20\n\
23 -100 40\n\
69 -31 -85\n\
55 45 92\n\
29 81 -79\n\
-78 -92 -51\n\
-58 86 -97\n\
23 -82 19\n\
-71 -49 -54\n\
-2 12 -63\n\
50 -95 75\n\
-98 50 23\n\
-82 -70 17\n\
-8 48 61\n\
43 -12 -55\n\
-88 42 -66\n\
100 39 -7\n\
-59 14 -22\n\
87 -21 -47\n\
90 25 -67\n\
-16 -17 100\n\
39 80 81\n\
98 61 27\n\
-19 -22 61\n\
-21 59 -67\n\
46 19 -13\n\
-87 -39 -71\n\
80 82 -33\n\
46 -20 -91\n\
49 -5 18\n\
12 -38 39\n\
-16 70 -78\n\
-59 16 43\n\
60 -68 63\n\
9 23 51\n\
-46 -50 -81\n\
-23 -89 -33\n\
62 33 85\n\
48 83 74\n\
56 -41 71\n\
49 -11 59\n\
92 17 -20\n\
-93 -78 27\n\
-83 29 -33\n\
-4 -94 -5\n\
83 -49 -80\n\
-72 -69 91\n\
-86 -12 63\n\
96 -93 1\n\
27 62 -79\n\
-76 -11 -2\n\
22 -76 65\n\
-100 68 61\n\
7 -30 -15\n\
-12 90 -33\n\
-51 92 -74\n\
-25 -65 34\n\
-38 78 84\n\
20 -46 47\n\
6 -65 99\n\
-1 -5 6\n\
-33 -80 -14\n\
-82 6 70\n\
83 -25 54\n\
39 64 71\n\
-51 -2 -29\n\
5 -9 64\n\
-10 57 38\n\
87 5 67\n\
47 72 -88\n\
-42 59 -91\n\
55 19 -76\n\
-51 45 8\n\
-31 10 -34\n\
21 97 39\n\
-98 56 5\n\
55 -81 -37\n\
-82 50 17\n\
-19 -69 -50\n\
-20 -87 96\n\
5 -93 -64\n\
-27 -35 -70\n\
-35 47 -19\n\
92 56 -26\n\
39 -28 -78\n\
43 -91 66\n\
-95 -26 81\n\
-33 -24 93\n\
9 67 -84\n\
26 -46 20\n\
76 22 -97\n\
-87 22 67\n\
-19 35 6\n\
78 -76 18\n\
38 27 29\n\
-17 -27 31\n\
44 -75 -48\n\
24 78 93\n\
-63 26 -77\n\
90 75 -85\n\
-53 -31 -17\n\
-11 33 45\n\
17 -66 83\n\
-11 56 -55\n\
46 26 -67\n\
21 -97 -12\n\
-8 91 -32\n\
56 -65 41\n\
68 -84 90\n\
-15 17 -51\n\
-62 67 -92\n\
73 -11 62\n\
79 -19 75\n\
53 25 33\n\
69 62 -19\n\
14 -4 32\n\
77 -48 47\n\
49 31 -27\n\
72 2 -7\n\
7 12 -15\n\
91 45 60\n\
58 -26 84\n\
13 12 66\n\
23 7 -17\n\
-83 17 72\n\
54 -34 -48\n\
49 14 -23\n\
68 -31 99\n\
-48 92 -40\n\
-24 4 68\n\
99 -55 -43\n\
-44 17 12\n\
-53 17 61\n\
85 -98 23\n\
75 -85 47\n\
-51 79 -2\n\
92 67 88\n\
-34 86 53\n\
25 21 -100\n\
90 -20 45\n\
-84 12 3\n\
56 97 94\n\
80 34 12\n\
34 -53 69\n\
93 89 -33\n\
-62 -9 5\n\
-90 -51 6\n\
9 73 -40\n\
69 -39 20\n\
-50 64 7\n\
-76 63 41\n\
-90 -16 -86\n\
1 8 -53\n\
7 97 -96\n\
44 -83 31\n\
82 41 94\n\
57 10 36\n\
-23 5 -43\n\
32 -52 -67\n\
94 -21 83\n\
56 -52 89\n\
70 -30 20\n\
-41 -95 -29\n\
54 2 25\n\
21 23 -22\n\
52 -3 44\n\
-51 23 47\n\
95 -69 -14\n\
97 7 84\n\
22 -48 -36\n\
-36 3 86\n\
95 -94 -61\n\
-60 58 -73\n\
-77 17 -40\n\
-19 91 -88\n\
-98 -45 -56\n\
71 82 54\n\
72 -3 100\n\
100 -60 -26\n\
-92 76 60\n\
49 36 47\n\
-98 -82 -16\n\
-93 66 62\n\
3 10 -5\n\
-97 41 -37\n\
58 -69 11\n\
-38 70 -4\n\
13 -63 74\n\
8 83 -23\n\
-32 -73 -21\n\
45 61 6\n\
86 3 32\n\
50 36 -92\n\
-42 -49 -47\n\
-49 15 52\n\
-65 -47 6\n\
99 80 -41\n\
58 -67 99\n\
-13 -32 -88\n\
-40 45 -87\n\
36 79 -41\n\
-47 10 -19\n\
-34 -93 81\n\
-96 14 -8\n\
-95 -55 -66\n\
-21 -47 89\n\
-40 42 -28\n\
41 51 47\n\
41 -29 -43\n\
-28 -78 -7\n\
47 84 21\n\
-55 85 -58\n\
-36 84 -25\n\
-11 -46 -87\n\
5 23 48\n\
86 -63 95\n\
-23 53 -25\n\
-54 -71 -52\n\
37 63 -3\n\
93 19 40\n\
16 -97 -45\n\
45 -48 -9\n\
-78 53 -28\n\
94 -67 -86\n\
9 1 2\n\
98 -46 32\n\
26 12 -74\n\
10 -13 8\n\
21 -63 -80\n\
-89 87 -61\n\
-25 -33 36\n\
-84 1 15\n\
13 -44 63\n\
33 1 -96\n\
73 -95 -61\n\
-25 -10 28\n\
-64 50 3\n\
-28 20 -81\n\
23 -73 -62\n\
-92 -57 64\n\
-48 -15 57\n\
-77 39 -72\n\
-15 44 98\n\
56 -57 -9\n\
86 66 50\n\
-96 65 55\n\
75 -16 -68\n\
24 10 -27\n\
-58 -61 -18\n\
60 63 -57\n\
49 -7 -38\n\
-33 -93 -100\n\
2 98 -10\n\
81 62 -88\n\
6 2 36\n\
-65 -20 47\n\
-100 -11 17\n\
-83 91 1\n\
-63 93 65\n\
-30 -46 5\n\
74 -25 38\n\
-43 -92 26\n\
-63 -73 5\n\
67 -28 11\n\
34 -9 -96\n\
-57 -47 -81\n\
-9 -64 34\n\
-31 -27 -53\n\
-61 -20 82\n\
-25 -7 -27\n\
20 -26 63\n\
-21 -100 -99\n\
-3 -98 -80\n\
27 52 -92\n\
-39 68 -97\n\
48 -51 -1\n\
45 84 -92\n\
-96 -42 56\n\
47 24 99\n\
28 70 9\n\
-100 -82 -89\n\
-92 -37 -36\n\
-42 -76 16\n\
44 98 -36\n\
-83 -63 -37\n\
30 -86 53\n\
71 -78 -82\n\
-80 -3 -37\n\
7 -42 86\n\
10 -66 -84\n\
-17 87 57\n\
-14 -75 -20\n\
-56 -61 -82\n\
-81 62 23\n\
-88 -15 72\n\
-42 76 14\n\
-40 -59 -94\n\
39 9 -35\n\
1 83 -52\n\
-29 36 70\n\
27 48 -6\n\
-59 85 -9\n\
-27 81 -71\n\
50 2 55\n\
57 -52 38\n\
-56 79 93\n\
11 78 -23\n\
-81 56 46\n\
-23 -66 -48\n\
80 23 -24\n\
-62 -3 -53\n\
92 -74 -49\n\
79 74 -35\n\
71 47 -17\n";//satisfiable
*/

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
