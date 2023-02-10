#include "command.h"

#include <typeinfo>

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
