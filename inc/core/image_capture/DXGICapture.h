#pragma once
#include "CaptureBase.h"
#include <windows.h>
#include <atlbase.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

class DXGICapture : public CaptureBase
{
public:
	DXGICapture() = default;
	~DXGICapture() override = default;

	bool Init() override;
	FrameBGRA CaptureFrame() override;

private:
	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	D3D_FEATURE_LEVEL m_featureLevel{};

	CComPtr<IDXGIOutputDuplication> m_outputDuplication;

	bool createDevice();
	bool createOutputDuplication();

	static bool isSupportedFormat(DXGI_FORMAT fmt);
};