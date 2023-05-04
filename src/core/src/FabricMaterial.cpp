#include "cesium/omniverse/FabricMaterial.h"

#include "cesium/omniverse/FabricAsset.h"
#include "cesium/omniverse/FabricAttributesBuilder.h"
#include "cesium/omniverse/FabricMaterialDefinition.h"
#include "cesium/omniverse/FabricUtil.h"
#include "cesium/omniverse/GltfUtil.h"
#include "cesium/omniverse/Tokens.h"
#include "cesium/omniverse/UsdUtil.h"

#ifdef CESIUM_OMNI_MSVC
#pragma push_macro("OPAQUE")
#undef OPAQUE
#endif

#include <CesiumGltf/Model.h>
#include <carb/flatcache/FlatCacheUSD.h>
#include <omni/ui/ImageProvider/DynamicTextureProvider.h>
#include <spdlog/fmt/fmt.h>

#include <array>

namespace cesium::omniverse {

FabricMaterial::FabricMaterial(pxr::SdfPath path, const FabricMaterialDefinition& materialDefinition)
    : _materialDefinition(materialDefinition)
    , _materialPath(path) {

    initialize(path, materialDefinition);

    // Remove this function once dynamic material values are supported in Kit 105
    setInitialValues(materialDefinition);
}

FabricMaterial::~FabricMaterial() {
    FabricUtil::destroyPrim(_materialPath);
    FabricUtil::destroyPrim(_shaderPath);
    FabricUtil::destroyPrim(_displacementPath);
    FabricUtil::destroyPrim(_surfacePath);
}

void FabricMaterial::setActive(bool active) {
    if (!active) {
        reset();
    }
}

pxr::SdfPath FabricMaterial::getPath() const {
    return _materialPath;
}

const FabricMaterialDefinition& FabricMaterial::getMaterialDefinition() const {
    return _materialDefinition;
}

void FabricMaterial::initialize(pxr::SdfPath path, const FabricMaterialDefinition& materialDefinition) {
    const auto hasBaseColorTexture = materialDefinition.hasBaseColorTexture();

    auto sip = UsdUtil::getFabricStageInProgress();

    const auto materialPath = path;
    const auto shaderPath = materialPath.AppendChild(UsdTokens::Shader);
    const auto displacementPath = materialPath.AppendChild(UsdTokens::displacement);
    const auto surfacePath = materialPath.AppendChild(UsdTokens::surface);
    const auto lookupColorPath = materialPath.AppendChild(UsdTokens::lookup_color);
    const auto textureCoordinate2dPath = materialPath.AppendChild(UsdTokens::texture_coordinate_2d);

    const auto shaderPathFabricUint64 = carb::flatcache::asInt(shaderPath).path;
    const auto lookupColorPathFabricUint64 = carb::flatcache::asInt(lookupColorPath).path;
    const auto textureCoordinate2dPathFabricUint64 = carb::flatcache::asInt(textureCoordinate2dPath).path;

    // Material
    {
        const auto materialPathFabric = carb::flatcache::Path(carb::flatcache::asInt(materialPath));
        sip.createPrim(materialPathFabric);

        FabricAttributesBuilder attributes;

        attributes.addAttribute(FabricTypes::_terminals, FabricTokens::_terminals);
        attributes.addAttribute(FabricTypes::Material, FabricTokens::Material);
        attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
        attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);

        attributes.createAttributes(materialPathFabric);

        sip.setArrayAttributeSize(materialPathFabric, FabricTokens::_terminals, 2);

        auto terminalsFabric = sip.getArrayAttributeWr<uint64_t>(materialPathFabric, FabricTokens::_terminals);

        terminalsFabric[0] = shaderPathFabricUint64;
        terminalsFabric[1] = shaderPathFabricUint64;
    }

