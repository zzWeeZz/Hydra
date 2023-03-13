#pragma once
#include "Vulkan/vulkan.h"
#include "Hydra/API/ResourceInterface/Image.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
namespace Hydra
{
	class VulkanImage : public Image
	{
	public:
		VulkanImage(ImageSpecification& specs);

		[[nodiscard]] FORCEINLINE AllocatedImage GetAllocatedImage() { return m_Image; }
		[[nodiscard]] FORCEINLINE AllocatedImage GetAllocatedImage() const { return m_Image; }

	private:
		AllocatedImage m_Image;
	};
}
