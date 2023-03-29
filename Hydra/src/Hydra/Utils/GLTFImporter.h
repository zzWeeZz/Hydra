#pragma once
#include <vector>
#include <glm/glm/glm.hpp>
#include <filesystem>
namespace tinygltf
{
	class Node;
	class Model;
}

namespace Hydra
{
	class Submesh;
	class Material;
	struct Primitive
	{
		uint32_t firstIndex;
		uint32_t indexCount;
		int32_t materialIndex;
	};

	struct Mesh
	{
		std::vector<Primitive> primitives;
	};

	struct Node
	{
		Node* parent;
		std::vector<Node> children;
		Mesh mesh;
		glm::mat4 transform;
	};

	class GLTFImporter
	{
	public:
		static void Import(const std::filesystem::path& filepath, std::vector<Submesh>& outVertex, std::vector<Ref<Material>>& materials);
	private:
		static void LoadNode(const tinygltf::Node& node, const tinygltf::Model& model, Node* parent, std::vector<Submesh>& outMeshes, size_t pathHash);
	};
}
