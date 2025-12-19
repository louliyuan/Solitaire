#include "CardView.h"
#include "../utils/Constants.h"

CardView::CardView()
    : _cardId(-1)
    , _cardSprite(nullptr)
    , _frontSprite(nullptr)
    , _backSprite(nullptr)
    , _isFaceUp(false)
{
}

CardView::~CardView()
{
}

CardView* CardView::create(const CardModel& cardModel)
{
    CardView* ret = new (std::nothrow) CardView();
    if (ret && ret->init(cardModel))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(const CardModel& cardModel)
{
    if (!Node::init())
    {
        return false;
    }
    
    _cardId = cardModel.getId();
    _isFaceUp = cardModel.getIsFaceUp();
    
    // 设置节点大小
    setContentSize(cocos2d::Size(GameConstants::kCardWidth, GameConstants::kCardHeight));
    setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    
    // 创建卡牌精灵
    createCardSprite(cardModel);
    
    // 设置位置
    setPosition(cardModel.getPosition());
    
    return true;
}

void CardView::createCardSprite(const CardModel& cardModel)
{
    // 创建正面精灵
    std::string frontPath = getFrontImagePath(cardModel.getFace(), cardModel.getSuit());
    _frontSprite = cocos2d::Sprite::create(frontPath);
    if (_frontSprite)
    {
        _frontSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
        _frontSprite->setVisible(_isFaceUp);
        addChild(_frontSprite, 1);
    }
    
    // 创建背面精灵
    std::string backPath = getBackImagePath();
    _backSprite = cocos2d::Sprite::create(backPath);
    if (_backSprite)
    {
        _backSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
        _backSprite->setVisible(!_isFaceUp);
        addChild(_backSprite, 1);
    }
}

void CardView::updateWithModel(const CardModel& cardModel)
{
    _cardId = cardModel.getId();
    setPosition(cardModel.getPosition());
    
    if (_isFaceUp != cardModel.getIsFaceUp())
    {
        _isFaceUp = cardModel.getIsFaceUp();
        if (_frontSprite)
        {
            _frontSprite->setVisible(_isFaceUp);
        }
        if (_backSprite)
        {
            _backSprite->setVisible(!_isFaceUp);
        }
    }
}

void CardView::playFlipAnimation(bool isFaceUp, const std::function<void()>& callback)
{
    _isFaceUp = isFaceUp;
    
    // 简单的翻转动画：缩放到0然后恢复
    auto scaleDown = cocos2d::ScaleTo::create(GameConstants::kCardFlipTime / 2, 0.0f, 1.0f);
    auto changeSprite = cocos2d::CallFunc::create([this, isFaceUp]() {
        if (_frontSprite)
        {
            _frontSprite->setVisible(isFaceUp);
        }
        if (_backSprite)
        {
            _backSprite->setVisible(!isFaceUp);
        }
    });
    auto scaleUp = cocos2d::ScaleTo::create(GameConstants::kCardFlipTime / 2, 1.0f, 1.0f);
    
    cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
    actions.pushBack(scaleDown);
    actions.pushBack(changeSprite);
    actions.pushBack(scaleUp);
    
    if (callback)
    {
        auto callbackAction = cocos2d::CallFunc::create(callback);
        actions.pushBack(callbackAction);
    }
    
    auto sequence = cocos2d::Sequence::create(actions);
    runAction(sequence);
}

void CardView::playMoveAnimation(const cocos2d::Vec2& targetPos, const std::function<void()>& callback)
{
    auto moveTo = cocos2d::MoveTo::create(GameConstants::kCardMoveTime, targetPos);
    
    if (callback)
    {
        auto callbackAction = cocos2d::CallFunc::create(callback);
        auto sequence = cocos2d::Sequence::create(moveTo, callbackAction, nullptr);
        runAction(sequence);
    }
    else
    {
        runAction(moveTo);
    }
}

void CardView::setClickable(bool clickable)
{
    // 可以通过改变透明度或颜色来表示是否可点击
    if (clickable)
    {
        setOpacity(255);
    }
    else
    {
        setOpacity(180);
    }
}

std::string CardView::getFrontImagePath(CardFace face, CardSuit suit)
{
    // 根据花色和点数构建图片路径
    // 格式：cards/suit_face.png
    // 例如：cards/hearts_10.png, cards/spades_ace.png
    
    std::string suitStr;
    switch (suit)
    {
        case CardSuit::CLUBS:    suitStr = "clubs"; break;
        case CardSuit::DIAMONDS: suitStr = "diamonds"; break;
        case CardSuit::HEARTS:   suitStr = "hearts"; break;
        case CardSuit::SPADES:   suitStr = "spades"; break;
        default: suitStr = "none"; break;
    }
    
    std::string faceStr;
    int faceValue = static_cast<int>(face);
    if (faceValue >= 2 && faceValue <= 10)
    {
        faceStr = std::to_string(faceValue);
    }
    else
    {
        switch (face)
        {
            case CardFace::ACE:   faceStr = "ace"; break;
            case CardFace::JACK:  faceStr = "jack"; break;
            case CardFace::QUEEN: faceStr = "queen"; break;
            case CardFace::KING:  faceStr = "king"; break;
            default: faceStr = "none"; break;
        }
    }
    
    return "cards/" + suitStr + "_" + faceStr + ".png";
}

std::string CardView::getBackImagePath()
{
    return "cards/card_back.png";
}
