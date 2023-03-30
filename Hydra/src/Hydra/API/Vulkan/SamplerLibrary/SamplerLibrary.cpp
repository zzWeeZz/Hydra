#include "HYpch.h"
#include "SamplerLibrary.h"
#include "Hydra/API/Vulkan/VulkanAllocator.h"
#include "Hydra/API/Vulkan/Backend/VulkanDevice.h"

void Hydra::SamplerLibrary::Initalize(Ref<VulkanDevice> device)
{
	m_Device = device;
}

void Hydra::SamplerLibrary::Add(const std::string& key, const VulkanFilter& filter, const VulkanAddress& address, const VulkanMipmapMode& mipmapMode, bool isAnisotropic)
{
    VkSamplerCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = FormatToVkFormat(filter);
    info.minFilter = FormatToVkFormat(filter);
    info.addressModeU = FormatToVkFormat(address);
    info.addressModeV = FormatToVkFormat(address);
    info.addressModeW = FormatToVkFormat(address);
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_ALWAYS;
    
	info.mipmapMode = FormatToVkFormat(mipmapMode);
	info.mipLodBias = 0.0f;
	info.minLod = 7.0f;
	info.maxLod = 11.0f;
	info.anisotropyEnable = isAnisotropic;
	info.maxAnisotropy = 1.0f;
    
    VkSampler sampler;
    HY_VK_CHECK(vkCreateSampler(m_Device.lock()->GetHandle(), &info, nullptr, &sampler));
    m_Samplers[key] = sampler;
    VulkanAllocator::CustomDeletion([&, key]() {HY_CORE_INFO("TitanAllocator: Destroying sampler: {0}", key.c_str()); vkDestroySampler(m_Device.lock()->GetHandle(), m_Samplers[key], nullptr); });
}

VkSampler& Hydra::SamplerLibrary::Get(const std::string& key)
{
    if (!m_Samplers.contains(key))
    {
        HY_CORE_ERROR("Samper {0} does not exist!", key);
    }

    return m_Samplers[key];
}
