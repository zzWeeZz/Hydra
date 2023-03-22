#pragma once
#include "vulkan/vulkan.h"
#include <Hydra/API/Vulkan/Backend/VulkanDevice.h>
namespace Hydra
{
	class DescriptorLayoutCache
	{
	public:
		VkDescriptorSetLayout CreateDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info, Ptr<VulkanDevice> device);

		void Shutdown();
		struct DescriptorLayoutInfo
		{
			std::vector<VkDescriptorSetLayoutBinding> bindings;

			bool operator==(const DescriptorLayoutInfo& other) const;

			size_t hash() const;
		};
	private:


		struct DescriptorLayoutHash
		{
			std::size_t operator()(const DescriptorLayoutInfo& k) const
			{
				return k.hash();
			}
		};

		std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> m_LayoutCache;
		Ptr<VulkanDevice> m_Device;
	};
}