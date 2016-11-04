#include "config_parser.h"

#include <fstream>
#define PICOJSON_USE_INT64
#include "../utilities/pico_json.h"

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	ConfigParser::ConfigParser()
	{
	}
	
	//------------------------------------------------------------------------------------------------------
	ConfigParser::~ConfigParser()
	{
	}
	
	//------------------------------------------------------------------------------------------------------
	Config ConfigParser::Parse(const std::string& json_config)
	{
		Config ret;

		std::ifstream myfile;
		myfile.open(json_config);

		std::string contents;
		std::string line;
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				contents += line;
				contents += "\n";
			}

			myfile.close();
		}

		picojson::value v;
		std::string err = picojson::parse(v, contents);

		if (!v.is<picojson::object>())
		{
			std::cerr << "Config file corrupted. Using default." << std::endl;
			return ret;
		}

		const picojson::value::object& obj = v.get<picojson::object>();

		ret.fullscreen			= obj.find("fullscreen")		!= obj.end() ? obj.at("fullscreen").get<bool>() : false;
		ret.adapter				= obj.find("adapter")			!= obj.end() ? static_cast<UINT>(obj.at("adapter").get<int64_t>()) : 0;
		ret.free_camera			= obj.find("free_camera")		!= obj.end() ? obj.at("free_camera").get<bool>() : false;
		ret.frustum_culling		= obj.find("frustum_culling")	!= obj.end() ? obj.at("frustum_culling").get<bool>() : false;
		ret.mouse_lock			= obj.find("mouse_lock")		!= obj.end() ? obj.at("mouse_lock").get<bool>() : true;
		ret.render_resolution	= obj.find("render_resolution") != obj.end() ? static_cast<UINT>(obj.at("render_resolution").get<int64_t>()) : 0;
		ret.window_resolution	= obj.find("window_resolution") != obj.end() ? static_cast<UINT>(obj.at("window_resolution").get<int64_t>()) : 0;
		ret.scene				= obj.find("scene")				!= obj.end() ? static_cast<UINT>(obj.at("scene").get<int64_t>()) : 0;

		return ret;
	}
	
	//------------------------------------------------------------------------------------------------------
	void ConfigParser::Serialize(const std::string& config_path, Config& config)
	{
		std::initializer_list<std::pair<const std::string, picojson::value>> list = 
		{
			std::pair<std::string, picojson::value>("fullscreen", picojson::value(config.fullscreen)),
			std::pair<std::string, picojson::value>("adapter", picojson::value(static_cast<double>(config.adapter))),
			std::pair<std::string, picojson::value>("free_camera", picojson::value(config.free_camera)),
			std::pair<std::string, picojson::value>("frustum_culling", picojson::value(config.frustum_culling)),
			std::pair<std::string, picojson::value>("mouse_lock", picojson::value(config.mouse_lock)),
			std::pair<std::string, picojson::value>("render_resolution", picojson::value(static_cast<double>(config.render_resolution))),
			std::pair<std::string, picojson::value>("window_resolution", picojson::value(static_cast<double>(config.window_resolution))),
			std::pair<std::string, picojson::value>("scene", picojson::value(static_cast<double>(config.scene))),
		};

		picojson::value v = picojson::value(picojson::object(list));

		std::string output = v.serialize(true);

		std::ofstream file(config_path);

		if (file.is_open())
		{
			file << output;
			file.close();
		}
	}
}