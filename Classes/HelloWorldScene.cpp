#include "gamescene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene * HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()						
{
    if (!Scene::init())
    {
        return false;
    }

	winSize = Director::getInstance()->getWinSize();

    listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
  
	initBuildScene();
    initBG();
    initGameData();
	initTower();
    initEnemy();
	initBullet();
	initUI();	
	 
    return true;
}

void HelloWorld::tick(float dt)
{
	for (Sprite *pTower : _tower)
	{
		TowerData *td = (TowerData*)pTower->getUserData();
		Vec2 towerPos = pTower->getPosition();
		
		if (pTower->isVisible() && !td->isDetective)
		{           
			for (Sprite *pEnemy : _enemy)
			{
				Vec2 enemyPos = pEnemy->getPosition();
												
				float dist = towerPos.distance(enemyPos);
				if (dist <= 200)
				{
					td->isDetective = true;
					td->dEnemy = pEnemy;
					pTower->setUserData(td);	
				}
			}
		}
		else if (pTower->isVisible() && td->dEnemy != nullptr)
		{
			float angle;
			if (td->isDetective)
			{
				angle = (towerPos - td->dEnemy->getPosition()).getAngle();

				pTower->setRotation(-CC_RADIANS_TO_DEGREES(angle) + 90);
			}
			Vec2 enemyPos = td->dEnemy->getPosition();

			float dist = towerPos.distance(enemyPos);
			if (dist > 200)
			{
				td->isDetective = false;
				td->dEnemy = nullptr;
				pTower->setUserData(td);
			}

			td->delayFire += dt;
			if (td->delayFire >= 0.5f)
			{
				td->delayFire = 0;
				if (td->isDetective)
				{
					setBullet(towerPos, enemyPos, -CC_RADIANS_TO_DEGREES(angle) + 90);
				}
			}

			pTower->setUserData(td);
		}
	}
	intersectMissile();
}

void HelloWorld::initBuildScene()
{
	TTFConfig ttfconfig("fonts/K_Damage.ttf", 30.0f);
	ttfconfig.outlineSize = 3;
	ttfconfig.bold = true;
	
	auto label1 = Label::createWithTTF(ttfconfig, "START");
	auto item1 = MenuItemLabel::create(label1,
		CC_CALLBACK_1(HelloWorld::StartGame, this));
	item1->setColor(Color3B::BLUE);

	auto menu = Menu::create(item1, nullptr);
	menu->setPosition(Vec2(1020, 64));
	menu->setTag(TAG_LABEL_BUILDSCENE);
	this->addChild(menu,100);

	
}
void HelloWorld::StartGame(Ref* pSender)
{
	auto menu = (Label*)this->getChildByTag(TAG_LABEL_BUILDSCENE);
	menu->setVisible(false);

	this->schedule(schedule_selector(HelloWorld::setEnemy1));   
	this->schedule(schedule_selector(HelloWorld::tick));
    
}
void HelloWorld::initBG()
{
	winSize = Director::getInstance()->getWinSize();
    // 타일맵
    map = TMXTiledMap::create("tile/tile.tmx");
	metainfo = map->getLayer("metainfo");
	metainfo->setVisible(false);
	this->addChild(map, 0, 11);
	
	
    // 건설 버튼	
	buildBtn = MenuItemImage::create("Images/88.png", "Images/88_1.png",
		CC_CALLBACK_1(HelloWorld::doClick1, this));
	buildBtn->setPosition(Vec2(1224, 0));
	buildBtn->setAnchorPoint(Vec2(1, 0));
	
	auto button = Menu::create(buildBtn, nullptr);
	button->setPosition(Vec2::ZERO);
	this->addChild(button);
	
    buildTouch = false;
}
void HelloWorld::initGameData()
{
    winSize = Director::getInstance()->getWinSize();

	lifeCount = 19;
	
	_bullet.clear();
	_enemy.clear();
	_tower.clear();

	pScore = 0;

}
bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
    auto touchpoint = touch->getLocation();

    Vec2 tilecoord = this->tileCoordForPosition(
    Vec2(touch->getLocation().x, touch->getLocation().y));
			
    if (buildTouch == true && pCount!=0)
    {	
        this->buildTower(Vec2(tilecoord.x, tilecoord.y));
        buildTouch = false;
		metainfo->setVisible(false);
    }	

    return true;
}

