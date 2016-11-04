#include "config_manager.h"

#include <QtWidgets\qdesktopwidget.h>
#include "config_manager.cpp"

#include "../utilities/pico_json.h"
#include "config_parser.h"

#include <codecvt>
#include <io.h>
#include <filesystem>
#include <sstream>
#include <fstream>

using namespace std;
using namespace std::tr2::sys;

namespace engine
{
	//------------------------------------------------------------------------------------------------------
	ConfigManager::ConfigManager()
	{
		int argc = 0;
		QApplication QApp(argc, nullptr);

		main_window_ = new QMainWindow();
		setup_dialog_ = new Ui::SetupDialog();
		setup_dialog_->setupUi(main_window_);

		main_window_->activateWindow();
		main_window_->show();

		QDesktopWidget* desktop = qApp->desktop();
		QRect rect = desktop->availableGeometry(desktop->primaryScreen());
		int screens = desktop->screenCount();
		QSize size = main_window_->size();

		FindAvailableAdapters();
		FindAvailableWindowResolutions();
		FindAvailableRenderResolutions();
		FindAvailableScenes();

		FillGPUBox();
		FillWindowResolutionBox();
		FillRenderResolutionBox();
		FillSceneBox();

		ConfigParser parser;
		config_ = parser.Parse("../resources/config.cfg");

		setup_dialog_->selection_gpu->setCurrentIndex(config_.adapter);
		setup_dialog_->selection_window_resolution->setCurrentIndex(config_.window_resolution);
		setup_dialog_->selection_render_resolution->setCurrentIndex(config_.render_resolution);
		setup_dialog_->selection_scene->setCurrentIndex(config_.scene);
		setup_dialog_->is_fullscreen->setChecked(config_.fullscreen);
		setup_dialog_->is_frustum_cullling->setChecked(config_.frustum_culling);
		setup_dialog_->is_free_camera->setChecked(config_.free_camera);
		setup_dialog_->is_mouse_lock->setChecked(config_.mouse_lock);

		if (config_.fullscreen)
		{
			FillWindowResolutionBox();
		}

		QObject::connect(setup_dialog_->launch_box, SIGNAL(accepted()), this, SLOT(Launch()));
		QObject::connect(setup_dialog_->launch_box, SIGNAL(rejected()), this, SLOT(Shutdown()));
		QObject::connect(setup_dialog_->is_fullscreen, SIGNAL(stateChanged(int)), this, SLOT(FullScreenStateChange(int)));

		running_ = true;

		while (true)
		{
			if (running_ == true)
			{
				qApp->processEvents();
			}
			else
			{
				QApp.quit();
				break;
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	ConfigManager::~ConfigManager()
	{

	}

	//------------------------------------------------------------------------------------------------------
	const Config& ConfigManager::GetConfig()
	{
		return config_;
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::Launch()
	{
		config_.adapter = setup_dialog_->selection_gpu->currentIndex();
		config_.fullscreen = setup_dialog_->is_fullscreen->checkState() == Qt::CheckState::Checked;
		config_.window_resolution = setup_dialog_->selection_window_resolution->currentIndex();
		config_.render_resolution = setup_dialog_->selection_render_resolution->currentIndex();
		config_.scene = setup_dialog_->selection_scene->currentIndex();
		config_.mouse_lock = setup_dialog_->is_mouse_lock->checkState() == Qt::CheckState::Checked;
		config_.free_camera = setup_dialog_->is_free_camera->checkState() == Qt::CheckState::Checked;
		config_.frustum_culling = setup_dialog_->is_frustum_cullling->checkState() == Qt::CheckState::Checked;

		ConfigParser parser;
		parser.Serialize("../resources/config.cfg", config_);

		delete setup_dialog_;
		delete main_window_;
		qApp->quit();

		running_ = false;
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::Shutdown()
	{
		running_ = false;
		exit(0);
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FullScreenStateChange(int state)
	{
		FillWindowResolutionBox();
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FindAvailableAdapters()
	{
		available_adapters_.resize(0);

		for (int i = setup_dialog_->selection_gpu->count(); i >= 0; i--)
		{
			setup_dialog_->selection_gpu->removeItem(i);
		}

		IDXGIFactory1* factory;
		CHECKHR(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

		UINT i = 0;
		IDXGIAdapter1* adapter;
		ID3D12Device* device;
		while (factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				i++;
				continue;
			}

			if (D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)) != S_OK)
			{
				i++;
				continue;
			}

			device->Release();
			available_adapters_.push_back(adapter);
			i++;
		}

		factory->Release();
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FindAvailableWindowResolutions()
	{
		available_window_resolutions_.resize(0);

		available_window_resolutions_.push_back({ 160, 90, "16:9" });
		available_window_resolutions_.push_back({ 640, 360, "16:9" });
		available_window_resolutions_.push_back({ 1024, 768, "4:3" });
		available_window_resolutions_.push_back({ 1024, 576, "16:9" });
		available_window_resolutions_.push_back({ 1280, 960, "4:3" });
		available_window_resolutions_.push_back({ 1280, 720, "16:9" });
		available_window_resolutions_.push_back({ 1366, 768, "16:9" });
		available_window_resolutions_.push_back({ 1400, 1050, "4:3" });
		available_window_resolutions_.push_back({ 1600, 1200, "4:3" });
		available_window_resolutions_.push_back({ 1600, 900, "16:9" });
		available_window_resolutions_.push_back({ 1920, 1440, "4:3" });
		available_window_resolutions_.push_back({ 1920, 1080, "16:9" });
		available_window_resolutions_.push_back({ 2048, 1536, "4:3" });
		available_window_resolutions_.push_back({ 2560, 1440, "16:9" });
		available_window_resolutions_.push_back({ 3840, 2160, "16:9" });
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FindAvailableRenderResolutions()
	{
		available_render_resolutions_.resize(0);

		available_render_resolutions_.push_back({ 160, 90, "16:9" });
		available_render_resolutions_.push_back({ 640, 360, "16:9" });
		available_render_resolutions_.push_back({ 1024, 768, "4:3" });
		available_render_resolutions_.push_back({ 1024, 576, "16:9" });
		available_render_resolutions_.push_back({ 1280, 960, "4:3" });
		available_render_resolutions_.push_back({ 1280, 720, "16:9" });
		available_render_resolutions_.push_back({ 1366, 768, "16:9" });
		available_render_resolutions_.push_back({ 1400, 1050, "4:3" });
		available_render_resolutions_.push_back({ 1600, 1200, "4:3" });
		available_render_resolutions_.push_back({ 1600, 900, "16:9" });
		available_render_resolutions_.push_back({ 1920, 1440, "4:3" });
		available_render_resolutions_.push_back({ 1920, 1080, "16:9" });
		available_render_resolutions_.push_back({ 2048, 1536, "4:3" });
		available_render_resolutions_.push_back({ 2560, 1440, "16:9" });
		available_render_resolutions_.push_back({ 3840, 2160, "16:9" });
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FindAvailableScenes()
	{
		available_scenes_.resize(0);

		std::string resource_path = "../resources/models/";

		for (recursive_directory_iterator i(resource_path), end; i != end; ++i)
		{
			if (!is_directory(i->path()))
			{
				auto ppath = i->path().relative_path();
				std::wstring wpath(ppath.c_str());

				using convert_type = std::codecvt_utf8<wchar_t>;
				std::wstring_convert<convert_type, wchar_t> converter;
				std::string path = converter.to_bytes(wpath);

				std::string extension = path.substr(path.size() - 4, 4);

				if (extension == ".fbx")
				{
					available_scenes_.push_back(path);
				}
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FillGPUBox()
	{
		for (int i = 0; i < available_adapters_.size(); i++)
		{
			IDXGIAdapter1* adapter = available_adapters_[i];

			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			using convert_type = std::codecvt_utf8<wchar_t>;
			std::wstring_convert<convert_type, wchar_t> converter;
			std::string converted_str = converter.to_bytes(desc.Description);

			setup_dialog_->selection_gpu->addItem(QString(converted_str.c_str()));
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FillWindowResolutionBox()
	{
		for (int i = setup_dialog_->selection_window_resolution->count(); i >= 0; i--)
		{
			setup_dialog_->selection_window_resolution->removeItem(i);
		}

		if (setup_dialog_->is_fullscreen->checkState() != Qt::CheckState::Unchecked)
		{
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);

			int found = 0;
			for (int i = 0; i < available_window_resolutions_.size(); i++)
			{
				Resolution& res = available_window_resolutions_[i];

				if (res.width == width && res.height == height)
				{
					found = i;
					break;
				}
			}

			for (int i = setup_dialog_->selection_window_resolution->count(); i >= 0; i--)
			{
				setup_dialog_->selection_window_resolution->removeItem(i);
			}

			char widths[5];
			char heights[5];
			_itoa_s(available_window_resolutions_[found].width, widths, 10);
			_itoa_s(available_window_resolutions_[found].height, heights, 10);

			setup_dialog_->selection_window_resolution->addItem(QString(widths) + QString("x") + QString(heights) + QString(" (") + QString(available_window_resolutions_[found].aspect) + QString(")"));
			setup_dialog_->selection_window_resolution->setEnabled(false);
		}
		else
		{
			for (int i = 0; i < available_window_resolutions_.size(); i++)
			{
				char width[5];
				char height[5];
				_itoa_s(available_window_resolutions_[i].width, width, 10);
				_itoa_s(available_window_resolutions_[i].height, height, 10);

				setup_dialog_->selection_window_resolution->addItem(QString(width) + QString("x") + QString(height) + QString(" (") + QString(available_window_resolutions_[i].aspect) + QString(")"));
			}

			setup_dialog_->selection_window_resolution->setEnabled(true);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FillRenderResolutionBox()
	{
		for (int i = setup_dialog_->selection_render_resolution->count(); i >= 0; i--)
		{
			setup_dialog_->selection_render_resolution->removeItem(i);
		}

		for (int i = 0; i < available_render_resolutions_.size(); i++)
		{
			char width[5];
			char height[5];
			_itoa_s(available_render_resolutions_[i].width, width, 10);
			_itoa_s(available_render_resolutions_[i].height, height, 10);

			setup_dialog_->selection_render_resolution->addItem(QString(width) + QString("x") + QString(height) + QString(" (") + QString(available_render_resolutions_[i].aspect) + QString(")"));
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ConfigManager::FillSceneBox()
	{
		for (int i = setup_dialog_->selection_scene->count(); i >= 0; i--)
		{
			setup_dialog_->selection_scene->removeItem(i);
		}

		std::string resource_path = "../resources/models/";

		for (int i = 0; i < available_scenes_.size(); i++)
		{
			std::string file = available_scenes_[i].substr(resource_path.size(), available_scenes_[i].size() - resource_path.size());
			setup_dialog_->selection_scene->addItem(QString(file.c_str()));
		}
	}
}