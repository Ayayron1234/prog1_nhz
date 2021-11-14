#include "Math.h"

int Math_maxInt(int n_args, ...) {
	register int i;
	int max, a;
	va_list ap;

	va_start(ap, n_args);
	max = va_arg(ap, int);
	for (i = 2; i <= n_args; i++) {
		if ((a = va_arg(ap, int)) > max)
			max = a;
	}

	va_end(ap);
	return max;
}

double Math_maxDouble(int n_args, ...) {
	register int i;
	double max, a;
	va_list ap;

	va_start(ap, n_args);
	max = va_arg(ap, double);
	for (i = 2; i <= n_args; i++) {
		if ((a = va_arg(ap, double)) > max)
			max = a;
	}

	va_end(ap);
	return max;
}

double Math_minDouble(int n_args, ...) {
	register int i;
	double max, a;
	va_list ap;

	va_start(ap, n_args);
	max = va_arg(ap, double);
	for (i = 2; i <= n_args; i++) {
		if ((a = va_arg(ap, double)) < max)
			max = a;
	}

	va_end(ap);
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