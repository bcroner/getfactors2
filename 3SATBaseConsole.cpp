// 3SATBase.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef __3SATBASECONSOLE_C__
#define __3SATBASECONSOLE_C__

#include "3SATBaseConsole.hpp"

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <thread>

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
	__int64 * stack = new __int64 [h - l + 1];

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

bool SATSolver_less_than(__int64 a, __int64 b) {

	__int64 abs_a = a < 0 ? -a : a;
	__int64 abs_b = b < 0 ? -b : b;

	if ((abs_a < abs_b) || (abs_a == abs_b && a < b))
		return true;
	else
		return false;

}

/*
* 
* add- add a number 2^pos_parm to Z
* 
*/

bool SATSolver_add(SATSolver * me , __int64 cls_ix, __int64 * prev) {

	printf_s("cls %-3lld ", cls_ix);

	__int64 top = 0;
	__int64 abs_top = 0;
	__int64 jump = me->master->jumps[cls_ix] < 0 ? me->master->jumps[cls_ix] + 1 : me->master->jumps[cls_ix] - 1;
	__int64 abs_jump = jump < 0 ? -jump : jump;

	bool base_access = false;
	bool limit_access = false;


	// base access variables

	__int64 limit_0 = SATSolver_less_than(me->master->limits[prev[0]], me->master->limits[prev[1]]) ? me->master->limits[prev[0]] : me->master->limits[prev[1]];
	__int64 limit_1 = SATSolver_less_than(me->master->limits[prev[2]], me->master->limits[cls_ix]) ? me->master->limits[prev[2]] : me->master->limits[cls_ix];

	__int64 base_min = 0;
	__int64 limit_min = 0;

	__int64 l0 = me->master->limits[prev[0]] < 0 ? -me->master->limits[prev[0]] : me->master->limits[prev[0]];
	__int64 l1 = me->master->limits[prev[1]] < 0 ? -me->master->limits[prev[1]] : me->master->limits[prev[1]];
	__int64 l2 = me->master->limits[prev[2]] < 0 ? -me->master->limits[prev[2]] : me->master->limits[prev[2]];
	__int64 l3 = me->master->limits[cls_ix ] < 0 ? -me->master->limits[cls_ix ] : me->master->limits[cls_ix ];

	// check for base access

	bool base_has_void = false;
	for (__int64 i = 0; i < 3; i++)
		if (prev[i] == -1)
			base_has_void = true;

	if (! base_has_void ) {

		// check if we can do this

		if ((me->master->limits[prev[0]] == -me->master->limits[prev[2]] ||
			me->master->limits[prev[0]] == -me->master->limits[cls_ix] || 
			me->master->limits[prev[1]] == -me->master->limits[prev[2]] || 
			me->master->limits[prev[1]] == -me->master->limits[cls_ix]) &&
			(l0 == l2 && l0 == l3) && (l1 == l2 && l1 == l3))
			base_access = true;
	}

	// check for limit access

	bool limit_has_void = false;
	if (prev[2] == -1)
		limit_has_void = true;

	if (!limit_has_void && limit_1 != FALSE_3SAT)
		limit_access = -me->master->jumps[prev[2]] == me->master->jumps[cls_ix];
	///*
	if (base_access) {

		// determine the minimum base among the four clauses

		base_min = me->master->bases[prev[0]];

		for (__int64 i = 1; i < 3; i++)
			base_min = SATSolver_less_than(base_min, me->master->bases[prev[i]]) ? base_min : me->master->bases[prev[i]];

		// clear prev

		//for (__int64 i = 0; i < 3; i++)
		//	prev[i] = -1;

		// reset clause index

		//*cls_ix = -1;

		jump = base_min < 0 ? base_min + 1 : base_min - 1;

		abs_jump = jump < 0 ? -jump : jump;

		printf_s("%-6s %-5lld ", "base", jump);

	}
	//*/
	if (limit_access) {

		limit_min = SATSolver_less_than(me->master->limits[prev[2]], me->master->limits[cls_ix]) ? me->master->limits[prev[2]] : me->master->limits[cls_ix];

		jump = limit_min < 0 ? limit_min + 1 : limit_min - 1;

		abs_jump = jump < 0 ? -jump : jump;

		printf_s("%-6s %-5lld ", "limit", jump);

	}
	else
		printf_s("%-6s %-5lld ", "jump", jump);

	top = jump;
	abs_top = abs_jump;

	while ( abs_top < me->master->n ) {
		if (me->Z[abs_top])
			me->Z[abs_top] = false;
		else {
			me->Z[abs_top] = true;
			break;
		}
		abs_top++;
	}

	// zero out all lower bits of Z

	for (__int64 j = abs_top - 1; j >= 0; j--)
		me->Z[j] = false;

	return abs_top >= me->master->n - me->master->chops;
}

