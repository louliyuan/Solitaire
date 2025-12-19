#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * @brief 应用程序委托类
 * @details 程序入口，负责初始化和启动游戏
 */
class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
     * @brief 应用程序启动时调用
     * @return 是否启动成功
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief 应用程序进入后台时调用
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief 应用程序回到前台时调用
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_
