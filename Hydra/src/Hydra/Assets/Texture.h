#pragma once
#include "Hydra/API/ResourceInterface/Image.h"
#include <filesystem>
namespace Hydra
{
	class Texture
	{
	public:
		Texture(const std::filesystem::path& path);

		[[nodiscard]] FORCEINLINE Ptr<Image> GetImage() { return m_Image; }

		static Ref<Texture> Create(const std::filesystem::path& path);

	private:
		Ref<Image> m_Image;
	};
}
