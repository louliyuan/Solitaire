#ifndef __PLAY_FIELD_CONTROLLER_H__
#define __PLAY_FIELD_CONTROLLER_H__

#include "cocos2d.h"

// 前向声明
class GameController;
class GameModel;
class PlayFieldView;
class UndoManager;

/**
 * @class PlayFieldController
 * @brief 桌面控制器
 * @details 处理桌面牌的点击逻辑和业务
 */
class PlayFieldController : public cocos2d::Ref
{
public:
    /**
     * @brief 创建控制器
     * @param gameController 主控制器
     * @return 控制器实例
     */
    static PlayFieldController* create(GameController* gameController);
    
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
     * @brief 处理卡牌点击
     * @param cardId 被点击的卡牌ID
     */
    void handleCardClick(int cardId);
    
    /**
     * @brief 刷新视图
     */
    void refreshView();
    
private:
    PlayFieldController();
    virtual ~PlayFieldController();
    
    /**
     * @brief 将桌面牌移动到底牌
     * @param cardId 卡牌ID
     */
    void replaceTrayWithPlayFieldCard(int cardId);
    
    /**
     * @brief 更新卡牌遮挡状态和翻面状态
     */
    void updateCardStates();
    
private:
    GameController* _gameController;  // 主控制器（const类型，不修改）
};

#endif // __PLAY_FIELD_CONTROLLER_H__
