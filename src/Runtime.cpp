#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock;
    constexpr float kRefreshInterval = 0.10f;

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

    CCLabelBMFont* ensureLabel(CCNode* parent, char const* id) {
        if (auto* existing = static_cast<CCLabelBMFont*>(parent->getChildByID(id))) {
            return existing;
        }

        auto* label = CCLabelBMFont::create("", "bigFont.fnt");
        if (!label) return nullptr;

        label->setID(id);
        label->setAnchorPoint({0.f, 1.f});
        parent->addChild(label, 10000);
        return label;
    }

    void setOverlayLine(
        PlayLayer* layer,
        char const* setting,
        char const* id,
        std::string const& value,
        float& y
    ) {
        auto* existing = layer->getChildByID(id);
        if (!Mod::get()->getSettingValue<bool>(setting)) {
            if (existing) existing->removeFromParentAndCleanup(true);
            return;
        }

        auto* label = ensureLabel(layer, id);
        if (!label) return;

        label->setString(value.c_str());
        label->setPosition({8.f, y});
        label->setScale(Mod::get()->getSettingValue<float>("labels-scale"));
        label->setOpacity(static_cast<GLubyte>(
            Mod::get()->getSettingValue<int>("labels-opacity")
        ));
        y -= 20.f * label->getScale();
    }

    void updateDiagnosticsOverlay(PlayLayer* layer, float dt) {
        static auto sessionStart = Clock::now();
        static float refreshAccumulator = 0.f;
        static float lastFrameTimeMs = 0.f;
        static bool safeModeNoticeShown = false;

        auto* mod = Mod::get();
        bool safeMode = mod->getSettingValue<bool>("safe-mode-enabled");
        if (safeMode && !safeModeNoticeShown && mod->getSettingValue<bool>("safe-mode-popup")) {
            FLAlertLayer::create(
                "Violetmod Safe Mode",
                "Safe Mode is enabled. Experimental Violetmod gameplay modules are disabled where supported.",
                "OK"
            )->show();
            safeModeNoticeShown = true;
        }
        if (!safeMode) safeModeNoticeShown = false;

        if (safeMode) {
            if (auto* label = ensureLabel(layer, "violetmod-overlay-safe")) {
                label->setString("Violetmod: SAFE MODE");
                auto size = CCDirector::get()->getWinSize();
                label->setPosition({8.f, size.height - 8.f});
                label->setScale(mod->getSettingValue<float>("labels-scale"));
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

        std::ostringstream fps, frameTime, attempts, clicks, speed, px, py;
        fps << "FPS (update estimate): " << std::fixed << std::setprecision(1)
            << (lastFrameTimeMs > 0.f ? 1000.f / lastFrameTimeMs : 0.f);
        frameTime << "Frame time: " << std::fixed << std::setprecision(2) << lastFrameTimeMs << " ms";
        attempts << "Attempts: " << layer->m_attempts;
        clicks << "Clicks: " << layer->m_clicks;
        speed << "Speed setting: " << std::fixed << std::setprecision(2)
              << mod->getSettingValue<float>("speedhack-value") << "x";

        setOverlayLine(layer, "label-fps", "violetmod-overlay-fps", fps.str(), y);
        setOverlayLine(layer, "label-frame-time", "violetmod-overlay-frame-time", frameTime.str(), y);
        setOverlayLine(layer, "label-session-time", "violetmod-overlay-session-time",
            "Session: " + formatDuration(elapsed), y);
        setOverlayLine(layer, "label-attempts", "violetmod-overlay-attempts", attempts.str(), y);
        setOverlayLine(layer, "label-clicks", "violetmod-overlay-clicks", clicks.str(), y);
        setOverlayLine(layer, "label-speed", "violetmod-overlay-speed", speed.str(), y);

        auto* player = layer->m_player1;
        if (player) {
            auto pos = player->getPosition();
            px << "Player X: " << std::fixed << std::setprecision(1) << pos.x;
            py << "Player Y: " << std::fixed << std::setprecision(1) << pos.y;
            setOverlayLine(layer, "label-player-x", "violetmod-overlay-player-x", px.str(), y);
            setOverlayLine(layer, "label-player-y", "violetmod-overlay-player-y", py.str(), y);
        } else {
            setOverlayLine(layer, "label-player-x", "violetmod-overlay-player-x", "Player X: --", y);
            setOverlayLine(layer, "label-player-y", "violetmod-overlay-player-y", "Player Y: --", y);
        }

        std::string levelName = "Level: (unknown)";
        if (layer->m_level && !layer->m_level->m_levelName.empty()) {
            levelName = std::string("Level: ") + layer->m_level->m_levelName.c_str();
        }
        setOverlayLine(layer, "label-level-name", "violetmod-overlay-level", levelName, y);
        setOverlayLine(layer, "label-practice-status", "violetmod-overlay-practice",
            layer->m_isPracticeMode ? "Mode: Practice" : "Mode: Normal", y);
    }
}

class $modify(VioletmodPlayLayer, PlayLayer) {
    void update(float dt) {
        PlayLayer::update(dt);
        updateDiagnosticsOverlay(this, dt);
    }
};