__int64 SATSolver_initializePowJump(SATSolver* me, __int64 * prev) {

	// initialize return value

	__int64 max_effective_jump = 0;
	__int64 abs_m_efftive_jump = 0;
	__int64 cls_ix = -1;
	bool limit_has_void = prev[2] == -1;
	bool base_has_void = prev[0] == -1 || prev[1] == -1 || prev[2] == -1;

	// check if any clauses are satisfied and find jump jumps corresponding to clauses

	for (__int64 i = 0; i < me->master->k; i++) {
		
		__int64 count_matches = 0;

		for (__int64 j = 0; j < 3; j++) {

			if (me->master->lst[i][j] == FALSE_3SAT) {
				count_matches++;
				continue;
			}

			__int64 abs_lst = me->master->encoding[me->master->lst[i][j] < 0 ? -me->master->lst[i][j] - 2 : me->master->lst[i][j] - 2];
			if ((me->master->lst[i][j] < 0 && !me->Z[abs_lst]) || (me->master->lst[i][j] > 0 && me->Z[abs_lst]))
				count_matches++;
		}

		if (count_matches < 3)
			continue;

		__int64 temp_base = me->master->bases[i];
		__int64 abs_temp_base = temp_base < 0 ? -temp_base : temp_base;
		__int64 temp_limit = me->master->limits[i];
		__int64 abs_temp_limit = temp_limit < 0 ? -temp_limit : temp_limit;
		__int64 temp_jump = me->master->jumps[i];
		__int64 abs_temp_jump = temp_jump < 0 ? -temp_jump : temp_jump;
		__int64 prcsd_limit = prev[2] < 0 ? 0 : SATSolver_less_than(me->master->limits[prev[2]], temp_limit) ? me->master->limits[prev[2]] : temp_limit;

		__int64 limit_0 = (prev[0] < 0 || prev[1] < 0) ? 0 : SATSolver_less_than(me->master->limits[prev[0]], me->master->limits[prev[1]]) ? me->master->limits[prev[0]] : me->master->limits[prev[1]];
		__int64 limit_1 = (prev[2] < 0) ? 0 : SATSolver_less_than(me->master->limits[prev[2]], me->master->limits[i]) ? me->master->limits[prev[2]] : me->master->limits[i];

		__int64 l0 = me->master->limits[prev[0]] < 0 ? -me->master->limits[prev[0]] : me->master->limits[prev[0]];
		__int64 l1 = me->master->limits[prev[1]] < 0 ? -me->master->limits[prev[1]] : me->master->limits[prev[1]];
		__int64 l2 = me->master->limits[prev[2]] < 0 ? -me->master->limits[prev[2]] : me->master->limits[prev[2]];
		__int64 l3 = me->master->limits[i      ] < 0 ? -me->master->limits[i      ] : me->master->limits[i      ];

		__int64 min_base = prev[0] < 0 ? 0 : me->master->bases[prev[0]];
		for (__int64 i = 1; i < 3; i++)
			min_base = prev[i] < 0 ? 0 : SATSolver_less_than(min_base, me->master->bases[prev[i]]) ? min_base: me->master->bases[prev[i]];
		min_base = SATSolver_less_than(min_base, temp_base) ? min_base : temp_base;

		if (SATSolver_less_than (max_effective_jump, temp_jump)) {
			cls_ix = i;
			max_effective_jump = temp_jump;
			abs_m_efftive_jump = max_effective_jump < 0 ? - max_effective_jump : max_effective_jump ;
		}
		if (!limit_has_void && -temp_jump == me->master->jumps[prev[2]] && abs_m_efftive_jump != l1 && abs_m_efftive_jump != l2 && SATSolver_less_than(max_effective_jump, prcsd_limit)) {
			cls_ix = i;
			max_effective_jump = prcsd_limit;
			abs_m_efftive_jump = max_effective_jump < 0 ? -max_effective_jump : max_effective_jump;
		}
		if (!limit_has_void && -temp_jump == me->master->jumps[prev[2]] && (abs_temp_limit == l1 && abs_temp_limit == l2) &&
			(abs_m_efftive_jump != abs_temp_limit || (abs_m_efftive_jump == abs_temp_limit && SATSolver_less_than (me->master->bases[cls_ix], me->master->bases[i])))) {
			cls_ix = i;
			max_effective_jump = prcsd_limit;
			abs_m_efftive_jump = max_effective_jump < 0 ? -max_effective_jump : max_effective_jump;
		}
		if (!base_has_void && - me->master->jumps[prev[0]] == me->master->jumps[prev[1]] && - me->master->jumps[prev[2]] == temp_jump &&
			(me->master->limits[prev[0]] == -me->master->limits[prev[2]] ||
				me->master->limits[prev[0]] == -me->master->limits[i] ||
				me->master->limits[prev[1]] == -me->master->limits[prev[2]] ||
				me->master->limits[prev[1]] == -me->master->limits[i]) &&
			(l0 == l2 && l0 == l3) && (l1 == l2 && l1 == l3) && SATSolver_less_than(max_effective_jump, min_base)) {
			cls_ix = i;
			max_effective_jump = min_base;
			abs_m_efftive_jump = max_effective_jump < 0 ? -max_effective_jump : max_effective_jump;
		}
	}

	return cls_ix;

}

