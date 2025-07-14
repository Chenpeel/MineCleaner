#include "gamewindow.h"

#include "NumDialog.h"
#include "fielddata.h"
#include "unititem.h"

// 删除全局变量，改为类成员变量
struct Pos {
  double pos_x;
  double pos_y;
  Pos(double pos_x, double pos_y) : pos_x(pos_x), pos_y(pos_y) {};
};
struct Mpos {
  int row;
  int col;
  Mpos(int row, int col) : row(row), col(col) {};
};
//
GameScene::GameScene(QWidget *parent)
    : QMainWindow(parent), view(new QGraphicsView(this)),
      scene(new MyGraphicsScene(this)), timer(new QTimer(this)),
      timeLabel(new QLabel(this)), isFirstLeftClick(true) { // 初始化成员变量
  int initWidth = 1600, initHeight = 1300;
  this->setFixedSize(initWidth, initHeight);
  scene = new MyGraphicsScene;
  QString iconPath = "../resource/img/icon/icon.png";
  this->setWindowIcon(QIcon(iconPath));
  this->setWindowTitle("扫雷(All Hell's Mathematics!)");
  QPushButton *backButton = new QPushButton(this);
  backButton->setGeometry(QRect(10, 10, 15, 20));
  QString backIcon = "../resource/img/icon/back.png";
  backButton->setIcon(QIcon(backIcon));
  backButton->setIconSize(QSize(10, 10));
  QStatusBar *statusBar = this->statusBar();
  timeLabel = new QLabel(this);
  statusBar->addWidget(backButton);
  statusBar->addPermanentWidget(timeLabel);
  statusBar->setFixedHeight(35);
  this->setCentralWidget(view);
  view->setScene(scene);
  connect(backButton, &QPushButton::clicked, this,
          [=]() { emit this->back_level(); });
}

GameScene::~GameScene() {
  delete scene;
  delete view;
}

void GameScene::initializeMap() {
  scene = new MyGraphicsScene;
  Field->reset();
  remain_mines = Field->getMinesNum();
  remain_init = Field->getWidth() * Field->getHeight();
  isFirstLeftClick = true; // 重置首次点击标记
  view->setScene(scene);
  showMap();
}

void GameScene::showMap() {
  if (!scene->umarix.empty() && !scene->umarix[0].empty()) {
    unit_w = scene->umarix[0][0]->boundingRect().width();
    unit_h = scene->umarix[0][0]->boundingRect().height();
  }
  int map_width = (Field->getWidth() + 2) * unit_w;
  int map_height = (Field->getHeight() + 3) * unit_h;
  this->setFixedSize(map_width, map_height);
  this->show();
  startTimer();
}

void GameScene::easy() {
  record.level = "easy: 10*6";
  Field->setWidthHeight(10, 6);
  initializeMap();
}

void GameScene::medium() {
  record.level = "medium: 20*12";
  Field->setWidthHeight(20, 12);
  initializeMap();
}

void GameScene::hard() {
  record.level = "hard: 35*24";
  Field->setWidthHeight(35, 24);
  initializeMap();
}

void GameScene::custom(int width, int height) {
  record.level =
      "custom: " + QString::number(width) + "*" + QString::number(height);
  Field->setWidthHeight(width, height);
  initializeMap();
}

void GameScene::startTimer() {
  record.elapsedTime = 0;
  timeLabel->setText(formatTime(record.elapsedTime));
  // 设置定时器每秒触发一次
  timer->setInterval(1000);
  connect(timer, &QTimer::timeout, this, &GameScene::updateTimer);
  timer->start();
}

void GameScene::updateTimer() {
  record.elapsedTime++;
  timeLabel->setText(formatTime(record.elapsedTime));
}

void GameScene::stopTimer() {
  timer->stop();
  // 记录系统日期+时间
  QDateTime current_date_time = QDateTime::currentDateTime();
  QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
  record.realTime = current_date;
  recordToLocal(record);
}

QString GameScene::formatTime(int seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds % 3600) / 60;
  int secs = seconds % 60;
  return QString("%1:%2:%3")
      .arg(hours, 2, 10, QChar('0'))
      .arg(minutes, 2, 10, QChar('0'))
      .arg(secs, 2, 10, QChar('0'));
}

