#pragma once

#include "cesium/omniverse/Context.h"
#include "cesium/omniverse/DataType.h"
#include "cesium/omniverse/FabricPropertyInfo.h"
#include "cesium/omniverse/GltfUtil.h"
#include "cesium/omniverse/Logger.h"

#include <CesiumGltf/ExtensionExtMeshFeatures.h>
#include <CesiumGltf/ExtensionMeshPrimitiveExtStructuralMetadata.h>
#include <CesiumGltf/ExtensionModelExtStructuralMetadata.h>
#include <CesiumGltf/PropertyAttribute.h>
#include <CesiumGltf/PropertyAttributeView.h>
#include <CesiumGltf/PropertyTableView.h>
#include <CesiumGltf/PropertyTexture.h>
#include <CesiumGltf/PropertyTextureView.h>

namespace cesium::omniverse {
struct FabricPropertyDescriptor;
struct FabricTextureData;
} // namespace cesium::omniverse

namespace cesium::omniverse::MetadataUtil {

template <typename Callback, typename UnsupportedCallback>
void forEachPropertyAttributeProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    const auto pStructuralMetadataModel = model.getExtension<CesiumGltf::ExtensionModelExtStructuralMetadata>();
    if (!pStructuralMetadataModel) {
        return;
    }

    const auto pStructuralMetadataPrimitive =
        primitive.getExtension<CesiumGltf::ExtensionMeshPrimitiveExtStructuralMetadata>();
    if (!pStructuralMetadataPrimitive) {
        return;
    }

    for (const auto& propertyAttributeIndex : pStructuralMetadataPrimitive->propertyAttributes) {
        const auto pPropertyAttribute =
            model.getSafe(&pStructuralMetadataModel->propertyAttributes, static_cast<int32_t>(propertyAttributeIndex));
        if (!pPropertyAttribute) {
            context.getLogger()->warn("Property attribute index {} is out of range.", propertyAttributeIndex);
            continue;
        }

        const auto propertyAttributeView = CesiumGltf::PropertyAttributeView(model, *pPropertyAttribute);
        if (propertyAttributeView.status() != CesiumGltf::PropertyAttributeViewStatus::Valid) {
            context.getLogger()->warn(
                "Property attribute is invalid and will be ignored. Status code: {}",
                static_cast<int>(propertyAttributeView.status()));
            continue;
        }

        propertyAttributeView.forEachProperty(
            primitive,
            [&context,
             callback = std::forward<Callback>(callback),
             &unsupportedCallback,
             &propertyAttributeView,
             &pStructuralMetadataModel,
             &pPropertyAttribute](const std::string& propertyId, const auto& propertyAttributePropertyView) {
                if (propertyAttributePropertyView.status() != CesiumGltf::PropertyAttributePropertyViewStatus::Valid) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "Property \"{}\" is invalid and will be ignored. Status code: {}",
                            propertyId,
                            static_cast<int>(propertyAttributePropertyView.status())));
                    return;
                }

