#pragma once
#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
namespace Hydra
{

	struct ImageSpecification
	{
		ImageType imageType = ImageType::Image2D;
		uint32_t width = 100;
		uint32_t height = 100;
		uint32_t depth = 1;
		uint32_t mipLevels = 1;
		uint32_t arrayLayers = 1;
		ImageFormat format = ImageFormat::RGBA8UN;
		ImageUsage usage;

		uint32_t memSize;

		void* ImageData = nullptr;
	};

	struct MipSpecification
	{
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t mipLevel = 0;
		size_t MipSize = 0;
		void* MipData = nullptr;
	};

	class Image : public Resource
	{
	public:
		Image() = default;
		virtual ~Image(){}
	protected:
		ImageSpecification m_Specs;
	};
}