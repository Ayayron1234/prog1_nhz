#pragma once
#include <stdarg.h>

typedef struct Vec2 {
	double x;
	double y;
} Vec2;

typedef struct Vec2Int {
	int x;
	int y;
} Vec2Int;

/**
* Gets the maximum of a given list of integers.
* @param n_args number of integers.
* @returns The integer with the greatest value. 
*/
int Math_maxInt(int n_args, ...);

/**
* Gets the maximum of a given list of doubles.
* @param n_args number of doubles.
* @returns The double with the greatest value.
*/
double Math_maxDouble(int n_args, ...);

/**
* Gets the minimum of a given list of doubles.
* @param n_args number of doubles.
* @returns The double with the smallest value.
*/
double Math_minDouble(int n_args, ...);

/**
* Calculates the sum of two two dimensional vectors. 
* @param a The first vector.
* @param b The second vector.
* @returns The sum of the two given vectors. 
*/
Vec2 Vec2_add(Vec2 a, Vec2 b);

/**
* Calculates the result of a vector multiplied by a number. 
* @param a a two dimensional vector
* @param m The value which the vector's members should be multiplied by. 
* @returns The resulting two dimensional vector. 
*/
Vec2 Vec2_imul(Vec2 a, double n);

/**
* Increases a vector by another vector.
* @param target A pointer to the vector which should be increased. 
* @param value A two dimensional vector which the target should be increased by.
*/
void Vec2_increaseByVec(Vec2* target, Vec2 value);