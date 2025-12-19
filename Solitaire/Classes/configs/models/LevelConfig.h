#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <vector>
#include "cocos2d.h"
#include "../../utils/Constants.h"

/**
 * @struct CardConfigData
 * @brief 单张卡牌的配置数据
 */
struct CardConfigData
{
    CardFace face;            // 卡牌点数
    CardSuit suit;            // 卡牌花色
    cocos2d::Vec2 position;   // 卡牌位置
    
    CardConfigData()
        : face(CardFace::NONE)
        , suit(CardSuit::NONE)
        , position(cocos2d::Vec2::ZERO)
    {
    }
};

/**
 * @class LevelConfig
 * @brief 关卡配置数据类
 * @details 存储从JSON文件加载的静态关卡数据
 */
class LevelConfig
{
public:
    /**
     * @brief 构造函数
     */
    LevelConfig();
    
    /**
     * @brief 析构函数
     */
    ~LevelConfig();
    
    // Getters
    int getLevelId() const { return _levelId; }
    const std::vector<CardConfigData>& getPlayfieldCards() const { return _playfieldCards; }
    const std::vector<CardConfigData>& getStackCards() const { return _stackCards; }
    
    // Setters
    void setLevelId(int levelId) { _levelId = levelId; }
    void setPlayfieldCards(const std::vector<CardConfigData>& cards) { _playfieldCards = cards; }
    void setStackCards(const std::vector<CardConfigData>& cards) { _stackCards = cards; }
    
    /**
     * @brief 添加桌面卡牌配置
     */
    void addPlayfieldCard(const CardConfigData& card);
    
    /**
     * @brief 添加牌堆卡牌配置
     */
    void addStackCard(const CardConfigData& card);
    
    /**
     * @brief 清空配置
     */
    void clear();
    
private:
    int _levelId;                               // 关卡ID
    std::vector<CardConfigData> _playfieldCards; // 桌面牌配置
    std::vector<CardConfigData> _stackCards;     // 牌堆配置
};

#endif // __LEVEL_CONFIG_H__
