#include "cesium/omniverse/FabricMaterialDefinition.h"

#include "cesium/omniverse/GltfUtil.h"

#ifdef CESIUM_OMNI_MSVC
#pragma push_macro("OPAQUE")
#undef OPAQUE
#endif

#include <CesiumGltf/Model.h>

namespace cesium::omniverse {

namespace {

std::vector<FeatureIdType> getFeatureIdTypes(const FeaturesInfo& featuresInfo, bool disableTextures) {
    const auto& featureIds = featuresInfo.featureIds;

    std::vector<FeatureIdType> featureIdTypes;
    featureIdTypes.reserve(featureIds.size());

    for (const auto& featureId : featureIds) {
        if (std::holds_alternative<std::monostate>(featureId.featureIdStorage)) {
            featureIdTypes.push_back(FeatureIdType::INDEX);
        } else if (std::holds_alternative<uint64_t>(featureId.featureIdStorage)) {
            featureIdTypes.push_back(FeatureIdType::ATTRIBUTE);
        } else if (std::holds_alternative<TextureInfo>(featureId.featureIdStorage)) {
            if (!disableTextures) {
                featureIdTypes.push_back(FeatureIdType::TEXTURE);
            }
        }
    }

    return featureIdTypes;
}

} // namespace

FabricMaterialDefinition::FabricMaterialDefinition(
    const MaterialInfo& materialInfo,
    const FeaturesInfo& featuresInfo,
    uint64_t imageryLayerCount,
    bool disableTextures,
    const pxr::SdfPath& tilesetMaterialPath)
    : _hasVertexColors(materialInfo.hasVertexColors)
    , _hasBaseColorTexture(disableTextures ? false : materialInfo.baseColorTexture.has_value())
    , _featureIdTypes(::cesium::omniverse::getFeatureIdTypes(featuresInfo, disableTextures))
    , _imageryLayerCount(disableTextures ? 0 : imageryLayerCount)
    , _tilesetMaterialPath(tilesetMaterialPath) {}

bool FabricMaterialDefinition::hasVertexColors() const {
    return _hasVertexColors;
}

bool FabricMaterialDefinition::hasBaseColorTexture() const {
    return _hasBaseColorTexture;
}

const std::vector<FeatureIdType>& FabricMaterialDefinition::getFeatureIdTypes() const {
    return _featureIdTypes;
}

uint64_t FabricMaterialDefinition::getFeatureIdIndexCount() const {
    return static_cast<uint64_t>(std::count(_featureIdTypes.begin(), _featureIdTypes.end(), FeatureIdType::INDEX));
}

uint64_t FabricMaterialDefinition::getFeatureIdAttributeCount() const {
    return static_cast<uint64_t>(std::count(_featureIdTypes.begin(), _featureIdTypes.end(), FeatureIdType::ATTRIBUTE));
}

uint64_t FabricMaterialDefinition::getFeatureIdTextureCount() const {
    return static_cast<uint64_t>(std::count(_featureIdTypes.begin(), _featureIdTypes.end(), FeatureIdType::TEXTURE));
}

uint64_t FabricMaterialDefinition::getImageryLayerCount() const {
    return _imageryLayerCount;
}

bool FabricMaterialDefinition::hasTilesetMaterial() const {
    return !_tilesetMaterialPath.IsEmpty();
}

const pxr::SdfPath& FabricMaterialDefinition::getTilesetMaterialPath() const {
    return _tilesetMaterialPath;
}

// In C++ 20 we can use the default equality comparison (= default)
bool FabricMaterialDefinition::operator==(const FabricMaterialDefinition& other) const {
    return _hasVertexColors == other._hasVertexColors && _hasBaseColorTexture == other._hasBaseColorTexture &&
           _featureIdTypes == other._featureIdTypes && _imageryLayerCount == other._imageryLayerCount &&
           _tilesetMaterialPath == other._tilesetMaterialPath;
}

} // namespace cesium::omniverse
