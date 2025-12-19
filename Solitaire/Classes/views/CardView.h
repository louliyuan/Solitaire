#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"

/**
 * @class CardView
 * @brief 单张卡牌的视图类
 * @details 负责单张卡牌的渲染和简单动画
 *          不包含业务逻辑，只负责显示
 */
class CardView : public cocos2d::Node
{
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型
     * @return 卡牌视图实例
     */
    static CardView* create(const CardModel& cardModel);
    
    /**
     * @brief 初始化
     * @param cardModel 卡牌数据模型
     * @return 是否初始化成功
     */
    bool init(const CardModel& cardModel);
    
    /**
     * @brief 更新卡牌显示
     * @param cardModel 卡牌数据模型
     */
    void updateWithModel(const CardModel& cardModel);
    
    /**
     * @brief 播放翻牌动画
     * @param isFaceUp 是否翻到正面
     * @param callback 动画结束回调
     */
    void playFlipAnimation(bool isFaceUp, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放移动动画
     * @param targetPos 目标位置
     * @param callback 动画结束回调
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPos, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 获取卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 设置是否可点击
     */
    void setClickable(bool clickable);
    
private:
    CardView();
    virtual ~CardView();
    
    /**
     * @brief 创建卡牌精灵
     */
    void createCardSprite(const CardModel& cardModel);
    
    /**
     * @brief 获取卡牌正面图片路径
     */
    std::string getFrontImagePath(CardFace face, CardSuit suit);
    
    /**
     * @brief 获取卡牌背面图片路径
     */
    std::string getBackImagePath();
    
private:
    int _cardId;                        // 卡牌ID
    cocos2d::Sprite* _cardSprite;       // 卡牌精灵
    cocos2d::Sprite* _frontSprite;      // 正面精灵
    cocos2d::Sprite* _backSprite;       // 背面精灵
    bool _isFaceUp;                     // 是否正面朝上
};

#endif // __CARD_VIEW_H__
