#pragma once
#include "wrl.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3d12sdklayers.h"

template<typename T>
using WinRef = Microsoft::WRL::ComPtr<T>;

#define HY_DX_ID(x) IID_PPV_ARGS(x.GetAddressOf())
#define TN_DX_CHECK(X) {HRESULT hr = X; if(FAILED(hr)){ TN_CORE_ASSERT(false,std::system_category().message(hr).c_str());}}