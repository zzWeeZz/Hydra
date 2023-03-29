#pragma once
#include <filesystem>
#include <string>
#include "Hydra/Utils/Memory.h"
#include "Hydra/Assets/Texture.h"
namespace Hydra
{
	class Texture;
	class Material
	{
	public:
		Material(const std::string& matName, const std::filesystem::path& path);

		Ptr<Texture> GetColorTexture() { return m_Texture; }

		static Ref<Material> Create(const std::string& matName, const std::filesystem::path& path);
	private:
		Ref<Texture> m_Texture;
	};
}
