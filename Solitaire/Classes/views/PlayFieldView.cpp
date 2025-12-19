#include "PlayFieldView.h"
#include "ui/CocosGUI.h"

PlayFieldView::PlayFieldView()
{
}

PlayFieldView::~PlayFieldView()
{
}

PlayFieldView* PlayFieldView::create()
{
    PlayFieldView* ret = new (std::nothrow) PlayFieldView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PlayFieldView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // 设置触摸监听
    auto listener = cocos2d::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](cocos2d::Touch* touch, cocos2d::Event* event) {
        auto location = touch->getLocation();
        
        // 检查是否点击了某张卡牌
        for (auto& pair : _cardViews)
        {
            CardView* cardView = pair.second;
            if (cardView && cardView->isVisible())
            {
                cocos2d::Rect cardRect = cardView->getBoundingBox();
                if (cardRect.containsPoint(location))
                {
                    onCardClicked(cardView);
                    return true;
                }
            }
        }
        return false;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void PlayFieldView::addCardView(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    int cardId = cardView->getCardId();
    _cardViews[cardId] = cardView;
    addChild(cardView, cardView->getLocalZOrder());
}

void PlayFieldView::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        CardView* cardView = it->second;
        if (cardView)
        {
            cardView->removeFromParent();
        }
        _cardViews.erase(it);
    }
}

CardView* PlayFieldView::getCardViewById(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}

void PlayFieldView::clearAllCards()
{
    for (auto& pair : _cardViews)
    {
        if (pair.second)
        {
            pair.second->removeFromParent();
        }
    }
    _cardViews.clear();
}

void PlayFieldView::setOnCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
}

void PlayFieldView::onCardClicked(CardView* cardView)
{
    if (!cardView)
    {
        return;
    }
    
    if (_onCardClickCallback)
    {
        _onCardClickCallback(cardView->getCardId());
    }
}
