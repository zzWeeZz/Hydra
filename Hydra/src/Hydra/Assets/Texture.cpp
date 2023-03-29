#include "HYpch.h"
#include "Texture.h"
#include <Hydra/Rendering/GraphicsContext.h>
#define TINYDDSLOADER_IMPLEMENTATION
#include "tinydds/tinyddsloader.h"
namespace Hydra
{
	using namespace tinyddsloader;
	Texture::Texture(const std::filesystem::path& path)
	{
		ImageSpecification specs = {};

		if (path.extension() == ".dds")
		{
			DDSFile dds;

			auto ret = dds.Load(path.string().c_str());
			if (tinyddsloader::Result::Success == ret)
			{
				//HY_CORE_ERROR("Failed to load dds file: {}", path.string());

				specs.width = dds.GetWidth();
				specs.height = dds.GetHeight();
				specs.depth = dds.GetDepth();
				/*	for (uint32_t arrayIdx = 0; arrayIdx < dds.GetArraySize(); arrayIdx++)
					{
						for (uint32_t mipIdx = 0; mipIdx < dds.GetMipCount(); mipIdx++)
						{
							const auto* imageData = dds.GetImageData(mipIdx, arrayIdx);
							std::cout << "Array[" << arrayIdx << "] "
								<< "Mip[" << mipIdx << "]: "
								<< "(" << imageData->m_width << ", "
								<< imageData->m_height << ", " << imageData->m_depth
								<< ")\n";
						}
					}*/
				specs.format = ImageFormat::BC7UN;
				specs.mipLevels = dds.GetMipCount();
				 specs.ImageData = dds.GetImageData()->m_mem;
				 //specs.memSize = dds.GetImageData()->m_memSlicePitch * dds.GetImageData()->m_memPitch;
				 GraphicsContext::GetDevice().lock()->CreateImage(specs, m_Image);


				 for (size_t i = 1; i < dds.GetMipCount(); ++i)
				 {
					 MipSpecification mipSpecs = {};
					 mipSpecs.height = dds.GetImageData(i)->m_height;
					 mipSpecs.width = dds.GetImageData(i)->m_width;

					 mipSpecs.mipLevel = i;
					 mipSpecs.MipData = dds.GetImageData(i)->m_mem;
					 mipSpecs.MipSize = dds.GetImageData(i)->m_memSlicePitch;

					 GraphicsContext::GetDevice().lock()->AddMipToImage(m_Image, mipSpecs);
				 }
			}

		}


	}
	Ref<Texture> Texture::Create(const std::filesystem::path& path)
	{
		return std::make_shared<Texture>(path);
	}
}