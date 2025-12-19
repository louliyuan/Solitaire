#include "PlayFieldController.h"
#include "GameController.h"
#include "../models/GameModel.h"
#include "../models/UndoModel.h"
#include "../views/GameView.h"
#include "../views/PlayFieldView.h"
#include "../views/CardView.h"
#include "../services/GameLogicService.h"
#include "../managers/UndoManager.h"

PlayFieldController::PlayFieldController()
    : _gameController(nullptr)
{
}

PlayFieldController::~PlayFieldController()
{
}

PlayFieldController* PlayFieldController::create(GameController* gameController)
{
    PlayFieldController* ret = new (std::nothrow) PlayFieldController();
    if (ret && ret->init(gameController))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool PlayFieldController::init(GameController* gameController)
{
    _gameController = gameController;
    return true;
}

void PlayFieldController::initView()
{
    if (!_gameController)
    {
        return;
    }
    
    GameView* gameView = _gameController->getGameView();
    if (!gameView)
    {
        return;
    }
    
    PlayFieldView* playFieldView = gameView->getPlayFieldView();
    if (!playFieldView)
    {
        return;
    }
    
    // 设置卡牌点击回调
    playFieldView->setOnCardClickCallback([this](int cardId) {
        handleCardClick(cardId);
    });
}

void PlayFieldController::handleCardClick(int cardId)
{
    if (!_gameController)
    {
        return;
    }
    
    GameModel* gameModel = _gameController->getGameModel();
    if (!gameModel)
    {
        return;
    }
    
    // 查找被点击的卡牌
    CardModel* clickedCard = gameModel->findPlayFieldCardById(cardId);
    if (!clickedCard)
    {
        CCLOG("Card not found: %d", cardId);
        return;
    }
    
    // 检查卡牌是否可以点击
    if (!GameLogicService::canCardBeClicked(*clickedCard))
    {
        CCLOG("Card cannot be clicked (not face up or blocked)");
        return;
    }
    
    // 检查是否可以与底牌匹配
    const CardModel& trayCard = gameModel->getCurrentTrayCard();
    if (!GameLogicService::canMatch(*clickedCard, trayCard))
    {
        CCLOG("Card cannot match with tray card");
        return;
    }
    
    // 执行匹配
    replaceTrayWithPlayFieldCard(cardId);
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int cardId)
{
    if (!_gameController)
    {
        return;
    }
    
    GameModel* gameModel = _gameController->getGameModel();
    GameView* gameView = _gameController->getGameView();
    UndoManager* undoManager = _gameController->getUndoManager();
    
    if (!gameModel || !gameView || !undoManager)
    {
        return;
    }
    
    // 查找卡牌
    CardModel* card = gameModel->findPlayFieldCardById(cardId);
    if (!card)
    {
        return;
    }
    
    // 创建回退命令
    UndoCommand command;
    command.actionType = UndoActionType::MOVE_PLAYFIELD_TO_TRAY;
    command.cardId = cardId;
    command.prevTrayCard = gameModel->getCurrentTrayCard();
    command.prevPosition = card->getPosition();
    command.prevIsFaceUp = card->getIsFaceUp();
    command.prevZOrder = card->getZOrder();
    
    // 保存命令
    undoManager->pushCommand(command);
    
    // 更新Model：将卡牌设为新的底牌
    CardModel newTrayCard = *card;
    newTrayCard.setIsFaceUp(true);
    gameModel->setCurrentTrayCard(newTrayCard);
    
    // 从桌面移除该卡牌
    gameModel->removePlayFieldCard(cardId);
    
    // 更新其他卡牌状态
    updateCardStates();
    
    // 更新View：播放动画
    PlayFieldView* playFieldView = gameView->getPlayFieldView();
    StackView* stackView = gameView->getStackView();
    
    if (playFieldView && stackView)
    {
        CardView* cardView = playFieldView->getCardViewById(cardId);
        if (cardView)
        {
            // 播放移动动画到底牌位置
            cocos2d::Vec2 trayPos = stackView->getTrayCardPosition();
            cardView->playMoveAnimation(trayPos, [this, playFieldView, stackView, cardView, newTrayCard]() {
                // 动画结束后，从桌面移除，添加到牌堆区
                playFieldView->removeCardView(cardView->getCardId());
                
                // 创建新的底牌视图
                CardView* newTrayView = CardView::create(newTrayCard);
                stackView->setTrayCard(newTrayView);
                
                // 刷新视图
                refreshView();
            });
        }
    }
    
    // 更新回退按钮状态
    gameView->updateUndoButton(undoManager->canUndo());
}

void PlayFieldController::updateCardStates()
{
    if (!_gameController)
    {
        return;
    }
    
    GameModel* gameModel = _gameController->getGameModel();
    if (!gameModel)
    {
        return;
    }
    
    // 更新遮挡状态
    GameLogicService::updateCardBlockingStates(gameModel);
    
    // 检查是否有卡牌需要翻开
    auto& cards = gameModel->getPlayFieldCards();
    for (auto& card : cards)
    {
        // 如果卡牌未被遮挡且未翻开，则翻开它
        if (!card.getIsBlocked() && !card.getIsFaceUp())
        {
            card.setIsFaceUp(true);
        }
    }
}

void PlayFieldController::refreshView()
{
    if (!_gameController)
    {
        return;
    }
    
    GameModel* gameModel = _gameController->getGameModel();
    GameView* gameView = _gameController->getGameView();
    
    if (!gameModel || !gameView)
    {
        return;
    }
    
    PlayFieldView* playFieldView = gameView->getPlayFieldView();
    if (!playFieldView)
    {
        return;
    }
    
    // 更新所有卡牌视图
    const auto& cards = gameModel->getPlayFieldCards();
    for (const auto& card : cards)
    {
        CardView* cardView = playFieldView->getCardViewById(card.getId());
        if (cardView)
        {
            cardView->updateWithModel(card);
            cardView->setClickable(GameLogicService::canCardBeClicked(card));
        }
    }
}
