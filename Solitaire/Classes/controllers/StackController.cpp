#include "StackController.h"
#include "GameController.h"
#include "../models/GameModel.h"
#include "../models/UndoModel.h"
#include "../views/GameView.h"
#include "../views/StackView.h"
#include "../views/CardView.h"
#include "../managers/UndoManager.h"

StackController::StackController()
    : _gameController(nullptr)
{
}

StackController::~StackController()
{
}

StackController* StackController::create(GameController* gameController)
{
    StackController* ret = new (std::nothrow) StackController();
    if (ret && ret->init(gameController))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StackController::init(GameController* gameController)
{
    _gameController = gameController;
    return true;
}

void StackController::initView()
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
    
    StackView* stackView = gameView->getStackView();
    if (!stackView)
    {
        return;
    }
    
    // 设置牌堆点击回调
    stackView->setOnStackClickCallback([this]() {
        handleStackClick();
    });
}

void StackController::handleStackClick()
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
    
    // 检查牌堆是否为空
    if (gameModel->getDrawStack().empty())
    {
        CCLOG("Draw stack is empty");
        return;
    }
    
    // 执行抽牌
    drawCardFromStack();
}

void StackController::drawCardFromStack()
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
    
    auto& drawStack = gameModel->getDrawStack();
    if (drawStack.empty())
    {
        return;
    }
    
    // 创建回退命令
    UndoCommand command;
    command.actionType = UndoActionType::DRAW_FROM_STACK;
    command.prevTrayCard = gameModel->getCurrentTrayCard();
    command.cardId = drawStack[0].getId();
    
    // 保存命令
    undoManager->pushCommand(command);
    
    // 从牌堆抽第一张牌
    CardModel newTrayCard = drawStack[0];
    newTrayCard.setIsFaceUp(true);
    
    // 更新Model
    gameModel->setCurrentTrayCard(newTrayCard);
    drawStack.erase(drawStack.begin());
    
    // 更新View
    StackView* stackView = gameView->getStackView();
    if (stackView)
    {
        // 创建新的底牌视图
        CardView* newTrayView = CardView::create(newTrayCard);
        
        // 先设置在牌堆位置，然后动画移动到底牌位置
        newTrayView->setPosition(stackView->getStackPosition());
        stackView->addChild(newTrayView);
        
        newTrayView->playMoveAnimation(stackView->getTrayCardPosition(), [stackView, newTrayView]() {
            // 动画结束后设置为底牌
            stackView->setTrayCard(newTrayView);
        });
        
        // 更新牌堆数量
        stackView->updateStackCount(static_cast<int>(drawStack.size()));
    }
    
    // 更新回退按钮状态
    gameView->updateUndoButton(undoManager->canUndo());
}

void StackController::refreshView()
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
    
    StackView* stackView = gameView->getStackView();
    if (!stackView)
    {
        return;
    }
    
    // 更新底牌视图
    const CardModel& trayCard = gameModel->getCurrentTrayCard();
    CardView* trayView = CardView::create(trayCard);
    stackView->setTrayCard(trayView);
    
    // 更新牌堆数量
    stackView->updateStackCount(static_cast<int>(gameModel->getDrawStack().size()));
}
