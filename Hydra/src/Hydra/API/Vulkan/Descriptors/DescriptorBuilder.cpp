#include "HYpch.h"
#include "DescriptorBuilder.h"

#include "Hydra/API/Vulkan/Descriptors/DescriptorAllocator.h"
#include "Hydra/API/Vulkan/Descriptors/DescriptorLayoutCache.h"

namespace Hydra
{
	DescriptorBuilder DescriptorBuilder::Begin(DescriptorLayoutCache* layoutCache, DescriptorAllocator* allocator, Ptr<VulkanDevice> device)
	{
		DescriptorBuilder builder;
		builder.m_Cache = layoutCache;
		builder.m_Alloc = allocator;
		builder.m_Device = device;
		return builder;
	}
	DescriptorBuilder& DescriptorBuilder::BindBuffer(VkDescriptorSetLayoutBinding& binding, VkDescriptorBufferInfo* bufferInfo)
	{
		m_Bindings.push_back(binding);

		//create the descriptor write
		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = binding.descriptorType;
		newWrite.pBufferInfo = bufferInfo;
		newWrite.dstBinding = binding.binding;

		m_Writes.push_back(newWrite);
		return *this;
	}
	DescriptorBuilder& DescriptorBuilder::BindImage(VkDescriptorSetLayoutBinding& binding, VkDescriptorImageInfo* imageInfo)
	{
		
		m_Bindings.push_back(binding);

		VkWriteDescriptorSet newWrite{};
		newWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		newWrite.pNext = nullptr;

		newWrite.descriptorCount = 1;
		newWrite.descriptorType = binding.descriptorType;
		newWrite.pImageInfo = imageInfo;
		newWrite.dstBinding = binding.binding;

		m_Writes.push_back(newWrite);
		return *this;
	}
	bool DescriptorBuilder::Build(VkDescriptorSet& set, VkDescriptorSetLayout& layout)
	{
		//build layout first
		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;

		layoutInfo.pBindings = m_Bindings.data();
		layoutInfo.bindingCount = static_cast<uint32_t>(m_Bindings.size());

		layout = m_Cache->CreateDescriptorLayout(&layoutInfo, m_Device);

		//allocate descriptor
		bool success = m_Alloc->Allocate(&set, layout);
		if (!success)
		{
			return false;
		};

		//write descriptor
		for (VkWriteDescriptorSet& w : m_Writes) 
		{
			w.dstSet = set;
		}

		vkUpdateDescriptorSets(m_Device.lock()->GetHandle(), static_cast<uint32_t>(m_Writes.size()), m_Writes.data(), 0, nullptr);

		return true;
	}
	bool DescriptorBuilder::Build(VkDescriptorSet& set)
	{
		VkDescriptorSetLayout layout;
		return Build(set, layout);
	}
	void DescriptorBuilder::Reset()
	{
		m_Bindings.clear();
		m_Writes.clear();
	}
}