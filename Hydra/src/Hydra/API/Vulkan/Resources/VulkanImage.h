#pragma once
#include "Vulkan/vulkan.h"
#include "Hydra/API/ResourceInterface/Image.h"
#include "Hydra/API/Vulkan/VulkanUtils.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
namespace Hydra
{
	class VulkanDevice;

	class VulkanImage : public Image
	{
	public:
		VulkanImage(ImageSpecification& specs, Ptr<VulkanDevice> device);

		[[nodiscard]] FORCEINLINE AllocatedImage GetAllocatedImage() { return m_Image; }
		[[nodiscard]] FORCEINLINE AllocatedImage GetAllocatedImage() const { return m_Image; }

		[[nodiscard]] FORCEINLINE VkImageView GetView() { return m_View; }

	private:
		void Validate();
		AllocatedImage m_Image;
		VkImageView m_View;
		Ptr<VulkanDevice> m_Device;
	};
}
