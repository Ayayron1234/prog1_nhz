#pragma once
#include "ComponentBaseClass.h"
#include "Vec2.h"

namespace ECS {

	struct RotationMatrix {
		double _00;
		double _01;
		double _10;
		double _11;

		RotationMatrix(double angle = 0) {
			this->SetFromAngle(angle);
		}

		void SetFromAngle(double angle) {
			this->_00 = cos(angle);
			this->_01 = -sin(angle);
			this->_10 = sin(angle);
			this->_11 = cos(angle);
		}

		void RotateBy(double angle) {
			RotationMatrix _rotation(angle);
			//this->_00 = this->_00 * _rotation._00 + this->_10 * _rotation._01;
			//this->_01 = this->_00 * _rotation._10 + this->_01 * _rotation._01;
			//this->_00 = this->_00 * _rotation._00 + this->_01 * _rotation._01;
			//this->_00 = this->_00 * _rotation._00 + this->_01 * _rotation._01;
		}
	};

	class Transform : public Component {
	public:
		Vec2 position;
		RotationMatrix rotation;
		Vec2 scale;

		Transform(int entity_id = 0, Vec2 position = Vec2(), RotationMatrix rotation = RotationMatrix(0), Vec2 scale = Vec2(1, 1)) {
			this->ENTITY_ID = entity_id;
			this->position = position;
			this->rotation = rotation;
			this->scale = scale;
		}

		ComponentVersionMap* getVersionMap() {
			ComponentVersionMap* versionMap = Component::getVersionMap(this);
			versionMap->componentType = ECS::TRANSFORM_COMPONENT;

			versionMap
				->addMember(ComponentVersionMapMember("position", sizeof(this->position), &this->position, "Vec2"))
				->addMember(ComponentVersionMapMember("rotation", sizeof(this->rotation), &this->rotation, "Matrix_2x2"))
				->addMember(ComponentVersionMapMember("scale", sizeof(this->scale), &this->scale, "Vec2"));

			return versionMap;
		};
	};

	template <> struct ECS::ComponentTypeSelect<ECS::TRANSFORM_COMPONENT> {
		typedef Transform Type;
	};

}
