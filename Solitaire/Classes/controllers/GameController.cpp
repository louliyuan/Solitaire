#include "GameController.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/LevelGeneratorService.h"
#include "../services/GameLogicService.h"
#include "../views/CardView.h"
#include "../views/PlayFieldView.h"
#include "../views/StackView.h"

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
    , _playFieldController(nullptr)
    , _stackController(nullptr)
{
}

GameController::~GameController()
{
    CC_SAFE_RELEASE_NULL(_gameModel);
    CC_SAFE_RELEASE_NULL(_undoManager);
    CC_SAFE_RELEASE_NULL(_playFieldController);
    CC_SAFE_RELEASE_NULL(_stackController);
}

GameController* GameController::create()
{
    GameController* ret = new (std::nothrow) GameController();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameController::init()
{
    // 创建数据模型
    _gameModel = new GameModel();
    _gameModel->retain();
    
    // 创建管理器
    _undoManager = UndoManager::create();
    _undoManager->retain();
    
    return true;
}

bool GameController::startGame(int levelId)
{
    // 加载关卡配置
    LevelConfig levelConfig;
    if (!LevelConfigLoader::loadLevelConfig(levelId, levelConfig))
    {
        CCLOG("Failed to load level %d", levelId);
        return false;
    }
    
    levelConfig.setLevelId(levelId);
    
    // 生成游戏数据
    if (!LevelGeneratorService::generateGameModel(levelConfig, *_gameModel))
    {
        CCLOG("Failed to generate game model");
        return false;
    }
    
    // 清空回退栈
    _undoManager->clear();
    
    // 创建视图
    initView();
    
    // 初始化子控制器
    initSubControllers();
    
    // 刷新视图
    refreshView();
    
    return true;
}

void GameController::initView()
{
    // 创建游戏视图
    _gameView = GameView::create();
    
    // 设置回退按钮回调
    _gameView->setOnUndoClickCallback([this]() {
        handleUndo();
    });
}

void GameController::initSubControllers()
{
    // 创建子控制器
    _playFieldController = PlayFieldController::create(this);
    _playFieldController->retain();
    _playFieldController->initView();
    
    _stackController = StackController::create(this);
    _stackController->retain();
    _stackController->initView();
}

void GameController::refreshView()
{
    if (!_gameView || !_gameModel)
    {
        return;
    }
    
    PlayFieldView* playFieldView = _gameView->getPlayFieldView();
    if (playFieldView)
    {
        // 清空旧的卡牌
        playFieldView->clearAllCards();
        
        // 创建所有桌面卡牌视图
        const auto& cards = _gameModel->getPlayFieldCards();
        for (const auto& card : cards)
        {
            CardView* cardView = CardView::create(card);
            cardView->setLocalZOrder(card.getZOrder());
            playFieldView->addCardView(cardView);
            
            // 设置可点击状态
            cardView->setClickable(GameLogicService::canCardBeClicked(card));
        }
    }
    
    // 刷新牌堆视图
    if (_stackController)
    {
        _stackController->refreshView();
    }
    
    // 更新回退按钮
    _gameView->updateUndoButton(_undoManager->canUndo());
}

void GameController::handleUndo()
{
    if (!_undoManager->canUndo())
    {
        CCLOG("Cannot undo");
        return;
    }
    
    // 弹出回退命令
    UndoCommand command = _undoManager->popCommand();
    
    if (command.actionType == UndoActionType::MOVE_PLAYFIELD_TO_TRAY)
    {
        // 恢复桌面牌
        CardModel restoredCard = _gameModel->getCurrentTrayCard();
        restoredCard.setPosition(command.prevPosition);
        restoredCard.setIsFaceUp(command.prevIsFaceUp);
        restoredCard.setZOrder(command.prevZOrder);
        
        // 恢复底牌
        _gameModel->setCurrentTrayCard(command.prevTrayCard);
        
        // 将卡牌添加回桌面
        auto& playFieldCards = _gameModel->getPlayFieldCards();
        playFieldCards.push_back(restoredCard);
        
        // 更新遮挡状态
        GameLogicService::updateCardBlockingStates(_gameModel);
    }
    else if (command.actionType == UndoActionType::DRAW_FROM_STACK)
    {
        // 将底牌放回牌堆
        CardModel cardToReturn = _gameModel->getCurrentTrayCard();
        cardToReturn.setIsFaceUp(false);
        
        auto& drawStack = _gameModel->getDrawStack();
        drawStack.insert(drawStack.begin(), cardToReturn);
        
        // 恢复之前的底牌
        _gameModel->setCurrentTrayCard(command.prevTrayCard);
    }
    
    // 刷新视图
    refreshView();
    
    // 检查游戏状态
    checkGameState();
}

void GameController::checkGameState()
{
    if (!_gameModel || !_gameView)
    {
        return;
    }
    
    // 检查是否胜利
    if (GameLogicService::checkWin(_gameModel))
    {
        CCLOG("You win!");
        _gameView->showWinMessage();
        return;
    }
    
    // 检查是否失败
    if (GameLogicService::checkLose(_gameModel))
    {
        CCLOG("Game over!");
        _gameView->showLoseMessage();
        return;
    }
}
