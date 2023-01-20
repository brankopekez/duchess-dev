#include "game.h"

#include <iostream>

const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : window_{sf::VideoMode(900, 900), "Duchess-GUI", sf::Style::Close},
      board_{720},
      press_{false},
      drag_{false} {
  LoadTextures();
  //board_.SetTexture(textures_.Get("chessboard"));
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 0, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 1, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 2, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kQueen, 3, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kKing, 4, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 5, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 6, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 7, 0, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 0, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 1, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 2, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 3, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 4, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 5, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 6, 1, textures_);
  board_.NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 7, 1, textures_);

  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 0, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 1, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 2, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 3, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 4, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 5, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 6, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 7, 6, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 0, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 1, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 2, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kQueen, 3, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kKing, 4, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 5, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 6, 7, textures_);
  board_.NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 7, 7, textures_);

  board_.SetPosition({90, 90});
}

void Game::Run() {
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (window_.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;
    while (timeSinceLastUpdate > kTimePerFrame) {
      timeSinceLastUpdate -= kTimePerFrame;
      ProcessInput();
      Update(kTimePerFrame);
    }
    Render();
  }
}

void Game::LoadTextures() {
  //textures_.Load("chessboard", "Media/Textures/chessboard.png");
  //textures_.Load("white_square", "Media/Textures/white_square.png");
  //textures_.Load("black_square", "Media/Textures/black_square.png");
  const std::string chess_pieces = "Media/Textures/chess_pieces.png";
  int d = 90;
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kKing, chess_pieces,
                 sf::IntRect(0, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kKing, chess_pieces,
                 sf::IntRect(0, d, d, d));
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kQueen, chess_pieces,
                 sf::IntRect(d, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kQueen, chess_pieces,
                 sf::IntRect(d, d, d, d));
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kBishop, chess_pieces,
                 sf::IntRect(2 * d, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kBishop, chess_pieces,
                 sf::IntRect(2 * d, d, d, d));
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kKnight, chess_pieces,
                 sf::IntRect(3 * d, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kKnight, chess_pieces,
                 sf::IntRect(3 * d, d, d, d));
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kRook, chess_pieces,
                 sf::IntRect(4 * d, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kRook, chess_pieces,
                 sf::IntRect(4 * d, d, d, d));
  textures_.Load(Chessman::Color::kWhite | Chessman::Type::kPawn, chess_pieces,
                 sf::IntRect(5 * d, 0, d, d));
  textures_.Load(Chessman::Color::kBlack | Chessman::Type::kPawn, chess_pieces,
                 sf::IntRect(5 * d, d, d, d));
}

void Game::ProcessInput() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window_.close();
        break;
      case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
          press_ = true;
          board_.SelectSquareAt({(float)event.mouseButton.x, (float)event.mouseButton.y});
          std::cout << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
        }
        break;
      case sf::Event::MouseMoved:
        if (press_) {
          drag_ = true;
          board_.MoveSelectedPiece({(float)event.mouseMove.x, (float)event.mouseMove.y});
          std::cout << event.mouseMove.x << ", " << event.mouseMove.y << std::endl;
        }
        break;
      case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left && press_) {
          if (drag_ && !board_.Move({(float)event.mouseButton.x, (float)event.mouseButton.y})) {
            board_.ResetSelectedSquare();
          }
          drag_ = false;
          press_ = false;
        }
        break;
    }
  }
}

void Game::Update(sf::Time elapsedTime) {}

void Game::Render() {
  window_.clear(sf::Color::White);
  window_.draw(board_);
  window_.display();
}
