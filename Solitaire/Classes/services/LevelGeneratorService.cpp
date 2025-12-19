#include "LevelGeneratorService.h"

int LevelGeneratorService::_nextCardId = 1;

bool LevelGeneratorService::generateGameModel(const LevelConfig& config, GameModel& outGameModel)
{
    outGameModel.clear();
    _nextCardId = 1;
    
    // 设置关卡ID
    outGameModel.setLevelId(config.getLevelId());
    
    // 生成桌面牌
    std::vector<CardModel> playfieldCards;
    const auto& playfieldConfigs = config.getPlayfieldCards();
    
    for (size_t i = 0; i < playfieldConfigs.size(); ++i)
    {
        CardModel card = createCardFromConfig(playfieldConfigs[i]);
        
        // 设置层级：按数组顺序，后面的在上面
        card.setZOrder(static_cast<int>(i));
        
        // 默认都是正面朝下
        card.setIsFaceUp(false);
        
        playfieldCards.push_back(card);
    }
    
    outGameModel.setPlayFieldCards(playfieldCards);
    
    // 生成牌堆
    std::vector<CardModel> stackCards;
    const auto& stackConfigs = config.getStackCards();
    
    for (const auto& stackConfig : stackConfigs)
    {
        CardModel card = createCardFromConfig(stackConfig);
        card.setIsFaceUp(false);
        stackCards.push_back(card);
    }
    
    outGameModel.setDrawStack(stackCards);
    
    // 设置初始底牌：从牌堆抽第一张
    if (!stackCards.empty())
    {
        CardModel initialTrayCard = stackCards[0];
        initialTrayCard.setIsFaceUp(true);
        outGameModel.setCurrentTrayCard(initialTrayCard);
        
        // 从牌堆移除第一张
        stackCards.erase(stackCards.begin());
        outGameModel.setDrawStack(stackCards);
    }
    
    // 初始化遮挡状态
    // 简单策略：最上层的牌（zOrder最大的）翻开
    if (!playfieldCards.empty())
    {
        // 将最上层的几张牌设为翻开状态
        // 根据位置判断：Y坐标最大的牌
        std::vector<int> topCardIndices;
        float maxY = -1;
        
        for (size_t i = 0; i < playfieldCards.size(); ++i)
        {
            if (playfieldCards[i].getPosition().y > maxY)
            {
                maxY = playfieldCards[i].getPosition().y;
            }
        }
        
        // 将Y坐标最大的牌翻开
        for (size_t i = 0; i < playfieldCards.size(); ++i)
        {
            if (playfieldCards[i].getPosition().y >= maxY - 10) // 允许小误差
            {
                playfieldCards[i].setIsFaceUp(true);
            }
        }
        
        outGameModel.setPlayFieldCards(playfieldCards);
    }
    
    return true;
}

CardModel LevelGeneratorService::createCardFromConfig(const CardConfigData& configData)
{
    CardModel card(_nextCardId++, configData.face, configData.suit);
    card.setPosition(configData.position);
    return card;
}
