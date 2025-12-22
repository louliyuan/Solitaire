# Solitaire 纸牌游戏项目文档

## 项目概述

这是一个基于 Cocos2d-x 3.17 开发的纸牌消除游戏，采用严格的 MVC 架构设计。

### 游戏规则

1. **桌面牌区**：显示待消除的纸牌，部分牌翻开，部分覆盖
2. **底牌区**：显示当前可用于匹配的牌
3. **备用牌堆**：玩家可以从中抽取新牌补充底牌
4. **消除规则**：桌面牌的点数必须比底牌点数大1或小1（例如：底牌8可以匹配7或9）
5. **回退功能**：支持无限次回退操作

### 技术规格

- **引擎版本**：Cocos2d-x 3.17 (C++)
- **设计分辨率**：1080 x 2080 (FIXED_WIDTH 策略)
- **架构模式**：MVC（Model-View-Controller）

## 项目架构

### 目录结构

```
Classes/
├── AppDelegate.h/cpp       # 程序入口
├── configs/                # 静态配置相关类
│   ├── models/
│   │   └── LevelConfig.h/cpp      # 关卡数据结构定义
│   └── loaders/
│       └── LevelConfigLoader.h/cpp # 读取JSON配置
├── models/                 # 动态数据模型
│   ├── CardModel.h/cpp    # 单张卡牌数据
│   ├── GameModel.h/cpp    # 整个牌局的数据
│   └── UndoModel.h        # 回退栈数据结构
├── views/                  # 视图层
│   ├── GameView.h/cpp     # 游戏主场景UI
│   ├── CardView.h/cpp     # 单张卡牌的Sprite封装
│   ├── PlayFieldView.h/cpp # 桌面牌区域容器
│   └── StackView.h/cpp    # 牌堆区域容器
├── controllers/            # 控制层
│   ├── GameController.h/cpp       # 总控，协调各子Controller
│   ├── PlayFieldController.h/cpp  # 处理桌面牌点击逻辑
│   └── StackController.h/cpp      # 处理牌堆点击逻辑
├── managers/               # 状态管理
│   └── UndoManager.h/cpp  # 管理回退栈的操作逻辑
├── services/               # 无状态服务
│   ├── GameLogicService.h/cpp     # 判定匹配规则
│   └── LevelGeneratorService.h/cpp # 将Config转换为Model
└── utils/                  # 通用工具
    └── Constants.h         # 枚举和常量定义

Resources/
└── configs/
    └── level1.json         # 关卡配置文件
```

### 架构层次说明

#### 1. Models（数据模型层）
- **职责**：存储游戏数据和状态，支持序列化
- **约束**：不包含业务逻辑
- **主要类**：
  - `CardModel`：存储单张卡牌的状态（花色、点数、位置、翻面状态）
  - `GameModel`：存储整局游戏数据（桌面牌、牌堆、当前底牌）
  - `UndoModel`：回退命令结构

#### 2. Configs（配置层）
- **职责**：静态配置数据的定义和加载
- **主要类**：
  - `LevelConfig`：关卡配置数据结构
  - `LevelConfigLoader`：从JSON加载配置

#### 3. Views（视图层）
- **职责**：界面展示和接收用户输入
- **约束**：不包含业务逻辑，通过回调与Controller通信
- **主要类**：
  - `GameView`：主游戏场景
  - `CardView`：单张卡牌视图，负责翻牌和移动动画
  - `PlayFieldView`：桌面牌容器，处理触摸事件
  - `StackView`：牌堆区域

#### 4. Controllers（控制层）
- **职责**：处理业务逻辑，连接View和Model
- **主要类**：
  - `GameController`：游戏主控制器，管理游戏流程
  - `PlayFieldController`：处理桌面牌点击逻辑
  - `StackController`：处理抽牌逻辑

#### 5. Managers（管理器层）
- **职责**：作为Controller的成员，可持有Model数据
- **约束**：
  - 禁止单例模式
  - 禁止反向依赖Controller
  - 通过回调接口与其他模块交互
- **主要类**：
  - `UndoManager`：管理回退栈

