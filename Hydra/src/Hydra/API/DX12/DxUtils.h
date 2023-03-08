#pragma once
#include "wrl.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3d12sdklayers.h"
#include <filesystem>
#include "Hydra/API/ApiUtils.h"

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
