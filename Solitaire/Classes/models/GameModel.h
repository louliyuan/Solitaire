#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <vector>
#include "CardModel.h"

/**
 * @class GameModel
 * @brief 整个游戏的数据模型
 * @details 存储当前游戏局面的所有数据，包括桌面牌、牌堆、当前底牌等
 *          支持序列化和反序列化（用于存档恢复）
 */
class GameModel
{
public:
    /**
     * @brief 构造函数
     */
    GameModel();
    
    /**
     * @brief 析构函数
     */
    ~GameModel();
    
    // Getters
    std::vector<CardModel>& getPlayFieldCards() { return _playFieldCards; }
    const std::vector<CardModel>& getPlayFieldCards() const { return _playFieldCards; }
    
    std::vector<CardModel>& getDrawStack() { return _drawStack; }
    const std::vector<CardModel>& getDrawStack() const { return _drawStack; }
    
    CardModel& getCurrentTrayCard() { return _currentTrayCard; }
    const CardModel& getCurrentTrayCard() const { return _currentTrayCard; }
    
    int getLevelId() const { return _levelId; }
    
    // Setters
    void setPlayFieldCards(const std::vector<CardModel>& cards) { _playFieldCards = cards; }
    void setDrawStack(const std::vector<CardModel>& cards) { _drawStack = cards; }
    void setCurrentTrayCard(const CardModel& card) { _currentTrayCard = card; }
    void setLevelId(int levelId) { _levelId = levelId; }
    
    /**
     * @brief 根据ID查找桌面牌
     * @param cardId 卡牌ID
     * @return 找到的卡牌指针，未找到返回nullptr
     */
    CardModel* findPlayFieldCardById(int cardId);
    
    /**
     * @brief 从桌面移除指定卡牌
     * @param cardId 卡牌ID
     * @return 是否成功移除
     */
    bool removePlayFieldCard(int cardId);
    
    /**
     * @brief 清空所有数据
     */
    void clear();
    
private:
    std::vector<CardModel> _playFieldCards;  // 桌面上的牌
    std::vector<CardModel> _drawStack;       // 待抽取的牌堆
    CardModel _currentTrayCard;              // 当前底牌
    int _levelId;                            // 关卡ID
};

#endif // __GAME_MODEL_H__
