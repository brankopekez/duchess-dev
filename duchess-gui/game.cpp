#include "game.h"

#include <iostream>

const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() : window_{}, board_{nullptr}, press_{false}, drag_{false}, pick_{false} {
  // Enable anti-aliasing
  sf::ContextSettings settings;
  settings.antialiasingLevel = 8;

  // Create window and enable vertical sync
  window_.create(sf::VideoMode(900, 900), "Duchess-GUI", sf::Style::Close, settings);
  window_.setVerticalSyncEnabled(true);

  LoadTextures();
  BuildScene();
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

void Game::LoadTextures(bool smooth) {
  const std::string chess_pieces = "Media/Textures/chess_pieces.png";
  textures_.Load(Textures::kChessPieces, chess_pieces);
  textures_.Smooth(smooth);
}

void Game::BuildScene() {
  // Initialize the different layers
  for (std::size_t i = 0; i < kLayerCount; ++i) {
    std::unique_ptr<SceneNode> layer(new SceneNode());
    scene_layers_[i] = layer.get();

    scene_graph_.AttachChild(std::move(layer));
  }

  // Prepare the chessboard
  std::unique_ptr<Chessboard> b{new Chessboard(720)};
  b->setPosition({90, 90});
  board_ = b.get();
  scene_layers_[kBoard]->AttachChild(std::move(b));

  // Add pieces
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 0, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 1, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 2, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kQueen, 3, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKing, 4, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 5, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 6, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 7, 7, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 0, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 1, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 2, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 3, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 4, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 5, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 6, 6, textures_);
  board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 7, 6, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 0, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 1, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 2, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 3, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 4, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 5, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 6, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 7, 1, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 0, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 1, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 2, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kQueen, 3, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKing, 4, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 5, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 6, 0, textures_);
  board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 7, 0, textures_);
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
          board_->Pick({(float)event.mouseButton.x, (float)event.mouseButton.y});
          //std::cout << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
        }
        break;
      case sf::Event::MouseMoved:
        if (board_->IsPiecePicked() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          board_->Drag({(float)event.mouseMove.x, (float)event.mouseMove.y});
          //std::cout << event.mouseMove.x << ", " << event.mouseMove.y << std::endl;
        }
        break;
      case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left) {
          board_->Release({(float)event.mouseButton.x, (float)event.mouseButton.y});
        }
        break;
    }
  }
}

void Game::Update(sf::Time elapsedTime) {}

void Game::Render() {
  window_.clear(sf::Color::White);
  window_.draw(scene_graph_);
  window_.display();
}