bool SATSolver_GreaterThan(bool* a, bool* b, __int64 n) {

	__int64 i;

	for (i = n - 1; i >= 0; i--)
		if (a[i] && !b[i])
			return true;
		else if (!a[i] && b[i])
			return false;

	return false;

}

bool SATSolver_isSat(SATSolver* me, __int64 chop, bool* arr) {

	bool jump_occurred = false;
	bool crossed_boundary = false;

	__int64* prev = new __int64[3];

	for (__int64 i = 0; i < 3; i++)
		prev[i] = -1;
		
	printf_s("chop: %lld\n", chop);

	for (__int64 i = 0; i < me->master->n; i++)
		me->Z[i] = me->master->begin[chop][i];

	if (!true)
		for (__int64 i = 0; i < me->master->n; i++)
			printf_s("%lld", (__int64)me->Z[i]);
	printf_s("\n");

	// main loop- until end condition

	__int64 count = 0;

	__int64 cls_ix = SATSolver_initializePowJump(me, prev);
	
	if (cls_ix == -1) {
		for (__int64 i = 0; i < me->master->n; i++)
			arr[me->master->decoding[i]] = !me->Z[i];
		return true;
	}
	else
		jump_occurred = true;

	crossed_boundary = SATSolver_add(me, cls_ix, prev);

	for (__int64 i = 0; i < 2; i++)
		prev[i] = prev[i + 1];

	prev [2] = cls_ix;

	count++;

	//if (count % (1 * 1048576) == 0) {

	if (true) {
		for (__int64 i = 0; i < me->master->n; i++)
			printf_s("%lld", (__int64) me->Z[i]);
		printf_s("\n");
	}

	while ( ! crossed_boundary && jump_occurred) {

		jump_occurred = false;

		cls_ix = SATSolver_initializePowJump(me, prev);

		if (cls_ix == -1)
			break;
		else
			jump_occurred = true;

		crossed_boundary = SATSolver_add(me, cls_ix, prev);

		for (__int64 i = 0; i < 2; i++)
			prev[i] = prev[i + 1];

		prev[2] = cls_ix;

		count++;

		//if (count % (1 * 1048576) == 0) {

		if (true) {

			for (__int64 i = 0; i < me->master->n; i++)
				printf_s("%lld", (__int64) me->Z[i]);
			printf_s("\n");
		}

	}

	if (!true && jump_occurred) {
		for (__int64 i = 0; i < me->master->n; i++)
			printf_s("%lld", (__int64)me->Z[i]);
		printf_s("\n");
	}

	printf_s("count: %lld\n", (__int64)count);

	if (crossed_boundary || jump_occurred)
		return false;

	for (__int64 i = 0; i < me->master->n; i++)
		arr[me->master->decoding[i]] = ! me->Z[i];

	return true;
}

