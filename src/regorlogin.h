#ifndef REGORLOGIN_H
#define REGORLOGIN_H

#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpression>

#include "database_op.h"



class RegOrLogin : public QWidget {
  Q_OBJECT

 public:
  explicit RegOrLogin(QWidget *parent = nullptr);
  ~RegOrLogin();

 signals:
  void loginSuccess(bool isLogined, User user);
  void back_menu();

 private slots:
  void switchStatus();

 private:

  void clearWidgets();
  QLabel *usernameLabel;
  QLabel *passwordLabel;
  QLabel *emailLabel;
  QLineEdit *usernameLineEdit;
  QLineEdit *passwordLineEdit;
  QLineEdit *emailLineEdit;
  QPushButton *switchBtn;
  QPushButton *loginBtn;
  QPushButton *registerBtn;
  DatabaseOp *dbo;
  QVBoxLayout *mainLayout;

  bool isRegPage;
  bool isLogined;
  User user;
};

#endif  // REGORLOGIN_H
