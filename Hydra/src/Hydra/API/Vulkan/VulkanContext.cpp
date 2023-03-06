#include "HYpch.h"
#include "VulkanContext.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "Hydra/Application.h"
#include <Hydra/API/Vulkan/VulkanUtils.h>
#include <Hydra/API/Vulkan/Backend/VulkanPhysicalDevice.h>
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>

namespace Hydra
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		messageType;
		pUserData;
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			HY_CORE_TRACE("Validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			HY_CORE_INFO("Validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			HY_CORE_WARN("Validation layer: {0}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			HY_CORE_ERROR("Validation layer: {0}", pCallbackData->pMessage);
			break;
		}

		return VK_FALSE;
	}

	void VulkanContext::Initalize(const GraphicsContextSpecification& specs)
	{
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Titan";
		appInfo.applicationVersion = VK_API_VERSION_1_3;
		appInfo.pEngineName = "Titan Engine";
		appInfo.engineVersion = VK_API_VERSION_1_3;
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledLayerCount = 0;

		std::vector<const char*> extensions;
		for (size_t i = 0; i < glfwExtensionCount; ++i)
		{
			extensions.push_back(glfwExtensions[i]);
		}

		if (specs.debuging)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();


		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

		if (specs.debuging && !CheckValidationLayerSupport(validationLayers))
		{
			HY_CORE_ERROR("Validation layers requested, but not available!");
		}

		if (specs.debuging)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();

			VkDebugUtilsMessengerCreateInfoEXT DebugCreateInfo{};
			PopulateDebugMessengerCreateInfo(DebugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		HY_VK_CHECK(vkCreateInstance(&createInfo, nullptr, &m_Instance));

		uint32_t extensionPropertiesCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, nullptr);

		std::vector<VkExtensionProperties> extensionPropoties(extensionPropertiesCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertiesCount, extensionPropoties.data());

		HY_CORE_INFO("Available extensions: ");

		for (auto& extension : extensionPropoties)
		{
			HY_CORE_INFO(extension.extensionName);
		}
		PhysicalDeviceSpecifications pdSpecs{};
		auto vpd = std::make_shared<VulkanPhysicalDevice>(pdSpecs);
		vpd->Create(m_Instance, m_Surface);
		auto vd = std::make_shared<VulkanDevice>(vpd);
		vd->Create(vpd, validationLayers);
	}

	bool VulkanContext::CheckValidationLayerSupport(std::vector<const char*> validationLayer)
	{
		uint32_t layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayer)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	void VulkanContext::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = VulkanDebugCallback;
		createInfo.pUserData = nullptr;
	}
}