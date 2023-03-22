#include "HYpch.h"
#include "DescriptorLayoutCache.h"


namespace Hydra
{
	void DescriptorLayoutCache::Shutdown()
	{
		for (auto& pair : m_LayoutCache)
		{
			vkDestroyDescriptorSetLayout(m_Device.lock()->GetHandle(), pair.second, nullptr);
		}
	}
	VkDescriptorSetLayout DescriptorLayoutCache::CreateDescriptorLayout(VkDescriptorSetLayoutCreateInfo* info, Ptr<VulkanDevice> device)
	{
		if (m_Device.lock() == nullptr)
		{
			m_Device = device;
		}
		DescriptorLayoutInfo layoutInfo;
		layoutInfo.bindings.reserve(info->bindingCount);
		bool isSorted = true;
		int32_t lastBinding = -1;

		for (size_t i = 0; i < info->bindingCount; ++i)
		{
			layoutInfo.bindings.push_back(info->pBindings[i]);

			if (info->pBindings[i].binding > static_cast<uint32_t>(lastBinding))
			{
				lastBinding = info->pBindings[i].binding;
			}
			else
			{
				isSorted = false;
			}
		}
		if (!isSorted)
		{
			std::sort(layoutInfo.bindings.begin(), layoutInfo.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b)
				{
					return a.binding < b.binding;
				});
		}
		if (m_LayoutCache.contains(layoutInfo))
		{
			return m_LayoutCache[layoutInfo];
		}
		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(m_Device.lock()->GetHandle(), info, nullptr, &layout);

		m_LayoutCache[layoutInfo] = layout;
		return layout;
	}
	bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==(const DescriptorLayoutInfo& other) const
	{
		if (bindings.size() != other.bindings.size())
		{
			return false;
		}
		for (size_t i = 0; i < bindings.size(); ++i)
		{
			if (other.bindings[i].binding != bindings[i].binding)
			{
				return false;
			}
			if (other.bindings[i].descriptorType != bindings[i].descriptorType)
			{
				return false;
			}
			if (other.bindings[i].descriptorCount != bindings[i].descriptorCount)
			{
				return false;
			}
			if (other.bindings[i].stageFlags != bindings[i].stageFlags)
			{
				return false;
			}
		}
		return true;
	}
	size_t DescriptorLayoutCache::DescriptorLayoutInfo::hash() const
	{
		using std::size_t;
		using std::hash;

		size_t result = hash<size_t>()(bindings.size());

		for (const VkDescriptorSetLayoutBinding& b : bindings)
		{
			//pack the binding data into a single int64. Not fully correct but it's ok
			size_t binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

			//shuffle the packed binding data and xor it with the main hash
			result ^= hash<size_t>()(binding_hash);
		}

		return result;
	}
}