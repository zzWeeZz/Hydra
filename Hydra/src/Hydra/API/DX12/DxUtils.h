#pragma once
#include "wrl.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3d12sdklayers.h"
#include <filesystem>
#include "Hydra/API/ApiUtils.h"
#include <Hydra/API/ApiFormats.h>

template<typename T>
using WinRef = Microsoft::WRL::ComPtr<T>;

// From Introduction to 3D Game Programming in dx12 by Frank Luna
class DxException
{
public:
	DxException() = default;
	DxException(HRESULT hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
	{
		ErrorCode = hr;
		FunctionName = functionName;
		Filename = filename;
		LineNumber = lineNumber;
	}
	std::wstring ToWString()const
	{
		auto errorStr = std::system_category().message(ErrorCode);
		std::wstring errorWstr(errorStr.begin(), errorStr.end());
		return L"Error: " + errorWstr + L" in Function: " + FunctionName + L" In file: " + Filename + L" at line: " + std::to_wstring(LineNumber);
	}

	std::string ToString()
	{
		std::wstring wstr = ToWString();
		std::filesystem::path converter(wstr);
		return converter.string();
	}

	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int32_t LineNumber = -1;
};

#define HY_DX_ID(x) IID_PPV_ARGS(x.GetAddressOf())
#define HY_DX_CHECK(x) \
{ \
HRESULT hr__ = (x); \
std::string str = __FILE__; \
std::wstring wfn(str.begin(), str.end()); \
if(FAILED(hr__)) {  DxException ex(hr__, L#x, wfn,__LINE__); HY_CORE_ERROR("{}", ex.ToString()); } } \


namespace Hydra
{
	inline DXGI_FORMAT GetDxFormat(ImageFormat format)
	{
		DXGI_FORMAT dxFormat = {};
		switch (format)
		{
		case Hydra::ImageFormat::R8UN:
			dxFormat = DXGI_FORMAT_R8_UNORM;
			break;
		case Hydra::ImageFormat::RGBA8UN:
			dxFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case Hydra::ImageFormat::R32UI:
			dxFormat = DXGI_FORMAT_R32_UINT;
			break;
		case Hydra::ImageFormat::R32F:
			dxFormat = DXGI_FORMAT_R32_FLOAT;
			break;
		case Hydra::ImageFormat::R32S:
			dxFormat = DXGI_FORMAT_R32_SINT;
			break;
		case Hydra::ImageFormat::RG32F:
			dxFormat = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case Hydra::ImageFormat::RG32S:
			dxFormat = DXGI_FORMAT_R32G32_SINT;
			break;
		case Hydra::ImageFormat::RG32UI:
			dxFormat = DXGI_FORMAT_R32G32_UINT;
			break;
		case Hydra::ImageFormat::RGB32F:
			dxFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case Hydra::ImageFormat::RGB32S:
			dxFormat = DXGI_FORMAT_R32G32B32_SINT;
			break;
		case Hydra::ImageFormat::RGB32UI:
			dxFormat = DXGI_FORMAT_R32G32B32_UINT;
			break;
		case Hydra::ImageFormat::RGBA32F:
			dxFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		case Hydra::ImageFormat::RGBA32UI:
			dxFormat = DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		case Hydra::ImageFormat::RGBA32SI:
			dxFormat = DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		case Hydra::ImageFormat::RGBA16F:
			dxFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
			break;
		case Hydra::ImageFormat::RGBA16UN:
			dxFormat = DXGI_FORMAT_R16G16B16A16_UNORM;
			break;
		case Hydra::ImageFormat::RGBA16UI:
			dxFormat = DXGI_FORMAT_R16G16B16A16_UINT;
			break;
		case Hydra::ImageFormat::RGBA16SN:
			dxFormat = DXGI_FORMAT_R16G16B16A16_SNORM;
			break;
		case Hydra::ImageFormat::RGBA16SI:
			dxFormat = DXGI_FORMAT_R16G16B16A16_SINT;
			break;
		case Hydra::ImageFormat::Depth32:
			dxFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
			break;
		case Hydra::ImageFormat::Depth24:
			dxFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
			break;
		default:
			break;
		}
		return dxFormat;
	}
}
