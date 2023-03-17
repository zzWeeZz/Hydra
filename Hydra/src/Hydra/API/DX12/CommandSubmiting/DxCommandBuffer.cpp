#include "HYpch.h"
#include "DxCommandBuffer.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include <Hydra/API/DX12/CommandSubmiting/DxCommandQueue.h>
#include <Hydra/API/DX12/Resources/DxFramebuffer.h>
#include <d3dx12.h>
#include <Hydra/API/DX12/Backend/DxSwapchain.h>
namespace Hydra
{
	DxCommandBuffer::DxCommandBuffer(CommandBufferSpecification& specs)
	{
		m_Specs = specs;
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(specs.device.lock());
		auto dxCommandQueue = std::reinterpret_pointer_cast<DxCommandQueue>(specs.queue.lock());

		CommandQueueFetchData fetchData{};

		dxCommandQueue->FetchCommandQueueData(fetchData);
		D3D12_COMMAND_LIST_TYPE commandType = {};

		switch (fetchData.deviceQueue.lock()->GetType())
		{
		case QueueType::Graphics:
			commandType = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case QueueType::Transfer:
			commandType = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		case QueueType::Compute:
			commandType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			break;
		default:
			break;
		}

		HY_DX_CHECK(dxDevice->Get()->CreateCommandList(0, commandType, dxCommandQueue->Get(), nullptr, HY_DX_ID(m_CommandList)));
		m_CommandList->Close();
	}

	void DxCommandBuffer::Begin()
	{
		if (m_Specs.queue.expired())
		{
			return;
		}
		auto dxCommandQueue = std::reinterpret_pointer_cast<DxCommandQueue>(m_Specs.queue.lock());
		m_CommandList->Reset(dxCommandQueue->Get(), nullptr);

		/*auto beginRB = CD3DX12_RESOURCE_BARRIER::Transition(
			m_RenderTargets[m_FrameIndex].Get(),
			D3D12_RESOURCE_STATE_PRESENT,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		m_CommandList->ResourceBarrier(
			1,
			&beginRB);*/
	}

	void DxCommandBuffer::End()
	{
		if (m_Specs.queue.expired())
		{
			return;
		}
		HY_DX_CHECK(m_CommandList->Close());
	}

	void DxCommandBuffer::BeginFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, float color[4])
	{
		auto dxFramebuffer = std::reinterpret_pointer_cast<DxFramebuffer>(framebuffer);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(dxFramebuffer->GetHeap()->GetCPUDescriptorHandleForHeapStart(), frameIndex, dxFramebuffer->DescriptorSize());

		m_CommandList->ClearRenderTargetView(rtvHandle, color, 0, nullptr);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	}

	void DxCommandBuffer::EndFramebuffer(uint32_t frameIndex, Ref<Framebuffer>& framebuffer)
	{
	}

	void DxCommandBuffer::CopyFramebufferToSwapchain(uint32_t frameIndex, Ref<Framebuffer>& framebuffer, Ref<Swapchain> swapchain)
	{
		auto dxFramebuffer = std::reinterpret_pointer_cast<DxFramebuffer>(framebuffer);
		auto dxSwapchain = std::reinterpret_pointer_cast<DxSwapchain>(swapchain);

		{
			auto CopyRB = CD3DX12_RESOURCE_BARRIER::Transition(
				dxFramebuffer->GetResource(frameIndex),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_COPY_SOURCE);
			auto dcsRB = CD3DX12_RESOURCE_BARRIER::Transition(
				dxSwapchain->GetResource(frameIndex),
				D3D12_RESOURCE_STATE_PRESENT,
				D3D12_RESOURCE_STATE_COPY_DEST);
			std::array<CD3DX12_RESOURCE_BARRIER, 2> arr = { CopyRB, dcsRB };
			m_CommandList->ResourceBarrier(
				static_cast<UINT>(arr.size()),
				arr.data());
		}
		m_CommandList->CopyResource(dxSwapchain->GetResource(frameIndex), dxFramebuffer->GetResource(frameIndex));
		{
			auto CopyRB = CD3DX12_RESOURCE_BARRIER::Transition(
				dxFramebuffer->GetResource(frameIndex),
				D3D12_RESOURCE_STATE_COPY_SOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
			auto dcsRB = CD3DX12_RESOURCE_BARRIER::Transition(
				dxSwapchain->GetResource(frameIndex),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PRESENT);
			std::array<CD3DX12_RESOURCE_BARRIER, 2> arr = { CopyRB, dcsRB };
			m_CommandList->ResourceBarrier(
				static_cast<UINT>(arr.size()),
				arr.data());
		}
	}
}