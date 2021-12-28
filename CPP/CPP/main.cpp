#include "Vec2.h"
#include "PositionComponent.h"
#include <stdio.h>
#include <stdexcept>
#include <string>

using namespace std;

struct A {
	int id;
	double posX;
	double posY;
};
void testFunc(A component) {
	cout << "-------------" << endl;
	cout << "id: " << component.id << ", size: " << sizeof(int) << endl;
	cout << "posX: " << component.posX << ", size: " << sizeof(double) << endl;
	cout << "posY: " << component.posY << ", size: " << sizeof(double) << endl;
}

struct B {
	int id;
	double velocity;
};
void testFunc(B component) {
	cout << "-------------" << endl;
	cout << "id: " << component.id << ", size: " << sizeof(int) << endl;
	cout << "velocity: " << component.velocity << ", size: " << sizeof(double) << endl;
}

double osztas(double a, double b) {
	if (b == 0) throw runtime_error("Division by zero!");
	return a / b;
}

int main(void) {
	cout << "Hello World!\n" << endl;

	Vec2* a = new Vec2(2, 1);
	Vec2 b(1, 3);

	cout << "a = " << *a << endl;	
	cout << "b = " << b << endl;
	cout << "a + b = " << *a + b << endl << endl;

	*a += b;
	a->print();
	a->setLength(4);
	a->print();

	Vec2 c;
	cin >> c;
	c.print();

	cout << "\ndot product of a and b: " << Vec2::dot(*a, b) << endl << endl;

	try {
		std::cout << "5 / 1 = " << osztas(5, 1) << std::endl;
		std::cout << "6 / 0 = " << osztas(6, 0) << std::endl;
	}
	catch (runtime_error& err) {
		cerr << err.what() << endl;
	}
	cout << endl;

	int list[] = { 1, 4, 9, 16, 25 };
	for (int n : list) cout << n << endl;

	delete a;

	A arrA[10];
	for (int i = 0; i < 10; i++) { arrA[i].id = i; arrA[i].posX = 10; arrA[i].posY = 30; }
	B arrB[10];
	for (int i = 0; i < 10; i++) { arrB[i].id = i; arrB[i].velocity = 20; }
	void* arr[2] = { arrA, arrB };

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			switch (i)
			{
			case 0: testFunc(((A*)arr[i])[j]); break;
			case 1: testFunc(((B*)arr[i])[j]); break;
			default:
				break;
			}
		}
	}

	PositionComponent pos(1, c);
	ECS::ComponentVersionMap* bluePrint = pos.getVersionMap();

	cout << *((Vec2*)bluePrint->members[1].ptr);

	ECS::ComponentVersionMapMember* member = (*bluePrint)["value"];

	//cout << pos;
	for (int i = 0; i < 4; i++)
		cout << *bluePrint;

		return 0;
}
