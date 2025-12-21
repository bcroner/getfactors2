// 3SAT_framework.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATFRAMEWORK_H__
#define __3SATFRAMEWORK_H__


struct nlist { /* table entry: */
    struct nlist* next; /* next entry in chain */
    char* name; /* defined name */
    char* defn; /* replacement text */
};

// single bit

typedef struct {

    __int64 id;

} bit_3sat;

// integer with decimal

typedef struct {

    bit_3sat** bits; // bit string
    __int64   sz; // size in bits of number
    __int64   bd_sz; // before decimal size
    __int64   ad_sz; // after decimal size

} dec_3sat;

// natural number

typedef struct {

    bit_3sat** bits; // bit string
    __int64   sz; // size in bits of number

} nat_3sat;

bit_3sat* create_bit(__int64 * num_para);

dec_3sat* create_dec(__int64 * num_para, __int64 bd_sz_parm, __int64 ad_sz_parm);

nat_3sat* create_nat(__int64 * num_para, __int64 sz_parm);

char* and_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* nand_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* or_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* nor_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* not_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, __int64 * len_para);

char* xor_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* xnor_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* bitaddsum_3sat(__int64 * num_para, bit_3sat** sum, bit_3sat* c_in, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* bitaddcout_3sat(__int64 * num_para, bit_3sat** c_out, bit_3sat* c_in, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* implies_3sat(__int64 * num_para, bit_3sat** c, bit_3sat* a, bit_3sat* b, __int64 * len_para);

char* dec_add(__int64 * num_para, dec_3sat** c, dec_3sat* a, dec_3sat* b, bool discard_final_c_out, __int64* len_para);

char* nat_add(__int64 * num_para, nat_3sat** c, nat_3sat* a, nat_3sat* b, bool discard_final_c_out, __int64* len_para);

char* twos_comp_3sat(__int64 * num_para, dec_3sat** c, dec_3sat* a, __int64 * len_para);

char* dec_sub(__int64 * num_para, dec_3sat** c, dec_3sat* a, dec_3sat* b, __int64 * len_para);

char* dec_equals(__int64 * num_para, dec_3sat* a, dec_3sat* b, bool eq, __int64* len_para);

char* nat_equals(__int64 * num_para, nat_3sat* a, nat_3sat* b, bool eq, __int64* len_para);

char* dec_not_equals(__int64 * num_para, dec_3sat* a, dec_3sat* b, __int64* len_para);

char* nat_not_equals(__int64 * num_para, nat_3sat* a, nat_3sat* b, __int64* len_para);

char* dec_mul(__int64 * num_parm, dec_3sat** c, dec_3sat* a, dec_3sat* b, __int64 bd_sz, __int64 ad_sz, __int64* len_para);

char* nat_mul(__int64 * num_para, nat_3sat** c, nat_3sat* a, nat_3sat* b, __int64 sz, __int64* len_para);

char* dec_div(__int64 * num_parm, dec_3sat * c, dec_3sat * a, dec_3sat ** b, __int64* len_para);

char* dec_sqrt(__int64 * num_para, dec_3sat* c, dec_3sat** a, __int64* len_para);

//void bignum_from_str_oct(bn* c_bn, char* c_str, __int64 len);

inline __int64 octal_from_char(char c);

//void bignum_from_str_oct(bn* c_bn, char* c_str, int len);

inline __int64 decimal_from_char(char c);

inline __int64 int_from_hex_char(char c);

inline char hex_char_from_int(__int64 a);

bool* hex2bool(char* a, __int64 sz);

//void bignum_from_str_dec(bn* c_bn, char* c_str, __int64 len);

__int64** input_from_char_buf(char* buf_3sat, __int64 buf_3sat_sz, __int64* k, bool cnf);

char* dec_to_str(bool* decodable_buf, dec_3sat* a, __int64* str_sz);

char* nat_to_str(bool* decodable_buf, nat_3sat* a, __int64* str_sz);

char* dec_get_factors(char* c_str, __int64 c_str_buf_sz, __int64 * len_para);

char* nat_get_factors(char* c_str, __int64 c_str_buf_sz, __int64 * len_para);

#endif // !__3SATFRAMEWORK_H__