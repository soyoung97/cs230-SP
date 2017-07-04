/* 
 * CS:APP Data Lab 
 * <<20160413 Yoon So Young>>
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
/* 
 * fp_func4 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func4(unsigned uf) {
	//check whether uf is a normalized value or denormalized value.
        int exppart;
        unsigned x;
        //x is for checking the leftmost s. if uf&x=1, s is 1 and uf&x=0, s is 0.
        x= 2147483648;
        exppart=((uf<<1)>>24);
        //check if uf is a denormalized value or not.
        if (exppart==0){
                //if it is a denormalized value with exppart==0, we just have to shift one left for the fractional part.
		uf=((x&uf)+(uf<<1));
		return uf;
        }
        else if(exppart==255)
		//for NaN and infinity.
                return uf;
        //uf is just exppart+1 for normalized value.
	uf=(uf+8388608);

        return uf;

}
/* 
 * is_addition_no_problem - Will it be okay(no overflow) when we add x and y?
 *   If there is an overflow, return 0. Otherwise, return 1.
 *   Example: is_addition_no_problem(0x80000000,0x80000000) = 0,
 *            is_addition_no_problem(0x80000000,0x70000000) = 1, 
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 20
 *   Points: 3
 */
int is_addition_no_problem(int x, int y) {
	int xsign=x>>31;
	int ysign=y>>31;
	//make the sum of the two integer's sign:
	int sum_of_sign = xsign  + ysign; 	
	//make the real sum of the two integer:
	int real_sum =  x+y;
	int real_sum_sign = real_sum>>31;
	//make the result have 0  when there is an overflow, and 1  otherwise.
	int result=( !!( real_sum_sign+( sum_of_sign>>1)+1))|(!!(xsign)^!!(ysign));
	 return result;
}
/* 
 * is_x_fits_in_16_bit - Returns 1 if x fits in 16-bit, 2's complement.
 *   Examples: is_x_fits_in_16_bit(34000) = 0, is_x_fits_in_16_bit(-32768) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 8
 *   Points: 1
 */
int is_x_fits_in_16_bit(int x) {
	//if x can be expressed in 16 bit, shifting left to 16 and right to 16 will not change the value.
	int shiftedx;
	int value;
	shiftedx = (x<<16)>>16;
	//if x can be expressed in 16 bit, shiftedx-x will be 0. otherwise, it will not be 0.
	value= shiftedx+1+(~x);
	//make it return 1 if is expressed in 16 bit, and 0 otherwise.
	value= !(value);
	return value;

}
/* 
 * fp_func1 - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
unsigned fp_func1(unsigned uf) {
  	//check whether uf is a normalized value or denormalized value.
	int exppart, fracprt;
	unsigned x, roundpart;
	//x is for checking the leftmost s. if uf&x=1, s is 1 and uf&x=0, s is 0.
	x= 2147483648;
	exppart=((uf<<1)>>24);
	//define roundpart for rounding when dividing. if the leftmost is 11, roundpart is 1(add). if it is 01,00,or 10 roundpart is 0.
	roundpart=((uf&1)&((uf>>1)));
	fracprt=(uf&8388607);
	//check if uf is a denormalized value or not.
	if (exppart==0){
	//we round the leftmost fraction part.if it's 0, just shift. if it is not 0, shift and add 1.
		uf=((uf>>23)<<23)+(fracprt>>1)+roundpart;
		return uf;
	}
	else if(exppart==255)
		return uf;
	//if the exponential part is 1, then the normalized number changes into denormalized number. Resulting in having 0 for exponential part and shifting 1 for the fractional part.
        else if(exppart==1){
                uf=((uf<<1)>>2)+(uf&x)+roundpart;
                return uf;
        }
	//uf is signedpart((x&uf)+exp part((exppart-1)<<23)+fracpart(+fracprt) for normalized value.
	uf=((x&uf)+((exppart+(~0))<<23)+fracprt);
		
	return uf;
}
/* 
 * fp_func3 - Return bit-level equivalent of absolute value of f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 10
 *   Points: 2
 */
unsigned fp_func3(unsigned uf) {
  //the leftmost bit determines the sign- 0 for positive, and 1 for negative.
	if ((((uf<<1)>>24)==255)&&((uf<<9)!=0))
		return uf;
	uf=uf&2147483647;
	return uf;
}
/* 
 * fp_func2 - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Operations allowed to use: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max # of operations: 30
 *   Points: 4
 */
int fp_func2(unsigned uf) {
	int exppart;
	int s_part;
	int fracpart;
	//fracpart implies the fractional part of uf.
	fracpart=(uf&8388607);
	exppart=((uf<<1)>>24);
	//if uf is a denormalized num  or a small num, return 0.
	if (exppart<127){
		return 0;}
	//if uf is a valid number that can be expressed in 32-bit, two's complement, compute (1.frac_part)*2^(exp).
	else if (exppart<150){
		s_part=(uf>>31);;
		uf=((8388608+fracpart)>>(150-exppart));
		if ((s_part&1)==1){
			uf=-uf;}
		return uf;
		}	
	//if uf is a NaN or infinity, return this number.
	else {
		return 0x80000000u;}
}
/* 
 * extract_nth_byte - Extract the nth byte from a word x and return it.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: extract_nth_byte(0xFF34AB78,1) = 0xAB
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 6
 *   Points: 2
 */
