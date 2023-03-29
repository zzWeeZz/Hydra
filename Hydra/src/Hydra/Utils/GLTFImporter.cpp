#include "HYpch.h"
#include "GLTFImporter.h"
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE 
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <filesystem>
#include <iostream>

#include "tinygltf/tiny_gltf.h"

#include "Hydra/Assets/Mesh.h"
#include <Hydra/Assets/Material.h>

namespace Hydra
{
	void GLTFImporter::Import(const std::filesystem::path& filepath, std::vector<Submesh>& outMeshes, std::vector<Ref<Material>>& materials)
	{
		tinygltf::Model model;
		tinygltf::TinyGLTF loader;
		std::string err;
		std::string warn;
		if (filepath.extension() == ".glb")
		{
			bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filepath.string());
			if (!warn.empty())
			{
				HY_CORE_WARN("{0}", warn);
			}
			if (!err.empty())
			{
				HY_CORE_ERROR("{0}", err);
			}
			if (!ret)
			{
				HY_CORE_ERROR("Failed to load gltf file {0}", filepath.string());
				return;
			}
		}
		else if (filepath.extension() == ".gltf")
		{
			bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filepath.string());
			if (!warn.empty())
			{
				HY_CORE_WARN("{0}", warn);
			}
			if (!err.empty())
			{
				HY_CORE_ERROR("{0}", err);
			}
			if (!ret)
			{
				HY_CORE_ERROR("Failed to load gltf file {0}", filepath.string());
				return;
			}
		}

		const tinygltf::Scene& scene = model.scenes[model.defaultScene];
		std::hash<std::string> strHasher;
		const size_t pathHash = strHasher(filepath.string());
		for (int i : scene.nodes)
		{
			const tinygltf::Node& node = model.nodes[i];
			LoadNode(node, model, nullptr, outMeshes, pathHash);
		}
		materials.resize(model.materials.size());
		for (size_t i = 0; auto& mat : model.materials)
		{
			HY_CORE_INFO("{}", mat.name);
			materials[i] = Material::Create(mat.name, filepath);
			++i;
		}
	}
	void GLTFImporter::LoadNode(const tinygltf::Node& node, const tinygltf::Model& model, Node* parent, std::vector<Submesh>& outMeshes, size_t pathHash)
	{
		Node currentNode{};
		
		if (node.translation.size() == 3)
		{
			double dubScale[3] = { node.translation[0], node.translation[1], node.translation[2] };
			currentNode.transform = glm::translate(glm::mat4(1.f), glm::vec3(static_cast<float>(dubScale[0]), static_cast<float>(dubScale[1]), static_cast<float>(dubScale[2])));
		}

		if (node.scale.size() == 3)
		{
			double dubScale[3] = { node.scale[0], node.scale[1], node.scale[2] };
			currentNode.transform = currentNode.transform * glm::scale(glm::mat4(1.f), glm::vec3(static_cast<float>(dubScale[0]), static_cast<float>(dubScale[1]), static_cast<float>(dubScale[2])));
		}

		if (node.rotation.size() == 3)
		{
			double dubRotation[3] = { node.rotation[0], node.rotation[1], node.rotation[2] };
			currentNode.transform = currentNode.transform * glm::rotate(glm::mat4(1.f), static_cast<float>(dubRotation[0]), glm::vec3(1.f, 0.f, 0.f));
			currentNode.transform = currentNode.transform * glm::rotate(glm::mat4(1.f), static_cast<float>(dubRotation[1]), glm::vec3(0.f, 1.f, 0.f));
			currentNode.transform = currentNode.transform * glm::rotate(glm::mat4(1.f), static_cast<float>(dubRotation[2]), glm::vec3(0.f, 0.f, 1.f));
		}


		for (int i : node.children)
		{
			LoadNode(model.nodes[i], model, &currentNode, outMeshes, pathHash);
		}

		if (node.mesh > -1)
		{
			const tinygltf::Mesh mesh = model.meshes[node.mesh];

			for (const tinygltf::Primitive primative : mesh.primitives)
			{
				const float* positionBuffer = nullptr;
				const float* normalBuffer = nullptr;
				const float* texCoordsBuffer = nullptr;
				const float* tangentBuffer = nullptr;

				size_t vertexCount = 0;
				if (primative.attributes.find("POSITION") != primative.attributes.end())
				{
					const tinygltf::Accessor& accessor = model.accessors[primative.attributes.find("POSITION")->second];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
					positionBuffer = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);

					vertexCount = accessor.count;
				}

				if (primative.attributes.find("NORMAL") != primative.attributes.end())
				{
					const tinygltf::Accessor& accessor = model.accessors[primative.attributes.find("NORMAL")->second];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
					normalBuffer = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
				}

				if (primative.attributes.find("TEXCOORD_0") != primative.attributes.end())
				{
					const tinygltf::Accessor& accessor = model.accessors[primative.attributes.find("TEXCOORD_0")->second];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
					texCoordsBuffer = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
				}

				if (primative.attributes.find("TANGENT") != primative.attributes.end())
				{
					const tinygltf::Accessor& accessor = model.accessors[primative.attributes.find("TANGENT")->second];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];
					tangentBuffer = reinterpret_cast<const float*>(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);
				}
				std::vector<Vertex> outVerties;
				std::vector<uint16_t> outIndices;
				outVerties.resize(vertexCount);
				for (size_t index = 0; index < vertexCount; ++index)
				{
					Vertex& vertex = outVerties[index];
					vertex.Position = positionBuffer ? *(glm::vec3*)&positionBuffer[index * 3] : glm::vec3();
					vertex.Normal = glm::normalize(normalBuffer ? *(glm::vec3*)&normalBuffer[index * 3] : glm::vec3(1.f));
					vertex.Tangent = glm::normalize(tangentBuffer ? *(glm::vec3*)&tangentBuffer[index * 3] : glm::vec3(1.f));
					vertex.TexCoord = texCoordsBuffer ? *(glm::vec2*)&texCoordsBuffer[index * 2] : glm::vec2();
				}
				{
					const tinygltf::Accessor& accessor = model.accessors[primative.indices];
					const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
					const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

					switch (accessor.componentType)
					{
					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
					{
						const uint32_t* buf = reinterpret_cast<const uint32_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
						for (size_t index = 0; index < accessor.count; index++)
						{
							outIndices.emplace_back(buf[index]);
						}

						break;
					}

					case TINYGLTF_PARAMETER_TYPE_SHORT:
					{
						const int16_t* buf = reinterpret_cast<const int16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
						for (size_t index = 0; index < accessor.count; index++)
						{
							outIndices.emplace_back(buf[index]);
						}
						break;
					}

					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
					{
						const uint16_t* buf = reinterpret_cast<const uint16_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
						for (size_t index = 0; index < accessor.count; index++)
						{
							outIndices.emplace_back(buf[index]);
						}
						break;
					}

					case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
					{
						const uint8_t* buf = reinterpret_cast<const uint8_t*>(&buffer.data[accessor.byteOffset + bufferView.byteOffset]);
						for (size_t index = 0; index < accessor.count; index++)
						{
							outIndices.emplace_back(buf[index]);
						}
						break;
					}

					default:
						HY_CORE_ERROR("Index component not supported!");
						return;
					}
					outMeshes.emplace_back(outVerties, outIndices, primative.material);
				}
			}
		}
	}
}