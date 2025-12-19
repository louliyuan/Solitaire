#ifndef __GAME_LOGIC_SERVICE_H__
#define __GAME_LOGIC_SERVICE_H__

#include "../models/CardModel.h"
#include "../models/GameModel.h"

/**
 * @class GameLogicService
 * @brief 游戏逻辑服务类
 * @details 提供无状态的游戏规则判定服务，不持有数据
 *          所有方法都是静态的，可直接调用
 */
class GameLogicService
{
public:
    /**
     * @brief 判断两张牌是否可以匹配
     * @param card1 第一张牌
     * @param card2 第二张牌
     * @return 是否可以匹配
     * @details 匹配规则：点数相差1（例如：7和8，8和7，K和Q等）
     *          A(1)和2可以匹配，K(13)和Q(12)可以匹配
     */
    static bool canMatch(const CardModel& card1, const CardModel& card2);
    
    /**
     * @brief 判断两张牌的点数是否可以匹配
     * @param face1 第一张牌的点数
     * @param face2 第二张牌的点数
     * @return 是否可以匹配
     */
    static bool canMatchFaces(CardFace face1, CardFace face2);
    
    /**
     * @brief 判断桌面的牌是否可以被点击
     * @param card 要判断的牌
     * @param gameModel 游戏数据模型
     * @return 是否可以点击
     * @details 可点击条件：
     *          1. 牌必须是翻开状态(isFaceUp == true)
     *          2. 牌没有被其他牌遮挡(isBlocked == false)
     */
    static bool canCardBeClicked(const CardModel& card);
    
    /**
     * @brief 更新桌面牌的遮挡状态
     * @param gameModel 游戏数据模型
     * @details 检查所有桌面牌，判断哪些牌被遮挡
     *          遮挡规则：如果一张牌的位置与另一张牌重叠，且层级更低，则被遮挡
     */
    static void updateCardBlockingStates(GameModel* gameModel);
    
    /**
     * @brief 判断两张牌是否存在位置重叠（用于判断遮挡）
     * @param card1 第一张牌
     * @param card2 第二张牌
     * @return 是否重叠
     */
    static bool isCardOverlapping(const CardModel& card1, const CardModel& card2);
    
    /**
     * @brief 检查游戏是否胜利
     * @param gameModel 游戏数据模型
     * @return 是否胜利
     * @details 胜利条件：桌面所有牌都被消除
     */
    static bool checkWin(const GameModel* gameModel);
    
    /**
     * @brief 检查游戏是否失败
     * @param gameModel 游戏数据模型
     * @return 是否失败
     * @details 失败条件：桌面还有牌，但没有可匹配的牌，且牌堆已空
     */
    static bool checkLose(const GameModel* gameModel);
};

#endif // __GAME_LOGIC_SERVICE_H__
