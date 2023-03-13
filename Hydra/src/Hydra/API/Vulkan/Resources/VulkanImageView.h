#pragma once
#include "Vulkan/Vulkan.h"

#include "Hydra/API/ResourceInterface/ImageView.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
namespace Hydra
{
	class VulkanImageView : public ImageView
	{
	public:
	private:
		VkImageView m_ImageView;
	};
}
