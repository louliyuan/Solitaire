#include "UndoManager.h"

UndoManager::UndoManager()
{
}

UndoManager::~UndoManager()
{
    clear();
}

void UndoManager::pushCommand(const UndoCommand& command)
{
    _undoStack.push(command);
}

UndoCommand UndoManager::popCommand()
{
    if (_undoStack.empty())
    {
        return UndoCommand();
    }
    
    UndoCommand command = _undoStack.top();
    _undoStack.pop();
    return command;
}

UndoCommand UndoManager::peekCommand() const
{
    if (_undoStack.empty())
    {
        return UndoCommand();
    }
    
    return _undoStack.top();
}

bool UndoManager::canUndo() const
{
    return !_undoStack.empty();
}

void UndoManager::clear()
{
    while (!_undoStack.empty())
    {
        _undoStack.pop();
    }
}

size_t UndoManager::getStackSize() const
{
    return _undoStack.size();
}
