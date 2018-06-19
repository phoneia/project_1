#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class gamescene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(gamescene);

	Size winSize;

    float sound;
    int audio;
      

    void StartScene(Ref *pSender);
    void OptionScene(Ref *pSender);
	void ExitScene(Ref* pSender);
    
    
  


};
#endif