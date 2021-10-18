#ifndef VEC2_H
#define VEC2_H


typedef struct Vec2 {
	double x;
	double y;
} Vec2;

typedef struct Vec2Int {
	int x;
	int y;
} Vec2Int;

Vec2 Vec2_sum(Vec2 vecs[], int length);

void Vec2_print(Vec2 vec);

#endif
