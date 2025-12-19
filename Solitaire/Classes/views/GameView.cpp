#include "GameView.h"
#include "../utils/Constants.h"
#include "ui/CocosGUI.h"

GameView::GameView()
    : _playFieldView(nullptr)
    , _stackView(nullptr)
    , _undoButton(nullptr)
{
}

GameView::~GameView()
{
}

GameView* GameView::create()
{
    GameView* ret = new (std::nothrow) GameView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameView::init()
{
    if (!Scene::init())
    {
        return false;
    }
    
    createUI();
    
    return true;
}

void GameView::createUI()
{
    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    
    // 创建背景
    auto bg = cocos2d::LayerColor::create(cocos2d::Color4B(34, 139, 34, 255));
    addChild(bg, -10);
    
    // 创建桌面视图
    _playFieldView = PlayFieldView::create();
    _playFieldView->setPosition(0, GameConstants::kStackAreaHeight);
    _playFieldView->setContentSize(cocos2d::Size(GameConstants::kDesignWidth, 
                                                  GameConstants::kPlayFieldHeight));
    addChild(_playFieldView, 1);
    
    // 创建牌堆视图
    _stackView = StackView::create();
    _stackView->setPosition(0, 0);
    addChild(_stackView, 1);
    
    // 创建回退按钮
    _undoButton = cocos2d::ui::Button::create();
    _undoButton->setTitleText("Undo");
    _undoButton->setTitleFontSize(28);
    _undoButton->setPosition(cocos2d::Vec2(100, GameConstants::kDesignHeight - 50));
    _undoButton->addClickEventListener([this](cocos2d::Ref* sender) {
        if (_onUndoClickCallback)
        {
            _onUndoClickCallback();
        }
    });
    addChild(_undoButton, 10);
}

void GameView::setOnUndoClickCallback(const std::function<void()>& callback)
{
    _onUndoClickCallback = callback;
}

void GameView::playMatchAnimation(int cardId, const std::function<void()>& callback)
{
    // 这个方法在Controller中会被调用，用于播放匹配动画
    if (callback)
    {
        callback();
    }
}

void GameView::updateUndoButton(bool canUndo)
{
    if (_undoButton)
    {
        _undoButton->setEnabled(canUndo);
    }
}

void GameView::showWinMessage()
{
    auto label = cocos2d::Label::createWithSystemFont("You Win!", "Arial", 60);
    label->setPosition(GameConstants::kDesignWidth / 2, GameConstants::kDesignHeight / 2);
    label->setColor(cocos2d::Color3B::YELLOW);
    addChild(label, 100);
}

void GameView::showLoseMessage()
{
    auto label = cocos2d::Label::createWithSystemFont("Game Over!", "Arial", 60);
    label->setPosition(GameConstants::kDesignWidth / 2, GameConstants::kDesignHeight / 2);
    label->setColor(cocos2d::Color3B::RED);
    addChild(label, 100);
}