                const auto& schema = pStructuralMetadataModel->schema;
                if (!schema.has_value()) {
                    unsupportedCallback(
                        propertyId, fmt::format("No schema found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto pClassDefinition = propertyAttributeView.getClass();
                if (!pClassDefinition) {
                    unsupportedCallback(
                        propertyId, fmt::format("No class found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto pClassProperty = propertyAttributeView.getClassProperty(propertyId);
                if (!pClassProperty) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format("No class property found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto& propertyAttributeProperty = pPropertyAttribute->properties.at(propertyId);

                callback(
                    propertyId,
                    schema.value(),
                    *pClassDefinition,
                    *pClassProperty,
                    *pPropertyAttribute,
                    propertyAttributeProperty,
                    propertyAttributeView,
                    propertyAttributePropertyView);
            });
    }
}

template <typename Callback, typename UnsupportedCallback>
void forEachPropertyTextureProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    const auto pStructuralMetadataModel = model.getExtension<CesiumGltf::ExtensionModelExtStructuralMetadata>();
    if (!pStructuralMetadataModel) {
        return;
    }

    const auto pStructuralMetadataPrimitive =
        primitive.getExtension<CesiumGltf::ExtensionMeshPrimitiveExtStructuralMetadata>();
    if (!pStructuralMetadataPrimitive) {
        return;
    }

    for (const auto& propertyTextureIndex : pStructuralMetadataPrimitive->propertyTextures) {
        const auto pPropertyTexture =
            model.getSafe(&pStructuralMetadataModel->propertyTextures, static_cast<int32_t>(propertyTextureIndex));
        if (!pPropertyTexture) {
            context.getLogger()->warn(fmt::format("Property texture index {} is out of range.", propertyTextureIndex));
            continue;
        }

        const auto propertyTextureView = CesiumGltf::PropertyTextureView(model, *pPropertyTexture);
        if (propertyTextureView.status() != CesiumGltf::PropertyTextureViewStatus::Valid) {
            context.getLogger()->warn(
                "Property texture is invalid and will be ignored. Status code: {}",
                static_cast<int>(propertyTextureView.status()));
            continue;
        }

        propertyTextureView.forEachProperty(
            [&context,
             callback = std::forward<Callback>(callback),
             &unsupportedCallback,
             &propertyTextureView,
             &pStructuralMetadataModel,
             &pPropertyTexture](const std::string& propertyId, const auto& propertyTexturePropertyView) {
                if (propertyTexturePropertyView.status() != CesiumGltf::PropertyTexturePropertyViewStatus::Valid) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "Property \"{}\" is invalid and will be ignored. Status code: {}",
                            propertyId,
                            static_cast<int>(propertyTexturePropertyView.status())));
                    return;
                }

                const auto& schema = pStructuralMetadataModel->schema;
                if (!schema.has_value()) {
                    unsupportedCallback(
                        propertyId, fmt::format("No schema found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto pClassDefinition = propertyTextureView.getClass();
                if (!pClassDefinition) {
                    unsupportedCallback(
                        propertyId, fmt::format("No class found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto pClassProperty = propertyTextureView.getClassProperty(propertyId);
                if (!pClassProperty) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format("No class property found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                if (!propertyTexturePropertyView.getImage()) {
                    unsupportedCallback(
                        propertyId, fmt::format("No image found. Property \"{}\" will be ignored.", propertyId));
                    return;
                }

                const auto& propertyTextureProperty = pPropertyTexture->properties.at(propertyId);

                callback(
                    propertyId,
                    schema.value(),
                    *pClassDefinition,
                    *pClassProperty,
                    *pPropertyTexture,
                    propertyTextureProperty,
                    propertyTextureView,
                    propertyTexturePropertyView);
            });
    }
}

template <typename Callback, typename UnsupportedCallback>
void forEachPropertyTableProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    const auto pStructuralMetadataModel = model.getExtension<CesiumGltf::ExtensionModelExtStructuralMetadata>();
    if (!pStructuralMetadataModel) {
        return;
    }

    const auto pMeshFeatures = primitive.getExtension<CesiumGltf::ExtensionExtMeshFeatures>();
    if (!pMeshFeatures) {
        return;
    }

    for (uint64_t i = 0; i < pMeshFeatures->featureIds.size(); ++i) {
        const auto featureIdSetIndex = i;
        const auto& featureId = pMeshFeatures->featureIds[featureIdSetIndex];
        if (featureId.propertyTable.has_value()) {
            const auto pPropertyTable = model.getSafe(
                &pStructuralMetadataModel->propertyTables, static_cast<int32_t>(featureId.propertyTable.value()));
            if (!pPropertyTable) {
                context.getLogger()->warn(
                    fmt::format("Property table index {} is out of range.", featureId.propertyTable.value()));
                continue;
            }

            const auto propertyTableView = CesiumGltf::PropertyTableView(model, *pPropertyTable);
            if (propertyTableView.status() != CesiumGltf::PropertyTableViewStatus::Valid) {
                context.getLogger()->warn(
                    "Property table is invalid and will be ignored. Status code: {}",
                    static_cast<int>(propertyTableView.status()));
                continue;
            }

            propertyTableView.forEachProperty(
                [&context,
                 callback = std::forward<Callback>(callback),
                 &unsupportedCallback,
                 &propertyTableView,
                 &pStructuralMetadataModel,
                 &pPropertyTable,
                 featureIdSetIndex](const std::string& propertyId, const auto& propertyTablePropertyView) {
                    if (propertyTablePropertyView.status() != CesiumGltf::PropertyTablePropertyViewStatus::Valid) {
                        unsupportedCallback(
                            propertyId,
                            fmt::format(
                                "Property \"{}\" is invalid and will be ignored. Status code: {}",
                                propertyId,
                                static_cast<int>(propertyTablePropertyView.status())));
                        return;
                    }

                    const auto& schema = pStructuralMetadataModel->schema;
                    if (!schema.has_value()) {
                        unsupportedCallback(
                            propertyId, fmt::format("No schema found. Property \"{}\" will be ignored.", propertyId));
                        return;
                    }

                    const auto pClassDefinition = propertyTableView.getClass();
                    if (!pClassDefinition) {
                        unsupportedCallback(
                            propertyId, fmt::format("No class found. Property \"{}\" will be ignored.", propertyId));
                        return;
                    }

                    const auto pClassProperty = propertyTableView.getClassProperty(propertyId);
                    if (!pClassProperty) {
                        unsupportedCallback(
                            propertyId,
                            fmt::format("No class property found. Property \"{}\" will be ignored.", propertyId));
                        return;
                    }

                    const auto& propertyTableProperty = pPropertyTable->properties.at(propertyId);

                    callback(
                        propertyId,
                        schema.value(),
                        *pClassDefinition,
                        *pClassProperty,
                        *pPropertyTable,
                        propertyTableProperty,
                        propertyTableView,
                        propertyTablePropertyView,
                        featureIdSetIndex);
                });
        }
    }
}

template <typename Callback, typename UnsupportedCallback>
void forEachStyleablePropertyAttributeProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    forEachPropertyAttributeProperty(
        context,
        model,
        primitive,
        [&context, callback = std::forward<Callback>(callback), &unsupportedCallback](
            const std::string& propertyId,
            [[maybe_unused]] const CesiumGltf::Schema& schema,
            [[maybe_unused]] const CesiumGltf::Class& classDefinition,
            [[maybe_unused]] const CesiumGltf::ClassProperty& classProperty,
            [[maybe_unused]] const CesiumGltf::PropertyAttribute& propertyAttribute,
            const CesiumGltf::PropertyAttributeProperty& propertyAttributeProperty,
            [[maybe_unused]] const CesiumGltf::PropertyAttributeView& propertyAttributeView,
            const auto& propertyAttributePropertyView) {
            using RawType = decltype(propertyAttributePropertyView.getRaw(0));
            using TransformedType = typename std::decay_t<decltype(propertyAttributePropertyView.get(0))>::value_type;
            constexpr auto type = DataTypeUtil::getTypeReverse<RawType, TransformedType>();

            if constexpr (DataTypeUtil::isMatrix<type>()) {
                unsupportedCallback(
                    propertyId,
                    fmt::format(
                        "Matrix properties are not supported for styling. Property \"{}\" will be ignored.",
                        propertyId));
                return;
            } else {
                const auto& attribute = propertyAttributeProperty.attribute;

                // For some reason the static cast is needed in MSVC
                const auto propertyInfo = FabricPropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                    propertyAttributePropertyView.offset(),
                    propertyAttributePropertyView.scale(),
                    propertyAttributePropertyView.min(),
                    propertyAttributePropertyView.max(),
                    propertyAttributePropertyView.required(),
                    propertyAttributePropertyView.noData(),
                    propertyAttributePropertyView.defaultValue(),
                };

                const auto property =
                    FabricPropertyAttributePropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                        attribute,
                        propertyInfo,
                    };

                callback(propertyId, propertyAttributePropertyView, property);
            }
        },
        unsupportedCallback);
}

template <typename Callback, typename UnsupportedCallback>
void forEachStyleablePropertyTextureProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    forEachPropertyTextureProperty(
        context,
        model,
        primitive,
        [&context, callback = std::forward<Callback>(callback), &unsupportedCallback, &model](
            const std::string& propertyId,
            [[maybe_unused]] const CesiumGltf::Schema& schema,
            [[maybe_unused]] const CesiumGltf::Class& classDefinition,
            [[maybe_unused]] const CesiumGltf::ClassProperty& classProperty,
            [[maybe_unused]] const CesiumGltf::PropertyTexture& propertyTexture,
            const CesiumGltf::PropertyTextureProperty& propertyTextureProperty,
            [[maybe_unused]] const CesiumGltf::PropertyTextureView& propertyTextureView,
            const auto& propertyTexturePropertyView) {
            using RawType = decltype(propertyTexturePropertyView.getRaw(0.0, 0.0));
            using TransformedType =
                typename std::decay_t<decltype(propertyTexturePropertyView.get(0.0, 0.0))>::value_type;
            constexpr auto IsArray = HAS_MEMBER(RawType, size());

            if constexpr (IsArray) {
                unsupportedCallback(
                    propertyId,
                    fmt::format(
                        "Array properties are not supported for styling. Property \"{}\" will be ignored.",
                        propertyId));
                return;
            } else {
                constexpr auto type = DataTypeUtil::getTypeReverse<RawType, TransformedType>();

                if constexpr (DataTypeUtil::getComponentByteLength<type>() > 1) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "Only 8-bit per-component property texture properties are supported for styling. Property "
                            "\"{}\" will be ignored.",
                            propertyId));
                    return;
                } else {
                    const auto textureInfo = GltfUtil::getPropertyTexturePropertyInfo(model, propertyTextureProperty);

                    if (textureInfo.channels.size() != DataTypeUtil::getComponentCount<type>()) {
                        unsupportedCallback(
                            propertyId,
                            fmt::format(
                                "Properties with components that are packed across multiple texture channels are not "
                                "supported for styling. Property \"{}\" will be ignored.",
                                propertyId));
                        return;
                    }

                    if (textureInfo.channels.size() > 4) {
                        unsupportedCallback(
                            propertyId,
                            fmt::format(
                                "Properties with more than four channels are not supported for styling. Property "
                                "\"{}\" will be ignored.",
                                propertyId));
                        return;
                    }

                    const auto propertyInfo = FabricPropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                        propertyTexturePropertyView.offset(),
                        propertyTexturePropertyView.scale(),
                        propertyTexturePropertyView.min(),
                        propertyTexturePropertyView.max(),
                        propertyTexturePropertyView.required(),
                        propertyTexturePropertyView.noData(),
                        propertyTexturePropertyView.defaultValue(),
                    };

                    const auto property =
                        FabricPropertyTexturePropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                            textureInfo,
                            static_cast<uint64_t>(propertyTextureProperty.index),
                            propertyInfo,
                        };

