#pragma once
#ifndef __SceneTrans__HelloWorld__
#define __SceneTrans__HelloWorld__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif

USING_NS_CC;

#define TAG_LABEL_BUILDSCENE		10
#define MAX_ENEMY       300
#define MAX_TOWER		50
#define MAX_BULLET      100
#define MAX_EFFECT		50
#define MAX_HP			100
#define ENEMY_ACTION_MOVE_TAG	2500
#define ENEMY_ACTION_FUNC_TAG	2501
#define ENEMY_ACTION_SEQ_TAG	2502


struct EnemyData
{
	int hp;
	int maxhp;
	int speed;
};

struct TowerData
{
	
	bool isDetective = false;
	Sprite* dEnemy = nullptr;
	float delayFire;
};


class HelloWorld :public Scene
{
public:
    static Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

    Size winSize;

    TMXTiledMap* map;
	TMXLayer* Layer;
    TMXLayer* metainfo;

    MenuItem* buildBtn;    
    Touch* touchpoint;
	

    EnemyData *pEData[MAX_ENEMY];
    EnemyData eData[MAX_ENEMY];

	TowerData *pTData[MAX_TOWER];
	TowerData tData[MAX_TOWER];
 
	int index_Enemy;

	int lifeCount;

    void tick(float dt);

	void initBuildScene();
	void StartGame(Ref* pSender);

    void initBG();      // 배경 타일맵
    void initGameData();        // 게임 데이터 초기화
    bool onTouchBegan(Touch *touch, Event *event);

    void initTower();       // 타워 만들기  

    void buildTower(Vec2 location);       // 타워 설치 
    Vector<Sprite*> _tower;

    void setBullet(Vec2 towerPos, Vec2 enemyPos, float angle);      // 타워 앞에 이미지 발사 구현
    void resetBullet(Ref *pSender);        // 미사일 제거 구현
    Vector<Sprite*> _bullet;       // 미사일 저장할 벡터

    void initBullet();

	void initHPGage(Sprite *enemy);
	void initEnemy();
    void setEnemy1(float dt);   // 적 구현
    void resetEnemy(Ref* pSender);  // 적 제거 구현
    Vector<Sprite*> _enemy;    // 적의 hp를 저장할 벡터
		

	void intersectMissile();

	void initUI();
	void Score(int score);
	int pScore;

	void buildCount(int count);
	int pCount;

    void Life();	
    int pLife;
	Vector<Sprite*> _life;

    void initGameover();        // 게임오버 타이틀
    void setGameover();         // 게임오버 타이틀 작동 구현
    void resetEffect(Ref*pSender);

	   
	bool onBuildTouch = true;
    bool buildTouch;   


    EventListenerTouchOneByOne* listener;

    void doClose(Ref *pSender);    
    bool doClick1(Ref* pSender);

	void title(Ref* pSender);	
	void Exit(Ref* pSender);

	
    Vec2 tileCoordForPosition(Vec2 position);
  
};
#endif