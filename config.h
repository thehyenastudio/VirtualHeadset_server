#pragma once
#include <cstdint>
#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/log.h>

class Config {
public:
    static Config& Instance() {
        static Config instance;
        return instance;
    }

    // Constants
    static constexpr auto APP_NAME = wxT("Virtual Headset");
    static constexpr auto APP_VENDOR = wxT("The HYENA studio");
    static constexpr int APP_MAJOR_VERSION = 1;
    static constexpr int APP_MINOR_VERSION = 0;
    static constexpr int APP_PATCH_VERSION = 0;
    static constexpr auto APP_STAGE_VERSION = wxT("Prototype");

    wxString GetFullVersion() const {
        return wxString::Format("%d.%d.%d %s", APP_MAJOR_VERSION, APP_MINOR_VERSION, APP_PATCH_VERSION, APP_STAGE_VERSION);
    }

    // Getters
    const wxString& GetPcIpAddress() const { return pcIpAddress; }
    uint16_t GetControlPort() const { return controlPort; }
    uint16_t GetVideoStreamPort() const { return videoStreamPort; }
	bool GetStartMinimized() const { return startMinimized; }

    // Setters
    void SetPcIpAddress(const std::string& ip) { pcIpAddress = ip; }
    void SetControlPort(uint16_t port) { controlPort = port; }
    void SetVideoStreamPort(uint16_t port) { videoStreamPort = port; }
	void SetStartMinimized(bool minimized) { startMinimized = minimized; }

    // Save to config file
    void Save() {
        wxFileName configFile(wxStandardPaths::Get().GetExecutablePath());
        configFile.SetFullName("config.ini");
        wxString path = configFile.GetFullPath();

        wxFileConfig config(APP_NAME, APP_VENDOR, path);
        config.Write("Network/pcIpAddress", pcIpAddress);
        config.Write("Network/controlPort", wxString::Format("%u", controlPort));
        config.Write("Network/videoStreamPort", wxString::Format("%u", videoStreamPort));
        config.Flush();
    }

    // Load from config file
    void Load() {
        wxFileName configFile(wxStandardPaths::Get().GetExecutablePath());
        configFile.SetFullName("config.ini");
        if (!configFile.FileExists()) {
			wxLogMessage("Config file does not exist.");
            return;
		}
        wxString path = configFile.GetFullPath();

        wxFileConfig config(APP_NAME, APP_VENDOR, path);
        pcIpAddress = config.Read("Network/pcIpAddress", pcIpAddress);
        controlPort = static_cast<uint16_t>(config.Read("Network/controlPort", controlPort));
        videoStreamPort = static_cast<uint16_t>(config.Read("Network/videoStreamPort", videoStreamPort));
    }

private:
    Config() = default;
    ~Config() = default;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    wxString pcIpAddress = "127.0.0.1";
    uint16_t controlPort = 8888;
    uint16_t videoStreamPort = 8880;
	bool startMinimized = false;
};
