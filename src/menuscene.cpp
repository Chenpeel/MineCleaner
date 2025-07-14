#include "menuscene.h"

#include "ui_menuscene.h"

MenuScene::MenuScene(QWidget* parent)
    : QMainWindow(parent), ui(new UI::MenuScene) {
  ui->setupUI(this);
  regOrLogin = new RegOrLogin;
  LevelScene* level = new LevelScene;
  isLogined = false;
  //
  QString IconPrefix = "../resource/img/icon/";
  setFixedSize(800, 610);
  QString iconPath = IconPrefix + "icon.png";
  setWindowIcon(QIcon(iconPath));
  setWindowTitle("扫雷(All Hell's Mathematics!)");
  // 背景
  QPixmap background(IconPrefix + "ms_bg.png");
  QPalette palette(this->palette());
  palette.setBrush(QPalette::Window, background);
  this->setPalette(palette);
  this->setAutoFillBackground(true);
  /**
   * btn0 : Register / Login , 如未登入默认使用 name = Player
   * btn1 : 开始 --> 选择难度
   * btn2 : 记录查询
   * btn3 : 退出
   * btn4 : 退出登陆
   */
  // 创建按钮

  for (int i = 0; i < 5; ++i) {
    btn[i] = new QPushButton(QString("btn %1").arg(i), this);
    btn[i]->setFixedSize(180, 40);
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
  //
  QString startIconPath = (QString)(IconPrefix + "start.png");
  btn[0]->setIcon(QIcon(startIconPath));
  btn[0]->setText("  开始吧！");
  btn[0]->move(150, 277);
  connect(btn[0], &QPushButton::clicked, this, [=]() {
    this->hide();
    level->show();
  });
  //
  QString login_register_IconPath = (QString)(IconPrefix + "icon.png");
  btn[1]->setIcon(QIcon(login_register_IconPath));
  btn[1]->setText(" 注册/登入");
  btn[1]->move(175, 350);
  connect(btn[1], &QPushButton::clicked, this, [=]() {
    this->hide();
    regOrLogin->show();
  });
  connect(regOrLogin, &RegOrLogin::back_menu, [=]() {
    QTimer::singleShot(50, this, [=]() {
      regOrLogin->hide();
      this->show();
    });
  });
  connect(regOrLogin, &RegOrLogin::loginSuccess, this,
          &MenuScene::loginSuccess);
  //
  QString settingIconPath = (QString)(IconPrefix + "setting.png");
  btn[2]->setIcon(QIcon(settingIconPath));
  btn[2]->setText("  游戏记录");
  btn[2]->move(200, 423);

  //
  QString exitIconPath = (QString)(IconPrefix + "exit.png");
  btn[3]->setIcon(QIcon(exitIconPath));
  btn[3]->setText("    退出  ");
  btn[3]->move(225, 496);
  connect(btn[3], &QPushButton::clicked, this, [=]() {
    QTimer::singleShot(50, this, [=]() { QCoreApplication::quit(); });
  });
  connect(level, &LevelScene::back_menu, [=]() {
    QTimer::singleShot(50, this, [=]() {
      level->hide();
      this->show();
    });
  });
  //
  QString logoutIconPath = (QString)(IconPrefix + "back.png");
  btn[4]->setIcon(QIcon(logoutIconPath));
  btn[4]->setText("  退出登陆");
  btn[4]->move(200, 423);
  btn[4]->hide();
  connect(btn[4], &QPushButton::clicked, this, [=]() { logoutClear(); });

  //
  user = examineLoginInfo();
}

/**json格式
 * {"user":{"username": "Chenpeel",
 *          "email": "nlxnbd@163.com",
 *          "password":"************"}}
 */
void MenuScene::loginSuccess(bool isLogined, User user) {
  regOrLogin->hide();
  this->show();
  this->isLogined = isLogined;
  this->user = user;

  QString loginInfoPath = "../data/loginInfo";
  QFile loginFile(loginInfoPath);
  if (loginFile.open(QIODevice::WriteOnly)) {
    QJsonObject json;
    QJsonObject userJson;
    userJson["username/email"] = user.userName;
    userJson["password"] = user.password;
    json["user"] = userJson;
    QJsonDocument doc(json);
    loginFile.write(doc.toJson());
    loginFile.close();
  }
  loadPage();
}

void MenuScene::loadPage() {
  if (isLogined) {
    btn[1]->hide();
    btn[2]->move(175, 350);
    btn[4]->show();

  } else {
    btn[1]->show();
    btn[2]->move(200, 423);
    btn[4]->hide();
  }
}

User MenuScene::examineLoginInfo() {
  User _user;
  QString loginInfoPath = "../data/loginInfo";
  QFile loginFile(loginInfoPath);
  if (loginFile.exists()) {
    if (loginFile.open(QIODevice::ReadOnly)) {
      QJsonObject json = QJsonDocument::fromJson(loginFile.readAll()).object();
      _user.userName = json["user"].toObject()["username/email"].toString();
      _user.email = json["user"].toObject()["username/email"].toString();
      _user.password = json["user"].toObject()["password"].toString();
      DatabaseOp dbo;
      if (dbo.loginUser(_user)) {
        isLogined = true;
        QFile recordFile("../data/record.txt");
        if (recordFile.open(QIODevice::ReadWrite)) {
          QTextStream in(&recordFile);
          QString record = in.readAll();
          record.replace(QRegularExpression("^[^\n]+"), _user.userName);
          recordFile.resize(0);
          in << record;
          recordFile.close();
        }
        loadPage();
      }
    }
  }
  return _user;
}

void MenuScene::logoutClear() {
  isLogined = false;
  user.userName = "Player";
  user.email = "";
  user.password = "";
  QString loginInfoPath = "../data/loginInfo";
  QFile loginFile(loginInfoPath);
  if (loginFile.exists()) {
    loginFile.remove();
    // 将record.txt的第一列全部改成Player
    QFile recordFile("../data/record.txt");
    if (recordFile.open(QIODevice::ReadWrite)) {
      QTextStream in(&recordFile);
      QString record = in.readAll();
      record.replace(QRegularExpression("^[^\n]+"), "Player");
      recordFile.resize(0);
      in << record;
      recordFile.close();
    }
  }
  loadPage();
}

MenuScene::~MenuScene() { delete ui; }
