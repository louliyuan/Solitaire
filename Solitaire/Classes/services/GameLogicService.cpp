#include "GameLogicService.h"
#include "../utils/Constants.h"
#include <cmath>

bool GameLogicService::canMatch(const CardModel& card1, const CardModel& card2)
{
    return canMatchFaces(card1.getFace(), card2.getFace());
}

bool GameLogicService::canMatchFaces(CardFace face1, CardFace face2)
{
    int faceValue1 = static_cast<int>(face1);
    int faceValue2 = static_cast<int>(face2);
    
    // 点数差值必须为1
    int diff = std::abs(faceValue1 - faceValue2);
    return diff == 1;
}

bool GameLogicService::canCardBeClicked(const CardModel& card)
{
    // 必须是翻开的牌且没有被遮挡
    return card.getIsFaceUp() && !card.getIsBlocked();
}

void GameLogicService::updateCardBlockingStates(GameModel* gameModel)
{
    if (!gameModel)
    {
        return;
    }
    
    auto& cards = gameModel->getPlayFieldCards();
    
    // 首先将所有牌设置为未遮挡
    for (auto& card : cards)
    {
        card.setIsBlocked(false);
    }
    
    // 检查每对牌，判断是否存在遮挡关系
    for (size_t i = 0; i < cards.size(); ++i)
    {
        for (size_t j = 0; j < cards.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }
            
            // 如果 j 在 i 上方（zOrder更大或位置更高），且有重叠，则 i 被遮挡
            if (cards[j].getZOrder() > cards[i].getZOrder() || 
                cards[j].getPosition().y > cards[i].getPosition().y)
            {
                if (isCardOverlapping(cards[i], cards[j]))
                {
                    cards[i].setIsBlocked(true);
                    break;
                }
            }
        }
    }
}

bool GameLogicService::isCardOverlapping(const CardModel& card1, const CardModel& card2)
{
    // 简单的矩形碰撞检测
    cocos2d::Rect rect1(
        card1.getPosition().x - GameConstants::kCardWidth / 2,
        card1.getPosition().y - GameConstants::kCardHeight / 2,
        GameConstants::kCardWidth,
        GameConstants::kCardHeight
    );
    
    cocos2d::Rect rect2(
        card2.getPosition().x - GameConstants::kCardWidth / 2,
        card2.getPosition().y - GameConstants::kCardHeight / 2,
        GameConstants::kCardWidth,
        GameConstants::kCardHeight
    );
    
    return rect1.intersectsRect(rect2);
}

bool GameLogicService::checkWin(const GameModel* gameModel)
{
    if (!gameModel)
    {
        return false;
    }
    
    // 桌面所有牌都被消除
    return gameModel->getPlayFieldCards().empty();
}

bool GameLogicService::checkLose(const GameModel* gameModel)
{
    if (!gameModel)
    {
        return false;
    }
    
    // 桌面还有牌
    if (gameModel->getPlayFieldCards().empty())
    {
        return false;
    }
    
    // 牌堆已空
    if (!gameModel->getDrawStack().empty())
    {
        return false;
    }
    
    // 检查是否还有可匹配的牌
    const CardModel& trayCard = gameModel->getCurrentTrayCard();
    const auto& playfieldCards = gameModel->getPlayFieldCards();
    
    for (const auto& card : playfieldCards)
    {
        if (canCardBeClicked(card) && canMatch(card, trayCard))
        {
            return false; // 还有可匹配的牌
        }
    }
    
    return true; // 没有可匹配的牌了
}