#### 6. Services（服务层）
- **职责**：提供无状态的业务逻辑处理
- **约束**：
  - 不持有数据
  - 可以是单例或静态方法
- **主要类**：
  - `GameLogicService`：匹配规则判定、遮挡关系计算
  - `LevelGeneratorService`：将静态配置转换为运行时数据

## 核心流程

### 1. 游戏初始化流程

```
用户启动游戏
  ↓
GameController::startGame(levelId)
  ↓
LevelConfigLoader 加载 JSON 配置
  ↓
LevelGeneratorService 生成 GameModel
  ↓
初始化 UndoManager
  ↓
创建 GameView
  ↓
初始化子 Controllers
  ↓
刷新视图显示
```

### 2. 卡牌点击匹配流程

```
用户点击桌面卡牌
  ↓
PlayFieldView 捕获触摸事件
  ↓
回调 PlayFieldController::handleCardClick(cardId)
  ↓
检查卡牌是否可点击（翻开且未遮挡）
  ↓
检查是否可以与底牌匹配（点数差1）
  ↓
记录回退命令到 UndoManager
  ↓
更新 GameModel（桌面牌移除，底牌更新）
  ↓
播放移动动画
  ↓
更新其他卡牌状态（遮挡、翻面）
  ↓
刷新视图
```

### 3. 抽牌流程

```
用户点击备用牌堆
  ↓
StackView 捕获点击事件
  ↓
回调 StackController::handleStackClick()
  ↓
检查牌堆是否为空
  ↓
记录回退命令
  ↓
从牌堆取出第一张牌
  ↓
更新 GameModel（底牌更新，牌堆减少）
  ↓
播放抽牌动画
  ↓
刷新视图
```

### 4. 回退流程

```
用户点击回退按钮
  ↓
GameController::handleUndo()
  ↓
从 UndoManager 弹出回退命令
  ↓
根据命令类型执行逆向操作：
  - MOVE_PLAYFIELD_TO_TRAY: 将底牌放回桌面
  - DRAW_FROM_STACK: 将底牌放回牌堆
  ↓
更新 GameModel
  ↓
刷新视图
```

## 如何扩展

### 1. 添加新的卡牌类型

如果需要添加新的卡牌类型（例如：大小王），只需修改以下文件：

**步骤1：修改枚举定义**
```cpp
// Classes/utils/Constants.h
enum class CardFace
{
    // ... 现有定义 ...
    JOKER_RED = 14,   // 大王
    JOKER_BLACK = 15, // 小王
};
```

**步骤2：更新配置加载器**
```cpp
// Classes/configs/loaders/LevelConfigLoader.cpp
// parseCardData() 方法已经支持任意 CardFace 值，无需修改
```

**步骤3：更新视图**
```cpp
// Classes/views/CardView.cpp
std::string CardView::getFrontImagePath(CardFace face, CardSuit suit)
{
    // 添加新卡牌的图片路径映射
    if (face == CardFace::JOKER_RED) {
        return "cards/joker_red.png";
    }
    if (face == CardFace::JOKER_BLACK) {
        return "cards/joker_black.png";
    }
    // ... 现有逻辑 ...
}
```

**步骤4：更新游戏逻辑**
```cpp
// Classes/services/GameLogicService.cpp
bool GameLogicService::canMatchFaces(CardFace face1, CardFace face2)
{
    // 如果有大小王，添加特殊规则
    if (face1 == CardFace::JOKER_RED || face1 == CardFace::JOKER_BLACK) {
        return true; // 大小王可以匹配任意牌
    }
    // ... 现有逻辑 ...
}
```

### 2. 添加新的回退功能类型

如果需要添加新的操作类型（例如：道具使用），只需：

**步骤1：添加新的回退类型**
```cpp
// Classes/models/UndoModel.h
enum class UndoActionType
{
    // ... 现有类型 ...
    USE_HINT_ITEM,      // 使用提示道具
    USE_SHUFFLE_ITEM,   // 使用洗牌道具
};
```

