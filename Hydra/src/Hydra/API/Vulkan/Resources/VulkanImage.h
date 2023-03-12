#pragma once
#include "Vulkan/vulkan.h"
#include "Hydra/API/Image.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
namespace Hydra
{
	class VulkanImage : public Image
	{
	public:
		VulkanImage(ImageSpecification& specs);
	private:
		AllocatedImage m_Image;
	};
}
