#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

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
        FLAlertLayer::create(
            "Violetmod",
            "Violetmod is loaded!\nThe modular violet interface is being built.",
            "Close"
        )->show();
    }
};