bool * SATSolver_bool_pow(bool* base, __int64 pow, __int64 n) {

	bool* prod = new bool[n];
	for (__int64 i = 0; i < n ; i++)
		prod[i] = false;
	prod[pow] = true;

	return prod;

}

bool* SATSolver_bool_mul(bool* a, bool* b, __int64 n) {

	bool* sum = new bool[n];

	for (__int64 i = 0; i < n; i++)
		sum[i] = false;

	for (__int64 i = 0; i < n; i++) {
		bool* temp = new bool[n];

		for (__int64 j = 0; j < i; j++)
			temp[n - 1 - j] = false;

		for (__int64 j = 0; j < n - i; j++)
			temp[n - 1 - j - i] = b[n - 1 - i] && a[n - 1 - j];

		bool* new_sum = SATSolver_bool_add(temp, sum, n);
		bool* dump = sum;
		sum = new_sum;
		delete[] dump;
		delete[] temp;
	}

	return sum;

}

bool* SATSolver_bool_add(bool* a, bool* b, __int64 n) {

	bool carry = false;

	bool* ret = new bool[n];

	for (__int64 i = 0; i < n; i++) {

		bool sum_temp = (!a[i] && b[i]) || (a[i] && !b[i]);
		ret[i] = (!sum_temp && carry) || (sum_temp && !carry);
		carry = (a[i] && b[i]) || (a[i] && carry) || (b[i] && carry);

	}

	return ret;

}

bool* SATSolver_bool_prepare_end(bool* a, bool* b, __int64 n) {

	bool* ret = SATSolver_bool_add(a, b, n);

	__int64 position_first_true_bit;

	for (position_first_true_bit = n; !b[position_first_true_bit] && position_first_true_bit >= 0; position_first_true_bit--)
		;

	for (__int64 i = position_first_true_bit - 1; i >= 0; i--)
		ret[i] = true;

	return ret;

}


