#include "Vec2.h"
#include <stdio.h>


Vec2 Vec2_sum(Vec2 vecs[], int length)
{
	Vec2 v = { 0.0, 0.0 };
	for (length; length > 0; length--) {
		v.x += vecs[length - 1].x;
		v.y += vecs[length - 1].y;
	}
	return v;
}

void Vec2_print(Vec2 vec)
{
	printf("Vec2(%g, %g)", vec.x, vec.y);
}
