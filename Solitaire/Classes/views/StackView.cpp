#include "StackView.h"
#include "../utils/Constants.h"
#include "ui/CocosGUI.h"

StackView::StackView()
    : _trayCardView(nullptr)
    , _stackSprite(nullptr)
    , _stackCountLabel(nullptr)
    , _stackButton(nullptr)
{
    // 设置底牌和牌堆的位置
    _trayPosition = cocos2d::Vec2(GameConstants::kDesignWidth / 2 + 200, 
                                  GameConstants::kStackAreaHeight / 2);
    _stackPosition = cocos2d::Vec2(GameConstants::kDesignWidth / 2 - 200, 
                                   GameConstants::kStackAreaHeight / 2);
}

StackView::~StackView()
{
}

StackView* StackView::create()
{
    StackView* ret = new (std::nothrow) StackView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StackView::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    createUI();
    
    return true;
}

void StackView::createUI()
{
    // 创建背景
    auto bg = cocos2d::LayerColor::create(cocos2d::Color4B(50, 100, 50, 255), 
                                          GameConstants::kDesignWidth, 
                                          GameConstants::kStackAreaHeight);
    addChild(bg, -1);
    
    // 创建牌堆按钮
    _stackButton = cocos2d::ui::Button::create();
    _stackButton->setTitleText("Draw");
    _stackButton->setTitleFontSize(24);
    _stackButton->setPosition(_stackPosition);
    _stackButton->addClickEventListener([this](cocos2d::Ref* sender) {
        if (_onStackClickCallback)
        {
            _onStackClickCallback();
        }
    });
    addChild(_stackButton, 10);
    
    // 创建牌堆数量标签
    _stackCountLabel = cocos2d::Label::createWithSystemFont("0", "Arial", 20);
    _stackCountLabel->setPosition(_stackPosition.x, _stackPosition.y - 80);
    _stackCountLabel->setColor(cocos2d::Color3B::WHITE);
    addChild(_stackCountLabel, 10);
}

void StackView::setTrayCard(CardView* cardView)
{
    // 移除旧的底牌
    if (_trayCardView)
    {
        _trayCardView->removeFromParent();
    }
    
    _trayCardView = cardView;
    
    if (_trayCardView)
    {
        _trayCardView->setPosition(_trayPosition);
        addChild(_trayCardView, 5);
    }
}

void StackView::updateStackCount(int count)
{
    if (_stackCountLabel)
    {
        _stackCountLabel->setString("Stack: " + std::to_string(count));
    }
    
    // 如果牌堆为空，禁用按钮
    if (_stackButton)
    {
        _stackButton->setEnabled(count > 0);
    }
}

void StackView::setOnStackClickCallback(const std::function<void()>& callback)
{
    _onStackClickCallback = callback;
}

cocos2d::Vec2 StackView::getTrayCardPosition() const
{
    return _trayPosition;
}

cocos2d::Vec2 StackView::getStackPosition() const
{
    return _stackPosition;
}
