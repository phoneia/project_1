#include "gamescene.h"
#include "HelloWorldScene.h"
#include "AudioEngine.h"
#include "Option.h"


USING_NS_CC;

using namespace experimental;

Scene* gamescene::createScene()
{
    return gamescene::create();
}

bool gamescene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 메인 메뉴
    ///////////////////////////////////
	winSize = Director::getInstance()->getWinSize();

    auto pNode = Layer::create();
	this->addChild(pNode);

    auto pBack = Sprite::create("Images/3.3.png");
    pBack->setAnchorPoint(Vec2::ZERO);
    pBack->setPosition(Vec2::ZERO);

    pNode->addChild(pBack);

    TTFConfig ttfconfig("fonts/K_Damage.ttf", 35.0f);
    ttfconfig.outlineSize = 3;
    ttfconfig.bold = true;

    auto name = Label::createWithTTF(ttfconfig, "BLIZZARD RANDOM DEFENSE");
    name->setPosition(Vec2(winSize.width/2, winSize.height/2+150));
    name->setColor(Color3B(155, 130, 104));
    name->setScale(2.0f);
    name->enableOutline(Color4B::BLACK);
    this->addChild(name);


    auto label1 = Label::createWithTTF(ttfconfig, "START");
    auto item1 = MenuItemLabel::create(label1,
        CC_CALLBACK_1(gamescene::StartScene, this));
    item1->setColor(Color3B::RED);

    auto label2 = Label::createWithTTF(ttfconfig, "OPTION");
    auto item2 = MenuItemLabel::create(label2,
        CC_CALLBACK_1(gamescene::OptionScene, this));
    item2->setColor(Color3B::RED);

	auto label3 = Label::createWithTTF(ttfconfig, "EXIT");
	auto item3 = MenuItemLabel::create(label3,
		CC_CALLBACK_1(gamescene::ExitScene, this));
	item3->setColor(Color3B::RED);
    
    auto menu = Menu::create(item1, item2, item3, nullptr);
    menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 150));
    menu->alignItemsVerticallyWithPadding(20);
    this->addChild(menu);
        
    AudioEngine::play2d("introedit.mp3", true, 0.1f, nullptr);
   
    return true;
}

void gamescene::OptionScene(Ref * pSender)
{
    auto scene = OptionScene::createScene();
    Director::getInstance()->pushScene(scene);
}

void gamescene::StartScene(Ref * pSender)
{
    auto scene = HelloWorld::createScene();
	Director::getInstance()->pushScene(scene);   
}

void gamescene::ExitScene(Ref* pSender)
{
	exit(0);
}

