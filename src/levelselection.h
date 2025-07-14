#ifndef __LEVEL__SELECTION_H__
#define __LEVEL__SELECTION_H__

#include <QWidget>
#include <memory>

#include "gamewindow.h"

enum class GameDifficulty { Easy, Medium, Hard };
class LevelScene : public QWidget {
  Q_OBJECT

 public:
  explicit LevelScene(QWidget *parent = nullptr);
  ~LevelScene();

 signals:
  void back_menu();
 public slots:
  void newGame(int width, int height);
  void onBackLevel();

 private:
  void setupUI();
  void startNewGame(GameDifficulty difficulty);
  void startNewGame(int width, int height);
  void connectSignals(GameScene *game);
  void disconnectSignals(GameScene *game);
  std::unique_ptr<GameScene> game;
  QPushButton *back_menu_btn;
  QPushButton *btn[4];
};

#endif  // LEVELSELECTION_H
