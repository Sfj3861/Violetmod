#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock;
    constexpr float kRefreshInterval = 0.10f;

    struct OverlayLine {
        char const* setting;
        char const* id;
        char const* label;
    };

    constexpr OverlayLine kLines[] = {
        {"label-fps", "violetmod-overlay-fps", "FPS"},
        {"label-frame-time", "violetmod-overlay-frame-time", "Frame time"},
        {"label-session-time", "violetmod-overlay-session-time", "Session"}
    };

    std::string formatDuration(double seconds) {
        auto total = static_cast<unsigned long long>(seconds);
        auto hours = total / 3600;
        auto minutes = (total / 60) % 60;
        auto secs = total % 60;
        std::ostringstream out;
        out << std::setfill('0') << std::setw(2) << hours << ':'
            << std::setw(2) << minutes << ':' << std::setw(2) << secs;
        return out.str();
    }

    CCLabelBMFont* ensureLabel(CCNode* parent, char const* id, float y) {
        if (auto* existing = typeinfo_cast<CCLabelBMFont*>(parent->getChildByID(id))) {
            return existing;
        }

        auto* label = CCLabelBMFont::create("", "bigFont.fnt");
        if (!label) return nullptr;

        label->setID(id);
        label->setAnchorPoint({0.f, 1.f});
        label->setScale(Mod::get()->getSettingValue<float>("labels-scale"));
        label->setOpacity(static_cast<GLubyte>(
            Mod::get()->getSettingValue<int>("labels-opacity")
        ));
        label->setPosition({8.f, y});
        parent->addChild(label, 10000);
        return label;
    }

    void updateDiagnosticsOverlay(PlayLayer* layer, float dt) {
        static auto sessionStart = Clock::now();
        static float refreshAccumulator = 0.f;
        static float lastFrameTimeMs = 0.f;

        if (Mod::get()->getSettingValue<bool>("safe-mode-enabled")) {
            if (auto* label = ensureLabel(layer, "violetmod-overlay-safe", 0.f)) {
                label->setString("Violetmod: SAFE MODE");
                auto size = CCDirector::get()->getWinSize();
                label->setPosition({8.f, size.height - 8.f});
                label->setScale(Mod::get()->getSettingValue<float>("labels-scale"));
                label->setOpacity(255);
            }
        } else if (auto* safe = layer->getChildByID("violetmod-overlay-safe")) {
            safe->removeFromParentAndCleanup(true);
        }

        if (dt > 0.f) lastFrameTimeMs = dt * 1000.f;
        refreshAccumulator += dt;
        if (refreshAccumulator < kRefreshInterval) return;
        refreshAccumulator = 0.f;

        auto winSize = CCDirector::get()->getWinSize();
        float y = winSize.height - 8.f;
        auto elapsed = std::chrono::duration<double>(Clock::now() - sessionStart).count();

        for (auto const& line : kLines) {
            auto enabled = Mod::get()->getSettingValue<bool>(line.setting);
            auto* existing = layer->getChildByID(line.id);
            if (!enabled) {
                if (existing) existing->removeFromParentAndCleanup(true);
                continue;
            }

            auto* label = ensureLabel(layer, line.id, y);
            if (!label) continue;

            std::ostringstream value;
            value << line.label << ": ";
            if (std::string(line.setting) == "label-fps") {
                value << std::fixed << std::setprecision(1)
                      << (lastFrameTimeMs > 0.f ? 1000.f / lastFrameTimeMs : 0.f);
            } else if (std::string(line.setting) == "label-frame-time") {
                value << std::fixed << std::setprecision(2) << lastFrameTimeMs << " ms";
            } else {
                value << formatDuration(elapsed);
            }
            label->setString(value.str().c_str());
            label->setPosition({8.f, y});
            label->setScale(Mod::get()->getSettingValue<float>("labels-scale"));
            label->setOpacity(static_cast<GLubyte>(
                Mod::get()->getSettingValue<int>("labels-opacity")
            ));
            y -= 20.f * label->getScale();
        }
    }
}

class $modify(VioletmodPlayLayer, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        updateDiagnosticsOverlay(this, dt);
    }
};
