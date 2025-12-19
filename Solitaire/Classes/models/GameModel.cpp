#include "GameModel.h"

GameModel::GameModel()
    : _levelId(0)
{
}

GameModel::~GameModel()
{
}

CardModel* GameModel::findPlayFieldCardById(int cardId)
{
    for (auto& card : _playFieldCards)
    {
        if (card.getId() == cardId)
        {
            return &card;
        }
    }
    return nullptr;
}

bool GameModel::removePlayFieldCard(int cardId)
{
    for (auto it = _playFieldCards.begin(); it != _playFieldCards.end(); ++it)
    {
        if (it->getId() == cardId)
        {
            _playFieldCards.erase(it);
            return true;
        }
    }
    return false;
}

void GameModel::clear()
{
    _playFieldCards.clear();
    _drawStack.clear();
    _currentTrayCard = CardModel();
    _levelId = 0;
}
