// GENERATED FILE.  DO NOT EDIT.
#include <boost/python/class.hpp>
#include ".//tokens.h"

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

// Helper to return a static token as a string.  We wrap tokens as Python
// strings and for some reason simply wrapping the token using def_readonly
// bypasses to-Python conversion, leading to the error that there's no
// Python type for the C++ TfToken type.  So we wrap this functor instead.
class _WrapStaticToken {
public:
    _WrapStaticToken(const TfToken* token) : _token(token) { }

    std::string operator()() const
    {
        return _token->GetString();
    }

private:
    const TfToken* _token;
};

template <typename T>
void
_AddToken(T& cls, const char* name, const TfToken& token)
{
    cls.add_static_property(name,
                            boost::python::make_function(
                                _WrapStaticToken(&token),
                                boost::python::return_value_policy<
                                    boost::python::return_by_value>(),
                                boost::mpl::vector1<std::string>()));
}

} // anonymous

void wrapCesiumTokens()
{
    boost::python::class_<CesiumTokensType, boost::noncopyable>
        cls("Tokens", boost::python::no_init);
    _AddToken(cls, "cesiumAlpha", CesiumTokens->cesiumAlpha);
    _AddToken(cls, "cesiumAnchorAdjustOrientationForGlobeWhenMoving", CesiumTokens->cesiumAnchorAdjustOrientationForGlobeWhenMoving);
    _AddToken(cls, "cesiumAnchorDetectTransformChanges", CesiumTokens->cesiumAnchorDetectTransformChanges);
    _AddToken(cls, "cesiumAnchorGeoreferenceBinding", CesiumTokens->cesiumAnchorGeoreferenceBinding);
    _AddToken(cls, "cesiumAnchorHeight", CesiumTokens->cesiumAnchorHeight);
    _AddToken(cls, "cesiumAnchorLatitude", CesiumTokens->cesiumAnchorLatitude);
    _AddToken(cls, "cesiumAnchorLongitude", CesiumTokens->cesiumAnchorLongitude);
    _AddToken(cls, "cesiumAnchorPosition", CesiumTokens->cesiumAnchorPosition);
    _AddToken(cls, "cesiumBaseUrl", CesiumTokens->cesiumBaseUrl);
    _AddToken(cls, "cesiumCartographicPolygonBinding", CesiumTokens->cesiumCartographicPolygonBinding);
    _AddToken(cls, "cesiumCulledScreenSpaceError", CesiumTokens->cesiumCulledScreenSpaceError);
    _AddToken(cls, "cesiumDebugDisableGeometryPool", CesiumTokens->cesiumDebugDisableGeometryPool);
    _AddToken(cls, "cesiumDebugDisableGeoreferencing", CesiumTokens->cesiumDebugDisableGeoreferencing);
    _AddToken(cls, "cesiumDebugDisableMaterialPool", CesiumTokens->cesiumDebugDisableMaterialPool);
    _AddToken(cls, "cesiumDebugDisableMaterials", CesiumTokens->cesiumDebugDisableMaterials);
    _AddToken(cls, "cesiumDebugDisableTexturePool", CesiumTokens->cesiumDebugDisableTexturePool);
    _AddToken(cls, "cesiumDebugDisableTextures", CesiumTokens->cesiumDebugDisableTextures);
    _AddToken(cls, "cesiumDebugGeometryPoolInitialCapacity", CesiumTokens->cesiumDebugGeometryPoolInitialCapacity);
    _AddToken(cls, "cesiumDebugMaterialPoolInitialCapacity", CesiumTokens->cesiumDebugMaterialPoolInitialCapacity);
    _AddToken(cls, "cesiumDebugRandomColors", CesiumTokens->cesiumDebugRandomColors);
    _AddToken(cls, "cesiumDebugTexturePoolInitialCapacity", CesiumTokens->cesiumDebugTexturePoolInitialCapacity);
    _AddToken(cls, "cesiumDisplayName", CesiumTokens->cesiumDisplayName);
    _AddToken(cls, "cesiumEast", CesiumTokens->cesiumEast);
    _AddToken(cls, "cesiumEcefToUsdTransform", CesiumTokens->cesiumEcefToUsdTransform);
    _AddToken(cls, "cesiumEnableFogCulling", CesiumTokens->cesiumEnableFogCulling);
    _AddToken(cls, "cesiumEnableFrustumCulling", CesiumTokens->cesiumEnableFrustumCulling);
    _AddToken(cls, "cesiumEnforceCulledScreenSpaceError", CesiumTokens->cesiumEnforceCulledScreenSpaceError);
    _AddToken(cls, "cesiumExcludeSelectedTiles", CesiumTokens->cesiumExcludeSelectedTiles);
    _AddToken(cls, "cesiumForbidHoles", CesiumTokens->cesiumForbidHoles);
    _AddToken(cls, "cesiumFormat", CesiumTokens->cesiumFormat);
    _AddToken(cls, "cesiumGeoreferenceBinding", CesiumTokens->cesiumGeoreferenceBinding);
    _AddToken(cls, "cesiumGeoreferenceOriginHeight", CesiumTokens->cesiumGeoreferenceOriginHeight);
    _AddToken(cls, "cesiumGeoreferenceOriginLatitude", CesiumTokens->cesiumGeoreferenceOriginLatitude);
    _AddToken(cls, "cesiumGeoreferenceOriginLongitude", CesiumTokens->cesiumGeoreferenceOriginLongitude);
    _AddToken(cls, "cesiumInvertSelection", CesiumTokens->cesiumInvertSelection);
    _AddToken(cls, "cesiumIonAccessToken", CesiumTokens->cesiumIonAccessToken);
    _AddToken(cls, "cesiumIonAssetId", CesiumTokens->cesiumIonAssetId);
    _AddToken(cls, "cesiumIonServerApiUrl", CesiumTokens->cesiumIonServerApiUrl);
    _AddToken(cls, "cesiumIonServerApplicationId", CesiumTokens->cesiumIonServerApplicationId);
    _AddToken(cls, "cesiumIonServerBinding", CesiumTokens->cesiumIonServerBinding);
    _AddToken(cls, "cesiumIonServerUrl", CesiumTokens->cesiumIonServerUrl);
    _AddToken(cls, "cesiumLayer", CesiumTokens->cesiumLayer);
    _AddToken(cls, "cesiumLayers", CesiumTokens->cesiumLayers);
    _AddToken(cls, "cesiumLoadingDescendantLimit", CesiumTokens->cesiumLoadingDescendantLimit);
    _AddToken(cls, "cesiumMainThreadLoadingTimeLimit", CesiumTokens->cesiumMainThreadLoadingTimeLimit);
    _AddToken(cls, "cesiumMaximumCachedBytes", CesiumTokens->cesiumMaximumCachedBytes);
    _AddToken(cls, "cesiumMaximumLevel", CesiumTokens->cesiumMaximumLevel);
    _AddToken(cls, "cesiumMaximumScreenSpaceError", CesiumTokens->cesiumMaximumScreenSpaceError);
    _AddToken(cls, "cesiumMaximumSimultaneousTileLoads", CesiumTokens->cesiumMaximumSimultaneousTileLoads);
    _AddToken(cls, "cesiumMaximumTextureSize", CesiumTokens->cesiumMaximumTextureSize);
    _AddToken(cls, "cesiumMaximumZoomLevel", CesiumTokens->cesiumMaximumZoomLevel);
    _AddToken(cls, "cesiumMinimumLevel", CesiumTokens->cesiumMinimumLevel);
    _AddToken(cls, "cesiumMinimumZoomLevel", CesiumTokens->cesiumMinimumZoomLevel);
    _AddToken(cls, "cesiumNorth", CesiumTokens->cesiumNorth);
    _AddToken(cls, "cesiumOverlayRenderMethod", CesiumTokens->cesiumOverlayRenderMethod);
    _AddToken(cls, "cesiumPreloadAncestors", CesiumTokens->cesiumPreloadAncestors);
    _AddToken(cls, "cesiumPreloadSiblings", CesiumTokens->cesiumPreloadSiblings);
    _AddToken(cls, "cesiumProjectDefaultIonAccessToken", CesiumTokens->cesiumProjectDefaultIonAccessToken);
    _AddToken(cls, "cesiumProjectDefaultIonAccessTokenId", CesiumTokens->cesiumProjectDefaultIonAccessTokenId);
    _AddToken(cls, "cesiumRasterOverlayBinding", CesiumTokens->cesiumRasterOverlayBinding);
    _AddToken(cls, "cesiumRootTilesX", CesiumTokens->cesiumRootTilesX);
    _AddToken(cls, "cesiumRootTilesY", CesiumTokens->cesiumRootTilesY);
    _AddToken(cls, "cesiumSelectedIonServer", CesiumTokens->cesiumSelectedIonServer);
    _AddToken(cls, "cesiumShowCreditsOnScreen", CesiumTokens->cesiumShowCreditsOnScreen);
    _AddToken(cls, "cesiumSmoothNormals", CesiumTokens->cesiumSmoothNormals);
    _AddToken(cls, "cesiumSourceType", CesiumTokens->cesiumSourceType);
    _AddToken(cls, "cesiumSouth", CesiumTokens->cesiumSouth);
    _AddToken(cls, "cesiumSpecifyTileMatrixSetLabels", CesiumTokens->cesiumSpecifyTileMatrixSetLabels);
    _AddToken(cls, "cesiumSpecifyTilingScheme", CesiumTokens->cesiumSpecifyTilingScheme);
    _AddToken(cls, "cesiumSpecifyZoomLevels", CesiumTokens->cesiumSpecifyZoomLevels);
    _AddToken(cls, "cesiumStyle", CesiumTokens->cesiumStyle);
    _AddToken(cls, "cesiumSubTileCacheBytes", CesiumTokens->cesiumSubTileCacheBytes);
    _AddToken(cls, "cesiumSuspendUpdate", CesiumTokens->cesiumSuspendUpdate);
    _AddToken(cls, "cesiumTileHeight", CesiumTokens->cesiumTileHeight);
    _AddToken(cls, "cesiumTileMatrixSetId", CesiumTokens->cesiumTileMatrixSetId);
    _AddToken(cls, "cesiumTileMatrixSetLabelPrefix", CesiumTokens->cesiumTileMatrixSetLabelPrefix);
    _AddToken(cls, "cesiumTileMatrixSetLabels", CesiumTokens->cesiumTileMatrixSetLabels);
    _AddToken(cls, "cesiumTileWidth", CesiumTokens->cesiumTileWidth);
    _AddToken(cls, "cesiumUrl", CesiumTokens->cesiumUrl);
    _AddToken(cls, "cesiumUseWebMercatorProjection", CesiumTokens->cesiumUseWebMercatorProjection);
    _AddToken(cls, "cesiumWest", CesiumTokens->cesiumWest);
    _AddToken(cls, "clip", CesiumTokens->clip);
    _AddToken(cls, "ion", CesiumTokens->ion);
    _AddToken(cls, "overlay", CesiumTokens->overlay);
    _AddToken(cls, "url", CesiumTokens->url);
}
