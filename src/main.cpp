#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class VioletmodPopup final : public FLAlertLayer {
public:
    static VioletmodPopup* create() {
        auto* popup = new VioletmodPopup();
        if (popup && popup->init(360.f, 220.f, "GJ_square01.png", "Violetmod")) {
            popup->autorelease();
            return popup;
        }
        CC_SAFE_DELETE(popup);
        return nullptr;
    }

    bool init(float width, float height, char const* background, char const* title) {
        if (!FLAlertLayer::init(width, height, background, title, "Close", nullptr, 300.f)) {
            return false;
        }

        auto* message = CCLabelBMFont::create(
            "Violetmod is loaded!\nThe modular interface is being built.",
            "bigFont.fnt"
        );
        if (message) {
            message->setScale(0.42f);
            message->setPosition({m_mainLayer->getContentSize().width / 2.f,
                                  m_mainLayer->getContentSize().height / 2.f - 8.f});
            m_mainLayer->addChild(message);
        }
        return true;
    }
};

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

        auto* sprite = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
        if (!sprite) {
            log::warn("Violetmod: could not create the menu button sprite.");
            return true;
        }

        auto* button = CCMenuItemSpriteExtra::create(
            sprite,
            this,
            menu_selector(VioletmodMenuLayer::onVioletmodPressed)
        );
        button->setID("violetmod-open"_spr);
        menu->addChild(button);
        menu->updateLayout();

        return true;
    }

    void onVioletmodPressed(CCObject*) {
        auto* popup = VioletmodPopup::create();
        if (popup) {
            popup->show();
        }
    }
};