int extract_nth_byte(int x, int n) {
  	// first we want to get avoid of unrelevant data except the n_th index of byte value. so we shift until the wanted index of byte goes to the leftmost.
	//then we shift right until the wanted index of byte go to the rightmost
	//in case the value get  the minus sign because of the shift right operation, we operate x& 255, to make the rest have 0 no matter what value x had(0&0=0,0&1=1, meaning (0 or 1)&0=0.) and the wanted index value, which is placed on the rightmost, to get unaffected by computing (wanted index bit value)&1=(wanted index bit value)
	x=x<<(32+((~n)<<3));
	x=x>>24;
	x= x & 255;
	return x;
}
/* 
 * substitute_byte(x,n,c) - In a word x, replace the nth byte with a new byte c.
 *   The index of the byte at LSB is 0.
 *   The index of the byte at MSB is 3.
 *   Examples: substitute_byte(0xffffffff,1,0xab) = 0xffffabff
 *   Assume that 0 <= n <= 3 and 0 <= c <= 255.
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 10
 *   Points: 3
 */
int substitute_byte(int x, int n, int c) {
	// we extract  the (n_th index of x)'s value
	int templet;
	int num_to_subtract;
	//make the templet to extract number in the wanted index.
	templet= 255<<(n<<3);
	//extract number by using the templet.
	num_to_subtract = x & templet;
	// subtract num_to_subtract  and add the wanted c to x
	x= x+ (c<<(n<<3)) +(~num_to_subtract)+1;
	return x;	
}
	
/*
 * floor_log_2 - return floor(log base 2 of x), where x > 0
 *   Example: floor_log_2(16) = 4
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 90
 *   Points: 4
 */
int floor_log_2(int x) {
	int floor16;
	int floor8;
	int floor4;
	int floor2;
	int floor;
	int isthere16;
	int isthere8;
	int isthere4;
	int isthere2;
	int isthere1;
	//if x is not zero when divided by 2^(num), it means floor log 2 (num) is bigger than num.
	//isthere(num) checks if floor_log_2 num is bigger than num or not. if it is bigger, x>>num will not be zero, and isthere(num) will return 1. otherwise, it will return 0.
	//floor(n)stores the data whether floor_log_2 (x) has been bigger than num or not. if it has been, it stores the num data (to be added later on.) if it has not, it just returns 0.
	isthere16 = !!( x>>16);
	floor16 = isthere16<<4;
	//operating x = x>>(floor(num)) makes x x>>floor(num) when floor_log(x) was bigger than num, and makes x just x when floor_log(x) was smaller than num, to make operations work well on both cases.
	x = x >>(floor16);
	isthere8 = !!(x>>8);
	floor8 = isthere8<<3;
	x = x >>(floor8);
	//operating at the same way until 1.
	isthere4 = !!( x>>4);
	floor4= isthere4<<2;
        x = x >>(floor4);
        isthere2 = !!(x>>2);
	floor2= isthere2<<1;
        x = x >>(floor2);
	isthere1 = !!(x>>1);
        floor= floor16+floor8+floor4+floor2+isthere1;
	return floor;
}
/* 
 * is_fits_in_bits - Can x be represented in 2's complement integer with n-bit?
 *   1 <= n <= 32
 *   Examples: is_fits_in_bits(5,3) = 0, is_fits_in_bits(-4,3) = 1
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 15
 *   Points: 2
 */
int is_fits_in_bits(int x, int n) {
	//if x can be expressed in n- bit, shifting left to n  and right to n  will not change the value.
        int shiftedx;
        int value;
	int nbit;
	//nbit is 32-n.
	nbit= 33+(~n);
        shiftedx = (x<<nbit)>>nbit;
        //if x can be expressed in n bit, shiftedx-x will be 0. otherwise, it will not be 0.
        value= shiftedx+1+(~x);
        //make it return 1 if is expressed in n  bit, and 0 otherwise.
        value= !(value);
        return value;
   

}
/* hmb - How many bits are required to represent x in 2's complement?
 *  Examples: hmb(12) = 5
 *            hmb(298) = 10
 *            hmb(-5) = 4
 *            hmb(0)  = 1
 *            hmb(-1) = 1
 *            hmb(0x80000000) = 32
 *  Operations allowed to use: ! ~ & ^ | + << >>
 *  Max # of operations: 90
 *  Points: 4
 */
