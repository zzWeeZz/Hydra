#include "HYpch.h"
#include "DxSwapchain.h"
#include "Hydra/API/Context.h"
#include <Hydra/API/DX12/Backend/DxDevice.h>
#include "Hydra/API/DX12/Backend/DxPhysicalDevice.h"
#include "dx12helpers/d3dx12.h"
#include "Hydra/Application.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_NATIVE_INCLUDE_NONE
#include <GLFW/glfw3native.h>
#include <Hydra/API/DX12/DxDeviceQueue.h>
namespace Hydra
{
	DxSwapchain::DxSwapchain(const SwapchainSpecfications& specs) : Swapchain(specs)
	{
		Create(specs.context);
	}

	void DxSwapchain::Create(Ptr<Context> context)
	{
		DXGI_SWAP_CHAIN_DESC scDesc{};

		DXGI_MODE_DESC mDesc{};
		mDesc.Width = static_cast<uint32_t>(m_Specs.width);
		mDesc.Height = static_cast<uint32_t>(m_Specs.height);
		DXGI_RATIONAL ratio{};
		mDesc.RefreshRate = ratio;
		mDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		mDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		mDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scDesc.BufferDesc = mDesc;
		DXGI_SAMPLE_DESC sDesc{};
		sDesc.Count = 1;

		scDesc.SampleDesc = sDesc;
		scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scDesc.BufferCount = static_cast<uint32_t>(m_Specs.frameCount);
		scDesc.OutputWindow = glfwGetWin32Window(reinterpret_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow()));
		scDesc.Windowed = TRUE;
		scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		scDesc.Flags = 0;

		auto dxPhysicalDevice = std::reinterpret_pointer_cast<DxPhysicalDevice>(context.lock()->GetPhyicalDevice().lock());
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(context.lock()->GetDevice().lock());

		IDXGISwapChain* transferSwapchain = nullptr; // create a dummy swapchain0 inorder to move it to newer version.
		auto dxQueue = std::reinterpret_pointer_cast<DxDeviceQueue>(dxDevice->GetQueue(QueueType::Graphics).lock());
		HRESULT hr = dxPhysicalDevice->GetFactory()->CreateSwapChain(dxQueue->Get(), &scDesc, &transferSwapchain);

		if (FAILED(hr))
		{
			HY_CORE_ERROR("DX12: Failed to create Swapchain! can't continue swapchain creation.");
			return;
		}
		HY_CORE_INFO("DX12: Successfully created swapchain!");
		m_Swapchain = reinterpret_cast<IDXGISwapChain3*>(transferSwapchain);
		m_CurrentImage = m_Swapchain->GetCurrentBackBufferIndex();

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = g_FramesInFlight;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		HY_DX_CHECK(dxDevice->Get()->CreateDescriptorHeap(&rtvHeapDesc, HY_DX_ID(m_RTVDescriptorHeap)));

		auto rtvDescriptorSize = dxDevice->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			HY_DX_CHECK(m_Swapchain->GetBuffer(static_cast<UINT>(i), HY_DX_ID(m_RenderTargets[i])));
			dxDevice->Get()->CreateRenderTargetView(m_RenderTargets[i].Get(), nullptr, rtvHandle);

			rtvHandle.Offset(1, rtvDescriptorSize);
		}
		
		for (size_t i = 0; i < g_FramesInFlight; ++i)
		{
			HY_DX_CHECK(dxDevice->Get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, HY_DX_ID(m_Fences[i])));

			m_FenceValues[i] = 0;
		}

		m_FenceEvent = CreateEvent(nullptr, false, false, nullptr);
		if (m_FenceEvent == nullptr)
		{
			HY_CORE_ASSERT(false, "Could not create Fence Event!");
		}

	}
	
	void DxSwapchain::Resize(uint32_t width, uint32_t height)
	{
	}
	
	void DxSwapchain::Validate(Ptr<Context> context)
	{
	}
	void DxSwapchain::Present()
	{
		m_Swapchain->Present(0, 0);
		m_CurrentFrame = (m_CurrentFrame + 1) % g_FramesInFlight;
	}
	uint32_t DxSwapchain::PrepareNewFrame()
	{
		auto dxDevice = std::reinterpret_pointer_cast<DxDevice>(m_Specs.context.lock()->GetDevice().lock());
		auto dxQueue = std::reinterpret_pointer_cast<DxDeviceQueue>(dxDevice->GetQueue(QueueType::Graphics).lock());
		m_CurrentImage = m_Swapchain->GetCurrentBackBufferIndex();
		m_FenceValues[m_CurrentFrame]++;
		dxQueue->Get()->Signal(m_Fences[m_CurrentFrame].Get(), m_FenceValues[m_CurrentFrame]);
		return m_CurrentFrame;
	}
}