                    callback(propertyId, propertyTexturePropertyView, property);
                }
            }
        },
        unsupportedCallback);
}

template <typename Callback, typename UnsupportedCallback>
void forEachStyleablePropertyTableProperty(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive,
    Callback&& callback,
    const UnsupportedCallback& unsupportedCallback) {

    forEachPropertyTableProperty(
        context,
        model,
        primitive,
        [&context, callback = std::forward<Callback>(callback), &unsupportedCallback](
            const std::string& propertyId,
            [[maybe_unused]] const CesiumGltf::Schema& schema,
            [[maybe_unused]] const CesiumGltf::Class& classDefinition,
            [[maybe_unused]] const CesiumGltf::ClassProperty& classProperty,
            [[maybe_unused]] const CesiumGltf::PropertyTable& propertyTable,
            [[maybe_unused]] const CesiumGltf::PropertyTableProperty& propertyTableProperty,
            [[maybe_unused]] const CesiumGltf::PropertyTableView& propertyTableView,
            const auto& propertyTablePropertyView,
            uint64_t featureIdSetIndex) {
            using RawType = decltype(propertyTablePropertyView.getRaw(0));
            using TransformedType = typename std::decay_t<decltype(propertyTablePropertyView.get(0))>::value_type;
            constexpr auto IsArray = HAS_MEMBER(RawType, size());
            constexpr auto IsBoolean = std::is_same_v<RawType, bool>;
            constexpr auto IsString = std::is_same_v<RawType, std::string_view>;

            if constexpr (IsArray) {
                unsupportedCallback(
                    propertyId,
                    fmt::format(
                        "Array properties are not supported for styling. Property \"{}\" will be ignored.",
                        propertyId));
                return;
            } else if constexpr (IsBoolean) {
                unsupportedCallback(
                    propertyId,
                    fmt::format(
                        "Boolean properties are not supported for styling. Property \"{}\" will be ignored.",
                        propertyId));
                return;
            } else if constexpr (IsString) {
                unsupportedCallback(
                    propertyId,
                    fmt::format(
                        "String properties are not supported for styling. Property \"{}\" will be ignored.",
                        propertyId));
                return;
            } else {
                constexpr auto type = DataTypeUtil::getTypeReverse<RawType, TransformedType>();
                constexpr auto unnormalizedComponentType = DataTypeUtil::getUnnormalizedComponentType<type>();

                if constexpr (DataTypeUtil::isMatrix<type>()) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "Matrix properties are not supported for styling. Property \"{}\" will be ignored.",
                            propertyId));
                    return;
                } else if constexpr (unnormalizedComponentType == DataType::UINT32) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "UINT32 properties are not supported for styling due to potential precision loss. Property "
                            "\"{}\" will be ignored.",
                            propertyId));
                    return;
                } else if constexpr (unnormalizedComponentType == DataType::UINT64) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "UINT64 properties are not supported for styling due to potential precision loss. Property "
                            "\"{}\" will be ignored.",
                            propertyId));
                    return;
                } else if constexpr (unnormalizedComponentType == DataType::INT64) {
                    unsupportedCallback(
                        propertyId,
                        fmt::format(
                            "INT64 properties are not supported for styling due to potential precision loss. Property "
                            "\"{}\" will be ignored.",
                            propertyId));
                    return;
                } else {
                    if constexpr (unnormalizedComponentType == DataType::FLOAT64) {
                        unsupportedCallback(
                            propertyId,
                            fmt::format(
                                "64-bit float properties are converted to 32-bit floats for styling. Some precision "
                                "loss "
                                "may occur for property \"{}\".",
                                propertyId));
                    }

                    const auto propertyInfo = FabricPropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                        propertyTablePropertyView.offset(),
                        propertyTablePropertyView.scale(),
                        propertyTablePropertyView.min(),
                        propertyTablePropertyView.max(),
                        propertyTablePropertyView.required(),
                        propertyTablePropertyView.noData(),
                        propertyTablePropertyView.defaultValue(),
                    };

                    const auto property =
                        FabricPropertyTablePropertyInfo<static_cast<cesium::omniverse::DataType>(type)>{
                            featureIdSetIndex,
                            propertyInfo,
                        };

                    callback(propertyId, propertyTablePropertyView, property);
                }
            }
        },
        unsupportedCallback);
}

std::tuple<std::vector<FabricPropertyDescriptor>, std::map<std::string, std::string>> getStyleableProperties(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive);

std::vector<const CesiumGltf::ImageCesium*> getPropertyTextureImages(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive);

std::unordered_map<uint64_t, uint64_t> getPropertyTextureIndexMapping(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive);

std::vector<FabricTextureData> encodePropertyTables(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive);

uint64_t getPropertyTableTextureCount(
    const Context& context,
    const CesiumGltf::Model& model,
    const CesiumGltf::MeshPrimitive& primitive);

} // namespace cesium::omniverse::MetadataUtil
