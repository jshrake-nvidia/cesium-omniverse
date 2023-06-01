#include "cesium/omniverse/GltfUtil.h"

#ifdef CESIUM_OMNI_MSVC
#pragma push_macro("OPAQUE")
#undef OPAQUE
#endif

#include <CesiumGltf/AccessorView.h>
#include <CesiumGltf/ExtensionKhrMaterialsUnlit.h>
#include <CesiumGltf/Model.h>
#include <pxr/base/gf/range3d.h>
#include <spdlog/fmt/fmt.h>

#include <numeric>

namespace cesium::omniverse::GltfUtil {

namespace {

const CesiumGltf::MaterialPBRMetallicRoughness defaultPbrMetallicRoughness;

template <typename IndexType>
IndicesAccessor getIndicesAccessor(
    const CesiumGltf::MeshPrimitive& primitive,
    const CesiumGltf::AccessorView<IndexType>& indicesAccessorView) {
    if (indicesAccessorView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    if (primitive.mode == CesiumGltf::MeshPrimitive::Mode::TRIANGLES) {
        if (indicesAccessorView.size() % 3 != 0) {
            return {};
        }

        return IndicesAccessor(indicesAccessorView);
    }

    if (primitive.mode == CesiumGltf::MeshPrimitive::Mode::TRIANGLE_STRIP) {
        if (indicesAccessorView.size() <= 2) {
            return {};
        }

        return IndicesAccessor::FromTriangleStrips(indicesAccessorView);
    }

    if (primitive.mode == CesiumGltf::MeshPrimitive::Mode::TRIANGLE_FAN) {
        if (indicesAccessorView.size() <= 2) {
            return {};
        }

        return IndicesAccessor::FromTriangleFans(indicesAccessorView);
    }

    return {};
}

CesiumGltf::AccessorView<glm::fvec2> getTexcoordsView(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const std::string& semantic,
    uint64_t setIndex) {

    const auto texcoordAttribute = primitive.attributes.find(fmt::format("{}_{}", semantic, setIndex));
    if (texcoordAttribute == primitive.attributes.end()) {
        return {};
    }

    auto texcoordAccessor = model.getSafe<CesiumGltf::Accessor>(&model.accessors, texcoordAttribute->second);
    if (!texcoordAccessor) {
        return {};
    }

    auto texcoordsView = CesiumGltf::AccessorView<glm::fvec2>(model, *texcoordAccessor);

    if (texcoordsView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    return texcoordsView;
}

CesiumGltf::AccessorView<glm::fvec3>
getNormalsView(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive) {
    auto normalAttribute = primitive.attributes.find("NORMAL");
    if (normalAttribute == primitive.attributes.end()) {
        return {};
    }

    auto normalAccessor = model.getSafe<CesiumGltf::Accessor>(&model.accessors, normalAttribute->second);
    if (!normalAccessor) {
        return {};
    }

    const auto normalsView = CesiumGltf::AccessorView<glm::fvec3>(model, *normalAccessor);

    if (normalsView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    return normalsView;
}

CesiumGltf::AccessorView<glm::fvec3>
getPositionsView(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive) {
    auto positionAttribute = primitive.attributes.find("POSITION");
    if (positionAttribute == primitive.attributes.end()) {
        return {};
    }

    auto positionAccessor = model.getSafe<CesiumGltf::Accessor>(&model.accessors, positionAttribute->second);
    if (!positionAccessor) {
        return {};
    }

    auto positionsView = CesiumGltf::AccessorView<glm::fvec3>(model, *positionAccessor);
    if (positionsView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    return positionsView;
}

TexcoordsAccessor getTexcoords(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const std::string& semantic,
    uint64_t setIndex,
    const glm::fvec2& translation,
    const glm::fvec2& scale,
    bool flipVertical) {

    const auto texcoordsView = getTexcoordsView(model, primitive, semantic, setIndex);

    if (texcoordsView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    return TexcoordsAccessor(texcoordsView, translation, scale, flipVertical);
}

} // namespace

PositionsAccessor getPositions(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive) {
    const auto positionsView = getPositionsView(model, primitive);

    if (positionsView.status() != CesiumGltf::AccessorViewStatus::Valid) {
        return {};
    }

    return PositionsAccessor(positionsView);
}

std::optional<pxr::GfRange3d> getExtent(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive) {
    auto positionAttribute = primitive.attributes.find("POSITION");
    if (positionAttribute == primitive.attributes.end()) {
        return std::nullopt;
    }

    auto positionAccessor = model.getSafe<CesiumGltf::Accessor>(&model.accessors, positionAttribute->second);
    if (!positionAccessor) {
        return std::nullopt;
    }

    const auto& min = positionAccessor->min;
    const auto& max = positionAccessor->max;

    if (min.size() != 3 || max.size() != 3) {
        return std::nullopt;
    }

    return pxr::GfRange3d(pxr::GfVec3d(min[0], min[1], min[2]), pxr::GfVec3d(max[0], max[1], max[2]));
}

IndicesAccessor getIndices(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const PositionsAccessor& positions) {
    const auto indicesAccessor = model.getSafe<CesiumGltf::Accessor>(&model.accessors, primitive.indices);
    if (!indicesAccessor) {
        return IndicesAccessor(positions.size());
    }

    if (indicesAccessor->componentType == CesiumGltf::AccessorSpec::ComponentType::UNSIGNED_BYTE) {
        CesiumGltf::AccessorView<uint8_t> view{model, *indicesAccessor};
        return getIndicesAccessor(primitive, view);
    } else if (indicesAccessor->componentType == CesiumGltf::AccessorSpec::ComponentType::UNSIGNED_SHORT) {
        CesiumGltf::AccessorView<uint16_t> view{model, *indicesAccessor};
        return getIndicesAccessor(primitive, view);
    } else if (indicesAccessor->componentType == CesiumGltf::AccessorSpec::ComponentType::UNSIGNED_INT) {
        CesiumGltf::AccessorView<uint32_t> view{model, *indicesAccessor};
        return getIndicesAccessor(primitive, view);
    }

    return {};
}

NormalsAccessor getNormals(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const PositionsAccessor& positions,
    const IndicesAccessor& indices,
    bool smoothNormals) {

    const auto normalsView = getNormalsView(model, primitive);

    if (normalsView.status() == CesiumGltf::AccessorViewStatus::Valid) {
        return NormalsAccessor(normalsView);
    }

    if (smoothNormals) {
        return NormalsAccessor::GenerateSmooth(positions, indices);
    }

    // Otherwise if normals are missing and smoothNormals is false Omniverse will generate flat normals for us automatically
    return {};
}

TexcoordsAccessor getTexcoords(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    uint64_t setIndex,
    const glm::fvec2& translation,
    const glm::fvec2& scale) {
    return getTexcoords(model, primitive, "TEXCOORD", setIndex, translation, scale, true);
}

FaceVertexCountsAccessor getFaceVertexCounts(const IndicesAccessor& indices) {
    return FaceVertexCountsAccessor(indices.size() / 3);
}

pxr::GfVec3f getBaseColorFactor(const CesiumGltf::Material& material) {
    const auto& pbrMetallicRoughness = material.pbrMetallicRoughness;
    if (pbrMetallicRoughness.has_value()) {
        return pxr::GfVec3f(
            static_cast<float>(pbrMetallicRoughness.value().baseColorFactor[0]),
            static_cast<float>(pbrMetallicRoughness.value().baseColorFactor[1]),
            static_cast<float>(pbrMetallicRoughness.value().baseColorFactor[2]));
    }

    return getDefaultBaseColorFactor();
}

float getMetallicFactor(const CesiumGltf::Material& material) {
    if (material.hasExtension<CesiumGltf::ExtensionKhrMaterialsUnlit>()) {
        // Unlit materials aren't supported in Omniverse yet but we can hard code the metallic factor to something reasonable
        return 0.0f;
    }

    const auto& pbrMetallicRoughness = material.pbrMetallicRoughness;
    if (pbrMetallicRoughness.has_value()) {
        return static_cast<float>(pbrMetallicRoughness->metallicFactor);
    }

    return getDefaultMetallicFactor();
}

float getRoughnessFactor(const CesiumGltf::Material& material) {
    if (material.hasExtension<CesiumGltf::ExtensionKhrMaterialsUnlit>()) {
        // Unlit materials aren't supported in Omniverse yet but we can hard code the roughness factor to something reasonable
        return 1.0f;
    }

    const auto& pbrMetallicRoughness = material.pbrMetallicRoughness;
    if (pbrMetallicRoughness.has_value()) {
        return static_cast<float>(pbrMetallicRoughness->roughnessFactor);
    }

    return getDefaultRoughnessFactor();
}

pxr::GfVec3f getDefaultBaseColorFactor() {
    return pxr::GfVec3f(
        static_cast<float>(defaultPbrMetallicRoughness.baseColorFactor[0]),
        static_cast<float>(defaultPbrMetallicRoughness.baseColorFactor[1]),
        static_cast<float>(defaultPbrMetallicRoughness.baseColorFactor[2]));
}

float getDefaultMetallicFactor() {
    return static_cast<float>(defaultPbrMetallicRoughness.metallicFactor);
}

float getDefaultRoughnessFactor() {
    return static_cast<float>(defaultPbrMetallicRoughness.roughnessFactor);
}

std::optional<uint64_t> getBaseColorTextureIndex(const CesiumGltf::Model& model, const CesiumGltf::Material& material) {
    const auto& pbrMetallicRoughness = material.pbrMetallicRoughness;
    if (pbrMetallicRoughness.has_value() && pbrMetallicRoughness->baseColorTexture.has_value()) {
        const auto index = pbrMetallicRoughness->baseColorTexture->index;
        if (index >= 0 && static_cast<size_t>(index) < model.textures.size()) {
            return static_cast<uint64_t>(index);
        }
    }

    return std::nullopt;
}

bool getDoubleSided(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive) {
    if (primitive.material == -1) {
        return false;
    }

    const auto& material = model.materials[static_cast<size_t>(primitive.material)];
    return material.doubleSided;
}

TexcoordsAccessor getImageryTexcoords(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    uint64_t setIndex,
    const glm::fvec2& translation,
    const glm::fvec2& scale) {
    return getTexcoords(model, primitive, "_CESIUMOVERLAY", setIndex, translation, scale, false);
}

const CesiumGltf::ImageCesium& getImageCesium(const CesiumGltf::Model& model, const CesiumGltf::Texture& texture) {
    const auto imageId = static_cast<uint64_t>(texture.source);
    const auto& image = model.images[imageId];
    return image.cesium;
}

bool hasNormals(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive, bool smoothNormals) {
    return smoothNormals || getNormalsView(model, primitive).status() == CesiumGltf::AccessorViewStatus::Valid;
}

bool hasTexcoords(const CesiumGltf::Model& model, const CesiumGltf::MeshPrimitive& primitive, uint64_t setIndex) {
    return getTexcoordsView(model, primitive, "TEXCOORD", setIndex).status() == CesiumGltf::AccessorViewStatus::Valid;
}

bool hasImageryTexcoords(
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    uint64_t setIndex) {
    return getTexcoordsView(model, primitive, "_CESIUMOVERLAY", setIndex).status() ==
           CesiumGltf::AccessorViewStatus::Valid;
}

bool hasMaterial(const CesiumGltf::MeshPrimitive& primitive) {
    return primitive.material >= 0;
}

} // namespace cesium::omniverse::GltfUtil
