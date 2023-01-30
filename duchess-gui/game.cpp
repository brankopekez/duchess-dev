#include "game.h"

#include <iostream>

const sf::Time Game::kTimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    : window_{sf::VideoMode(900, 900), "Duchess-GUI", sf::Style::Close},
      board_{nullptr},
      press_{false},
      drag_{false} {
  window_.setVerticalSyncEnabled(true);
  LoadTextures();
  BuildScene();

  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 0, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 1, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 2, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kQueen, 3, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKing, 4, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kBishop, 5, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kKnight, 6, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kRook, 7, 7, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 0, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 1, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 2, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 3, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 4, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 5, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 6, 6, textures_);
  //board_->NewPiece(Chessman::Color::kBlack, Chessman::Type::kPawn, 7, 6, textures_);

  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 0, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 1, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 2, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 3, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 4, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 5, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 6, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kPawn, 7, 1, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 0, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 1, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 2, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kQueen, 3, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKing, 4, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kBishop, 5, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kKnight, 6, 0, textures_);
  //board_->NewPiece(Chessman::Color::kWhite, Chessman::Type::kRook, 7, 0, textures_);
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
  std::unique_ptr<Chessman> p1 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kRook, textures_);
  std::unique_ptr<Chessman> p2 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kKnight, textures_);
  std::unique_ptr<Chessman> p3 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kBishop, textures_);
  std::unique_ptr<Chessman> p4 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kQueen, textures_);
  std::unique_ptr<Chessman> p5 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kKing, textures_);
  std::unique_ptr<Chessman> p6 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kBishop, textures_);
  std::unique_ptr<Chessman> p7 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kKnight, textures_);
  std::unique_ptr<Chessman> p8 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kRook, textures_);
  std::unique_ptr<Chessman> p9 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p10 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p11 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p12 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p13 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p14 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p15 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p16 =
      std::make_unique<Chessman>(Chessman::Color::kBlack, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p17 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p18 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p19 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p20 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p21 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p22 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p23 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p24 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kPawn, textures_);
  std::unique_ptr<Chessman> p25 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kRook, textures_);
  std::unique_ptr<Chessman> p26 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kKnight, textures_);
  std::unique_ptr<Chessman> p27 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kBishop, textures_);
  std::unique_ptr<Chessman> p28 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kQueen, textures_);
  std::unique_ptr<Chessman> p29 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kKing, textures_);
  std::unique_ptr<Chessman> p30 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kBishop, textures_);
  std::unique_ptr<Chessman> p31 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kKnight, textures_);
  std::unique_ptr<Chessman> p32 =
      std::make_unique<Chessman>(Chessman::Color::kWhite, Chessman::Type::kRook, textures_);

  board_->AssignPiece(0, 7, p1.get());
  board_->AssignPiece(1, 7, p2.get());
  board_->AssignPiece(2, 7, p3.get());
  board_->AssignPiece(3, 7, p4.get());
  board_->AssignPiece(4, 7, p5.get());
  board_->AssignPiece(5, 7, p6.get());
  board_->AssignPiece(6, 7, p7.get());
  board_->AssignPiece(7, 7, p8.get());
  board_->AssignPiece(0, 6, p9.get());
  board_->AssignPiece(1, 6, p10.get());
  board_->AssignPiece(2, 6, p11.get());
  board_->AssignPiece(3, 6, p12.get());
  board_->AssignPiece(4, 6, p13.get());
  board_->AssignPiece(5, 6, p14.get());
  board_->AssignPiece(6, 6, p15.get());
  board_->AssignPiece(7, 6, p16.get());
  board_->AssignPiece(0, 1, p17.get());
  board_->AssignPiece(1, 1, p18.get());
  board_->AssignPiece(2, 1, p19.get());
  board_->AssignPiece(3, 1, p20.get());
  board_->AssignPiece(4, 1, p21.get());
  board_->AssignPiece(5, 1, p22.get());
  board_->AssignPiece(6, 1, p23.get());
  board_->AssignPiece(7, 1, p24.get());
  board_->AssignPiece(0, 0, p25.get());
  board_->AssignPiece(1, 0, p26.get());
  board_->AssignPiece(2, 0, p27.get());
  board_->AssignPiece(3, 0, p28.get());
  board_->AssignPiece(4, 0, p29.get());
  board_->AssignPiece(5, 0, p30.get());
  board_->AssignPiece(6, 0, p31.get());
  board_->AssignPiece(7, 0, p32.get());

  scene_layers_[kPieces]->AttachChild(std::move(p1));
  scene_layers_[kPieces]->AttachChild(std::move(p2));
  scene_layers_[kPieces]->AttachChild(std::move(p3));
  scene_layers_[kPieces]->AttachChild(std::move(p4));
  scene_layers_[kPieces]->AttachChild(std::move(p5));
  scene_layers_[kPieces]->AttachChild(std::move(p6));
  scene_layers_[kPieces]->AttachChild(std::move(p7));
  scene_layers_[kPieces]->AttachChild(std::move(p8));
  scene_layers_[kPieces]->AttachChild(std::move(p9));
  scene_layers_[kPieces]->AttachChild(std::move(p10));
  scene_layers_[kPieces]->AttachChild(std::move(p11));
  scene_layers_[kPieces]->AttachChild(std::move(p12));
  scene_layers_[kPieces]->AttachChild(std::move(p13));
  scene_layers_[kPieces]->AttachChild(std::move(p14));
  scene_layers_[kPieces]->AttachChild(std::move(p15));
  scene_layers_[kPieces]->AttachChild(std::move(p16));
  scene_layers_[kPieces]->AttachChild(std::move(p17));
  scene_layers_[kPieces]->AttachChild(std::move(p18));
  scene_layers_[kPieces]->AttachChild(std::move(p19));
  scene_layers_[kPieces]->AttachChild(std::move(p20));
  scene_layers_[kPieces]->AttachChild(std::move(p21));
  scene_layers_[kPieces]->AttachChild(std::move(p22));
  scene_layers_[kPieces]->AttachChild(std::move(p23));
  scene_layers_[kPieces]->AttachChild(std::move(p24));
  scene_layers_[kPieces]->AttachChild(std::move(p25));
  scene_layers_[kPieces]->AttachChild(std::move(p26));
  scene_layers_[kPieces]->AttachChild(std::move(p27));
  scene_layers_[kPieces]->AttachChild(std::move(p28));
  scene_layers_[kPieces]->AttachChild(std::move(p29));
  scene_layers_[kPieces]->AttachChild(std::move(p30));
  scene_layers_[kPieces]->AttachChild(std::move(p31));
  scene_layers_[kPieces]->AttachChild(std::move(p32));
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
          auto piece_ptr =
              board_->PieceAt({(float)event.mouseButton.x, (float)event.mouseButton.y});
          if (piece_ptr != selected_piece_) {
            if (selected_piece_) {
              std::unique_ptr<SceneNode> p{scene_layers_[kAir]->DetachChild(*selected_piece_)};
              scene_layers_[kPieces]->AttachChild(std::move(p));
            }
            selected_piece_ = piece_ptr;
            std::unique_ptr<SceneNode> p{scene_layers_[kPieces]->DetachChild(*selected_piece_)};
            scene_layers_[kAir]->AttachChild(std::move(p));
            board_->PickUpPiece(selected_piece_);
          }
          press_ = selected_piece_ != nullptr;
          if (!press_) {
            board_->UnmarkAll();
          }
          //std::cout << event.mouseButton.x << ", " << event.mouseButton.y << std::endl;
        }
        break;
      case sf::Event::MouseMoved:
        if (press_) {
          drag_ = true;
          selected_piece_->setPosition(
              sf::Vector2f{(float)event.mouseMove.x, (float)event.mouseMove.y} -
              sf::Vector2f{selected_piece_->GetSize() / 2, selected_piece_->GetSize() / 2});
          //board_->MoveSelectedPiece({(float)event.mouseMove.x, (float)event.mouseMove.y});
          //std::cout << event.mouseMove.x << ", " << event.mouseMove.y << std::endl;
        }
        break;
      case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left) {
          press_ = false;
          if (drag_) {
            if (!board_->Move({(float)event.mouseButton.x, (float)event.mouseButton.y})) {
              board_->ResetSelectedSquare();
            } else {
              board_->UnmarkAll();
              std::unique_ptr<SceneNode> p{scene_layers_[kAir]->DetachChild(*selected_piece_)};
              scene_layers_[kPieces]->AttachChild(std::move(p));
              selected_piece_ = nullptr;
            }
            drag_ = false;
          }
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
