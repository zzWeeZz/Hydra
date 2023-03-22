#pragma once
#include <vulkan/vulkan.h>

namespace Hydra
{
	class VulkanDevice;
	class DescriptorAllocator
	{
	public:
		

		void Initialize(Ptr<VulkanDevice> device);
		bool Allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);
		void ResetPools();

		void Shutdown();
	private:
		struct PoolSizes
		{
			std::vector<std::pair<VkDescriptorType, float>> sizes =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
			};
		};

		VkDescriptorPool GrabPool();
		VkDescriptorPool CreatePool(int32_t count, VkDescriptorPoolCreateFlags flags);

		VkDescriptorPool m_CurrentPool;

		PoolSizes m_DescriptorSizes;
		std::vector<VkDescriptorPool> m_UsedPools;
		std::vector<VkDescriptorPool> m_FreePools;
		Ptr<VulkanDevice> m_Device;
	};
}