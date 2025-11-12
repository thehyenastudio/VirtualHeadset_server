#pragma once
#include <atlbase.h>
#include <cstdio>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

#include "CaptureBase.h"

class DXGICapture : public CaptureBase
{
public:
	DXGICapture() = default;
	~DXGICapture() override = default;

	bool Init() override;
	AVFrame* CaptureFrame() override;

private:
	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGIOutputDuplication> m_outputDuplication;

	D3D_FEATURE_LEVEL m_featureLevel{};

	static bool isSupportedFormat(DXGI_FORMAT fmt);
};