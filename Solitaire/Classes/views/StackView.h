#ifndef __STACK_VIEW_H__
#define __STACK_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>

/**
 * @class StackView
 * @brief 牌堆区域视图
 * @details 显示备用牌堆和当前底牌
 *          通过回调接口与Controller交互
 */
class StackView : public cocos2d::Layer
{
public:
    /**
     * @brief 创建牌堆视图
     * @return 视图实例
     */
    static StackView* create();
    
    /**
     * @brief 初始化
     */
    virtual bool init() override;
    
    /**
     * @brief 设置当前底牌
     * @param cardView 底牌视图
     */
    void setTrayCard(CardView* cardView);
    
    /**
     * @brief 获取当前底牌视图
     */
    CardView* getTrayCard() { return _trayCardView; }
    
    /**
     * @brief 更新牌堆显示（剩余数量）
     * @param count 剩余牌数
     */
    void updateStackCount(int count);
    
    /**
     * @brief 设置牌堆点击回调
     * @param callback 回调函数
     */
    void setOnStackClickCallback(const std::function<void()>& callback);
    
    /**
     * @brief 获取底牌位置
     */
    cocos2d::Vec2 getTrayCardPosition() const;
    
    /**
     * @brief 获取牌堆位置
     */
    cocos2d::Vec2 getStackPosition() const;
    
private:
    StackView();
    virtual ~StackView();
    
    /**
     * @brief 创建UI
     */
    void createUI();
    
private:
    CardView* _trayCardView;                        // 当前底牌视图
    cocos2d::Sprite* _stackSprite;                  // 牌堆精灵
    cocos2d::Label* _stackCountLabel;               // 牌堆数量标签
    cocos2d::ui::Button* _stackButton;              // 牌堆按钮
    std::function<void()> _onStackClickCallback;    // 牌堆点击回调
    
    cocos2d::Vec2 _trayPosition;                    // 底牌位置
    cocos2d::Vec2 _stackPosition;                   // 牌堆位置
};

#endif // __STACK_VIEW_H__
