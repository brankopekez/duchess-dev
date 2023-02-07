#pragma once

#include <stack>

class Command {
 public:
  virtual void Execute() = 0;
};

class UndoableCommand : public Command {
 public:
  virtual void Undo() = 0;
};

class MoveCommand : public UndoableCommand {
 public:
  virtual void Execute() override;
  virtual void Undo() override;
};

class CommandManger {
 public:
  void ExecuteCommand(Command& cmd);
  void Undo();

 private:
  std::stack<Command> command_stack_;
};