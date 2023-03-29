#pragma once
#include "glm/glm/glm.hpp"
#include <glm/glm/gtx/quaternion.hpp>
#include "Hydra/API/ResourceInterface/Buffer.h"
#include "Hydra/Rendering/GraphicsContext.h"
#include "Hydra/Utils/GLTFImporter.h"
#include "Hydra/Assets/Material.h"
#include <filesystem>
namespace Hydra
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec2 TexCoord;
	};

	struct Submesh
	{
		Submesh(std::vector<Vertex>& verts, std::vector<uint16_t>& indices, int32_t materialId)
		{
			BufferCreateSpecification bufferSpecs = {};

			bufferSpecs.usage = BufferUsage::VertexBuffer;
			bufferSpecs.stride = sizeof(Vertex);
			bufferSpecs.count = verts.size();
			bufferSpecs.data = verts.data();
			bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

			GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, vertexBuffer);

			bufferSpecs.usage = BufferUsage::IndexBuffer;
			bufferSpecs.stride = sizeof(uint16_t);
			bufferSpecs.count = indices.size();
			bufferSpecs.data = indices.data();
			bufferSpecs.allocationUsage = MemoryUsage::CPU_To_GPU;

			GraphicsContext::GetDevice().lock()->CreateBuffer(bufferSpecs, indexBuffer);
			materialID = materialId;
		}

		Ref<Buffer> vertexBuffer;
		Ref<Buffer> indexBuffer;
		int32_t materialID = -1;
	};

	class MeshObject
	{
	public:
		void Loader(const std::filesystem::path& path)
		{
			GLTFImporter::Import(path, m_Submeshes, m_Materials);
		}
		std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
		std::vector<Ref<Material>>& GetMaterials() { return m_Materials; }
	private:
		std::vector<Submesh> m_Submeshes;
		std::vector<Ref<Material>> m_Materials;

	};
}
