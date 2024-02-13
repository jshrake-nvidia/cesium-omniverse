#pragma once

#include <CesiumRasterOverlays/RasterOverlay.h>
#include <pxr/usd/sdf/path.h>

namespace CesiumRasterOverlays {
class RasterOverlay;
}

namespace cesium::omniverse {

class Context;
enum class FabricOverlayRenderMethod;

class OmniRasterOverlay {
  public:
    OmniRasterOverlay(Context* pContext, const pxr::SdfPath& path);
    virtual ~OmniRasterOverlay() = default;
    OmniRasterOverlay(const OmniRasterOverlay&) = delete;
    OmniRasterOverlay& operator=(const OmniRasterOverlay&) = delete;
    OmniRasterOverlay(OmniRasterOverlay&&) noexcept = default;
    OmniRasterOverlay& operator=(OmniRasterOverlay&&) noexcept = default;

    [[nodiscard]] const pxr::SdfPath& getPath() const;
    [[nodiscard]] bool getShowCreditsOnScreen() const;
    [[nodiscard]] double getAlpha() const;
    [[nodiscard]] FabricOverlayRenderMethod getOverlayRenderMethod() const;
    [[nodiscard]] float getMaximumScreenSpaceError() const;
    [[nodiscard]] int getMaximumTextureSize() const;
    [[nodiscard]] int getMaximumSimultaneousTileLoads() const;
    [[nodiscard]] int getSubTileCacheBytes() const;

    [[nodiscard]] CesiumRasterOverlays::RasterOverlayOptions getUsdOptions();

    [[nodiscard]] virtual CesiumRasterOverlays::RasterOverlay* getRasterOverlay() const = 0;
    void updateRasterOverlayOptions(CesiumRasterOverlays::RasterOverlayOptions& options);
    virtual void reload() = 0;

  protected:
    Context* _pContext;
    pxr::SdfPath _path;

  private:
    void setOptionsFromUsd(CesiumRasterOverlays::RasterOverlayOptions& options);
};
} // namespace cesium::omniverse
