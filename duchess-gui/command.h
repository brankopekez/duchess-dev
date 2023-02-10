#pragma once

#include <memory>
#include <stack>

#include "chessboard.h"

class Command {
 public:
  virtual void Execute() = 0;
  virtual void Undo() = 0;
};

class MoveCommand : public Command {
 public:
  virtual void Execute() override;
  virtual void Undo() override;

 private:
  Chessboard* board_;
};

class CommandManger {
 public:
  void ExecuteCommand(std::shared_ptr<Command> command);
  void Undo();

 private:
  std::stack<std::shared_ptr<Command>> undo_stack_;
  std::stack<std::shared_ptr<Command>> redo_stack_;
};