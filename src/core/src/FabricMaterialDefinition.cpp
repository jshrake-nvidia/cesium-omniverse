#include "cesium/omniverse/FabricMaterialDefinition.h"

#include "cesium/omniverse/GltfUtil.h"

#ifdef CESIUM_OMNI_MSVC
#pragma push_macro("OPAQUE")
#undef OPAQUE
#endif

#include <CesiumGltf/Model.h>

namespace cesium::omniverse {

FabricMaterialDefinition::FabricMaterialDefinition(
    const MaterialInfo& materialInfo,
    bool hasImagery,
    bool disableTextures) {

    _hasBaseColorTexture = materialInfo.baseColorTexture.has_value();

    if (hasImagery) {
        _hasBaseColorTexture = true;
    }

    if (disableTextures) {
        _hasBaseColorTexture = false;
    }

    _hasVertexColors = materialInfo.hasVertexColors;
}

bool FabricMaterialDefinition::hasBaseColorTexture() const {
    return _hasBaseColorTexture;
}

bool FabricMaterialDefinition::hasVertexColors() const {
    return _hasVertexColors;
}

// In C++ 20 we can use the default equality comparison (= default)
bool FabricMaterialDefinition::operator==(const FabricMaterialDefinition& other) const {
    if (_hasBaseColorTexture != other._hasBaseColorTexture) {
        return false;
    }

    if (_hasVertexColors != other._hasVertexColors) {
        return false;
    }

    return true;
}

} // namespace cesium::omniverse