    // Displacement
    {
        const auto displacementPathFabric = carb::flatcache::Path(carb::flatcache::asInt(displacementPath));
        sip.createPrim(displacementPathFabric);

        FabricAttributesBuilder attributes;

        attributes.addAttribute(FabricTypes::_nodePaths, FabricTokens::_nodePaths);
        attributes.addAttribute(FabricTypes::_relationships_inputId, FabricTokens::_relationships_inputId);
        attributes.addAttribute(FabricTypes::_relationships_outputId, FabricTokens::_relationships_outputId);
        attributes.addAttribute(FabricTypes::_relationships_inputName, FabricTokens::_relationships_inputName);
        attributes.addAttribute(FabricTypes::_relationships_outputName, FabricTokens::_relationships_outputName);
        attributes.addAttribute(FabricTypes::primvars, FabricTokens::primvars);
        attributes.addAttribute(FabricTypes::MaterialNetwork, FabricTokens::MaterialNetwork);
        attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
        attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);

        attributes.createAttributes(displacementPathFabric);

        auto nodePathsCount = 1;
        auto inputsCount = 0;
        auto outputsCount = 0;

        if (hasBaseColorTexture) {
            nodePathsCount = 3;
            inputsCount = 2;
            outputsCount = 2;
        }

        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::_nodePaths, nodePathsCount);
        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::_relationships_inputId, inputsCount);
        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::_relationships_outputId, outputsCount);
        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::_relationships_inputName, inputsCount);
        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::_relationships_outputName, outputsCount);
        sip.setArrayAttributeSize(displacementPathFabric, FabricTokens::primvars, 0);

        // clang-format off
        auto nodePathsFabric = sip.getArrayAttributeWr<uint64_t>(displacementPathFabric, FabricTokens::_nodePaths);
        auto relationshipsInputIdFabric = sip.getArrayAttributeWr<uint64_t>(displacementPathFabric, FabricTokens::_relationships_inputId);
        auto relationshipsOutputIdFabric = sip.getArrayAttributeWr<uint64_t>(displacementPathFabric, FabricTokens::_relationships_outputId);
        auto relationshipsInputNameFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(displacementPathFabric, FabricTokens::_relationships_inputName);
        auto relationshipsOutputNameFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(displacementPathFabric, FabricTokens::_relationships_outputName);
        // clang-format on

        if (hasBaseColorTexture) {
            nodePathsFabric[0] = textureCoordinate2dPathFabricUint64;
            nodePathsFabric[1] = lookupColorPathFabricUint64;
            nodePathsFabric[2] = shaderPathFabricUint64;
            relationshipsInputIdFabric[0] = textureCoordinate2dPathFabricUint64;
            relationshipsInputIdFabric[1] = lookupColorPathFabricUint64;
            relationshipsOutputIdFabric[0] = lookupColorPathFabricUint64;
            relationshipsOutputIdFabric[1] = shaderPathFabricUint64;
            relationshipsInputNameFabric[0] = FabricTokens::out;
            relationshipsInputNameFabric[1] = FabricTokens::out;
            relationshipsOutputNameFabric[0] = FabricTokens::coord;
            relationshipsOutputNameFabric[1] = FabricTokens::diffuse_color_constant;
        } else {
            nodePathsFabric[0] = shaderPathFabricUint64;
        }
    }

    // Surface
    {
        const auto surfacePathFabric = carb::flatcache::Path(carb::flatcache::asInt(surfacePath));
        sip.createPrim(surfacePathFabric);

        FabricAttributesBuilder attributes;

        attributes.addAttribute(FabricTypes::_nodePaths, FabricTokens::_nodePaths);
        attributes.addAttribute(FabricTypes::_relationships_inputId, FabricTokens::_relationships_inputId);
        attributes.addAttribute(FabricTypes::_relationships_outputId, FabricTokens::_relationships_outputId);
        attributes.addAttribute(FabricTypes::_relationships_inputName, FabricTokens::_relationships_inputName);
        attributes.addAttribute(FabricTypes::_relationships_outputName, FabricTokens::_relationships_outputName);
        attributes.addAttribute(FabricTypes::primvars, FabricTokens::primvars);
        attributes.addAttribute(FabricTypes::MaterialNetwork, FabricTokens::MaterialNetwork);
        attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
        attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);

        attributes.createAttributes(surfacePathFabric);

        auto nodePathsCount = 1;
        auto inputsCount = 0;
        auto outputsCount = 0;

        if (hasBaseColorTexture) {
            nodePathsCount = 3;
            inputsCount = 2;
            outputsCount = 2;
        }

        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::_nodePaths, nodePathsCount);
        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::_relationships_inputId, inputsCount);
        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::_relationships_outputId, outputsCount);
        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::_relationships_inputName, inputsCount);
        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::_relationships_outputName, outputsCount);
        sip.setArrayAttributeSize(surfacePathFabric, FabricTokens::primvars, 0);

        // clang-format off
        auto nodePathsFabric = sip.getArrayAttributeWr<uint64_t>(surfacePathFabric, FabricTokens::_nodePaths);
        auto relationshipsInputIdFabric = sip.getArrayAttributeWr<uint64_t>(surfacePathFabric, FabricTokens::_relationships_inputId);
        auto relationshipsOutputIdFabric = sip.getArrayAttributeWr<uint64_t>(surfacePathFabric, FabricTokens::_relationships_outputId);
        auto relationshipsInputNameFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(surfacePathFabric, FabricTokens::_relationships_inputName);
        auto relationshipsOutputNameFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(surfacePathFabric, FabricTokens::_relationships_outputName);
        // clang-format on

        if (hasBaseColorTexture) {
            nodePathsFabric[0] = textureCoordinate2dPathFabricUint64;
            nodePathsFabric[1] = lookupColorPathFabricUint64;
            nodePathsFabric[2] = shaderPathFabricUint64;
            relationshipsInputIdFabric[0] = textureCoordinate2dPathFabricUint64;
            relationshipsInputIdFabric[1] = lookupColorPathFabricUint64;
            relationshipsOutputIdFabric[0] = lookupColorPathFabricUint64;
            relationshipsOutputIdFabric[1] = shaderPathFabricUint64;
            relationshipsInputNameFabric[0] = FabricTokens::out;
            relationshipsInputNameFabric[1] = FabricTokens::out;
            relationshipsOutputNameFabric[0] = FabricTokens::coord;
            relationshipsOutputNameFabric[1] = FabricTokens::diffuse_color_constant;
        } else {
            nodePathsFabric[0] = shaderPathFabricUint64;
        }
    }

    // Shader
    {
        const auto shaderPathFabric = carb::flatcache::Path(carb::flatcache::asInt(shaderPath));
        sip.createPrim(shaderPathFabric);

        FabricAttributesBuilder attributes;

        // clang-format off
        attributes.addAttribute(FabricTypes::info_id, FabricTokens::info_id);
        attributes.addAttribute(FabricTypes::info_sourceAsset_subIdentifier, FabricTokens::info_sourceAsset_subIdentifier);
        attributes.addAttribute(FabricTypes::_paramColorSpace, FabricTokens::_paramColorSpace);
        attributes.addAttribute(FabricTypes::_parameters, FabricTokens::_parameters);
        attributes.addAttribute(FabricTypes::diffuse_color_constant, FabricTokens::diffuse_color_constant);
        attributes.addAttribute(FabricTypes::metallic_constant, FabricTokens::metallic_constant);
        attributes.addAttribute(FabricTypes::reflection_roughness_constant, FabricTokens::reflection_roughness_constant);
        attributes.addAttribute(FabricTypes::specular_level, FabricTokens::specular_level);
        attributes.addAttribute(FabricTypes::Shader, FabricTokens::Shader);
        attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
        attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);
        // clang-format on

        attributes.createAttributes(shaderPathFabric);

        sip.setArrayAttributeSize(shaderPathFabric, FabricTokens::_paramColorSpace, 0);
        sip.setArrayAttributeSize(shaderPathFabric, FabricTokens::_parameters, 4);

        // clang-format off
        auto infoIdFabric = sip.getAttributeWr<carb::flatcache::Token>(shaderPathFabric, FabricTokens::info_id);
        auto infoSourceAssetSubIdentifierFabric = sip.getAttributeWr<carb::flatcache::Token>(shaderPathFabric, FabricTokens::info_sourceAsset_subIdentifier);
        auto parametersFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(shaderPathFabric, FabricTokens::_parameters);
        auto specularLevelFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::specular_level);
        // clang-format on

        *infoIdFabric = FabricTokens::OmniPBR_mdl;
        *infoSourceAssetSubIdentifierFabric = FabricTokens::OmniPBR;
        parametersFabric[0] = FabricTokens::diffuse_color_constant;
        parametersFabric[1] = FabricTokens::metallic_constant;
        parametersFabric[2] = FabricTokens::reflection_roughness_constant;
        parametersFabric[3] = FabricTokens::specular_level;
        *specularLevelFabric = 0.0f;
    }

    if (hasBaseColorTexture) {
        // Create the base color texture
        const auto baseColorTextureName =
            fmt::format("{}_base_color_texture", UsdUtil::getSafeName(materialPath.GetString()));
        const auto baseColorTexturePath =
            pxr::SdfAssetPath(fmt::format("{}{}", rtx::resourcemanager::kDynamicTexturePrefix, baseColorTextureName));
        _baseColorTexture = std::make_unique<omni::ui::DynamicTextureProvider>(baseColorTextureName);

        // texture_coordinate_2d
        {
            const auto textureCoordinate2dPathFabric =
                carb::flatcache::Path(carb::flatcache::asInt(textureCoordinate2dPath));
            sip.createPrim(textureCoordinate2dPathFabric);

            FabricAttributesBuilder attributes;

            // clang-format off
            attributes.addAttribute(FabricTypes::info_id, FabricTokens::info_id);
            attributes.addAttribute(FabricTypes::info_sourceAsset_subIdentifier, FabricTokens::info_sourceAsset_subIdentifier);
            attributes.addAttribute(FabricTypes::_paramColorSpace, FabricTokens::_paramColorSpace);
            attributes.addAttribute(FabricTypes::_parameters, FabricTokens::_parameters);
            attributes.addAttribute(FabricTypes::Shader, FabricTokens::Shader);
            attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
            attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);
            // clang-format on

            attributes.createAttributes(textureCoordinate2dPathFabric);

            sip.setArrayAttributeSize(textureCoordinate2dPathFabric, FabricTokens::_paramColorSpace, 0);
            sip.setArrayAttributeSize(textureCoordinate2dPathFabric, FabricTokens::_parameters, 0);

            // clang-format off
            auto infoIdFabric = sip.getAttributeWr<carb::flatcache::Token>(textureCoordinate2dPathFabric, FabricTokens::info_id);
            auto infoSourceAssetSubIdentifierFabric = sip.getAttributeWr<carb::flatcache::Token>(textureCoordinate2dPathFabric, FabricTokens::info_sourceAsset_subIdentifier);
            // clang-format on

            *infoIdFabric = FabricTokens::nvidia_support_definitions_mdl;
            *infoSourceAssetSubIdentifierFabric = FabricTokens::texture_coordinate_2d;
        }

        // lookup_color
        {
            const auto lookupColorPathFabric = carb::flatcache::Path(carb::flatcache::asInt(lookupColorPath));
            sip.createPrim(lookupColorPathFabric);

            FabricAttributesBuilder attributes;

            // clang-format off
            attributes.addAttribute(FabricTypes::wrap_u, FabricTokens::wrap_u);
            attributes.addAttribute(FabricTypes::wrap_v, FabricTokens::wrap_v);
            attributes.addAttribute(FabricTypes::tex, FabricTokens::tex);
            attributes.addAttribute(FabricTypes::info_id, FabricTokens::info_id);
            attributes.addAttribute(FabricTypes::info_sourceAsset_subIdentifier, FabricTokens::info_sourceAsset_subIdentifier);
            attributes.addAttribute(FabricTypes::_paramColorSpace, FabricTokens::_paramColorSpace);
            attributes.addAttribute(FabricTypes::_parameters, FabricTokens::_parameters);
            attributes.addAttribute(FabricTypes::Shader, FabricTokens::Shader);
            attributes.addAttribute(FabricTypes::_cesium_tilesetId, FabricTokens::_cesium_tilesetId);
            attributes.addAttribute(FabricTypes::_cesium_tileId, FabricTokens::_cesium_tileId);
            // clang-format on

            attributes.createAttributes(lookupColorPathFabric);

            // _paramColorSpace is an array of pairs: [texture_parameter_token, color_space_enum], [texture_parameter_token, color_space_enum], ...
            sip.setArrayAttributeSize(lookupColorPathFabric, FabricTokens::_paramColorSpace, 2);
            sip.setArrayAttributeSize(lookupColorPathFabric, FabricTokens::_parameters, 3);

            // clang-format off
            auto wrapUFabric = sip.getAttributeWr<int>(lookupColorPathFabric, FabricTokens::wrap_u);
            auto wrapVFabric = sip.getAttributeWr<int>(lookupColorPathFabric, FabricTokens::wrap_v);
            auto texFabric = sip.getAttributeWr<FabricAsset>(lookupColorPathFabric, FabricTokens::tex);
            auto infoIdFabric = sip.getAttributeWr<carb::flatcache::Token>(lookupColorPathFabric, FabricTokens::info_id);
            auto infoSourceAssetSubIdentifierFabric = sip.getAttributeWr<carb::flatcache::Token>(lookupColorPathFabric, FabricTokens::info_sourceAsset_subIdentifier);
            auto paramColorSpaceFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(lookupColorPathFabric, FabricTokens::_paramColorSpace);
            auto parametersFabric = sip.getArrayAttributeWr<carb::flatcache::Token>(lookupColorPathFabric, FabricTokens::_parameters);
            // clang-format on

            *wrapUFabric = 0; // clamp to edge
            *wrapVFabric = 0; // clamp to edge
            *texFabric = FabricAsset(baseColorTexturePath);
            *infoIdFabric = FabricTokens::nvidia_support_definitions_mdl;
            *infoSourceAssetSubIdentifierFabric = FabricTokens::lookup_color;
            paramColorSpaceFabric[0] = FabricTokens::tex;
            paramColorSpaceFabric[1] = FabricTokens::_auto;
            parametersFabric[0] = FabricTokens::tex;
            parametersFabric[1] = FabricTokens::wrap_u;
            parametersFabric[2] = FabricTokens::wrap_v;
        }
    }

    _materialPath = materialPath;
    _shaderPath = shaderPath;
    _displacementPath = displacementPath;
    _surfacePath = surfacePath;

    reset();
}

