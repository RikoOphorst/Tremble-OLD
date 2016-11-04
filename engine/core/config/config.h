#pragma once

#include "../rendering/direct3d.h"
#include "../math/math.h"
#include "resolution.h"

namespace engine
{
	struct Config
	{
		UINT adapter = 0;
		UINT window_resolution = 0;
		UINT render_resolution = 0;
		UINT scene = 0;
		bool mouse_lock = false;
		bool fullscreen = false;
		bool free_camera = false;
		bool frustum_culling = true;
	};
}