#include "command.h"

#include <typeinfo>

void CommandManger::ExecuteCommand(Command* cmd) {
  cmd->Execute();
  if (typeid(cmd) == typeid(UndoableCommand)) {
    command_stack_.push(cmd);
  }
}

void CommandManger::Undo() {
    if (command_stack_.size() > 0) {
    UndoableCommand* cmd = dynamic_cast<UndoableCommand*>(command_stack_.top());
    command_stack_.pop();
    cmd->Undo();
  }
}
