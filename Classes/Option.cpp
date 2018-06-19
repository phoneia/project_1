#include "Option.h"
#include "gamescene.h"
#include "AudioEngine.h"

USING_NS_CC;
using namespace experimental;

Scene * OptionScene::createScene()
{
    return OptionScene::create();
}

bool OptionScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    winSize = Director::getInstance()->getWinSize();

    auto bg = Sprite::create("Images/3.3.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(Vec2::ZERO);    
    bg->setOpacity(100);
    this->addChild(bg, 0);

    TTFConfig ttfconfig("fonts/K_Damage.ttf", 50.0f);
    ttfconfig.outlineSize = 3;
    ttfconfig.bold = true;

    auto UP = Label::createWithTTF(ttfconfig, "UP");
    auto item1 = MenuItemLabel::create(UP,
        CC_CALLBACK_1(OptionScene::upVolume, this));
    item1->setColor(Color3B::WHITE);

    auto DOWN = Label::createWithTTF(ttfconfig, "DOWN");
    auto item2 = MenuItemLabel::create(DOWN,
        CC_CALLBACK_1(OptionScene::downVolume, this));
    item2->setColor(Color3B::WHITE);

    auto BACK = Label::createWithTTF(ttfconfig, "BACK");
    auto item3 = MenuItemLabel::create(BACK,
        CC_CALLBACK_1(OptionScene::returnTitle, this));
    item3->setColor(Color3B::WHITE);
    item3->setPosition(Vec2(-612, 320));
    item3->setAnchorPoint(Vec2(0, 1));

    auto menu1 = Menu::create(item3, nullptr);
    this->addChild(menu1, 2);

    auto menu = Menu::create(item1, item2, nullptr);
    menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    menu->alignItemsHorizontallyWithPadding(30);
    this->addChild(menu, 1);

    return true;

}

void OptionScene::returnTitle(Ref * pSender)
{
    Director::getInstance()->popScene();
}

void OptionScene::upVolume(Ref * pSender)
{
    if (sound < 1.0f)
        sound += 0.1f;
    AudioEngine::setVolume(audio, sound);
}

void OptionScene::downVolume(Ref * pSender)
{
    if (sound > 0.0f)
        sound -= 0.1f;
    AudioEngine::setVolume(audio, sound);
}