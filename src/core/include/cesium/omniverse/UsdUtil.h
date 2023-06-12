#pragma once

#include <Cesium3DTilesSelection/ViewState.h>
#include <CesiumUsdSchemas/data.h>
#include <CesiumUsdSchemas/georeference.h>
#include <CesiumUsdSchemas/imagery.h>
#include <CesiumUsdSchemas/session.h>
#include <CesiumUsdSchemas/tilesetAPI.h>
#include <carb/flatcache/StageWithHistory.h>
#include <glm/glm.hpp>
#include <pxr/base/gf/matrix4d.h>
#include <pxr/base/gf/quatf.h>
#include <pxr/base/gf/vec3d.h>
#include <pxr/base/gf/vec3f.h>
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/common.h>

namespace CesiumGeospatial {
class Cartographic;
}

namespace cesium::omniverse {
struct Viewport;
}

namespace cesium::omniverse::UsdUtil {

struct Decomposed {
    pxr::GfVec3d position;
    pxr::GfQuatf orientation;
    pxr::GfVec3f scale;
};

class ScopedEdit {
  public:
    ScopedEdit(pxr::UsdStageRefPtr stage)
        : _stage(stage)
        , _sessionLayer(_stage->GetSessionLayer())
        , _sessionLayerWasEditable(_sessionLayer->PermissionToEdit())
        , _originalEditTarget(_stage->GetEditTarget()) {

        _sessionLayer->SetPermissionToEdit(true);
        _stage->SetEditTarget(pxr::UsdEditTarget(_sessionLayer));
    }

    ~ScopedEdit() {
        _sessionLayer->SetPermissionToEdit(_sessionLayerWasEditable);
        _stage->SetEditTarget(_originalEditTarget);
    }

  private:
    pxr::UsdStageRefPtr _stage;
    pxr::SdfLayerHandle _sessionLayer;
    bool _sessionLayerWasEditable;
    pxr::UsdEditTarget _originalEditTarget;
};

pxr::UsdStageRefPtr getUsdStage();
carb::flatcache::StageInProgress getFabricStageInProgress();
bool hasStage();
glm::dvec3 usdToGlmVector(const pxr::GfVec3d& vector);
glm::dmat4 usdToGlmMatrix(const pxr::GfMatrix4d& matrix);
pxr::GfVec3d glmToUsdVector(const glm::dvec3& vector);
pxr::GfMatrix4d glmToUsdMatrix(const glm::dmat4& matrix);
Decomposed glmToUsdMatrixDecomposed(const glm::dmat4& matrix);
glm::dmat4 computeUsdWorldTransform(const pxr::SdfPath& path);
bool isPrimVisible(const pxr::SdfPath& path);
pxr::TfToken getUsdUpAxis();
double getUsdMetersPerUnit();
pxr::SdfPath getRootPath();
pxr::SdfPath getPathUnique(const pxr::SdfPath& parentPath, const std::string& name);
std::string getSafeName(const std::string& name);
glm::dmat4 computeUsdToEcefTransform(const CesiumGeospatial::Cartographic& origin);
glm::dmat4 computeEcefToUsdTransform(const CesiumGeospatial::Cartographic& origin);
glm::dmat4 computeEcefToUsdTransformForPrim(const CesiumGeospatial::Cartographic& origin, const pxr::SdfPath& primPath);
glm::dmat4 computeUsdToEcefTransformForPrim(const CesiumGeospatial::Cartographic& origin, const pxr::SdfPath& primPath);
Cesium3DTilesSelection::ViewState
computeViewState(const CesiumGeospatial::Cartographic& origin, const pxr::SdfPath& primPath, const Viewport& viewport);
pxr::GfRange3d computeWorldExtent(const pxr::GfRange3d& localExtent, const glm::dmat4& localToUsdTransform);

pxr::CesiumData defineCesiumData(const pxr::SdfPath& path);
pxr::CesiumSession defineCesiumSession(const pxr::SdfPath& path);
pxr::CesiumGeoreference defineCesiumGeoreference(const pxr::SdfPath& path);
pxr::CesiumTilesetAPI defineCesiumTileset(const pxr::SdfPath& path);
pxr::CesiumImagery defineCesiumImagery(const pxr::SdfPath& path);

pxr::CesiumData getOrCreateCesiumData();
pxr::CesiumSession getOrCreateCesiumSession();
pxr::CesiumGeoreference getOrCreateCesiumGeoreference();
pxr::CesiumGeoreference getCesiumGeoreference(const pxr::SdfPath& path);
pxr::CesiumTilesetAPI getCesiumTileset(const pxr::SdfPath& path);
pxr::CesiumImagery getCesiumImagery(const pxr::SdfPath& path);
std::vector<pxr::CesiumImagery> getChildCesiumImageryPrims(const pxr::SdfPath& path);

bool isCesiumData(const pxr::SdfPath& path);
bool isCesiumSession(const pxr::SdfPath& path);
bool isCesiumGeoreference(const pxr::SdfPath& path);
bool isCesiumTileset(const pxr::SdfPath& path);
bool isCesiumImagery(const pxr::SdfPath& path);

bool primExists(const pxr::SdfPath& path);

[[maybe_unused]] void setGeoreferenceForTileset(const pxr::SdfPath& tilesetPath, const pxr::SdfPath& georeferencePath);

}; // namespace cesium::omniverse::UsdUtil
