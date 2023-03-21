#pragma once
#include "Hydra/API/DX12/Resources/InternalResources/DxBufferBase.h"
namespace Hydra
{
	class DxVertexBuffer : public DxBufferBase
	{
	public:
		DxVertexBuffer(BufferCreateSpecification& specs);
	private:
		D3D12_VERTEX_BUFFER_VIEW m_View;
	};
}
