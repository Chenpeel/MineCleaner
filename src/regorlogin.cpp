#include "regorlogin.h"

#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QRegularExpression>
#include <QTimer>

RegOrLogin::RegOrLogin(QWidget *parent) : QWidget(parent) {
  /** defalut is login only two label to input
   * first login(use username or email) and second password
   */
  // set window
  setFixedSize(800, 610);
  // set background
  isRegPage = false;
  QPixmap background("../resource/img/icon/ms_bg.png");
  QPalette palette(this->palette());
  palette.setBrush(QPalette::Window, background);
  this->setPalette(palette);
  this->setAutoFillBackground(true);
  this->setWindowTitle("Login");

  this->setWindowTitle("Login");
  usernameLabel = new QLabel("Username/Email", this);
  usernameLabel->setGeometry(130, 277, 100, 30);
  emailLabel = new QLabel("Email", this);
  emailLabel->setFixedSize(100, 30);
  emailLabel->hide();
  emailLineEdit = new QLineEdit(this);
  emailLineEdit->setFixedSize(150, 30);
  emailLineEdit->hide();
  passwordLabel = new QLabel("Password", this);
  passwordLabel->setGeometry(130, 327, 100, 30);
  usernameLabel->setStyleSheet("QLabel { color: black; }");
  passwordLabel->setStyleSheet("QLabel { color: black; }");
  emailLabel->setStyleSheet("QLabel { color: black; }");
  usernameLineEdit = new QLineEdit(this);
  usernameLineEdit->setGeometry(240, 277, 150, 30);
  passwordLineEdit = new QLineEdit(this);
  passwordLineEdit->setGeometry(240, 327, 150, 30);
  passwordLineEdit->setEchoMode(QLineEdit::Password);

  QPushButton *back_menu_btn = new QPushButton(this);
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
  connect(back_menu_btn, &QPushButton::clicked, this,
          [=]() { emit this->back_menu(); });

  switchBtn = new QPushButton("去注册", this);
  switchBtn->setGeometry(140, 480, 100, 40);
  switchBtn->setStyleSheet(
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
  switchBtn->setCursor(Qt::PointingHandCursor);
  loginBtn = new QPushButton("登录", this);
  loginBtn->setGeometry(280, 480, 100, 40);
  loginBtn->setStyleSheet(
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

  registerBtn = new QPushButton("注册", this);
  registerBtn->setGeometry(280, 480, 100, 40);
  registerBtn->setStyleSheet(
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
  registerBtn->hide();

  connect(loginBtn, &QPushButton::clicked, this, [=]() {
    user.userName = usernameLineEdit->text();
    user.email = usernameLineEdit->text();
    user.password = passwordLineEdit->text();
    usernameLineEdit->clear();
    emailLineEdit->clear();
    passwordLineEdit->clear();
    if (dbo->loginUser(user)) {
      isRegPage = false;
      isLogined = true;
      emit loginSuccess(isLogined, user);
    }
  });

  connect(switchBtn, &QPushButton::clicked, this, [=]() {
    isRegPage = !isRegPage;
    switchStatus();
  });
}

RegOrLogin::~RegOrLogin() {
  delete usernameLabel;
  delete passwordLabel;
  delete emailLabel;
  delete usernameLineEdit;
  delete passwordLineEdit;
  delete emailLineEdit;
  delete switchBtn;
  delete loginBtn;
  delete registerBtn;
  delete dbo;
}

void RegOrLogin::switchStatus() {
  if (isRegPage) {
    this->setWindowTitle("Register");
    switchBtn->setText("去登录");
    usernameLabel->setText("Username");
    usernameLineEdit->move(240, 277);
    emailLabel->show();
    emailLineEdit->show();
    emailLabel->move(130, 327);
    emailLineEdit->move(240, 327);
    passwordLabel->move(130, 377);
    passwordLineEdit->move(240, 377);
    loginBtn->hide();
    registerBtn->show();
    connect(registerBtn, &QPushButton::clicked, this, [=]() {
      user.userName = usernameLineEdit->text();
      user.email = emailLineEdit->text();
      user.password = passwordLineEdit->text();
      usernameLineEdit->clear();
      emailLineEdit->clear();
      passwordLineEdit->clear();
      if (dbo->registerUser(user)) {
        isRegPage = false;
        QLabel *tmp_label =
            new QLabel("Register Success!\n Please Login", this);
        tmp_label->setGeometry(350, 10, 100, 30);
        tmp_label->show();
        QTimer::singleShot(500, tmp_label, &QLabel::deleteLater);
        switchStatus();
      }
    });
  } else {
    this->setWindowTitle("Login");
    usernameLineEdit->move(240, 277);
    emailLabel->hide();
    emailLineEdit->hide();
    registerBtn->hide();
    loginBtn->show();
    passwordLabel->move(130, 327);
    passwordLineEdit->move(240, 327);
    switchBtn->setText("去注册");
    connect(loginBtn, &QPushButton::clicked, this, [=]() {
      user.userName = usernameLineEdit->text();
      user.email = usernameLineEdit->text();
      user.password = passwordLineEdit->text();
      usernameLineEdit->clear();
      emailLineEdit->clear();
      passwordLineEdit->clear();
      if (dbo->loginUser(user)) {
        isRegPage = false;
        isLogined = true;
        emit loginSuccess(isLogined, user);
      }
    });
  }
  this->update();
}
