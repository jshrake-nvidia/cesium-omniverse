#include ".//rasterOverlay.h"
#include "pxr/usd/usd/schemaRegistry.h"
#include "pxr/usd/usd/typed.h"

#include "pxr/usd/sdf/types.h"
#include "pxr/usd/sdf/assetPath.h"

PXR_NAMESPACE_OPEN_SCOPE

// Register the schema with the TfType system.
TF_REGISTRY_FUNCTION(TfType)
{
    TfType::Define<CesiumRasterOverlay,
        TfType::Bases< UsdTyped > >();
    
    // Register the usd prim typename as an alias under UsdSchemaBase. This
    // enables one to call
    // TfType::Find<UsdSchemaBase>().FindDerivedByName("CesiumRasterOverlayPrim")
    // to find TfType<CesiumRasterOverlay>, which is how IsA queries are
    // answered.
    TfType::AddAlias<UsdSchemaBase, CesiumRasterOverlay>("CesiumRasterOverlayPrim");
}

/* virtual */
CesiumRasterOverlay::~CesiumRasterOverlay()
{
}

/* static */
CesiumRasterOverlay
CesiumRasterOverlay::Get(const UsdStagePtr &stage, const SdfPath &path)
{
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return CesiumRasterOverlay();
    }
    return CesiumRasterOverlay(stage->GetPrimAtPath(path));
}

/* static */
CesiumRasterOverlay
CesiumRasterOverlay::Define(
    const UsdStagePtr &stage, const SdfPath &path)
{
    static TfToken usdPrimTypeName("CesiumRasterOverlayPrim");
    if (!stage) {
        TF_CODING_ERROR("Invalid stage");
        return CesiumRasterOverlay();
    }
    return CesiumRasterOverlay(
        stage->DefinePrim(path, usdPrimTypeName));
}

/* virtual */
UsdSchemaType CesiumRasterOverlay::_GetSchemaType() const {
    return CesiumRasterOverlay::schemaType;
}

/* static */
const TfType &
CesiumRasterOverlay::_GetStaticTfType()
{
    static TfType tfType = TfType::Find<CesiumRasterOverlay>();
    return tfType;
}

/* static */
bool 
CesiumRasterOverlay::_IsTypedSchema()
{
    static bool isTyped = _GetStaticTfType().IsA<UsdTyped>();
    return isTyped;
}

/* virtual */
const TfType &
CesiumRasterOverlay::_GetTfType() const
{
    return _GetStaticTfType();
}

UsdAttribute
CesiumRasterOverlay::GetIonAssetIdAttr() const
{
    return GetPrim().GetAttribute(CesiumTokens->cesiumIonAssetId);
}

UsdAttribute
CesiumRasterOverlay::CreateIonAssetIdAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(CesiumTokens->cesiumIonAssetId,
                       SdfValueTypeNames->Int64,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

UsdAttribute
CesiumRasterOverlay::GetIonAccessTokenAttr() const
{
    return GetPrim().GetAttribute(CesiumTokens->cesiumIonAccessToken);
}

UsdAttribute
CesiumRasterOverlay::CreateIonAccessTokenAttr(VtValue const &defaultValue, bool writeSparsely) const
{
    return UsdSchemaBase::_CreateAttr(CesiumTokens->cesiumIonAccessToken,
                       SdfValueTypeNames->String,
                       /* custom = */ false,
                       SdfVariabilityVarying,
                       defaultValue,
                       writeSparsely);
}

namespace {
static inline TfTokenVector
_ConcatenateAttributeNames(const TfTokenVector& left,const TfTokenVector& right)
{
    TfTokenVector result;
    result.reserve(left.size() + right.size());
    result.insert(result.end(), left.begin(), left.end());
    result.insert(result.end(), right.begin(), right.end());
    return result;
}
}

/*static*/
const TfTokenVector&
CesiumRasterOverlay::GetSchemaAttributeNames(bool includeInherited)
{
    static TfTokenVector localNames = {
        CesiumTokens->cesiumIonAssetId,
        CesiumTokens->cesiumIonAccessToken,
    };
    static TfTokenVector allNames =
        _ConcatenateAttributeNames(
            UsdTyped::GetSchemaAttributeNames(true),
            localNames);

    if (includeInherited)
        return allNames;
    else
        return localNames;
}

PXR_NAMESPACE_CLOSE_SCOPE

// ===================================================================== //
// Feel free to add custom code below this line. It will be preserved by
// the code generator.
//
// Just remember to wrap code in the appropriate delimiters:
// 'PXR_NAMESPACE_OPEN_SCOPE', 'PXR_NAMESPACE_CLOSE_SCOPE'.
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--