// 添加新方法，用于将屏幕坐标转换为网格坐标
bool GameScene::screenToGrid(double x, double y, int &row, int &col) {
  col = static_cast<int>(x / unit_w);
  row = static_cast<int>(y / unit_h);

  // 边界检查
  return (row >= 0 && row < Field->getHeight() && col >= 0 &&
          col < Field->getWidth());
}

// 添加新方法，安全地获取场景中的UnitItem
UnitItem *GameScene::getUnitItemAt(int row, int col) {
  if (row < 0 || row >= scene->umarix.size() || col < 0 ||
      col >= scene->umarix[0].size()) {
    return nullptr;
  }
  return scene->umarix[col][row]; // 注意坐标转换
}

// 添加方法，在游戏失败时显示所有地雷位置
void GameScene::revealAllMines() {
  for (int row = 0; row < Field->getHeight(); ++row) {
    for (int col = 0; col < Field->getWidth(); ++col) {
      if (Field->getMatrix()[row][col] == -1) {
        UnitItem *item = getUnitItemAt(row, col);
        if (item && item->getState() != EXPLOSIVE) {
          item->setState(EXPLOSIVE);
        }
      }
    }
  }
  scene->update();
}
/**
 * 鼠标事件
 * 左键 - 判断
 * 右键 - 标记
 */
void GameScene::mousePressEvent(QMouseEvent *event) {
  double x, y;
  x = event->position().x();
  y = event->position().y();

  // 使用安全的坐标转换
  int row, col;
  if (!screenToGrid(x, y, row, col)) {
    qDebug() << "点击位置不在游戏区域内";
    return;
  }

  qDebug() << "(" << x << "," << y << ")" << "---" << "(" << col << ", " << row
           << ")";

  // 获取点击的格子
  UnitItem *uitem = getUnitItemAt(row, col);
  if (uitem == nullptr) {
    qDebug() << "在位置(" << row << "," << col << ")找不到有效的格子";
    return;
  }

  // 处理左键点击
  if (event->button() == Qt::LeftButton) {
    Field->visited[row][col] = 1;

    // 获取当前格子的值（地雷为-1，数字为0-8）
    int N = Field->getMatrix()[row][col];

    if (INIT == uitem->getState()) {
      qDebug() << "格子值: " << N;

      // 处理踩雷的情况
      if (N == -1) {
        --remain_init;

        // 首次点击保护
        if (isFirstLeftClick) {
          do {
            initializeMap();
          } while (Field->getMatrix()[row][col] == -1); // 修复循环条件

          isFirstLeftClick = false;
          N = Field->getMatrix()[row][col];

          if (N == 0) {
            uitem->setState(EMPTY);
            empty_extand(x, y);
          } else if (N >= 1 && N <= 8) {
            uitem->setState(NUM);
            if (isNumComputeCorrect(N)) {
              uitem->setState(DIGIT, N);
            }
          }
        }
        // 非首次踩雷，游戏失败
        else {
          uitem->setState(EXPLOSIVE);
          revealAllMines(); // 显示所有地雷
          youLose();
          return;
        }
      }
      // 处理空白格子
      else if (N == 0) {
        --remain_init;
        isFirstLeftClick = false;
        uitem->setState(EMPTY);
        empty_extand(x, y);
        isWin();
        qDebug() << "空白格子";
      }
      // 处理数字格子
      else if (N >= 1 && N <= 8) {
        uitem->setState(NUM);
        isFirstLeftClick = false;

        // 数学题目判断
        if (isNumComputeCorrect(N)) {
          uitem->setState(DIGIT, N);
          --remain_init;
          isWin();
          qDebug() << "数字: " << N;
        } else {
          youLose();
          return;
        }
      } else {
        qDebug() << "错误的格子值: " << N;
      }
    }
    // 处理已显示为NUM状态的格子
    else if (NUM == uitem->getState()) {
      int N = Field->getMatrix()[row][col];
      qDebug() << "重新尝试NUM格子: " << N;

      if (isNumComputeCorrect(N)) {
        uitem->setState(DIGIT, N);
        --remain_init;
        isWin();
      }
    }
  }
  // 处理右键点击（标记地雷）
  else if (event->button() == Qt::RightButton) {
    if (INIT == uitem->getState()) {
      uitem->setState(PUZZLE);
      --remain_init;
      // 检查是否获胜
      isWin();
    } else if (PUZZLE == uitem->getState()) {
      uitem->setState(INIT);
      ++remain_init; // 取消标记，恢复计数
    }
  }
  scene->update();
}

