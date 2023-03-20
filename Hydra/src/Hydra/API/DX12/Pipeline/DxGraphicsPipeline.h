#pragma once
#include "Hydra/API/PipelineInterface/GraphicsPipeline.h"
#include "Hydra/API/DX12/DxUtils.h"
namespace Hydra
{
	class DxDevice;
	class DxGraphicsPipeline : public GraphicsPipeline
	{
	public:
		DxGraphicsPipeline(const GraphicsPipelineSpecification& specs, Ptr<DxDevice> dxDevice);

		[[nodiscard]] FORCEINLINE ID3D12PipelineState* Get() { return m_PipelineStateObject.Get(); }
		[[nodiscard]] FORCEINLINE ID3D12RootSignature* GetRoot() { return m_RootSignature.Get(); }

	private:
		void Validate();

		Ptr<DxDevice> m_DeviceHandle;
		WinRef<ID3D12PipelineState> m_PipelineStateObject; // pso containing a pipeline state

		WinRef<ID3D12RootSignature> m_RootSignature; // root signature defines data shaders will access
	};
}
