#pragma once

#include "config.h"

namespace engine
{
	class ConfigParser
	{
	public:
		ConfigParser();
		~ConfigParser();

		Config Parse(const std::string& config_path);
		void Serialize(const std::string& config_path, Config& config);
	};
}