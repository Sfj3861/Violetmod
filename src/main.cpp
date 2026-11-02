#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

namespace {
    constexpr char kOpenButtonID[] = "violetmod-open";

    class VioletmodPopup final : public Popup {
    protected:
        CCLabelBMFont* m_pageText = nullptr;

        bool init() {
            if (!Popup::init(320.f, 220.f)) {
                return false;
            }

            this->setTitle("Violetmod", "bigFont.fnt", 0.8f, 25.f);
            m_bgSprite->setColor(ccc3(128, 86, 190));

            auto* subtitle = CCLabelBMFont::create(
                "A modular Geometry Dash toolkit",
                "goldFont.fnt"
            );
            subtitle->setScale(0.42f);
            subtitle->setPosition({160.f, 166.f});
            m_mainLayer->addChild(subtitle);

            m_pageText = CCLabelBMFont::create("", "bigFont.fnt");
            m_pageText->setScale(0.43f);
            m_pageText->setPosition({160.f, 112.f});
            m_mainLayer->addChild(m_pageText);

            auto* menu = CCMenu::create();
            menu->setPosition({0.f, 0.f});
            m_mainLayer->addChild(menu);

            this->addNavButton(menu, "Home", 70.f, menu_selector(VioletmodPopup::onHome));
            this->addNavButton(menu, "Tools", 160.f, menu_selector(VioletmodPopup::onTools));
            this->addNavButton(menu, "About", 250.f, menu_selector(VioletmodPopup::onAbout));

            this->showPage(0);
            return true;
        }

        void addNavButton(CCMenu* menu, char const* title, float x, SEL_MenuHandler callback) {
            auto* label = CCLabelBMFont::create(title, "goldFont.fnt");
            label->setScale(0.48f);

            auto* item = CCMenuItemSpriteExtra::create(label, this, callback);
            item->setPosition({x, 38.f});
            menu->addChild(item);
        }

        void showPage(int page) {
            if (!m_pageText) return;

            switch (page) {
                case 0:
                    m_pageText->setString(
                        "Welcome to Violetmod!\n"
                        "Your lightweight, violet-themed GD toolkit.\n\n"
                        "Choose a section below to explore the project."
                    );
                    break;
                case 1:
                    m_pageText->setString(
                        "TOOLS\n"
                        "Frame Counter  - planned\n"
                        "Hitbox Viewer  - planned\n"
                        "Show Trajectory - planned\n\n"
                        "These tools are not active yet."
                    );
                    break;
                case 2:
                    m_pageText->setString(
                        "ABOUT VIOLETMOD\n"
                        "Independent project built with Geode.\n"
                        "Designed for a clear, touch-friendly UI.\n\n"
                        "Version 0.1.1 - interface foundation"
                    );
                    break;
            }
        }

        void onHome(CCObject*) { this->showPage(0); }
        void onTools(CCObject*) { this->showPage(1); }
        void onAbout(CCObject*) { this->showPage(2); }

    public:
        static VioletmodPopup* create() {
            auto* ret = new VioletmodPopup();
            if (ret->init()) {
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
        if (!MenuLayer::init()) {
            return false;
        }

        auto* menu = this->getChildByID("bottom-menu");
        if (!menu) {
            log::warn("Violetmod: bottom-menu was not found; skipping menu button.");
            return true;
        }

        if (menu->getChildByID(kOpenButtonID)) {
            return true;
        }

        auto* sprite = CCLabelBMFont::create("V", "goldFont.fnt");
        if (!sprite) {
            log::warn("Violetmod: could not create the menu button label.");
            return true;
        }
        sprite->setScale(0.8f);

        auto* button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(VioletmodMenuLayer::onVioletmodPressed)
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
