#pragma once
#include "Hydra/API/ResourceInterface/Image.h"
#include "d3d12.h"
#include "Hydra/API/DX12/DxAllocator.h"
#include <Hydra/API/DX12/DxUtils.h>
namespace Hydra
{
	class DxDevice;

	class DxImage : public Image
	{
	public:
		DxImage(ImageSpecification& specs, Ptr<DxDevice> device);

		HY_GET_INLINE AllocatedImage& GetImage() { return m_Image; }
		HY_GET_INLINE WinRef<ID3D12DescriptorHeap>& GetHeap() { return m_DescriptorHeap; }
	private:
		void Validate();
		AllocatedImage m_Image;
		WinRef<ID3D12DescriptorHeap> m_DescriptorHeap;
		Ptr<DxDevice> m_Device;
	};
}