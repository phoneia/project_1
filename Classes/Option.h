#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class OptionScene :public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(OptionScene);

    Size winSize;

    void returnTitle(Ref* pSender);

    float sound;
    int audio;

    void upVolume(Ref *pSender);
    void downVolume(Ref *pSender);



};
#endif