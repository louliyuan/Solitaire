#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "cocos2d.h"

// 前向声明
class GameController;

/**
 * @class StackController
 * @brief 牌堆控制器
 * @details 处理牌堆抽牌逻辑
 */
class StackController : public cocos2d::Ref
{
public:
    /**
     * @brief 创建控制器
     * @param gameController 主控制器
     * @return 控制器实例
     */
    static StackController* create(GameController* gameController);
    
    /**
     * @brief 初始化
     * @param gameController 主控制器
     */
    bool init(GameController* gameController);
    
    /**
     * @brief 初始化视图
     */
    void initView();
    
    /**
     * @brief 处理牌堆点击（抽牌）
     */
    void handleStackClick();
    
    /**
     * @brief 刷新视图
     */
    void refreshView();
    
private:
    StackController();
    virtual ~StackController();
    
    /**
     * @brief 从牌堆抽牌
     */
    void drawCardFromStack();
    
private:
    GameController* _gameController;  // 主控制器（const类型，不修改）
};

#endif // __STACK_CONTROLLER_H__
