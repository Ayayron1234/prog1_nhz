#pragma once

typedef struct Vec2 {
	double x;
	double y;
} Vec2;

typedef struct Vec2Int {
	int x;
	int y;
} Vec2Int;

int Math_maxInt(int* arr, int length);
double Math_maxDouble(double* arr, int length);
double Math_minDouble(double* arr, int length);
Vec2 Vec2_add(Vec2 a, Vec2 b);
Vec2 Vec2_imul(Vec2 a, double n);
void Vec2_increaseByVec(Vec2* target, Vec2 value);