**步骤2：扩展UndoCommand结构**
```cpp
// Classes/models/UndoModel.h
struct UndoCommand
{
    // ... 现有字段 ...
    
    // 为新操作添加必要的字段
    std::vector<CardModel> shuffleBeforeState; // 洗牌前的状态
};
```

**步骤3：实现回退逻辑**
```cpp
// Classes/controllers/GameController.cpp
void GameController::handleUndo()
{
    UndoCommand command = _undoManager->popCommand();
    
    // 添加新的回退处理
    if (command.actionType == UndoActionType::USE_SHUFFLE_ITEM) {
        // 恢复洗牌前的状态
        _gameModel->setPlayFieldCards(command.shuffleBeforeState);
    }
    // ... 其他类型处理 ...
}
```

### 3. 添加新的Controller

如果需要添加新功能模块（例如：道具系统），建议添加新的Controller：

**步骤1：创建新Controller**
```cpp
// Classes/controllers/ItemController.h
class ItemController : public cocos2d::Ref
{
public:
    static ItemController* create(GameController* gameController);
    void useHintItem();
    void useShuffleItem();
private:
    GameController* _gameController;
};
```

**步骤2：在GameController中集成**
```cpp
// Classes/controllers/GameController.h
class GameController
{
private:
    ItemController* _itemController; // 添加成员
};

// Classes/controllers/GameController.cpp
void GameController::initSubControllers()
{
    // ... 现有代码 ...
    _itemController = ItemController::create(this);
    _itemController->retain();
}
```

## 编码规范

### 命名规范
- 类名和文件名：大写字母开头（PascalCase）
- 函数名和变量名：驼峰风格，小写字母开头（camelCase）
- 类的私有成员和方法：以下划线开头（_memberName）
- 常量变量名：以小写字母k开头（kConstantName）

### 代码质量要求
- 每个类必须添加注释，说明功能、职责和使用场景
- 成员变量和公共方法必须添加注释
- 函数超过50行需要重构
- 类超过500行需要重构
- 遵循单一职责原则

### 各层约束

**Models层**
- 只存储数据，不包含业务逻辑
- 支持序列化和反序列化

**Views层**
- 只负责显示，不包含业务逻辑
- 可持有const类型的controller和model指针
- 通过回调接口与Controller交互

**Controllers层**
- 协调Model和View
- 处理业务逻辑

**Managers层**
- 作为Controller成员变量
- 可持有和加工Model数据
- 禁止单例模式
- 禁止反向依赖Controller

**Services层**
- 无状态服务
- 禁止持有数据
- 可以是单例或静态方法

## 配置文件格式

### 关卡配置（level*.json）

```json
{
    "Playfield": [
        {
            "CardFace": 12,     // 点数：A=1, 2-10, J=11, Q=12, K=13
            "CardSuit": 0,      // 花色：梅花=0, 方块=1, 红桃=2, 黑桃=3
            "Position": {"x": 250, "y": 1000}
        }
    ],
    "Stack": [
        {
            "CardFace": 4,
            "CardSuit": 0,
            "Position": {"x": 0, "y": 0}  // 牌堆位置由代码控制
        }
    ]
}
```

## 测试建议

### 单元测试
1. 测试GameLogicService的匹配规则
2. 测试UndoManager的栈操作
3. 测试LevelConfigLoader的JSON解析

### 集成测试
1. 测试完整的点击-匹配-回退流程
2. 测试抽牌-回退流程
3. 测试胜利/失败判定

## 已知限制和改进建议

### 当前版本限制
1. 图片资源路径是硬编码的，需要准备对应的卡牌图片
2. 遮挡检测使用简单的矩形碰撞，可能需要更精确的算法
3. 没有实现音效和粒子特效

### 改进建议
1. 添加卡牌图片资源配置文件
2. 实现更智能的遮挡检测算法
3. 添加音效系统
4. 添加成就和统计系统
5. 支持多种游戏模式

## 总结

本项目严格遵循MVC架构，各层职责明确，代码结构清晰。通过合理的设计，可以轻松扩展新功能，如添加新卡牌类型、新的回退功能类型等。项目代码注释完整，便于理解和维护。