bool* SATSolver_int2bool(__int64 a, __int64 n_parm) {

	// create the return boolean array and initialize

	bool* ret = new bool[n_parm];

	for (__int64 i = 0; i < n_parm; i++)
		ret[i] = false;


	// for each bit position, determine true/false value, using a signed 64 bit signed integer

	for (__int64 i = 64 - 2; i >= 0; i--) {

		// form simple power
		__int64 simple_pow = 1;

		for (__int64 j = 0; j < i; j++)
			simple_pow *= 2;

		if (a >= simple_pow) {
			ret[i] = true;
			a = a - simple_pow;
		}
	}

	return ret;

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

/* instantiate SATSolver class with list of clauses
* 
* lst: list of clauses
* k_parm: number of clauses (size of lst)
* n_parm: number of variables
* chop: power of 2, chopping up search space
* pos: position in chopping up search space
* 
* */
void SATSolver_create(SATSolver * me, SATSolverMaster * master , __int64** lst, __int64 k_parm, __int64 n_parm, __int64 chop) {

	// valcheck

	if (lst == NULL)
		return;
	if (k_parm <= 0)
		return;
	if (n_parm <= 0)
		return;

	// instantiate class member variables

	me->master = master;
	me->Z = new bool [n_parm];

}

void SATSolverMaster_create(SATSolverMaster* master, __int64** lst, __int64 k_parm, __int64 n_parm, __int64 chops_parm) {

	// chops

	master->chops = chops_parm;

	// calculate chops

	if (master->chops < 2)
		master->chops = 2;

	__int64 search_sz = 1;

	for (__int64 i = 0; i < master->chops; i++)
		search_sz *= 2;

	master->begin = new bool* [search_sz];

	// set up first element to check: me->begin

	for (__int64 chop = 0; chop < search_sz; chop++)
		master->begin[chop] = SATSolver_create_boundary(true, master->chops, chop, n_parm);

	// populate histogram of the variables

	master->n = n_parm;
	master->k = k_parm;

	__int64* histogram = new __int64[n_parm];

	for (__int64 i = 0; i < n_parm; i++)
		histogram[i] = 0;

	for (__int64 i = 0; i < k_parm; i++)
		for (__int64 j = 0; j < 3; j++) {
			// check for existence of true TRUE_3SAT or false FALSE_3SAT
			if (lst[i][j] == FALSE_3SAT || lst[i][j] == TRUE_3SAT)
				continue;
			__int64 tmp = (lst[i][j] < 0 ? -lst[i][j] : lst[i][j]) - 2;
			__int64 cur = histogram[tmp];
			histogram[tmp] = cur + 1;
		}

	// create a sorted copy of the histogram

	__int64* histogram_srt = new __int64[n_parm];

	for (__int64 i = 0; i < n_parm; i++)
		histogram_srt[i] = histogram[i];

	MyQSort(histogram_srt, 0, n_parm - 1);

	// create a decoding based on the histogram

	master->decoding = new __int64[n_parm];
	master->encoding = new __int64[n_parm];

	for (__int64 i = 0; i < n_parm; i++) {
		__int64 count = histogram_srt[i];
		__int64 pos = 0;
		while (histogram[pos] != count)
			pos++;
		master->decoding[n_parm - 1 - i] = pos;
		master->encoding[pos] = n_parm - 1 - i;
		histogram[pos] = -1;
	}

	master->lst = lst;

	// order list of k clauses in cls_tly (clause tally) by lowest-order literal of each clause

	master->jumps = new __int64[k_parm];
	master->limits = new __int64[k_parm];
	master->bases = new __int64[k_parm];
	for (__int64 i = 0; i < k_parm; i++) {
		master->jumps[i] = 0;
		master->limits[i] = 0;
		master->bases[i] = 0;
	}

	for (__int64 i = 0; i < k_parm; i++) {

		__int64 count_tf = 0;
		for (int j = 0; j < 3; j++)
			count_tf += lst[i][j] == TRUE_3SAT || lst[i][j] == FALSE_3SAT ? 1 : 0;

		__int64 ix = 0;
		__int64 mylst[3];
		for (int j = 0; j < 3; j++)
			if (lst[i][j] != TRUE_3SAT && lst[i][j] != FALSE_3SAT) {
				mylst[ix] = lst[i][j];
				ix++;
			}

		if (count_tf == 0) {

			__int64 absl0 = mylst[0] < 0 ? -mylst[0] : mylst[0];
			__int64 absl1 = mylst[1] < 0 ? -mylst[1] : mylst[1];
			__int64 absl2 = mylst[2] < 0 ? -mylst[2] : mylst[2];

			
			__int64 a = master->encoding[absl0 - 2] + 1;
			__int64 b = master->encoding[absl1 - 2] + 1;
			__int64 c = master->encoding[absl2 - 2] + 1;

			__int64 hi = 0;
			__int64 md = 0;
			__int64 lo = 0;

			hi = a;
			if (b > hi)
				hi = b;
			if (c > hi)
				hi = c;

			lo = a;
			if (b < lo)
				lo = b;
			if (c < lo)
				lo = c;

			md = a;
			if (lo == md || hi == md)
				md = b;
			if (lo == md || hi == md)
				md = c;

			master->jumps[i] = lo == a ? mylst[0] < 0 ? -lo : lo : lo == b ? mylst[1] < 0 ? -lo : lo : mylst[2] < 0 ? -lo : lo;
			master->limits[i] = md == a ? mylst[0] < 0 ? -md : md : md == b ? mylst[1] < 0 ? -md : md : mylst[2] < 0 ? -md : md;
			master->bases[i] = hi == a ? mylst[0] < 0 ? -hi : hi : hi == b ? mylst[1] < 0 ? -hi : hi : mylst[2] < 0 ? -hi : hi;

		}
		else if (count_tf == 1) {

			__int64 absl0 = mylst[0] < 0 ? -mylst[0] : mylst[0];
			__int64 absl1 = mylst[1] < 0 ? -mylst[1] : mylst[1];


			__int64 a = master->encoding[absl0 - 2] + 1;
			__int64 b = master->encoding[absl1 - 2] + 1;

			__int64 hi = 0;
			__int64 lo = 0;

			hi = a;
			if (b > hi)
				hi = b;

			lo = a;
			if (b < lo)
				lo = b;

			master->jumps[i] = lo == a ? mylst[0] < 0 ? -lo : lo : mylst[1] < 0 ? -lo : lo;
			master->limits[i] = hi == a ? mylst[0] < 0 ? -hi : hi : mylst[1] < 0 ? -hi : hi;
			master->bases[i] = master->limits[i];
		}
		else { // count_tf = 2

			__int64 absl0 = mylst[0] < 0 ? -mylst[0] : mylst[0];

			__int64 a = master->encoding[absl0 - 2] + 1;

			__int64 lo = a;

			master->jumps[i] = mylst[0] < 0 ? -lo : lo;
			master->limits[i] = master->jumps[i];
			master->bases[i] = master->limits[i];

		}
	}
}

void SATSolverMaster_destroy(SATSolverMaster* master) {

	__int64 count_chops = 1;

	for (__int64 i = 0; i < master->chops; i++)
		count_chops *= 2;

	for (__int64 i = 0; i < count_chops; i++)
		delete[] master->begin[i];
	delete[] master->begin;

	delete[] master->decoding;
	delete[] master->encoding;

	delete[] master->jumps;
	delete[] master->limits;
	delete[] master->bases;
}

void SATSolver_destroy(SATSolver * me) {

	delete[] me->Z;

}

std::mutex m;
std::condition_variable cv;
bool done = false;
bool ready = true;
bool solved = false;
__int64 thread_id = -1;

void thread_3SAT(__int64 tid, SATSolverMaster *master, bool * arr, __int64 ** lst, __int64 k_parm, __int64 n_parm, __int64 chop) {

	SATSolver* s = new SATSolver();
	SATSolver_create(s, master, lst, k_parm, n_parm, chop);

	bool sat = SATSolver_isSat(s, chop, arr);

	{
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, [] {return ready; });
		ready = false;
		done = sat;
		thread_id = tid;
		cv.notify_all();
	}
}

