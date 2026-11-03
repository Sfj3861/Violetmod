#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

using namespace geode::prelude;

namespace {
    using Clock = std::chrono::steady_clock;

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
        auto win = CCDirector::get()->getWinSize();
        float overlayX = std::clamp(Mod::get()->getSettingValue<float>("overlay-x"), 0.f, std::max(0.f, win.width - 20.f));
        label->setPosition({overlayX, y});
        label->setScale(Mod::get()->getSettingValue<float>("labels-scale"));
        label->setOpacity(static_cast<GLubyte>(
            Mod::get()->getSettingValue<int>("labels-opacity")
        ));
        y -= Mod::get()->getSettingValue<float>("overlay-line-spacing") * label->getScale();
    }

    void updateDiagnosticsOverlay(PlayLayer* layer, float dt) {
        static PlayLayer* trackedLayer = nullptr;
        static Clock::time_point sessionStart = Clock::now();
        static Clock::time_point tpsWindowStart = Clock::now();
        static float refreshAccumulator = 0.f;
        static float lastFrameTimeMs = 0.f;
        static float smoothedFrameTimeMs = 0.f;
        static unsigned int updatesInWindow = 0;
        static unsigned long long sessionFrameCount = 0;
        static float measuredTps = 0.f;
        static int previousClicks = 0;
        static float clicksPerSecond = 0.f;
        static Clock::time_point clickWindowStart = Clock::now();
        static bool safeModeNoticeShown = false;

        auto* mod = Mod::get();
        auto now = Clock::now();
        if (trackedLayer != layer) {
            trackedLayer = layer;
            sessionStart = now;
            tpsWindowStart = now;
            clickWindowStart = now;
            refreshAccumulator = 0.f;
            updatesInWindow = 0;
            sessionFrameCount = 0;
            previousClicks = layer->m_clicks;
            measuredTps = 0.f;
            clicksPerSecond = 0.f;
            lastFrameTimeMs = 0.f;
            smoothedFrameTimeMs = 0.f;
        }
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

        if (dt > 0.f) {
            lastFrameTimeMs = dt * 1000.f;
            smoothedFrameTimeMs = smoothedFrameTimeMs <= 0.f
                ? lastFrameTimeMs : (smoothedFrameTimeMs * 0.85f + lastFrameTimeMs * 0.15f);
            ++updatesInWindow;
            ++sessionFrameCount;
        }
        auto tpsElapsed = std::chrono::duration<float>(now - tpsWindowStart).count();
        if (tpsElapsed >= 1.f) {
            measuredTps = tpsElapsed > 0.f
                ? static_cast<float>(updatesInWindow) / tpsElapsed : 0.f;
            updatesInWindow = 0;
            tpsWindowStart = now;
        }
        float clickElapsed = std::chrono::duration<float>(now - clickWindowStart).count();
        if (clickElapsed >= 1.f) {
            int clickDelta = std::max(0, layer->m_clicks - previousClicks);
            clicksPerSecond = static_cast<float>(clickDelta) / clickElapsed;
            previousClicks = layer->m_clicks;
            clickWindowStart = now;
        }
        float refreshInterval = std::clamp(mod->getSettingValue<float>("overlay-refresh"), 0.03f, 0.5f);
        refreshAccumulator += std::max(0.f, dt);
        if (refreshAccumulator < refreshInterval) return;
        refreshAccumulator = std::fmod(refreshAccumulator, refreshInterval);

        auto winSize = CCDirector::get()->getWinSize();
        float x = std::clamp(mod->getSettingValue<float>("overlay-x"), 0.f, winSize.width - 20.f);
        float y = std::clamp(winSize.height - mod->getSettingValue<float>("overlay-y"), 20.f, winSize.height);
        float lineSpacing = std::clamp(mod->getSettingValue<float>("overlay-line-spacing"), 10.f, 40.f);
        auto elapsed = std::chrono::duration<double>(now - sessionStart).count();

        std::ostringstream fps, frameTime, attempts, clicks, speed, px, py, tps, clickRate, frameCounter;
        fps << "FPS (update estimate): " << std::fixed << std::setprecision(1)
            << (lastFrameTimeMs > 0.f ? 1000.f / lastFrameTimeMs : 0.f);
        frameTime << "Frame time: " << std::fixed << std::setprecision(2) << smoothedFrameTimeMs << " ms";
        clickRate << "Click rate: " << std::fixed << std::setprecision(1) << clicksPerSecond << "/s";
        tps << "TPS (PlayLayer updates): " << std::fixed << std::setprecision(1) << measuredTps;
        frameCounter << "Frames: " << sessionFrameCount;
        attempts << "Attempts: " << layer->m_attempts;
        clicks << "Clicks: " << layer->m_clicks;
        speed << "Speedhack: " << ((mod->getSettingValue<bool>("feature-speedhack") && layer->m_isPracticeMode && !safeMode) ? "ACTIVE " : "inactive ")
              << std::fixed << std::setprecision(2) << mod->getSettingValue<float>("speedhack-value") << "x";

        setOverlayLine(layer, "label-fps", "violetmod-overlay-fps", fps.str(), y);
        setOverlayLine(layer, "label-frame-time", "violetmod-overlay-frame-time", frameTime.str(), y);
        setOverlayLine(layer, "label-frame-counter", "violetmod-overlay-frame-counter", frameCounter.str(), y);
        setOverlayLine(layer, "label-tps", "violetmod-overlay-tps", tps.str(), y);
        setOverlayLine(layer, "label-physics-tps", "violetmod-overlay-physics-tps", "Physics TPS: separate engine counter unavailable", y);
        setOverlayLine(layer, "label-session-time", "violetmod-overlay-session-time",
            "Session: " + formatDuration(elapsed), y);
        setOverlayLine(layer, "label-attempts", "violetmod-overlay-attempts", attempts.str(), y);
        setOverlayLine(layer, "label-clicks", "violetmod-overlay-clicks", clicks.str(), y);
        setOverlayLine(layer, "label-click-rate", "violetmod-overlay-click-rate", clickRate.str(), y);
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
        float realDt = dt;
        auto* mod = Mod::get();
        bool safeMode = mod->getSettingValue<bool>("safe-mode-enabled");
        bool speedhack = mod->getSettingValue<bool>("feature-speedhack");
        // Time manipulation is confined to Practice Mode to protect normal runs.
        if (!safeMode && speedhack && m_isPracticeMode) {
            float multiplier = mod->getSettingValue<float>("speedhack-value");
            if (std::isfinite(multiplier)) dt *= std::clamp(multiplier, 0.1f, 5.f);
        }
        PlayLayer::update(dt);
        updateDiagnosticsOverlay(this, realDt);
    }
};
