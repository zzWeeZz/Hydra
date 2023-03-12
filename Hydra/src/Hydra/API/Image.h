#pragma once
#include "Hydra/API/Resource.h"
#include "Hydra/API/ApiFormats.h"
namespace Hydra
{

	struct ImageSpecification
	{
		ImageType imageType;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		uint32_t mipLevels;
		uint32_t arrayLayers;
		ImageFormat format;
		ImageUsage usage;

		MemoryUsage memoryUsage;
	};

	class Image : public Resource
	{
	public:
		Image() = default;
		virtual ~Image(){}
	};
}