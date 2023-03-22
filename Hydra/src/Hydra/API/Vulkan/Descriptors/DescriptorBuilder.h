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

		DescriptorBuilder& BindBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo, VkDescriptorType type, VkShaderStageFlags stageFlags);
		DescriptorBuilder& BindImage(uint32_t binding, VkDescriptorImageInfo* imageInfo, VkDescriptorType type, VkShaderStageFlags stageFlags, size_t descriptorCount = 1);

		bool Build(VkDescriptorSet& set, VkDescriptorSetLayout& layout);
		bool Build(VkDescriptorSet& set);
	private:
		std::vector<VkWriteDescriptorSet> m_Writes;
		std::vector<VkDescriptorSetLayoutBinding> m_Bindings;

		DescriptorLayoutCache* m_Cache;
		DescriptorAllocator* m_Alloc;
		Ptr<VulkanDevice> m_Device;
	};
}