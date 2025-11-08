#include <core/WindowManager.h>

MainWindow* WindowManager::GetMainWindow()
{
	if (m_mainWindow == nullptr) {
		m_mainWindow = new MainWindow();
	}
	return m_mainWindow;
}

PreviewWindow* WindowManager::GetPreviewWindow()
{
	if (m_previewWindow == nullptr) {
		m_previewWindow = new PreviewWindow();
	}
	return m_previewWindow;
}

void WindowManager::LoseMainWindow()
{
	m_mainWindow = nullptr;
}

void WindowManager::LosePreviewWindow()
{
	m_previewWindow = nullptr;
}

WindowManager::~WindowManager()
{
	if (m_mainWindow != nullptr) {
		m_mainWindow->Close();
	}
	if (m_previewWindow != nullptr) {
		m_previewWindow->Close();
	}
}
