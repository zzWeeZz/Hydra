#pragma once
#include "vulkan/vulkan.h"
namespace Hydra
{
	class DescriptorLayoutCache;
	class DescriptorAllocator;
	class VulkanDevice;
	class DescriptorBuilder
	{
	public:
		static DescriptorBuilder Begin(DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator, Ptr<VulkanDevice> device);

		DescriptorBuilder& BindBuffer(VkDescriptorSetLayoutBinding& binding, VkDescriptorBufferInfo* bufferInfo);
		DescriptorBuilder& BindImage(VkDescriptorSetLayoutBinding& binding, VkDescriptorImageInfo* imageInfo);

		bool Build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);
		bool Build(VkDescriptorSet& set);
		void Reset();
	private:
		std::vector<VkWriteDescriptorSet> m_Writes;
		std::vector<VkDescriptorSetLayoutBinding> m_Bindings;

		DescriptorLayoutCache* m_Cache;
		DescriptorAllocator* m_Alloc;
		Ptr<VulkanDevice> m_Device;
	};
}