void HelloWorld::initTower()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAX_TOWER; i++)
	{
		pTData[i] = &tData[i];

		int num = rand_0_1() * 11;

		if (num ==0)
		{
			auto tower = Sprite::create("Images/tower/abomination.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/abomination.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 9; i++)
			{
				sprintf(str, "abomination%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.1f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		
		else if (num == 1)
		{
			auto tower = Sprite::create("Images/tower/kaa.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/kaa.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 12; i++)
			{
				sprintf(str, "kaa%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.2f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 2)
		{
			auto tower = Sprite::create("Images/tower/fouldog.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/fouldog.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 7; i++)
			{
				sprintf(str, "fouldog%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.3f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 3)
		{
			auto tower = Sprite::create("Images/tower/viper.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/viper.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 9; i++)
			{
				sprintf(str, "viper%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 1.4f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 4)
		{
			auto tower = Sprite::create("Images/tower/hulk.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/hulk.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 13; i++)
			{
				sprintf(str, "hulk%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.5f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 5)
		{
			auto tower = Sprite::create("Images/tower/watcher.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/watcher.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 12; i++)
			{
				sprintf(str, "watcher%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.6f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num ==6)
		{
			auto tower = Sprite::create("Images/tower/beast.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/beast.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 11; i++)
			{
				sprintf(str, "beast%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.7f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 7)
		{
			auto tower = Sprite::create("Images/tower/duriel.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/duriel.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 13; i++)
			{
				sprintf(str, "duriel%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.8f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 8)
		{
			auto tower = Sprite::create("Images/tower/andariel.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/andariel.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 17; i++)
			{
				sprintf(str, "andariel%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 0.9f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 9)
		{
			auto tower = Sprite::create("Images/tower/baal.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/baal.plist");
    
			char str[100] = { 0 };
			for (int i = 1; i < 13; i++)
			{
				sprintf(str, "baal%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 1.0f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
		else if (num == 10)
		{
			auto tower = Sprite::create("Images/tower/mephisto.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/mephisto.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 14; i++)
			{
				sprintf(str, "mephisto%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 1.1f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}		
		else 
		{
			auto tower = Sprite::create("Images/tower/diablo.png");
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/tower/diablo.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 13; i++)
			{
				sprintf(str, "diablo%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			tower->runAction(rep);
			tower->setPosition(Vec2(-3000, -3000));
			tower->setVisible(false);
			pTData[i]->isDetective = false;
			pTData[i]->delayFire = 3.0f;
			tower->setUserData(pTData[i]);
			tower->setScale(0.5f);
			_tower.pushBack(tower);
		}
	
	}	
}

void HelloWorld::buildTower(Vec2 location)
{    
	for (Sprite* pTower : _tower)
	{
		if (!pTower->isVisible())
		{
			pTower->setPosition(location.x, location.y);
			this->addChild(pTower);
			pTower->setVisible(true);
			break;
		}
	}	
	buildCount(1);
}


void HelloWorld::setBullet(Vec2 towerPos, Vec2 enemyPos, float angle)
{	
    for (Sprite* pBullet : _bullet)
    {
		if (!pBullet->isVisible())
        {
            pBullet->setVisible(true);
            pBullet->setPosition(towerPos);
		
            auto fire = Sequence::create(
                MoveTo::create((towerPos.distance(enemyPos))/300, enemyPos),
                CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetBullet, this)),
                nullptr);
		
            pBullet->runAction(fire);
            break;
        }  
    }
}

void HelloWorld::resetBullet(Ref * pSender)
{
    auto bullet = (Sprite*)pSender;
	bullet->stopAllActions();
	bullet->setVisible(false);
	bullet->setPosition(Vec2(-3000, -3000));
}

void HelloWorld::initBullet()
{
    for (int i = 0; i < MAX_BULLET; i++)
    {
		
		auto bullet = Sprite::create("Images/4.png");
		bullet->setVisible(false);
		bullet->setPosition(Vec2(-3000, -3000));
		
        _bullet.pushBack(bullet);
		this->addChild(bullet);
    }
}

void HelloWorld::initHPGage(Sprite *enemy)
{
	auto pWhite = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
	
	ProgressTimer *HP = ProgressTimer::create(pWhite);
	HP->setType(ProgressTimer::Type::BAR);
	HP->setMidpoint(Vec2::ZERO);
	HP->setBarChangeRate(Vec2(1, 0));
	HP->setPercentage(100.0f);
	HP->setColor(Color3B::GREEN);
	HP->setName("HPGage");
	enemy->addChild(HP,501);
}

void HelloWorld::initEnemy()
{
	
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		pEData[i] = &eData[i];
		
		int ran = rand_0_1() * 6;				

		if (ran == 0)
		{
			pEData[i]->hp = pEData[i]->maxhp = 10;
            pEData[i]->speed = 10.0f;
			auto enemy = Sprite::create("Images/enemy/ghost.png");
			enemy->setUserData(pEData[i]);;

			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 25, enemy->getPositionY() + 40));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 25, enemy->getPositionY() + 40));
			enemy->addChild(reHP,500);

			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/enemy/ghost.plist");
            
			char str[100] = { 0 };
			for (int i = 1; i < 10; i++)
			{
				sprintf(str, "ghost%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
            
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			enemy->runAction(rep);
            enemy->setPosition(Vec2(-3000, -3000));
            enemy->setVisible(false);
			this->addChild(enemy);
			_enemy.pushBack(enemy);
        }

		else if(ran==1)
		{
            auto enemy = Sprite::create("Images/enemy/ultralisk.png");
			pEData[i]->hp = pEData[i]->maxhp = 20;
            pEData[i]->speed = 10.0f;
			enemy->setUserData(pEData[i]);
			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 49, enemy->getPositionY() + 90));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 49, enemy->getPositionY() + 90));
			enemy->addChild(reHP, 500);

			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/enemy/ultralisk.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 12; i++)
			{
				sprintf(str, "ultralisk%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}
			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			enemy->runAction(rep);
            enemy->setPosition(Vec2(-3000, -3000));
            enemy->setVisible(false);
            this->addChild(enemy);

			_enemy.pushBack(enemy);
		}
		else if (ran == 2)
		{
			auto enemy = Sprite::create("Images/enemy/hydralisk.png");
		    pEData[i]->hp = pEData[i]->maxhp = 5;
			pEData[i]->speed = 10.0f;
		    enemy->setUserData(pEData[i]);
			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 21, enemy->getPositionY() + 50));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 21, enemy->getPositionY() + 50));
			enemy->addChild(reHP, 500);

		    Vector <SpriteFrame*> animFrames;
		    auto mob = SpriteFrameCache::getInstance();
		    mob->addSpriteFramesWithFile("Images/enemy/hydralisk.plist");
		
		    char str[100] = { 0 };
		    for (int i = 1; i < 13; i++)
		    {
		        sprintf(str, "hydralisk%d.png", i);
		        SpriteFrame* frame = mob->getSpriteFrameByName(str);
		        animFrames.pushBack(frame);
		    }
		
		    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		    auto animate = Animate::create(animation);
		    auto rep = RepeatForever::create(animate);
		    enemy->runAction(rep);
			enemy->setPosition(Vec2(-3000, -3000));
			enemy->setVisible(false);
			this->addChild(enemy);
		    _enemy.pushBack(enemy);
		}
			
		else if (ran ==3)
		{
			auto enemy = Sprite::create("Images/enemy/zergling.png");
		    pEData[i]->hp = pEData[i]->maxhp = 1;
			pEData[i]->speed = 10.0f;
		    enemy->setUserData(pEData[i]);
			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 20, enemy->getPositionY() + 40));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 20, enemy->getPositionY() + 40));
			enemy->addChild(reHP, 500);

		    Vector <SpriteFrame*> animFrames;
		    auto mob = SpriteFrameCache::getInstance();
		    mob->addSpriteFramesWithFile("Images/enemy/zergling.plist");
		
		    char str[100] = { 0 };
		    for (int i = 1; i < 13; i++)
		    {
		        sprintf(str, "zergling%d.png", i);
		        SpriteFrame* frame = mob->getSpriteFrameByName(str);
		        animFrames.pushBack(frame);
		    }
					  
		    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		    auto animate = Animate::create(animation);
		    auto rep = RepeatForever::create(animate);
		    enemy->runAction(rep);
			enemy->setPosition(Vec2(-3000, -3000));
			enemy->setVisible(false);
			this->addChild(enemy);
		    _enemy.pushBack(enemy);
		}
		
		else if (ran == 4)
		{
			auto enemy = Sprite::create("Images/enemy/zealot.png");
		    pEData[i]->hp = pEData[i]->maxhp = 4;
			pEData[i]->speed = 10.0f;
		    enemy->setUserData(pEData[i]);
			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 20, enemy->getPositionY() + 42));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 20, enemy->getPositionY() + 42));
			enemy->addChild(reHP, 500);

		    Vector <SpriteFrame*> animFrames;
		    auto mob = SpriteFrameCache::getInstance();
		    mob->addSpriteFramesWithFile("Images/enemy/zealot.plist");
		
		    char str[100] = { 0 };
		    for (int i = 1; i < 10; i++)
		    {
		        sprintf(str, "zealot%d.png", i);
		        SpriteFrame* frame = mob->getSpriteFrameByName(str);
		        animFrames.pushBack(frame);
		    }
		
		    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
		    auto animate = Animate::create(animation);
		    auto rep = RepeatForever::create(animate);
		    enemy->runAction(rep);
			enemy->setPosition(Vec2(-3000, -3000));
			enemy->setVisible(false);
			this->addChild(enemy);
		    _enemy.pushBack(enemy);
		}
		else if (ran == 5)
		{
			auto enemy = Sprite::create("Images/enemy/marine.png");
			pEData[i]->hp = pEData[i]->maxhp = 2;
			pEData[i]->speed = 20.0f;
			enemy->setUserData(pEData[i]);
			
			initHPGage(enemy);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			HP->setPosition(Vec2(enemy->getPositionX() + 11, enemy->getPositionY() + 40));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 11, enemy->getPositionY() + 40));
			enemy->addChild(reHP, 500);
	
			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/enemy/marine.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 10; i++)
			{
				sprintf(str, "marine%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
				animFrames.pushBack(frame);
			}

			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			enemy->runAction(rep);
			enemy->setPosition(Vec2(-3000, -3000));
			enemy->setVisible(false);
			this->addChild(enemy);
			_enemy.pushBack(enemy);
		}
		else if (ran == 6)
		{
			auto enemy = Sprite::create("Images/enemy/kerrigan.png");
			pEData[i]->hp = pEData[i]->maxhp = 100;
			pEData[i]->speed = 10.0f;
			enemy->setUserData(pEData[i]);
			ProgressTimer *HP = (ProgressTimer*)enemy->getChildByName("HPGage");
			initHPGage(enemy);
			HP->setPosition(Vec2(enemy->getPositionX() + 25, enemy->getPositionY() + 40));

			auto reHP = Sprite::create("Images/white-512x512.png", Rect(0, 0, 32, 5));
			reHP->setColor(Color3B::RED);
			reHP->setPosition(Vec2(enemy->getPositionX() + 25, enemy->getPositionY() + 40));
			enemy->addChild(reHP, 500);

			Vector <SpriteFrame*> animFrames;
			auto mob = SpriteFrameCache::getInstance();
			mob->addSpriteFramesWithFile("Images/enemy/kerrigan.plist");

			char str[100] = { 0 };
			for (int i = 1; i < 11; i++)
			{
				sprintf(str, "kerrigan%d.png", i);
				SpriteFrame* frame = mob->getSpriteFrameByName(str);
					animFrames.pushBack(frame);
			}

			auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
			auto animate = Animate::create(animation);
			auto rep = RepeatForever::create(animate);
			enemy->runAction(rep);
			enemy->setPosition(Vec2(-3000, -3000));
			enemy->setVisible(false);
			this->addChild(enemy);
			_enemy.pushBack(enemy);
		}
	}
}

void HelloWorld::setEnemy1(float dt)
{
	  int ar[3] = { 204,476,748 };

      static float delay_Spawn = 0;
       	
		  delay_Spawn += dt;
		
		  if (delay_Spawn >= 1.0f)
		  {
			delay_Spawn = 0;
			index_Enemy++;
			if (index_Enemy >= MAX_ENEMY)
			{
				index_Enemy = 0;
			}
			Sprite* pEnemy = _enemy.at(index_Enemy);
			pEnemy->setVisible(true);
			pEnemy->setPosition(ar[rand() % 3], winSize.height);

			EnemyData *pED = (EnemyData*)pEnemy->getUserData();

			// HP 회복
			ProgressTimer *pProgTM = (ProgressTimer*)pEnemy->getChildByName("HPGage");
			pProgTM->setPercentage(100);
			pED->hp = pED->maxhp;
			    
			auto moveby = MoveBy::create(pED->speed, Vec2(0, -winSize.height));
			moveby->setTag(ENEMY_ACTION_MOVE_TAG);
			auto callFunc = CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEnemy, this));
			callFunc->setTag(ENEMY_ACTION_FUNC_TAG);
			auto seq = Sequence::create(moveby, callFunc, nullptr);
			seq->setTag(ENEMY_ACTION_SEQ_TAG);

			pEnemy->runAction(seq);
		  }
}

void HelloWorld::resetEnemy(Ref * pSender)
{
    auto enemy = (Sprite*)pSender;

	enemy->setVisible(false);
	enemy->setPosition(Vec2(-3000, -3000));
	
    _life.at(lifeCount)->setVisible(false);
	lifeCount--;
		if (lifeCount<0)
		{
			lifeCount = 0;				

			initGameover();
		}
}

void HelloWorld::intersectMissile()
{
	for (Sprite* pBullet : _bullet)
	{
		Rect rectBullet = pBullet->getBoundingBox();
		if (pBullet->isVisible())
		{
			for (Sprite* pEnemy : _enemy)
			{
                Rect rectEnemy = pEnemy->getBoundingBox();
                EnemyData *ed = (EnemyData*)pEnemy->getUserData();
				if (pEnemy->isVisible())
				{	
					if (rectBullet.intersectsRect(rectEnemy))
					{		
						ed->hp--;

						float eHP = ed->hp * 100 / ed->maxhp;

						ProgressTimer *pProgTM = (ProgressTimer*)(pEnemy->getChildByName("HPGage"));

						pProgTM->setPercentage(eHP);
					
						if (ed->hp <= 0)
						{
							pEnemy->stopActionByTag(ENEMY_ACTION_MOVE_TAG);
							pEnemy->stopActionByTag(ENEMY_ACTION_FUNC_TAG);
							pEnemy->stopActionByTag(ENEMY_ACTION_SEQ_TAG);
							

							pEnemy->setVisible(false);
														
							Score(10);

                            auto effect = Sprite::create("Images/effect.png");
                            Vector <SpriteFrame*> animFrames;
                            auto mob = SpriteFrameCache::getInstance();
                            mob->addSpriteFramesWithFile("Images/effect.plist");

                            char str[100] = { 0 };
                            for (int i = 1; i < 5; i++)
                            {
                                sprintf(str, "effect%d.png", i);
                                SpriteFrame* frame = mob->getSpriteFrameByName(str);
                                animFrames.pushBack(frame);
                            }
                            auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
                            auto animate = Animate::create(animation);                               
							effect->runAction(Sequence::create(animate, DelayTime::create(0.01f),
                                CallFuncN::create(CC_CALLBACK_1(HelloWorld::resetEffect,this)),nullptr));
							effect->setPosition((pEnemy->getPosition()));							
							effect->setVisible(true);
							effect->setScale(0.7f);
							pEnemy->setPosition(Vec2(-3000, -3000));
                            this->addChild(effect);
						}
						
						resetBullet(pBullet);
						pEnemy->setUserData(ed);        
						
					}
				}				
			}
		}
	}
}

void HelloWorld::initUI()
{
	
	TTFConfig ttfconfig("fonts/K_Damage.ttf", 35.0f);
	ttfconfig.outlineSize = 3;
	ttfconfig.bold = true;

	auto score = Label::createWithTTF(ttfconfig, " ");
	score->setPosition(Vec2(winSize.width-220, winSize.height));
	score->setAnchorPoint(Vec2(0, 1));
	score->enableOutline(Color4B::BLACK);
	score->setTag(999);
	this->addChild(score,50);

	auto buildcount = Label::createWithTTF(ttfconfig, " ");
	buildcount->setPosition(Vec2(winSize.width-220, winSize.height - 128));
	buildcount->setAnchorPoint(Vec2(0, 1));
	buildcount->enableOutline(Color4B::BLACK);
	buildcount->setTag(998);
	this->addChild(buildcount, 50);

    auto life = Label::createWithTTF(ttfconfig, "LIFE :");
    life->setPosition(Vec2(winSize.width - 220, winSize.height - 256));
    life->setAnchorPoint(Vec2(0, 1));
    life->enableOutline(Color4B::BLACK);
	this->addChild(life, 50);
    
	Score(0);
	buildCount(-9);
	Life();
}

void HelloWorld::Score(int score)
{
	pScore += score;

	auto scoreResult = (Label*)this->getChildByTag(999);
	scoreResult->setString(StringUtils::format("SCORE : \n     %d", pScore));

	if (pScore % 100 == 0)
	{
		pCount++;

		auto buildResult = (Label*)this->getChildByTag(998);
		buildResult->setString(StringUtils::format("COUNT : \n     %d", pCount));
	}
	
}

void HelloWorld::buildCount(int count)
{
	pCount -= count;

	auto buildResult = (Label*)this->getChildByTag(998);
	buildResult->setString(StringUtils::format("COUNT : \n     %d", pCount));
	
}

void HelloWorld::Life()
{
	for (int i = 0; i < 20; i++)
	{
		int index = i % 5;
		int rowIndex = i / 5;
		auto life = Sprite::create("Images/56.png");
		life->setPosition(Vec2(winSize.width - 180 + index*30, winSize.height - 320 -rowIndex*30));
		life->setScale(0.5f);
		_life.pushBack(life);
        this->addChild(life, 50);
    }
}

void HelloWorld::initGameover()
{
	winSize = Director::getInstance()->getWinSize();
	Director::getInstance()->pause();

	auto end = LayerColor::create(Color4B::WHITE);
	end->setOpacity(50);
	end->setAnchorPoint(Vec2(0, 0));
	this->addChild(end);
	
	setGameover();
}

void HelloWorld::setGameover()
{
	TTFConfig ttfconfig("fonts/K_Damage.ttf", 100.0f);
	ttfconfig.outlineSize = 3;
	ttfconfig.bold = true;
		
	auto label1 = Label::createWithTTF(ttfconfig, "TITLE");
	auto item1 = MenuItemLabel::create(label1,
		CC_CALLBACK_1(HelloWorld::title, this));
	item1->setColor(Color3B::WHITE);
    
	auto label2 = Label::createWithTTF(ttfconfig, "EXIT");
	auto item2 = MenuItemLabel::create(label2,
		CC_CALLBACK_1(HelloWorld::Exit, this));
	item2->setColor(Color3B::WHITE);

	auto menu = Menu::create(item1, item2, nullptr);
	menu->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
	menu->alignItemsVerticallyWithPadding(50);
	this->addChild(menu,205);
}

void HelloWorld::resetEffect(Ref * pSender)
{   
    auto effect = (Sprite*)pSender;
	effect->setVisible(false);
	effect->setPosition(-3000, -3000);
}

void HelloWorld::doClose(Ref * pSender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(scene);
}

bool HelloWorld::doClick1(Ref* pSender)
{
    buildTouch = true;
	if (pCount != 0)
	{
		metainfo->setVisible(true);
	}
	if (pCount == 0)
	{
		metainfo->setVisible(false);
	}
    	
    return buildTouch;
}

void HelloWorld::title(Ref * pSender)
{
	Director::getInstance()->popScene();
    Director::getInstance()->resume();
}

void HelloWorld::Exit(Ref * pSender)
{
	exit(0);
}

Vec2 HelloWorld::tileCoordForPosition(Vec2 position)
{
    int touchX = position.x / map->getTileSize().width;
    int touchY = position.y / map->getTileSize().height;

    int x = (map->getTileSize().width * touchX) + (map->getTileSize().width / 2);
    int y = (map->getTileSize().height * touchY) + (map->getTileSize().height / 2);
	
    return Vec2(x, y);
}
