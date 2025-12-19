#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"
#include <functional>

/**
 * @class GameView
 * @brief 游戏主场景视图
 * @details 管理整个游戏界面，包括桌面区、牌堆区等
 *          通过回调接口与GameController交互
 */
class GameView : public cocos2d::Scene
{
public:
    /**
     * @brief 创建游戏视图
     * @return 视图实例
     */
    static GameView* create();
    
    /**
     * @brief 初始化
     */
    virtual bool init() override;
    
    /**
     * @brief 获取桌面视图
     */
    PlayFieldView* getPlayFieldView() { return _playFieldView; }
    
    /**
     * @brief 获取牌堆视图
     */
    StackView* getStackView() { return _stackView; }
    
    /**
     * @brief 设置回退按钮点击回调
     * @param callback 回调函数
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);
    
    /**
     * @brief 播放匹配动画
     * @param cardId 卡牌ID
     * @param callback 动画结束回调
     */
    void playMatchAnimation(int cardId, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 更新回退按钮状态
     * @param canUndo 是否可以回退
     */
    void updateUndoButton(bool canUndo);
    
    /**
     * @brief 显示胜利提示
     */
    void showWinMessage();
    
    /**
     * @brief 显示失败提示
     */
    void showLoseMessage();
    
private:
    GameView();
    virtual ~GameView();
    
    /**
     * @brief 创建UI
     */
    void createUI();
    
private:
    PlayFieldView* _playFieldView;                  // 桌面视图
    StackView* _stackView;                          // 牌堆视图
    cocos2d::ui::Button* _undoButton;               // 回退按钮
    std::function<void()> _onUndoClickCallback;     // 回退按钮回调
};

#endif // __GAME_VIEW_H__
