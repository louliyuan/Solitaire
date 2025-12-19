#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include <stack>
#include "../models/UndoModel.h"
#include "../models/GameModel.h"

/**
 * @class UndoManager
 * @brief 回退管理器
 * @details 管理回退栈，处理撤销操作逻辑
 *          作为Controller的成员变量，不是单例
 *          可持有model数据并对model数据进行加工
 */
class UndoManager
{
public:
    /**
     * @brief 构造函数
     */
    UndoManager();
    
    /**
     * @brief 析构函数
     */
    ~UndoManager();
    
    /**
     * @brief 将操作压入回退栈
     * @param command 回退命令
     */
    void pushCommand(const UndoCommand& command);
    
    /**
     * @brief 执行回退操作
     * @param gameModel 游戏数据模型
     * @return 回退的命令，如果栈空返回空命令
     */
    UndoCommand popCommand();
    
    /**
     * @brief 获取栈顶命令但不弹出
     * @return 栈顶命令
     */
    UndoCommand peekCommand() const;
    
    /**
     * @brief 检查是否可以回退
     * @return 是否可以回退
     */
    bool canUndo() const;
    
    /**
     * @brief 清空回退栈
     */
    void clear();
    
    /**
     * @brief 获取回退栈的大小
     * @return 栈大小
     */
    size_t getStackSize() const;
    
private:
    std::stack<UndoCommand> _undoStack;  // 回退栈
};

#endif // __UNDO_MANAGER_H__
