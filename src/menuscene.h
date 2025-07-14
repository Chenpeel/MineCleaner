#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include <QCoreApplication>
#include <QCryptographicHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>

#include "gamewindow.h"
#include "levelselection.h"
#include "regorlogin.h"

QT_BEGIN_NAMESPACE
namespace UI {
class MenuScene;
}
QT_END_NAMESPACE

class MenuScene : public QMainWindow {
  Q_OBJECT
 public:
  MenuScene(QWidget *parent = nullptr);
  ~MenuScene();

 private:
  UI::MenuScene *ui;
  QPushButton *btn[5];
  RegOrLogin *regOrLogin;
  void logoutClear();
  User examineLoginInfo();
  void loadPage();
  bool isLogined;
  User user;

 public slots:
  void loginSuccess(bool isLogined, User user);
};

#endif  // __MENU_SCENE_H__
