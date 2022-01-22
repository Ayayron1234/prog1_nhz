#include "Vec2.h"
#include "PositionComponent.h"
#include "TileComponent.hpp"
#include "Transform.h"
#include "ECS.h"
#include <stdio.h>
#include <stdexcept>
#include <string>
#include <chrono>
#include "GameWindow.hpp"
#include "ResourceManager.hpp"

using namespace std;

double osztas(double a, double b) {
	if (b == 0) throw runtime_error("Division by zero!");
	return a / b;
}

int main(void) {
	// ---------------------- fps ----------------------
	int lastUpdateTime = SDL_GetTicks();
	int nFrames = 0;
	int fps = 0;
	// -------------------------------------------------

	//cout << "Hello World!\n" << endl;

	//Vec2* a = new Vec2(2, 1);
	//Vec2 b(1, 3);

	//cout << "a = " << *a << endl;	
	//cout << "b = " << b << endl;
	//cout << "a + b = " << *a + b << endl << endl;

	//*a += b;
	//a->print();
	//a->setLength(4);
	//a->print();

	//Vec2 c;
	//cin >> c;
	//c.print();

	//cout << "\ndot product of a and b: " << Vec2::dot(*a, b) << endl << endl;

	//try {
	//	std::cout << "5 / 1 = " << osztas(5, 1) << std::endl;
	//	std::cout << "6 / 0 = " << osztas(6, 0) << std::endl;
	//}
	//catch (runtime_error& err) {
	//	cerr << err.what() << endl;
	//}
	//cout << endl;

	//int list[] = { 1, 4, 9, 16, 25 };
	//for (int n : list) cout << n << endl;

	//delete a;
	GameWindow window("Test Window");
	window.SetBackgroundColor(10, 30, 80);

	ECS::ResourceManager resourceManager = ECS::ResourceManager();
	//resourceManager.RegisterResource(ECS::Tilemap::LoadFromFile("/saves/tilemap.png", Vec2Int(), window.renderer), ECS::Resource::ResourceType::TILEMAP)

	PositionComponent pos(1, Vec2(123, 321));
	ECS::ComponentVersionMap* posVersion = pos.getVersionMap();

	ECS::ComponentVersionMapMember* member = (*posVersion)["value"];
	cout << *((Vec2*)member->ptr);
	cout << *posVersion;

	ECS::Tile tile(ECS::ResourceReference(), 2, Vec2Int(2, 3), Vec2(1.4, 5.3));
	ECS::ComponentVersionMap* tileVersion = tile.getVersionMap();
	cout << *tileVersion;

	ECS::Transform transform(1, Vec2(30, 20));
	ECS::ComponentVersionMap* transformVersion = transform.getVersionMap();
	cout << *transformVersion;

	// ---- this will be loaded from save file ----
	ECS::ComponentVersionMap* versionMaps[ECS::NUMBER_OF_COMPONENT_TYPES];
	versionMaps[0] = posVersion;
	versionMaps[1] = tileVersion;
	versionMaps[2] = transformVersion;
	// --------------------------------------------

	ECS::EntityManager entityManager(versionMaps, 500);

	ECS::EntityManager::ComponentList* compList = entityManager.GetComponentList(ECS::TRANSFORM_COMPONENT);

	entityManager.CreateComponent<ECS::TRANSFORM_COMPONENT>(ECS::Transform(2, Vec2(12, 21)));
	
	auto x = compList->at<ECS::TRANSFORM_COMPONENT>(0);

	while (window.isRunning) {
		// ---------------------- fps ----------------------
		nFrames++;
		int now = SDL_GetTicks();
		if (now - lastUpdateTime >= 1000) {
			fps = nFrames;
			cout << "FPS: " << fps << endl;
			nFrames = 0;
			lastUpdateTime = now;
		}
		// -------------------------------------------------

		window.Render();
	}

	return 0;
}

//template <class T, std::enable_if<std::_Maximum<9>::, int>>
