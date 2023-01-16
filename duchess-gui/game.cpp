#include "game.h"

const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : window_{sf::VideoMode(900, 900), "Duchess-GUI", sf::Style::Close},
      board_{720} {
  LoadTextures();
  //board_.SetTexture(textures_.Get("chessboard"));

  Chessman p{"black_rook", textures_};
  p.SetSize(720.f / 8.f);
  pieces_.push_back(std::move(p));
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
  textures_.Load("white_king", chess_pieces, sf::IntRect(0, 0, d, d));
  textures_.Load("black_king", chess_pieces, sf::IntRect(0, d, d, d));
  textures_.Load("white_queen", chess_pieces, sf::IntRect(d, 0, d, d));
  textures_.Load("black_queen", chess_pieces, sf::IntRect(d, d, d, d));
  textures_.Load("white_bishop", chess_pieces, sf::IntRect(2 * d, 0, d, d));
  textures_.Load("black_bishop", chess_pieces, sf::IntRect(2 * d, d, d, d));
  textures_.Load("white_knight", chess_pieces, sf::IntRect(3 * d, 0, d, d));
  textures_.Load("black_knight", chess_pieces, sf::IntRect(3 * d, d, d, d));
  textures_.Load("white_rook", chess_pieces, sf::IntRect(4 * d, 0, d, d));
  textures_.Load("black_rook", chess_pieces, sf::IntRect(4 * d, d, d, d));
  textures_.Load("white_pawn", chess_pieces, sf::IntRect(5 * d, 0, d, d));
  textures_.Load("black_pawn", chess_pieces, sf::IntRect(5 * d, d, d, d));
}

void Game::ProcessInput() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::Closed:
        window_.close();
        break;
    }
  }
}

void Game::Update(sf::Time elapsedTime) {}

void Game::Render() {
  window_.clear(sf::Color::White);
  board_.SetPosition({90, 90});
  pieces_[0].setPosition(90, 90);
  window_.draw(board_);
  window_.draw(pieces_[0]);
  window_.display();
}
