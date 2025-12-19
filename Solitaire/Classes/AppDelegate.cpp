#include "AppDelegate.h"
#include "controllers/GameController.h"
#include "utils/Constants.h"

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化Director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview)
    {
        // 创建窗口
        glview = GLViewImpl::createWithRect("SolitaireMatch", 
                                            cocos2d::Rect(0, 0, 1080, 2080), 
                                            0.5);
        director->setOpenGLView(glview);
    }

    // 设置设计分辨率
    glview->setDesignResolutionSize(GameConstants::kDesignWidth, 
                                     GameConstants::kDesignHeight, 
                                     ResolutionPolicy::FIXED_WIDTH);

    // 设置帧率
    director->setAnimationInterval(1.0f / 60);

    // 创建游戏控制器并启动游戏
    auto gameController = GameController::create();
    gameController->retain();
    
    if (gameController->startGame(1))
    {
        // 切换到游戏场景
        auto scene = gameController->getGameView();
        director->runWithScene(scene);
    }
    else
    {
        CCLOG("Failed to start game");
        return false;
    }

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