void FabricMaterial::reset() {
    if (!UsdUtil::hasStage()) {
        return;
    }

    auto sip = UsdUtil::getFabricStageInProgress();

    if (_baseColorTexture != nullptr) {
        // Clear the texture
        const auto bytes = std::array<uint8_t, 4>{{255, 255, 255, 255}};
        const auto size = carb::Uint2{1, 1};
        _baseColorTexture->setBytesData(bytes.data(), size, omni::ui::kAutoCalculateStride, carb::Format::eRGBA8_SRGB);
    }

    const auto baseColorFactor = GltfUtil::getDefaultBaseColorFactor();
    const auto metallicFactor = GltfUtil::getDefaultMetallicFactor();
    const auto roughnessFactor = GltfUtil::getDefaultRoughnessFactor();

    const auto shaderPathFabric = carb::flatcache::Path(carb::flatcache::asInt(_shaderPath));

    // clang-format off
    auto diffuseColorConstantFabric = sip.getAttributeWr<pxr::GfVec3f>(shaderPathFabric, FabricTokens::diffuse_color_constant);
    auto metallicConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::metallic_constant);
    auto reflectionRoughnessConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::reflection_roughness_constant);
    // clang-format on

    *diffuseColorConstantFabric = baseColorFactor;
    *metallicConstantFabric = metallicFactor;
    *reflectionRoughnessConstantFabric = roughnessFactor;

    FabricUtil::setTilesetIdAndTileId(_materialPath, -1, -1);
    FabricUtil::setTilesetIdAndTileId(_shaderPath, -1, -1);
    FabricUtil::setTilesetIdAndTileId(_displacementPath, -1, -1);
    FabricUtil::setTilesetIdAndTileId(_surfacePath, -1, -1);
}

