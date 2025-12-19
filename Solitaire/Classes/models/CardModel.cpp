#include "CardModel.h"

CardModel::CardModel()
    : _id(-1)
    , _face(CardFace::NONE)
    , _suit(CardSuit::NONE)
    , _position(cocos2d::Vec2::ZERO)
    , _isFaceUp(false)
    , _zOrder(0)
    , _isBlocked(false)
{
}

CardModel::CardModel(int id, CardFace face, CardSuit suit)
    : _id(id)
    , _face(face)
    , _suit(suit)
    , _position(cocos2d::Vec2::ZERO)
    , _isFaceUp(false)
    , _zOrder(0)
    , _isBlocked(false)
{
}

CardModel::~CardModel()
{
}

void CardModel::copyFrom(const CardModel& other)
{
    _id = other._id;
    _face = other._face;
    _suit = other._suit;
    _position = other._position;
    _isFaceUp = other._isFaceUp;
    _zOrder = other._zOrder;
    _isBlocked = other._isBlocked;
}