bool SATSolver_threads(__int64** lst, __int64 k_parm, __int64 n_parm, bool ** arr) {

	//__int64 num_threads = n_parm < 50 ? 1 : std::thread::hardware_concurrency() ;
	__int64 num_threads = 1 ;

	std::thread ** threadblock = new std::thread * [num_threads];

	for (__int64 i = 0; i < num_threads; i++)
		threadblock[i] = NULL;

	bool** arrs = new bool*[num_threads];

	for (__int64 i = 0; i < num_threads; i++)
		arrs[i] = new bool[n_parm];

	// get the right number of chops- at least 2^chops

	__int64 chops = 2;
	
	if (num_threads > 4) {

		__int64 count_chops = 1;

		__int64 counter = 0;

		for (counter = 0; count_chops < num_threads; counter++)
			count_chops *= 2;

		if (count_chops == num_threads)
			chops = counter;

		else
			chops = counter + 1;

	}

	SATSolverMaster* master = new SATSolverMaster();
	SATSolverMaster_create(master, lst, k_parm, n_parm, chops);

	///*

	__int64 search_sz = 1;

	for (__int64 i = 0; i < master->chops; i++)
		search_sz *= 2;


	__int64 pos = 0;
	for (pos = 0; pos < num_threads; pos++)
		threadblock[pos] = new std::thread(thread_3SAT, pos , master, arrs[pos], lst, k_parm, n_parm, pos);

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
				threadblock[thread_id] = new std::thread(thread_3SAT, thread_id, master, arrs[thread_id], lst, k_parm, n_parm, pos);
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

	SATSolverMaster_destroy(master);
	delete master;

	return solved;
}

#endif