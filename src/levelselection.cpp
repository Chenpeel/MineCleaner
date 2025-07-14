#include "levelselection.h"

#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#include "InputDialog.h"
#include "gamewindow.h"
#include "menuscene.h"

LevelScene::LevelScene(QWidget *parent) : QWidget(parent), game(nullptr) {
  setupUI();
  connect(back_menu_btn, &QPushButton::clicked, this, &LevelScene::back_menu);

  connect(btn[0], &QPushButton::clicked, this, [=]() {
    qDebug() << "Easy button clicked";
    QTimer::singleShot(50, this, [=]() { startNewGame(GameDifficulty::Easy); });
  });
  connect(btn[1], &QPushButton::clicked, this, [=]() {
    qDebug() << "Medium button clicked";
    QTimer::singleShot(50, this,
                       [=]() { startNewGame(GameDifficulty::Medium); });
  });
  connect(btn[2], &QPushButton::clicked, this, [=]() {
    qDebug() << "Hard button clicked";
    QTimer::singleShot(50, this, [=]() { startNewGame(GameDifficulty::Hard); });
  });
  connect(btn[3], &QPushButton::clicked, this, [=]() {
    qDebug() << "Custom button clicked";
    InputDialog dialog(this);
    while (dialog.exec() == QDialog::Accepted) {
      int width = dialog.getWidth();
      int height = dialog.getHeight();
      if (width > 7 && width <= 100 && height > 5 && height <= 80) {
        QTimer::singleShot(50, this, [=]() { startNewGame(width, height); });
        break;
      }
    }
  });
}

void LevelScene::setupUI() {
  this->setFixedSize(400, 330);
  this->setWindowTitle("扫雷(All Hell's Mathematics!)");
  this->setWindowIcon(QIcon("../resource/img/icon/icon.png"));

  back_menu_btn = new QPushButton(this);
  back_menu_btn->setIcon(QIcon("../resource/img/icon/back.png"));
  back_menu_btn->setStyleSheet(
      "QPushButton {"
      "background-color: #D7BEF7;"
      "color: white;"
      "border-radius: 10px;"
      "padding: 0px;"
      "}"
      "QPushButton:hover {"
      "background-color: #887C9F;"
      "}"
      "QPushButton:pressed {"
      "background-color: #1abc9c;"
      "}");
  back_menu_btn->setFixedSize(20, 20);
  back_menu_btn->move(10, 10);

  for (int i = 0; i < 4; ++i) {
    btn[i] = new QPushButton(QString("btn %1").arg(i), this);
    btn[i]->setFixedSize(160, 40);
    btn[i]->setStyleSheet(
        "QPushButton {"
        "background-color: #D7BEF7;"
        "color: white;"
        "border-radius: 7px;"
        "padding: 10px;"
        "}"
        "QPushButton:hover {"
        "background-color: #887C9F;"
        "}"
        "QPushButton:pressed {"
        "background-color: #1abc9c;"
        "}");
  }
  btn[0]->move(120, 70);
  btn[1]->move(120, 130);
  btn[2]->move(120, 190);
  btn[3]->move(120, 250);
  btn[0]->setText("简单");
  btn[1]->setText("中等");
  btn[2]->setText("偏上");
  btn[3]->setText("创造");
}

void LevelScene::connectSignals(GameScene *game) {
  qDebug() << "Connecting signals";
  connect(game, &GameScene::back_level, this, &LevelScene::onBackLevel);
  connect(game, &GameScene::newGame, this, &LevelScene::newGame);
}

void LevelScene::disconnectSignals(GameScene *game) {
  qDebug() << "Disconnecting signals";
  disconnect(game, &GameScene::back_level, this, &LevelScene::onBackLevel);
  disconnect(game, &GameScene::newGame, this, &LevelScene::newGame);
}

void LevelScene::startNewGame(GameDifficulty difficulty) {
  if (game) {
    disconnectSignals(game.get());
    game.reset();
  }
  game = std::make_unique<GameScene>(this);
  switch (difficulty) {
    case GameDifficulty::Easy:
      game->easy();
      break;
    case GameDifficulty::Medium:
      game->medium();
      break;
    case GameDifficulty::Hard:
      game->hard();
      break;
  }
  connectSignals(game.get());
  this->hide();
}

void LevelScene::startNewGame(int width, int height) {
  if (game) {
    disconnectSignals(game.get());
    game.reset();
  }
  game = std::make_unique<GameScene>(this);
  game->custom(width, height);
  connectSignals(game.get());
  this->hide();
}

void LevelScene::newGame(int width, int height) {
  qDebug() << "Starting new game with width:" << width << " height:" << height;
  startNewGame(width, height);
}

void LevelScene::onBackLevel() {
  qDebug() << "Back to level selection";
  if (game) {
    game->hide();
    game.reset();
  }
  this->show();
}

LevelScene::~LevelScene() {}
