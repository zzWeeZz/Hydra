#pragma once
#include "Hydra/API/CommandBuffer.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxCommandBuffer : public CommandBuffer
	{
	public:
		DxCommandBuffer(CommandBufferSpecification& specs);
	private:
		WinRef<ID3D12CommandList> m_CommandList;
	};
}
