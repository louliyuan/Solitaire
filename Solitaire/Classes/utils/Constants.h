#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

/**
 * @file Constants.h
 * @brief 定义游戏中使用的常量和枚举类型
 * @details 包含卡牌花色、点数等基础枚举定义
 */

/**
 * @enum CardSuit
 * @brief 卡牌花色类型枚举
 */
enum class CardSuit
{
    NONE = -1,      // 无花色
    CLUBS = 0,      // 梅花 ♣
    DIAMONDS = 1,   // 方块 ♦
    HEARTS = 2,     // 红桃 ♥
    SPADES = 3,     // 黑桃 ♠
    NUM_CARD_SUIT_TYPES
};

/**
 * @enum CardFace
 * @brief 卡牌点数类型枚举
 */
enum class CardFace
{
    NONE = -1,      // 无点数
    ACE = 1,        // A
    TWO = 2,        // 2
    THREE = 3,      // 3
    FOUR = 4,       // 4
    FIVE = 5,       // 5
    SIX = 6,        // 6
    SEVEN = 7,      // 7
    EIGHT = 8,      // 8
    NINE = 9,       // 9
    TEN = 10,       // 10
    JACK = 11,      // J
    QUEEN = 12,     // Q
    KING = 13,      // K
    NUM_CARD_FACE_TYPES
};

// 游戏配置常量
namespace GameConstants
{
    // 设计分辨率
    const float kDesignWidth = 1080.0f;
    const float kDesignHeight = 2080.0f;
    
    // 区域尺寸
    const float kPlayFieldHeight = 1500.0f;
    const float kStackAreaHeight = 580.0f;
    
    // 卡牌尺寸
    const float kCardWidth = 140.0f;
    const float kCardHeight = 190.0f;
    
    // 动画时间
    const float kCardMoveTime = 0.3f;
    const float kCardFlipTime = 0.2f;
}

#endif // __CONSTANTS_H__
