#include "cesium/omniverse/CesiumOmniverse.h"

#include "cesium/omniverse/OmniTileset.h"

#include <unordered_map>

namespace Cesium {

namespace {
int currentId = 0;
std::unordered_map<int, std::unique_ptr<OmniTileset>> tilesets;
} // namespace

void initialize(const char* cesiumMemLocation) noexcept {
    OmniTileset::init(cesiumMemLocation);
}

void finalize() noexcept {
    OmniTileset::shutdown();
}

int addTilesetUrl(const pxr::UsdStageRefPtr* stage, const char* url) noexcept {
    const int tilesetId = currentId++;
    tilesets.insert({tilesetId, std::make_unique<OmniTileset>(*stage, url)});
    return tilesetId;
}

int addTilesetIon(const pxr::UsdStageRefPtr* stage, int64_t ionId, const char* ionToken) noexcept {
    const int tilesetId = currentId++;
    tilesets.insert({tilesetId, std::make_unique<OmniTileset>(*stage, ionId, ionToken)});
    return tilesetId;
}

void removeTileset(int tileset) noexcept {
    tilesets.erase(tileset);
}

void addIonRasterOverlay(int tileset, const char* name, int64_t ionId, const char* ionToken) noexcept {
    const auto iter = tilesets.find(tileset);
    if (iter != tilesets.end()) {
        iter->second->addIonRasterOverlay(name, ionId, ionToken);
    }
}

void updateFrame(
    int tileset,
    const pxr::GfMatrix4d* viewMatrix,
    const pxr::GfMatrix4d* projMatrix,
    double width,
    double height) noexcept {
    const auto iter = tilesets.find(tileset);
    if (iter != tilesets.end()) {
        iter->second->updateFrame(*viewMatrix, *projMatrix, width, height);
    }
}

void setGeoreferenceOrigin(double longitude, double latitude, double height) noexcept {
    Cesium::Georeference::instance().setOrigin(CesiumGeospatial::Ellipsoid::WGS84.cartographicToCartesian(
        CesiumGeospatial::Cartographic(glm::radians(longitude), glm::radians(latitude), height)));
}

} // namespace Cesium