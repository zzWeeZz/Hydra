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
//
//inline void MemcpySubresource(
//	_In_ const D3D12_MEMCPY_DEST* pDest,
//	_In_ const D3D12_SUBRESOURCE_DATA* pSrc,
//	SIZE_T RowSizeInBytes,
//	UINT NumRows,
//	UINT NumSlices)
//{
//	for (UINT z = 0; z < NumSlices; ++z)
//	{
//		BYTE* pDestSlice = reinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
//		const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * z;
//		for (UINT y = 0; y < NumRows; ++y)
//		{
//			memcpy(pDestSlice + pDest->RowPitch * y,
//				pSrcSlice + pSrc->RowPitch * y,
//				RowSizeInBytes);
//		}
//	}
//}
//
//inline UINT64 UpdateSubresources(
//	_In_ ID3D12GraphicsCommandList* pCmdList,
//	_In_ ID3D12Resource* pDestinationResource,
//	_In_ ID3D12Resource* pIntermediate,
//	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
//	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources,
//	UINT64 RequiredSize,
//	_In_reads_(NumSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* pLayouts,
//	_In_reads_(NumSubresources) const UINT* pNumRows,
//	_In_reads_(NumSubresources) const UINT64* pRowSizesInBytes,
//	_In_reads_(NumSubresources) const D3D12_SUBRESOURCE_DATA* pSrcData)
//{
//	// Minor validation
//	D3D12_RESOURCE_DESC IntermediateDesc = pIntermediate->GetDesc();
//	D3D12_RESOURCE_DESC DestinationDesc = pDestinationResource->GetDesc();
//	if (IntermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
//		IntermediateDesc.Width < RequiredSize + pLayouts[0].Offset ||
//		RequiredSize >(SIZE_T) - 1 ||
//		(DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
//			(FirstSubresource != 0 || NumSubresources != 1)))
//	{
//		return 0;
//	}
//
//	BYTE* pData;
//	
//	HRESULT hr = pIntermediate->Map(0, nullptr, reinterpret_cast<void**>(&pData));
//	if (FAILED(hr))
//	{
//		return 0;
//	}
//
//	for (UINT i = 0; i < NumSubresources; ++i)
//	{
//		if (pRowSizesInBytes[i] > (SIZE_T)-1) return 0;
//		D3D12_MEMCPY_DEST DestData = { pData + pLayouts[i].Offset, pLayouts[i].Footprint.RowPitch, pLayouts[i].Footprint.RowPitch * pNumRows[i] };
//		MemcpySubresource(&DestData, &pSrcData[i], (SIZE_T)pRowSizesInBytes[i], pNumRows[i], pLayouts[i].Footprint.Depth);
//	}
//	pIntermediate->Unmap(0, NULL);
//
//	if (DestinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
//	{
//		D3D12_BOX SrcBox = {
//			UINT(pLayouts[0].Offset), 0, 0,
//			UINT(pLayouts[0].Offset + pLayouts[0].Footprint.Width), 0, 0 };
//		pCmdList->CopyBufferRegion(
//			pDestinationResource, 0, pIntermediate, pLayouts[0].Offset, pLayouts[0].Footprint.Width);
//	}
//	else
//	{
//		for (UINT i = 0; i < NumSubresources; ++i)
//		{
//			D3D12_TEXTURE_COPY_LOCATION Dst = {};
//			Dst.pResource = pDestinationResource;
//			Dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
//			Dst.SubresourceIndex = i + FirstSubresource;
//			D3D12_TEXTURE_COPY_LOCATION Src = {};
//			Src.pResource = pIntermediate;
//			Src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
//			Src.PlacedFootprint = pLayouts[i];
//			pCmdList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
//		}
//	}
//	return RequiredSize;
//}
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
