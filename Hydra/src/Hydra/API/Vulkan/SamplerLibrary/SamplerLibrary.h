#pragma once
#include <vulkan/vulkan.h>
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include <unordered_map>
namespace Hydra
{
	class VulkanDevice;
	class SamplerLibrary
	{
	public:
		void Initalize(Ref<VulkanDevice> device);
		void Add(const std::string& key, const VulkanFilter& filter, const VulkanAddress& address, const VulkanMipmapMode& mipmapMode, bool isAnisotropic);
		VkSampler& Get(const std::string& key);
	private:
		std::unordered_map<std::string, VkSampler> m_Samplers;
		Ptr<VulkanDevice> m_Device;
	};
}