int hmb(int x) {
	int is1,is2,is4,is8,is16;
	int all;
	int result;
	int minus_MSB;
	//minus_MSB returns minus_MSB of x.
	minus_MSB= x>>31;
	x = x^minus_MSB;//this operation returns x if x>0, and (~x) of x<0. the number of bits requires to represent -x equals that of (~x).(x>0)
	x = (x>>1 |x);
	x = (x>>2 |x);
	x = (x>>4 |x);
	x = (x>>8 |x);
	x = (x>>16|x);//makes x be filled with all of 1 when reading left to right  after meeting the first 1.
	//ex: 00..00101000 ->00..00111111
	//is(num) returns 0 if x can be expressed in bits smaller than (num), and returns 1 if x cannot.
	is16 = !!(x>>16);
	//if x cannot be expressed in bits smaller than (num), shift x (num)times.
	x = x >> (is16<<4);
	is8  = !!(x>>8);
	x = x >> (is8<<3);
	is4  = !!(x>>4);
	x = x >> (is4<<2);
	is2  = !!(x>>2);
	x = x >> (is2<<1);
	is1  = !!(x>>1);
	x = x >> (is1);
	//return the number of bits needed to represent x.
	all=(is16<<4)+(is8<<3)+(is4<<2)+(is2<<1)+(is1)+x;
	result= all+1;
	return result;
	




}
/* 
 * divide_by_power_of_2 - Return the same value as x/(2^n), for 0 <= n <= 30
 *                        Please round to 0.
 *   Examples: divide_by_power_of_2(15,1) = 7, divide_by_power_of_2(-33,4) = -2
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 15
 *   Points: 2
 */
int divide_by_power_of_2(int x, int n) {
	//store the minus_MSB of x;
	int minus_MSB;
	int divide_all;
	int result;
	int plus;
	//is0 check if n is 0 or not.if n is 0, return -1. if not, return 0.
	int is0;
	minus_MSB= (x>>31);
	is0=((!n)<<31)>>31;
	//when n is 0, we just return the original value, 0.
	divide_all= x>>n;
	//when x is negative, if the value is not 0 for n digits on the left, we have to add 1 to round to 0. and if the value for the rightmost n digits are 0, it means that the value is always even when dividing, so we need not add +1 . so we add plus when x is negative.
	//when x is positive, we just have to shift x by n.
	//when n is zero, plus becomes 1, so we have to add -1 when n is zero. so we add  is0(-1 when n=0, 0 when n is not 0) to plus.
	plus= !!(x<<(33+(~n)))+is0;
	//when x is negative, minus_MSB is -1, and plus&minus_MSB is plus, adding plus to result. When x is positive, result is just divide_all.
	result= (divide_all)+(plus&(minus_MSB));	
	return result;


}
/* 
 * rr - For given x, rotate it to the right by n.
 *   Can assume that 0 <= n <= 31
 *   Examples: rr(0xF7654321,4) = 0x1F765432
 *   Max # of operations: 25
 *   Points: 3 
 */
int rr(int x, int n) {
	int right_part;
	int left_part;
	int a;
	//shift x right by n.
	right_part =(x>>n);
	//make the value which was initially at the rightmost part  that will go to the leftmost part.
	a= 33+(~n);
	left_part = x<< a;
	//make the right_part have value 0 except the wanted index(right part)
	right_part= (right_part)&((1<< a)+(~0) );
	//add the two.
	x= right_part + left_part;
	return x;
}
/* 
 * bang - return !x, but you should not use ! in the function.
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Operations allowed to use: ~ & ^ | + << >>
 *   Max # of operations: 12
 *   Points: 4 
 */
int bang(int x) {
	int value;
	//makes x be filled with all of 1 when reading left to right  after meeting the first 1.
        //ex: 00..00101000 ->00..00111111
	x = (x>>1 | x);
	x = (x>>2 | x);
	x = (x>>4 | x);
	x = (x>>8 | x);
	x = (x>>16| x);
	//if x was not zero, after operating the upper one, there will always be 1 at the LSB.
	//value return 0 if x was not zero, and return 1 if x was zero.
	value=((x+1)&1);
	return value;
}
/* 
 * is_subtraction_no_problem - Can x-y be done without any problem(overflow)?
 *   If there is an overflow, return 0. Otherwise, return 1.
 *   Example: is_subtraction_no_problem(0x80000000,0x80000000) = 1,
 *            is_subtraction_no_problem(0x80000000,0x70000000) = 0, 
 *   Operations allowed to use: ! ~ & ^ | + << >>
 *   Max # of operations: 20
 *   Points: 3
 */
int is_subtraction_no_problem(int x, int y) {
	int signx;
	int signy;
	int subtraction;
	int result;
	//make sign of x and y- if positive, 0  and if negative, -1.
	signx= x>>31;
	signy= y>>31;
	//make the sign of the overflow.
	subtraction= (x+1+(~y))>>31;
	//calculate something that always turn 0 when there is an overflow, and return something else than 0 when there is not an overflow.
	result= (signx<<1)+signy+subtraction+2;
	//make the result return 0 when there is an overflow, and 1 otherwise.
	result= !!(result);
	return result;

}
