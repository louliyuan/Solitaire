#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"

// 前向声明
class PlayFieldController;
class StackController;

/**
 * @class GameController
 * @brief 游戏主控制器
 * @details 管理整个游戏流程，协调各子Controller
 *          持有GameModel和各Manager
 */
class GameController : public cocos2d::Ref
{
public:
    /**
     * @brief 创建游戏控制器
     * @return 控制器实例
     */
    static GameController* create();
    
    /**
     * @brief 初始化
     */
    bool init();
    
    /**
     * @brief 开始游戏
     * @param levelId 关卡ID
     * @return 是否成功开始
     */
    bool startGame(int levelId);
    
    /**
     * @brief 处理回退操作
     */
    void handleUndo();
    
    /**
     * @brief 获取游戏视图
     */
    GameView* getGameView() { return _gameView; }
    
    /**
     * @brief 获取游戏模型
     */
    GameModel* getGameModel() { return _gameModel; }
    
    /**
     * @brief 获取Undo管理器
     */
    UndoManager* getUndoManager() { return _undoManager; }
    
private:
    GameController();
    virtual ~GameController();
    
    /**
     * @brief 初始化视图
     */
    void initView();
    
    /**
     * @brief 初始化子控制器
     */
    void initSubControllers();
    
    /**
     * @brief 刷新视图显示
     */
    void refreshView();
    
    /**
     * @brief 检查游戏状态（胜利/失败）
     */
    void checkGameState();
    
private:
    GameModel* _gameModel;                      // 游戏数据模型
    GameView* _gameView;                        // 游戏视图
    UndoManager* _undoManager;                  // 回退管理器
    
    PlayFieldController* _playFieldController;  // 桌面控制器
    StackController* _stackController;          // 牌堆控制器
};

#endif // __GAME_CONTROLLER_H__
