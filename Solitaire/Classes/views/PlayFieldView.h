#ifndef __PLAY_FIELD_VIEW_H__
#define __PLAY_FIELD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <functional>
#include <map>

/**
 * @class PlayFieldView
 * @brief 桌面牌区域视图
 * @details 管理所有桌面卡牌的显示
 *          通过回调接口与Controller交互
 */
class PlayFieldView : public cocos2d::Layer
{
public:
    /**
     * @brief 创建桌面视图
     * @return 视图实例
     */
    static PlayFieldView* create();
    
    /**
     * @brief 初始化
     */
    virtual bool init() override;
    
    /**
     * @brief 添加卡牌视图
     * @param cardView 卡牌视图
     */
    void addCardView(CardView* cardView);
    
    /**
     * @brief 移除卡牌视图
     * @param cardId 卡牌ID
     */
    void removeCardView(int cardId);
    
    /**
     * @brief 根据ID获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图，未找到返回nullptr
     */
    CardView* getCardViewById(int cardId);
    
    /**
     * @brief 清空所有卡牌
     */
    void clearAllCards();
    
    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数，参数为卡牌ID
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);
    
private:
    PlayFieldView();
    virtual ~PlayFieldView();
    
    /**
     * @brief 处理卡牌点击
     * @param cardView 被点击的卡牌视图
     */
    void onCardClicked(CardView* cardView);
    
private:
    std::map<int, CardView*> _cardViews;  // 卡牌ID到视图的映射
    std::function<void(int)> _onCardClickCallback;  // 卡牌点击回调
};

#endif // __PLAY_FIELD_VIEW_H__
