#include <GSIO/mesh.h>
#include <filesystem>
#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/tools.hpp>

namespace fg = fastgltf;

GSIO_response GSIO_meshasset::LoadData(GSIO_assetdatastructure assetData)
{
    std::string filepath = assetData.fullPath+".glb";
    std::filesystem::path path = std::filesystem::path(filepath);
    static constexpr auto supportedExtensions =
        fastgltf::Extensions::KHR_mesh_quantization |
        fastgltf::Extensions::KHR_texture_transform |
        fastgltf::Extensions::KHR_materials_variants;

    fastgltf::Parser parser(supportedExtensions);

    constexpr auto gltfOptions =
        fastgltf::Options::DontRequireValidAssetMember |
        fastgltf::Options::AllowDouble |
        fastgltf::Options::LoadExternalBuffers |
        fastgltf::Options::LoadExternalImages |
        fastgltf::Options::GenerateMeshIndices;

    auto gltfFile = fastgltf::MappedGltfFile::FromPath(path);
    if (!bool(gltfFile)) {
        return {false, std::string("Failed to open glTF file: ") + std::string(fastgltf::getErrorMessage(gltfFile.error()))};
    }

    auto assetLoad = parser.loadGltf(gltfFile.get(), path.parent_path(), gltfOptions);
    if (assetLoad.error() != fastgltf::Error::None) {
        return {false, std::string("Failed to load glTF file: ") + std::string(fastgltf::getErrorMessage(assetLoad.error()))};
    }

    fastgltf::Asset asset = std::move(assetLoad.get());

    for (auto& mesh : asset.meshes)
    {
        for (auto it = mesh.primitives.begin(); it != mesh.primitives.end(); ++it)
        {
            auto positionIt = it->findAttribute("POSITION");
            assert(positionIt != it->attributes.end());
            assert(it->indicesAccessor.has_value());
            
            uint8_t matIndex = 0;
            if (it->materialIndex.has_value())
            {
                matIndex = (uint8_t)it->materialIndex.value();
            }

            auto& positionAccessor = asset.accessors[positionIt->accessorIndex];
            if (!positionAccessor.bufferViewIndex.has_value())
                continue;

            const auto& indexAccessor = asset.accessors[it->indicesAccessor.value()];
            indices.resize(indexAccessor.count);
            fastgltf::copyFromAccessor<uint32_t>(asset, indexAccessor, indices.data());

            std::size_t vertexOffset = vertices.size();
            vertices.resize(vertexOffset + positionAccessor.count);

            fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, positionAccessor, [&](fastgltf::math::fvec3 pos, std::size_t idx) {
                vertices[vertexOffset + idx].position = glm::vec3(pos.x(), pos.y(), pos.z());
                vertices[vertexOffset + idx].uv = glm::vec2(0.0f);
                vertices[vertexOffset + idx].normal = glm::vec3(0.0f, 1.0f, 0.0f);
                vertices[vertexOffset + idx].materialIndex = matIndex;
            });

            if (const auto texcoord = it->findAttribute("TEXCOORD_0"); texcoord != it->attributes.end()) {
                auto& texCoordAccessor = asset.accessors[texcoord->accessorIndex];
                if (texCoordAccessor.bufferViewIndex.has_value()) {
                    fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec2>(asset, texCoordAccessor, [&](fastgltf::math::fvec2 uv, std::size_t idx) {
                        vertices[vertexOffset + idx].uv = glm::vec2(uv.x(), uv.y());
                    });
                }
            }

            if (const auto normal = it->findAttribute("NORMAL"); normal != it->attributes.end()) {
                auto& normalAccessor = asset.accessors[normal->accessorIndex];
                if (normalAccessor.bufferViewIndex.has_value()) {
                    fastgltf::iterateAccessorWithIndex<fastgltf::math::fvec3>(asset, normalAccessor, [&](fastgltf::math::fvec3 norm, std::size_t idx) {
                        vertices[vertexOffset + idx].normal = glm::vec3(norm.x(), norm.y(), norm.z());
                    });
                }
            }
        }
        break;
    }

    return {true};
}