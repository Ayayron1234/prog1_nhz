#include "Math.h"

int Math_maxInt(int* arr, int length) {
	int max = arr[0];
	for (int i = 0; i < length; i++) {
		max = (arr[i] > max) ? arr[i] : max;
	}
	return max;
}

double Math_maxDouble(double* arr, int length) {
	double max = arr[0];
	for (int i = 0; i < length; i++) {
		max = (arr[i] > max) ? arr[i] : max;
	}
	return max;
}

double Math_minDouble(double* arr, int length) {
	double max = arr[0];
	for (int i = 0; i < length; i++) {
		max = (arr[i] < max) ? arr[i] : max;
	}
	return max;
}

Vec2 Vec2_add(Vec2 a, Vec2 b) {
	return (Vec2) { a.x + b.x, a.y + b.y };
}

Vec2 Vec2_imul(Vec2 a, double n) {
	return (Vec2) { a.x* n, a.y* n };
}

void Vec2_increaseByVec(Vec2* target, Vec2 value) {
	target->x += value.x;
	target->y += value.y;
}