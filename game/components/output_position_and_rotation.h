#pragma once
#include "engine/engine_include.h"

namespace engine
{
	class OutputPositionRotation : public Component
	{
	public:
		void Update()
		{
			Vector3 rotation_degrees = GetNode()->GetRotationDegrees();
			Vector3 position = GetNode()->GetPosition();
			std::cout << "p: " << position.GetX() << " " << position.GetY() << " " << position.GetZ() << " r: "<< rotation_degrees.GetX() << " " << rotation_degrees.GetY() << " " << rotation_degrees.GetZ() << std::endl;
		}
	};
}