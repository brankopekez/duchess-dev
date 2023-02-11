#include "command.h"

#include <typeinfo>

MoveCommand::MoveCommand(Chessboard& board) : board_{&board} {}

void MoveCommand::Execute() {}

void CommandManger::ExecuteCommand(std::shared_ptr<Command> command) {
  redo_stack_ = std::stack<std::shared_ptr<Command>>{};
  command->Execute();
  undo_stack_.push(command);
}

void CommandManger::Undo() {
  if (undo_stack_.size() > 0) {
    undo_stack_.top()->Undo();            // undo most recently executed command
    undo_stack_.push(undo_stack_.top());  // add undone command to undo stack
    redo_stack_.pop();                    // remove top entry from undo stack
  }
}

void CommandManger::Redo() {
  if (redo_stack_.size() <= 0) {
    return;
  }
  redo_stack_.top()->Redo();            // redo most recently executed command
  undo_stack_.push(redo_stack_.top());  // add undone command to redo stack
  redo_stack_.pop();                    // remove top entry from redo stack
}
