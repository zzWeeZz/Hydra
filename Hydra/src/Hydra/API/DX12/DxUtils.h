#pragma once
#include "wrl.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3d12sdklayers.h"

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
	std::wstring ToString()const
	{
		auto errorStr = std::system_category().message(ErrorCode);
		std::wstring errorWstr(errorStr.begin(), errorStr.end());
		return L"Error: " + errorWstr + L" in Function: " + FunctionName + L" In file: " + Filename + L" at line: " + std::to_wstring(LineNumber);
	}
	HRESULT ErrorCode = S_OK;
	std::wstring FunctionName;
	std::wstring Filename;
	int32_t LineNumber = -1;
};

#define HY_DX_ID(x) IID_PPV_ARGS(x.GetAddressOf())
#define TN_DX_CHECK(x) \
{ \
HRESULT hr__ = (x); \
std::wstring wfn = AnsiToWString(__FILE__); \
if(FAILED(hr__)) {  DxException ex(hr__, L#x, wfn,__LINE__); HY_CORE_ERROR(ex.ToString()); } \
