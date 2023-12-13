#include "cesium/omniverse/OmniImagery.h"

#include "cesium/omniverse/Context.h"
#include "cesium/omniverse/OmniIonServer.h"
#include "cesium/omniverse/UsdUtil.h"

#include <CesiumUsdSchemas/imagery.h>

namespace cesium::omniverse {

OmniImagery::OmniImagery(const pxr::SdfPath& path)
    : _path(path) {}

pxr::SdfPath OmniImagery::getPath() const {
    return _path;
}

std::string OmniImagery::getName() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);
    return imagery.GetPrim().GetName().GetString();
}

int64_t OmniImagery::getIonAssetId() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);

    int64_t ionAssetId;
    imagery.GetIonAssetIdAttr().Get<int64_t>(&ionAssetId);

    return ionAssetId;
}

std::optional<CesiumIonClient::Token> OmniImagery::getIonAccessToken() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);

    std::string ionAccessToken;
    imagery.GetIonAccessTokenAttr().Get<std::string>(&ionAccessToken);

    if (!ionAccessToken.empty()) {
        CesiumIonClient::Token t;
        t.token = ionAccessToken;
        return t;
    }

    const auto ionServerPath = getIonServerPath();

    if (ionServerPath.IsEmpty()) {
        return std::nullopt;
    }

    const auto ionServer = OmniIonServer(ionServerPath);

    const auto token = ionServer.getToken();

    if (token.token.empty()) {
        return std::nullopt;
    }

    return token;
}

std::string OmniImagery::getIonApiUrl() const {
    const auto ionServerPath = getIonServerPath();

    if (ionServerPath.IsEmpty()) {
        return {};
    }

    const auto ionServer = OmniIonServer(ionServerPath);

    return ionServer.getIonServerApiUrl();
}

pxr::SdfPath OmniImagery::getIonServerPath() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);

    pxr::SdfPathVector targets;
    imagery.GetIonServerBindingRel().GetForwardedTargets(&targets);

    if (targets.empty()) {
        return {};
    }

    auto ionServerPath = targets.front();

    if (!UsdUtil::isCesiumIonServer(ionServerPath)) {
        return {};
    }

    return ionServerPath;
}

bool OmniImagery::getShowCreditsOnScreen() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);

    bool showCreditsOnScreen;
    imagery.GetShowCreditsOnScreenAttr().Get<bool>(&showCreditsOnScreen);

    return showCreditsOnScreen;
}

double OmniImagery::getAlpha() const {
    const auto imagery = UsdUtil::getCesiumImagery(_path);

    float alpha;
    imagery.GetAlphaAttr().Get<float>(&alpha);

    return static_cast<double>(alpha);
}

} // namespace cesium::omniverse
