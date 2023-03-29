#include "HYpch.h"
#include "GraphicsContext.h"
#include "Hydra/Application.h"
namespace Hydra
{
	void GraphicsContext::Intitalize(API api)
	{
		ContextConstructSpecifications contextCSpecs{};
		contextCSpecs.contextSpecs.debuging = false;
		contextCSpecs.contextSpecs.queueTypes = QueueType::Graphics | QueueType::Transfer | QueueType::Compute;
		contextCSpecs.context = s_Context;
		contextCSpecs.api = api;

		std::string name = "Hydra Engine running on ";
		
		switch (api)
		{
		case Hydra::API::Dx12:
			name += "DirectX 12";
			break;
		case Hydra::API::Vulkan:
			name += "Vulkan 1.3";
			break;
		default:
			break;
		}

		Application::GetWindow().SetWindowName(name);
		Factory::ConstructContext(contextCSpecs);
		s_Context = contextCSpecs.context;
	}
	void GraphicsContext::Shutdown()
	{
		s_Context->Shutdown();
		s_Context.reset();
	}
	Ptr<Context> GraphicsContext::GetContext()
	{
		return s_Context;
	}
	Ptr<Device> GraphicsContext::GetDevice()
	{
		return s_Context->GetDevice();
	}
	Ptr<PhysicalDevice> GraphicsContext::GetPhysicalDevice()
	{
		return s_Context->GetPhyicalDevice();
	}
	Ptr<Swapchain> GraphicsContext::GetSwapchain()
	{
		return s_Context->GetSwapchain();
	}
}