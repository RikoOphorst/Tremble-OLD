#pragma once

#pragma warning(disable:4099) // disable "no pdf was found with linked library"
#pragma message("precompiling")

#include <cstdio>
#include <cstdarg>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <exception>
#include <iostream>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <comdef.h>
#include <vector>
#include <queue>
#include <map>
#include <codecvt>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <unordered_map>
#include <wincodec.h>
#include <PxPhysicsAPI.h>
#include <io.h>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <wrl.h>
#include <ppltasks.h>
#include <cmath>
#include <fbxsdk.h>
#include <typeindex>
#include <assert.h>

#include "core/rendering/direct3d.h"

#define PICOJSON_USE_INT64
#include "core/utilities/pico_json.h"

#include <QtWidgets\qdesktopwidget.h>

#ifdef _DEBUG
#include <bitset>
#endif