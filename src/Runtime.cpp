#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <algorithm>
#include <chrono>
#include <deque>
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

    void updateVisualDebug(PlayLayer* layer) {
        static PlayLayer* trackedLayer = nullptr;
        static std::deque<CCPoint> playerPath;
        auto* mod = Mod::get();

        auto* draw = static_cast<CCDrawNode*>(layer->getChildByID("violetmod-visual-debug"));
        bool showBounds = mod->getSettingValue<bool>("feature-show-hitboxes");
        bool showTrail = mod->getSettingValue<bool>("feature-show-hitbox-trail");
        bool showPath = mod->getSettingValue<bool>("feature-show-trajectory");
        bool enabled = (showBounds || showTrail || showPath) &&
            !mod->getSettingValue<bool>("safe-mode-enabled");

        if (!enabled || !layer->m_player1) {
            if (draw) draw->clear();
            if (!enabled) playerPath.clear();
            return;
        }

        if (!draw) {
            draw = CCDrawNode::create();
            if (!draw) return;
            draw->setID("violetmod-visual-debug");
            layer->addChild(draw, 9998);
        }

        auto* player = layer->m_player1;
        if (trackedLayer != layer) {
            trackedLayer = layer;
            playerPath.clear();
        }

        // Store the player's position in PlayLayer coordinates so the trace
        // follows the world/camera rather than being pinned to the screen.
        auto worldOrigin = player->convertToWorldSpace(CCPointZero);
        auto localOrigin = layer->convertToNodeSpace(worldOrigin);
        float dx = playerPath.empty() ? 0.f : localOrigin.x - playerPath.back().x;
        float dy = playerPath.empty() ? 0.f : localOrigin.y - playerPath.back().y;
        if (playerPath.empty() || std::hypot(dx, dy) >= 1.5f) {
            playerPath.push_back(localOrigin);
            if (playerPath.size() > 90) playerPath.pop_front();
        }

        draw->clear();
        if (showPath && playerPath.size() > 1) {
            for (size_t i = 1; i < playerPath.size(); ++i) {
                float alpha = 0.15f + 0.65f *
                    (static_cast<float>(i) / static_cast<float>(playerPath.size()));
                draw->drawSegment(
                    playerPath[i - 1], playerPath[i], 1.4f,
                    ccColor4F{0.25f, 0.85f, 1.f, alpha}
                );
            }
        }

        auto size = player->getContentSize();
        auto anchor = player->getAnchorPoint();
        float width = size.width * std::abs(player->getScaleX());
        float height = size.height * std::abs(player->getScaleY());
        auto drawBounds = [&](CCPoint const& p, float alpha, float thickness) {
            float l = p.x - width * anchor.x;
            float r = l + width;
            float b = p.y - height * anchor.y;
            float t = b + height;
            auto color = ccColor4F{1.f, 0.28f, 0.28f, alpha};
            draw->drawSegment({l, b}, {r, b}, thickness, color);
            draw->drawSegment({r, b}, {r, t}, thickness, color);
            draw->drawSegment({r, t}, {l, t}, thickness, color);
            draw->drawSegment({l, t}, {l, b}, thickness, color);
        };

        if (showTrail) {
            size_t count = std::min<size_t>(playerPath.size(), 8);
            for (size_t i = 0; i < count; ++i) {
                auto const& p = playerPath[playerPath.size() - 1 - i];
                float alpha = 0.08f + 0.035f * static_cast<float>(count - i);
                float l = p.x - width * anchor.x;
                float r = l + width;
                float b = p.y - height * anchor.y;
                float t = b + height;
                auto color = ccColor4F{1.f, 0.55f, 0.2f, alpha};
                draw->drawSegment({l, b}, {r, b}, 0.8f, color);
                draw->drawSegment({r, b}, {r, t}, 0.8f, color);
                draw->drawSegment({r, t}, {l, t}, 0.8f, color);
                draw->drawSegment({l, t}, {l, b}, 0.8f, color);
            }
        }
        if (showBounds) {
            drawBounds(localOrigin, 0.95f, 1.6f);
        }
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
        float minY = std::min(20.f, std::max(0.f, winSize.height));
        float y = std::clamp(
            winSize.height - mod->getSettingValue<float>("overlay-y"),
            minY,
            std::max(minY, winSize.height)
        );
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
        updateVisualDebug(this);
    }
};
