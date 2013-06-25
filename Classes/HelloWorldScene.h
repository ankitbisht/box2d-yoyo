#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
	HelloWorld();
	~HelloWorld();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    //void mInit(); 
	
    //b2World *_world;  
    b2Body *_body;  
    cocos2d::CCSprite *_ball;
	b2World* m_world; 
    b2Body* m_groundBody; 
    void tick(float dt);  

    //virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event); 
   // virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event); 
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event); 
	//void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__