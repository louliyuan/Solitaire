#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "CardModel.h"

/**
 * @enum UndoActionType
 * @brief 回退操作类型枚举
 */
enum class UndoActionType
{
    NONE,                           // 无操作
    MOVE_PLAYFIELD_TO_TRAY,        // 从桌面移动到底牌
    DRAW_FROM_STACK                 // 从牌堆抽牌
};

/**
 * @struct UndoCommand
 * @brief 回退命令结构体
 * @details 记录一次操作的所有信息，用于撤销操作
 */
struct UndoCommand
{
    UndoActionType actionType;      // 操作类型
    int cardId;                     // 被操作的卡牌ID
    CardModel prevTrayCard;         // 操作前的底牌（用于恢复）
    cocos2d::Vec2 prevPosition;     // 操作前的位置
    bool prevIsFaceUp;              // 操作前的翻面状态
    int prevZOrder;                 // 操作前的层级
    
    // 用于保存需要重新翻开的牌
    std::vector<int> flippedCardIds;  // 此次操作导致翻开的卡牌ID列表
    
    /**
     * @brief 构造函数
     */
    UndoCommand()
        : actionType(UndoActionType::NONE)
        , cardId(-1)
        , prevPosition(cocos2d::Vec2::ZERO)
        , prevIsFaceUp(false)
        , prevZOrder(0)
    {
    }
};

#endif // __UNDO_MODEL_H__
