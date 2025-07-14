#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPushButton>
#include <QRegularExpression>
#include <QScreen>
#include <QStatusBar>
#include <QTimer>
#include <QVBoxLayout>
#include <iostream>
#include <queue>
#include <random>

#include "MyGraphicsScene.h"
#include "fielddata.h"
#include "unititem.h"

struct recordInfo {
  QString userName;
  int elapsedTime;
  QString level;
  QString realTime;
};

class GameScene : public QMainWindow {
  Q_OBJECT
public:
  GameScene(QWidget *parent = nullptr);
  ~GameScene();

signals:
  void back_level();
  void newGame(int width, int height);

public slots:
  void easy();
  void medium();
  void hard();
  void custom(int width, int height);

private:
  int unit_w;
  int unit_h;
  int map_width;
  int map_height;
  QGraphicsView *view;
  MyGraphicsScene *scene;
  QTimer *timer;
  QLabel *timeLabel;
  recordInfo record;
  void showMap();
  void startTimer();
  void updateTimer();
  void stopTimer();
  QString formatTime(int seconds);

private:
  UnitItem *uitem;
  UnitMatrix map;
  int remain_init;
  int remain_mines;
  bool isFirstLeftClick;
  void initializeMap();
  void empty_extand(double pos_x, double pos_y);
  void surround_extand(int row, int col, double pos_x, double pos_y);
  void youLose();
  bool isWin();
  bool isNumComputeCorrect(int N);
  void recordToLocal(recordInfo record);    // local record
  void recordTimeOnline(recordInfo record); // onlinesql record
  bool screenToGrid(double x, double y, int &row, int &col);
  UnitItem *getUnitItemAt(int row, int col);
  void revealAllMines();

protected:
  void mousePressEvent(QMouseEvent *event) override;
};
#endif
