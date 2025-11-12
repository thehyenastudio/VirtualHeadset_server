#include "core/WindowManager.h"

MainWindow* WindowManager::GetMainWindow()
{
	if (m_mainWindow == nullptr)
	{
		m_mainWindow = new MainWindow();
	}
	return m_mainWindow;
}

PreviewWindow* WindowManager::GetPreviewWindow()
{
	if (m_previewWindow == nullptr)
	{
		m_previewWindow = new PreviewWindow();
	}
	return m_previewWindow;
}

bool WindowManager::MainWindowShown() const
{
	return m_mainWindow && m_mainWindow->IsShown();
}

bool WindowManager::PreviewWindowShown() const
{
	return m_previewWindow && m_previewWindow->IsShown();
}

void WindowManager::CloseAllWindows()
{
	if (m_mainWindow != nullptr)
	{
		m_mainWindow->Close();
	}
	if (m_previewWindow != nullptr)
	{
		m_previewWindow->Close();
	}
}

void WindowManager::LoseMainWindow()
{
	m_mainWindow = nullptr;
}

void WindowManager::LosePreviewWindow()
{
	m_previewWindow = nullptr;
}