void FabricMaterial::setInitialValues(const FabricMaterialDefinition& materialDefinition) {
    auto sip = UsdUtil::getFabricStageInProgress();

    const auto shaderPathFabric = carb::flatcache::Path(carb::flatcache::asInt(_shaderPath));

    // clang-format off
    auto diffuseColorConstantFabric = sip.getAttributeWr<pxr::GfVec3f>(shaderPathFabric, FabricTokens::diffuse_color_constant);
    auto metallicConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::metallic_constant);
    auto reflectionRoughnessConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::reflection_roughness_constant);
    // clang-format on

    *diffuseColorConstantFabric = materialDefinition.getBaseColorFactor();
    *metallicConstantFabric = materialDefinition.getMetallicFactor();
    *reflectionRoughnessConstantFabric = materialDefinition.getRoughnessFactor();
}

void FabricMaterial::setTile(
    int64_t tilesetId,
    int64_t tileId,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    const CesiumGltf::ImageCesium* imagery) {

    auto sip = UsdUtil::getFabricStageInProgress();

    pxr::GfVec3f baseColorFactor;
    float metallicFactor;
    float roughnessFactor;

    const auto hasGltfMaterial = GltfUtil::hasMaterial(primitive);

    const CesiumGltf::ImageCesium* baseColorImage = nullptr;

    if (hasGltfMaterial) {
        const auto& material = model.materials[static_cast<size_t>(primitive.material)];

        baseColorFactor = GltfUtil::getBaseColorFactor(material);
        metallicFactor = GltfUtil::getMetallicFactor(material);
        roughnessFactor = GltfUtil::getRoughnessFactor(material);

        const auto baseColorTextureIndex = GltfUtil::getBaseColorTextureIndex(model, material);

        if (baseColorTextureIndex.has_value()) {
            baseColorImage = &GltfUtil::getImageCesium(model, model.textures[baseColorTextureIndex.value()]);
        }
    } else {
        baseColorFactor = GltfUtil::getDefaultBaseColorFactor();
        metallicFactor = GltfUtil::getDefaultMetallicFactor();
        roughnessFactor = GltfUtil::getDefaultRoughnessFactor();
    }

    // Imagery overrides the base color texture in the glTF
    if (imagery != nullptr) {
        baseColorImage = imagery;
    }

    if (baseColorImage != nullptr) {
        _baseColorTexture->setBytesData(
            reinterpret_cast<const uint8_t*>(baseColorImage->pixelData.data()),
            carb::Uint2{static_cast<uint32_t>(baseColorImage->width), static_cast<uint32_t>(baseColorImage->height)},
            omni::ui::kAutoCalculateStride,
            carb::Format::eRGBA8_SRGB);
    }

    const auto shaderPathFabric = carb::flatcache::Path(carb::flatcache::asInt(_shaderPath));

    // clang-format off
    auto diffuseColorConstantFabric = sip.getAttributeWr<pxr::GfVec3f>(shaderPathFabric, FabricTokens::diffuse_color_constant);
    auto metallicConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::metallic_constant);
    auto reflectionRoughnessConstantFabric = sip.getAttributeWr<float>(shaderPathFabric, FabricTokens::reflection_roughness_constant);
    // clang-format on

    *diffuseColorConstantFabric = baseColorFactor;
    *metallicConstantFabric = metallicFactor;
    *reflectionRoughnessConstantFabric = roughnessFactor;

    FabricUtil::setTilesetIdAndTileId(_materialPath, tilesetId, tileId);
    FabricUtil::setTilesetIdAndTileId(_shaderPath, tilesetId, tileId);
    FabricUtil::setTilesetIdAndTileId(_displacementPath, tilesetId, tileId);
    FabricUtil::setTilesetIdAndTileId(_surfacePath, tilesetId, tileId);
}
}; // namespace cesium::omniverse
