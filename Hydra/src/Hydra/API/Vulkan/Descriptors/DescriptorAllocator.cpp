#include "HYpch.h"
#include "DescriptorAllocator.h"
#include "Hydra/API/Vulkan/Backend/VulkanDevice.h"
namespace Hydra
{
	void DescriptorAllocator::Initialize(Ptr<VulkanDevice> device)
	{
		m_Device = device;
	}
	bool DescriptorAllocator::Allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout)
	{
		
		if (m_CurrentPool == VK_NULL_HANDLE)
		{
			m_CurrentPool = GrabPool();
			m_UsedPools.push_back(m_CurrentPool);
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;

		allocInfo.pSetLayouts = &layout;
		allocInfo.descriptorPool = m_CurrentPool;
		allocInfo.descriptorSetCount = 1;

		auto allocResult = vkAllocateDescriptorSets(m_Device.lock()->GetHandle(), &allocInfo, set);

		bool needReAlloc = false;

		switch (allocResult)
		{
		case VK_SUCCESS:
			return true;
			break;
		case VK_ERROR_FRAGMENTED_POOL:
		case VK_ERROR_OUT_OF_POOL_MEMORY:
			needReAlloc = true;
			break;
		default:
			HY_CORE_ERROR("(DescriptorAllocator::Allocate) Some thing went really wrong when allocating descriptor set!");
			return false;
		}

		if (needReAlloc)
		{
			m_CurrentPool = GrabPool();
			m_UsedPools.push_back(m_CurrentPool);
			allocInfo.descriptorPool = m_CurrentPool;
			allocResult = vkAllocateDescriptorSets(m_Device.lock()->GetHandle(), &allocInfo, set);

			if (allocResult == VK_SUCCESS)
			{
				return true;
			}
		}
		HY_CORE_ERROR("(DescriptorAllocator::Allocate) Some thing went really wrong when allocating descriptor set!");
		return false;
	}
	void DescriptorAllocator::ResetPools()
	{
		for (auto pool : m_UsedPools)
		{
			vkResetDescriptorPool(m_Device.lock()->GetHandle(), pool, 0);
			m_FreePools.push_back(pool);
		}

		m_UsedPools.clear();

		m_CurrentPool = VK_NULL_HANDLE;
	}
	void DescriptorAllocator::Shutdown()
	{
		for (auto pool : m_FreePools)
		{
			vkDestroyDescriptorPool(m_Device.lock()->GetHandle(), pool, nullptr);
		}
		for (auto pool : m_UsedPools)
		{
			vkDestroyDescriptorPool(m_Device.lock()->GetHandle(), pool, nullptr);
		}
	}
	VkDescriptorPool DescriptorAllocator::GrabPool()
	{
		if (!m_FreePools.empty())
		{
			VkDescriptorPool pool = m_FreePools.back();
			m_FreePools.pop_back();
			return pool;
		}
		else
		{
			return CreatePool(1000, 0);
		}
		return VkDescriptorPool();
	}
	VkDescriptorPool DescriptorAllocator::CreatePool(int32_t count, VkDescriptorPoolCreateFlags flags)
	{
		std::vector<VkDescriptorPoolSize> sizes;
		sizes.reserve(m_DescriptorSizes.sizes.size());
		for (auto& sz : m_DescriptorSizes.sizes)
		{
			sizes.emplace_back(sz.first, static_cast<uint32_t>(sz.second * static_cast<float>(count)));
		}

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = flags;
		poolInfo.maxSets = count;
		poolInfo.poolSizeCount = static_cast<uint32_t>(sizes.size());
		poolInfo.pPoolSizes = sizes.data();

		VkDescriptorPool descPool;
		vkCreateDescriptorPool(m_Device.lock()->GetHandle(), &poolInfo, nullptr, &descPool);

		return descPool;
	}
}