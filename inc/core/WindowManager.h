#pragma once
#include "../windows/MainWindow.h"
#include "../windows/PreviewWindow.h"

class WindowManager
{
public:
	static WindowManager& Instance() {
		static WindowManager instance;
		return instance;
	}

	MainWindow* GetMainWindow();
	PreviewWindow* GetPreviewWindow();

	void LoseMainWindow();
	void LosePreviewWindow();

private:
	WindowManager() = default;
	~WindowManager();

	WindowManager(const WindowManager&) = delete;
	WindowManager& operator=(const WindowManager&) = delete;

	MainWindow* m_mainWindow = nullptr;
	PreviewWindow* m_previewWindow = nullptr;
};