// 递归扩展空白区域
void GameScene::empty_extand(double x, double y) {
  int row, col;
  if (!screenToGrid(x, y, row, col)) {
    return;
  }

  // 方向数组：上、左、右、下
  const int direction[4][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

  std::queue<std::pair<int, int>> que;
  que.push(std::make_pair(row, col));
  Field->visited[row][col] = 1;

  while (!que.empty()) {
    std::pair<int, int> cur = que.front();
    que.pop();
    int r = cur.first;
    int c = cur.second;

    for (int i = 0; i < 4; ++i) {
      int nr = r + direction[i][0]; // 新行
      int nc = c + direction[i][1]; // 新列

      // 检查新坐标是否在边界内且未访问过
      if (nr >= 0 && nr < Field->getHeight() && nc >= 0 &&
          nc < Field->getWidth() && Field->visited[nr][nc] == 0) {

        Field->visited[nr][nc] = 1; // 标记为已访问

        // 安全地获取对应的UnitItem
        UnitItem *uitem = getUnitItemAt(nr, nc);
        if (uitem != nullptr) {
          int N = Field->getMatrix()[nr][nc];
          if (N == 0) { // 如果是空白格子
            --remain_init;
            que.push(std::make_pair(nr, nc)); // 继续扩展
            uitem->setState(EMPTY);
          } else if (N >= 1 && N <= 8) { // 如果是数字格子
            uitem->setState(NUM);        // 只显示为NUM状态，不自动回答数学题
          }
        } else {
          qDebug() << "无法在(" << nr << "," << nc << ")找到有效的格子";
        }
      }
    }
  }
  scene->update();
}

// 修复周围扩展方法
void GameScene::surround_extand(int row, int col, double pos_x, double pos_y) {
  if (isFirstLeftClick) {
    empty_extand(pos_x, pos_y);
    return;
  }

  // 检查周围空白格子的数量
  int emptyCount = 0;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int nr = row + i;
      int nc = col + j;
      if (nr >= 0 && nr < Field->getHeight() && nc >= 0 &&
          nc < Field->getWidth()) {
        if (Field->getMatrix()[nr][nc] == 0) { // 修正这里，检查周围的格子
          ++emptyCount;
        }
      }
    }
  }

  // 如果周围有多个空白格子，有一定概率进行扩展
  if (emptyCount > 1) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    int randomValue = dis(gen);
    if (randomValue < 95) {
      empty_extand(pos_x, pos_y);
    }
  }
}

void GameScene::youLose() {
  stopTimer();
  QMessageBox::information(this, "Game Over", "You Lose");
}

bool GameScene::isWin() {
  // 胜利条件：剩余未点击的格子数量等于地雷数量
  if (remain_init == remain_mines) {
    stopTimer();
    QMessageBox::information(this, "You Win!", "\n用时: " + timeLabel->text());
    return true;
  }
  return false;
}

bool GameScene::isNumComputeCorrect(int N) {
  NumDialog numDialog(N, this);
  if (numDialog.exec() == QDialog::Accepted && numDialog.getResult()) {
    qDebug() << "数学题答对: " << N;
    return true;
  }
  return false;
}

void GameScene::recordToLocal(recordInfo record) {
  QString examineLoginInfoPath = "../data/loginInfo";
  QFile examineLoginFile(examineLoginInfoPath);
  if (examineLoginFile.exists()) {
    if (examineLoginFile.open(QIODevice::ReadOnly)) {
      QJsonObject json =
          QJsonDocument::fromJson(examineLoginFile.readAll()).object();
      record.userName = json["user"].toObject()["username/email"].toString();
    }
  }
  examineLoginFile.close();
  QString recordPath = "../data/record.txt";
  QFile recordFile(recordPath);
  if (recordFile.open(QIODevice::ReadWrite)) {
    QTextStream in(&recordFile);
    QString recordIn = in.readAll();
    recordIn.replace(QRegularExpression("^[^\n]+"),
                     record.userName + " " + record.realTime + " " +
                         record.level + " " +
                         QString::number(record.elapsedTime));
    recordFile.resize(0);
    in << recordIn;
    recordFile.close();
  }
  recordFile.close();
}
