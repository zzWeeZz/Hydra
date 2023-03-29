#include "HYpch.h"
#include "Material.h"
#include "Hydra/Assets/Texture.h"
namespace Hydra
{
	Material::Material(const std::string& matName, const std::filesystem::path& path)
	{
		const auto& colorTextureName = matName + "_c";

		auto texturePath = path.parent_path().string() + "/" + colorTextureName + ".dds";

		m_Texture = Texture::Create(texturePath);
	}
	Ref<Material> Material::Create(const std::string& matName, const std::filesystem::path& path)
	{
		return std::make_shared<Material>(matName, path);
	}
}