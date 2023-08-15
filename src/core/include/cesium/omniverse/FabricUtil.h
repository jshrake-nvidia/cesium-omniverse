#pragma once

#include "cesium/omniverse/RenderStatistics.h"

#include <glm/glm.hpp>
#include <omni/fabric/IPath.h>
#include <pxr/usd/sdf/path.h>

#include <string>

namespace cesium::omniverse {

struct FabricStatistics {
    uint64_t materialsCapacity{0};
    uint64_t materialsLoaded{0};
    uint64_t geometriesCapacity{0};
    uint64_t geometriesLoaded{0};
    uint64_t geometriesRendered{0};
    uint64_t trianglesLoaded{0};
    uint64_t trianglesRendered{0};
};

// -1 means the prim is not yet associated with a tileset
const auto NO_TILESET_ID = int64_t(-1);

} // namespace cesium::omniverse

namespace cesium::omniverse::FabricUtil {

std::string printFabricStage();
FabricStatistics getStatistics();
void destroyPrim(const omni::fabric::Path& path);
void setTilesetTransform(int64_t tilesetId, const glm::dmat4& ecefToUsdTransform);
void setTilesetId(const omni::fabric::Path& path, int64_t tilesetId);
omni::fabric::Path toFabricPath(const pxr::SdfPath& path);
omni::fabric::Path joinPaths(const omni::fabric::Path& absolutePath, const omni::fabric::Token& relativePath);

} // namespace cesium::omniverse::FabricUtil
