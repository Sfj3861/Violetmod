#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr char kOpenButtonID[] = "violetmod-open";

    ccColor3B getAccentColor() {
        auto theme = Mod::get()->getSettingValue<std::string>("ui-theme");
        if (theme == "midnight") return ccc3(76, 91, 150);
        if (theme == "pink") return ccc3(190, 91, 150);
        if (theme == "blue") return ccc3(65, 125, 205);
        if (theme == "mint") return ccc3(65, 155, 130);
        if (theme == "amber") return ccc3(190, 135, 55);
        if (theme == "mono") return ccc3(105, 105, 115);
        return ccc3(128, 86, 190);
    }

    class VioletmodPopup final : public Popup {
    protected:
        CCLabelBMFont* m_pageText = nullptr;
        int m_currentPage = 0;

        bool init() {
            auto width = Mod::get()->getSettingValue<float>("ui-popup-width");
            auto height = Mod::get()->getSettingValue<float>("ui-popup-height");
            auto scale = Mod::get()->getSettingValue<float>("ui-scale");
            bool compact = Mod::get()->getSettingValue<bool>("ui-compact");

            if (!Popup::init(width, height)) return false;

            m_bgSprite->setColor(getAccentColor());
            this->setTitle("Violetmod", "bigFont.fnt", 0.8f, 25.f);

            if (Mod::get()->getSettingValue<bool>("ui-show-subtitle")) {
                auto* subtitle = CCLabelBMFont::create(
                    "A customizable Geometry Dash toolkit", "goldFont.fnt"
                );
                subtitle->setScale((compact ? 0.36f : 0.42f) * scale);
                subtitle->setPosition({width / 2.f, height - 54.f});
                m_mainLayer->addChild(subtitle);
            }

            m_pageText = CCLabelBMFont::create("", "bigFont.fnt");
            m_pageText->setScale((compact ? 0.38f : 0.43f) * scale);
            m_pageText->setPosition({width / 2.f, height / 2.f - 4.f});
            m_pageText->setAnchorPoint({0.5f, 0.5f});
            m_mainLayer->addChild(m_pageText);

            auto* menu = CCMenu::create();
            menu->setPosition({0.f, 0.f});
            m_mainLayer->addChild(menu);

            float navY = compact ? 30.f : 38.f;
            this->addNavButton(menu, "Home", width * 0.22f, navY,
                menu_selector(VioletmodPopup::onHome));
            this->addNavButton(menu, "Tools", width * 0.50f, navY,
                menu_selector(VioletmodPopup::onTools));
            this->addNavButton(menu, "Settings", width * 0.78f, navY,
                menu_selector(VioletmodPopup::onSettings));

            this->showPage(0);
            return true;
        }

        void addNavButton(CCMenu* menu, char const* title, float x, float y, SEL_MenuHandler callback) {
            auto* label = CCLabelBMFont::create(title, "goldFont.fnt");
            label->setScale(0.43f);
            auto* item = CCMenuItemSpriteExtra::create(label, this, callback);
            item->setPosition({x, y});
            menu->addChild(item);
        }

        void showPage(int page) {
            if (!m_pageText) return;
            m_currentPage = page;

            switch (page) {
                case 0:
                    m_pageText->setString(
                        "WELCOME TO VIOLETMOD\n\n"
                        "A modular toolkit with a customizable interface.\n"
                        "Open the Settings tab in Geode to personalize it.\n\n"
                        "Choose Tools to see the feature catalogue."
                    );
                    break;
                case 1:
                    m_pageText->setString(
                        "FEATURE CATALOGUE\n\n"
                        "Frame Counter - planned\n"
                        "Hitbox Viewer - planned\n"
                        "Show Trajectory - planned\n"
                        "Practice utilities - planned\n"
                        "Session statistics - planned\n\n"
                        "Feature switches are defaults, not active tools yet."
                    );
                    break;
                case 2:
                    m_pageText->setString(
                        "CUSTOMIZATION\n\n"
                        "Open Geode > Violetmod > Settings.\n"
                        "Theme, scale, popup size and menu shortcut\n"
                        "can be adjusted there. Changes are saved by Geode."
                    );
                    break;
            }
        }

        void onHome(CCObject*) { this->showPage(0); }
        void onTools(CCObject*) { this->showPage(1); }
        void onSettings(CCObject*) { this->showPage(2); }

    public:
        static VioletmodPopup* create() {
            auto* ret = new VioletmodPopup();
            if (ret->init()) {
                ret->m_noElasticity = !Mod::get()->getSettingValue<bool>("ui-animations");
                ret->autorelease();
                return ret;
            }
            delete ret;
            return nullptr;
        }
    };
}

class $modify(VioletmodMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (!Mod::get()->getSettingValue<bool>("ui-button-enabled")) return true;

        auto* menu = this->getChildByID("bottom-menu");
        if (!menu) {
            log::warn("Violetmod: bottom-menu was not found; skipping menu button.");
            return true;
        }
        if (menu->getChildByID(kOpenButtonID)) return true;

        auto labelText = Mod::get()->getSettingValue<std::string>("ui-button-label");
        if (labelText.empty()) labelText = "V";

        auto* label = CCLabelBMFont::create(labelText.c_str(), "goldFont.fnt");
        if (!label) return true;

        label->setScale(Mod::get()->getSettingValue<float>("ui-button-scale"));
        label->setOpacity(static_cast<GLubyte>(
            Mod::get()->getSettingValue<int>("ui-button-opacity")
        ));

        auto* button = CCMenuItemSpriteExtra::create(
            label, this, menu_selector(VioletmodMenuLayer::onVioletmodPressed)
        );
        button->setID(kOpenButtonID);
        menu->addChild(button);
        menu->updateLayout();
        return true;
    }

    void onVioletmodPressed(CCObject*) {
        if (auto* popup = VioletmodPopup::create()) {
            popup->show();
        }
    }
};
