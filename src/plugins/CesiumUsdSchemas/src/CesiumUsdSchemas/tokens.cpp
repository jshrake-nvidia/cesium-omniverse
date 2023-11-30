#include ".//tokens.h"

PXR_NAMESPACE_OPEN_SCOPE

CesiumTokensType::CesiumTokensType() :
    cesiumAlpha("cesium:alpha", TfToken::Immortal),
    cesiumAnchorAdjustOrientationForGlobeWhenMoving("cesium:anchor:adjustOrientationForGlobeWhenMoving", TfToken::Immortal),
    cesiumAnchorDetectTransformChanges("cesium:anchor:detectTransformChanges", TfToken::Immortal),
    cesiumAnchorGeographicCoordinates("cesium:anchor:geographicCoordinates", TfToken::Immortal),
    cesiumAnchorGeoreferenceBinding("cesium:anchor:georeferenceBinding", TfToken::Immortal),
    cesiumAnchorPosition("cesium:anchor:position", TfToken::Immortal),
    cesiumAnchorRotation("cesium:anchor:rotation", TfToken::Immortal),
    cesiumAnchorScale("cesium:anchor:scale", TfToken::Immortal),
    cesiumCulledScreenSpaceError("cesium:culledScreenSpaceError", TfToken::Immortal),
    cesiumDebugDisableGeometryPool("cesium:debug:disableGeometryPool", TfToken::Immortal),
    cesiumDebugDisableGeoreferencing("cesium:debug:disableGeoreferencing", TfToken::Immortal),
    cesiumDebugDisableMaterialPool("cesium:debug:disableMaterialPool", TfToken::Immortal),
    cesiumDebugDisableMaterials("cesium:debug:disableMaterials", TfToken::Immortal),
    cesiumDebugDisableTexturePool("cesium:debug:disableTexturePool", TfToken::Immortal),
    cesiumDebugDisableTextures("cesium:debug:disableTextures", TfToken::Immortal),
    cesiumDebugGeometryPoolInitialCapacity("cesium:debug:geometryPoolInitialCapacity", TfToken::Immortal),
    cesiumDebugMaterialPoolInitialCapacity("cesium:debug:materialPoolInitialCapacity", TfToken::Immortal),
    cesiumDebugRandomColors("cesium:debug:randomColors", TfToken::Immortal),
    cesiumDebugTexturePoolInitialCapacity("cesium:debug:texturePoolInitialCapacity", TfToken::Immortal),
    cesiumEcefToUsdTransform("cesium:ecefToUsdTransform", TfToken::Immortal),
    cesiumEnableFogCulling("cesium:enableFogCulling", TfToken::Immortal),
    cesiumEnableFrustumCulling("cesium:enableFrustumCulling", TfToken::Immortal),
    cesiumEnforceCulledScreenSpaceError("cesium:enforceCulledScreenSpaceError", TfToken::Immortal),
    cesiumForbidHoles("cesium:forbidHoles", TfToken::Immortal),
    cesiumGeoreferenceBinding("cesium:georeferenceBinding", TfToken::Immortal),
    cesiumGeoreferenceOriginHeight("cesium:georeferenceOrigin:height", TfToken::Immortal),
    cesiumGeoreferenceOriginLatitude("cesium:georeferenceOrigin:latitude", TfToken::Immortal),
    cesiumGeoreferenceOriginLongitude("cesium:georeferenceOrigin:longitude", TfToken::Immortal),
    cesiumIonAccessToken("cesium:ionAccessToken", TfToken::Immortal),
    cesiumIonAssetId("cesium:ionAssetId", TfToken::Immortal),
    cesiumIonServerApiUrl("cesium:ionServerApiUrl", TfToken::Immortal),
    cesiumIonServerApplicationId("cesium:ionServerApplicationId", TfToken::Immortal),
    cesiumIonServerBinding("cesium:ionServerBinding", TfToken::Immortal),
    cesiumIonServerUrl("cesium:ionServerUrl", TfToken::Immortal),
    cesiumLoadingDescendantLimit("cesium:loadingDescendantLimit", TfToken::Immortal),
    cesiumMainThreadLoadingTimeLimit("cesium:mainThreadLoadingTimeLimit", TfToken::Immortal),
    cesiumMaximumCachedBytes("cesium:maximumCachedBytes", TfToken::Immortal),
    cesiumMaximumScreenSpaceError("cesium:maximumScreenSpaceError", TfToken::Immortal),
    cesiumMaximumSimultaneousTileLoads("cesium:maximumSimultaneousTileLoads", TfToken::Immortal),
    cesiumPreloadAncestors("cesium:preloadAncestors", TfToken::Immortal),
    cesiumPreloadSiblings("cesium:preloadSiblings", TfToken::Immortal),
    cesiumProjectDefaultIonAccessToken("cesium:projectDefaultIonAccessToken", TfToken::Immortal),
    cesiumProjectDefaultIonAccessTokenId("cesium:projectDefaultIonAccessTokenId", TfToken::Immortal),
    cesiumSelectedIonServer("cesium:selectedIonServer", TfToken::Immortal),
    cesiumShowCreditsOnScreen("cesium:showCreditsOnScreen", TfToken::Immortal),
    cesiumSmoothNormals("cesium:smoothNormals", TfToken::Immortal),
    cesiumSourceType("cesium:sourceType", TfToken::Immortal),
    cesiumSuspendUpdate("cesium:suspendUpdate", TfToken::Immortal),
    cesiumUrl("cesium:url", TfToken::Immortal),
    ion("ion", TfToken::Immortal),
    url("url", TfToken::Immortal),
    allTokens({
        cesiumAlpha,
        cesiumAnchorAdjustOrientationForGlobeWhenMoving,
        cesiumAnchorDetectTransformChanges,
        cesiumAnchorGeographicCoordinates,
        cesiumAnchorGeoreferenceBinding,
        cesiumAnchorPosition,
        cesiumAnchorRotation,
        cesiumAnchorScale,
        cesiumCulledScreenSpaceError,
        cesiumDebugDisableGeometryPool,
        cesiumDebugDisableGeoreferencing,
        cesiumDebugDisableMaterialPool,
        cesiumDebugDisableMaterials,
        cesiumDebugDisableTexturePool,
        cesiumDebugDisableTextures,
        cesiumDebugGeometryPoolInitialCapacity,
        cesiumDebugMaterialPoolInitialCapacity,
        cesiumDebugRandomColors,
        cesiumDebugTexturePoolInitialCapacity,
        cesiumEcefToUsdTransform,
        cesiumEnableFogCulling,
        cesiumEnableFrustumCulling,
        cesiumEnforceCulledScreenSpaceError,
        cesiumForbidHoles,
        cesiumGeoreferenceBinding,
        cesiumGeoreferenceOriginHeight,
        cesiumGeoreferenceOriginLatitude,
        cesiumGeoreferenceOriginLongitude,
        cesiumIonAccessToken,
        cesiumIonAssetId,
        cesiumIonServerApiUrl,
        cesiumIonServerApplicationId,
        cesiumIonServerBinding,
        cesiumIonServerUrl,
        cesiumLoadingDescendantLimit,
        cesiumMainThreadLoadingTimeLimit,
        cesiumMaximumCachedBytes,
        cesiumMaximumScreenSpaceError,
        cesiumMaximumSimultaneousTileLoads,
        cesiumPreloadAncestors,
        cesiumPreloadSiblings,
        cesiumProjectDefaultIonAccessToken,
        cesiumProjectDefaultIonAccessTokenId,
        cesiumSelectedIonServer,
        cesiumShowCreditsOnScreen,
        cesiumSmoothNormals,
        cesiumSourceType,
        cesiumSuspendUpdate,
        cesiumUrl,
        ion,
        url
    })
{
}

TfStaticData<CesiumTokensType> CesiumTokens;

PXR_NAMESPACE_CLOSE_SCOPE
