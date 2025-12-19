#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../utils/Constants.h"

/**
 * @class CardModel
 * @brief 单张卡牌的数据模型
 * @details 存储卡牌的所有状态信息，包括花色、点数、位置、翻面状态等
 *          不包含业务逻辑，仅作为数据容器
 */
class CardModel
{
public:
    /**
     * @brief 构造函数
     */
    CardModel();
    
    /**
     * @brief 带参数的构造函数
     * @param id 卡牌唯一标识符
     * @param face 卡牌点数
     * @param suit 卡牌花色
     */
    CardModel(int id, CardFace face, CardSuit suit);
    
    /**
     * @brief 析构函数
     */
    ~CardModel();
    
    // Getters
    int getId() const { return _id; }
    CardFace getFace() const { return _face; }
    CardSuit getSuit() const { return _suit; }
    cocos2d::Vec2 getPosition() const { return _position; }
    bool getIsFaceUp() const { return _isFaceUp; }
    int getZOrder() const { return _zOrder; }
    bool getIsBlocked() const { return _isBlocked; }
    
    // Setters
    void setId(int id) { _id = id; }
    void setFace(CardFace face) { _face = face; }
    void setSuit(CardSuit suit) { _suit = suit; }
    void setPosition(const cocos2d::Vec2& pos) { _position = pos; }
    void setIsFaceUp(bool isFaceUp) { _isFaceUp = isFaceUp; }
    void setZOrder(int zOrder) { _zOrder = zOrder; }
    void setIsBlocked(bool isBlocked) { _isBlocked = isBlocked; }
    
    /**
     * @brief 复制卡牌数据
     * @param other 要复制的卡牌模型
     */
    void copyFrom(const CardModel& other);
    
private:
    int _id;                    // 唯一标识符
    CardFace _face;             // 点数
    CardSuit _suit;             // 花色
    cocos2d::Vec2 _position;    // 坐标
    bool _isFaceUp;             // 是否翻开
    int _zOrder;                // 渲染层级
    bool _isBlocked;            // 是否被遮挡（不可点击）
};

#endif // __CARD_MODEL_H__
