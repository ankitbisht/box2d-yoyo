#include "HelloWorldScene.h"
#define PTM_RATIO 32.0f
#define FLOOR_HEIGHT    62.0f 
using namespace cocos2d;
using namespace CocosDenshion;
HelloWorld::HelloWorld()
{

}
HelloWorld::~HelloWorld(){
	
}
CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label shows "Hello World".

        // Create a label and initialize with string "Hello World".
        CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        // Get window size and place the label upper. 
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        pLabel->setPosition(ccp(size.width / 2, size.height - 50));

        // Add the label to HelloWorld layer as a child layer.
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("HelloWorld.png");
        CC_BREAK_IF(! pSprite);

        // Place the sprite on the center of the screen
        pSprite->setPosition(ccp(size.width/2, size.height/2));

        // Add the sprite to HelloWorld layer as a child layer.
        //this->addChild(pSprite, 0);

		///////////////////////////////////////////
		this->setTouchEnabled(true);
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		_ball = CCSprite::create("acorn.png");
		_ball->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(_ball);

		///////////////////////
		b2Vec2 gravity; 
		gravity.Set(0.0f, -10.0f); 
		bool doSleep = true; 
		m_world = new b2World(gravity); 
		m_world->SetAllowSleeping(doSleep); 
		m_world->SetContinuousPhysics(true); 
 
		// Define the ground body. 
		b2BodyDef groundBodyDef; 
		groundBodyDef.position.Set(0, 0); // bottom-left corner 
 
		// Call the body factory which allocates memory for the ground body 
		// from a pool and creates the ground box shape (also from a pool). 
		// The body is also added to the world. 
		m_groundBody = m_world->CreateBody(&groundBodyDef); 

		b2EdgeShape groundBox; 
		// bottom 
		groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO)); 
		groundBox.Set(b2Vec2(0,0), b2Vec2(screenSize.width*2.0f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox, 0); 
     
		// top 
		//groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO)); 
		groundBox.Set(b2Vec2(0,(0+0.0)), b2Vec2(screenSize.width*2.0f/PTM_RATIO,(0+0.0)));
		m_groundBody->CreateFixture(&groundBox, 0); 
     
		// left 
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(0,0)); 
		//groundBox.Set(b2Vec2(0,0), b2Vec2(0,0)); 
		m_groundBody->CreateFixture(&groundBox, 0); 
     
		// right 
		groundBox.Set(b2Vec2(screenSize.width/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width/PTM_RATIO,0)); 
		//groundBox.Set(b2Vec2(screenSize.width*1.5f/PTM_RATIO,0), b2Vec2(screenSize.width*1.5f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox, 0); 

		/////////////////////
		//创建周围世界边框，在屏幕的周围一圈  
		b2BodyDef ballBodyDef;  
		ballBodyDef.type = b2_dynamicBody;  
		ballBodyDef.position.Set(100/PTM_RATIO, 300/PTM_RATIO);  
		ballBodyDef.userData = _ball;  
		_body = m_world->CreateBody(&ballBodyDef);  
      
		b2CircleShape circle;  
		circle.m_radius = 15.0/PTM_RATIO;  
      
		b2FixtureDef ballShapeDef;
		ballShapeDef.shape = &circle;
		ballShapeDef.density = 1.0f;
		ballShapeDef.friction = 0.2f;
		ballShapeDef.restitution = 0.8f;  
		_body->CreateFixture(&ballShapeDef);
      
		schedule(schedule_selector(HelloWorld::tick)); 

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::tick(float dt)  
{  
    m_world->Step(dt, 10, 10);  
    for(b2Body *b = m_world->GetBodyList(); b; b=b->GetNext()) {   
        if (b->GetUserData() != NULL) {  
            CCSprite *ballData = (CCSprite *)b->GetUserData();  
            ballData->setPosition(ccp(b->GetPosition().x * PTM_RATIO,  
                                    b->GetPosition().y * PTM_RATIO));  
            ballData->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));  
        }   
    }  
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
		CCTouch* touch = (CCTouch*)( touches->anyObject() );
		CCPoint location = touch->getLocation();
		
		//创建周围世界边框，在屏幕的周围一圈  
		CCSprite *temp_ball = CCSprite::create("acorn.png");
		temp_ball->setPosition(location);
		this->addChild(temp_ball);

		
		b2Body *temp_body;

		b2BodyDef ballBodyDef;  
		ballBodyDef.type = b2_dynamicBody;  
		ballBodyDef.position.Set(location.x/PTM_RATIO, location.y/PTM_RATIO);  
		ballBodyDef.userData = temp_ball;  
		temp_body = m_world->CreateBody(&ballBodyDef);  
      
		b2CircleShape circle;  
		circle.m_radius = 14.0/PTM_RATIO;
      
		b2FixtureDef ballShapeDef;  
		ballShapeDef.shape = &circle;  
		ballShapeDef.density = 1.0f;  
		ballShapeDef.friction = 0.2f;  
		ballShapeDef.restitution = 0.8f;  
		temp_body->CreateFixture(&ballShapeDef);
		
		
		CCLog("touch x %f, %f", location.x,location.y);
}
