#include "MyGraphicsScene.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include "fielddata.h"
#include "unititem.h"

MyGraphicsScene::MyGraphicsScene(QObject *parent) : QGraphicsScene(parent) {
  initMap();
}

MyGraphicsScene::~MyGraphicsScene() { clearMap(); }

void MyGraphicsScene::initMap() {
  int w = Field->getWidth();
  int h = Field->getHeight();
  for (int x = 0; x < w; ++x) {
    umarix.push_back(UnitColumn());
    for (int y = 0; y < h; ++y) {
      umarix[x].push_back(new UnitItem(x, y));
      static int cw = umarix[0][0]->boundingRect().width();
      static int ch = umarix[0][0]->boundingRect().height();
      umarix[x][y]->setPos(x * cw, y * ch);
      addItem(umarix[x][y]);
    }
  }
}
void MyGraphicsScene::clearMap() {
  int w = Field->getWidth();
  int h = Field->getHeight();
  for (int x = 0; x < w; ++x) {
    for (int y = 0; y < h; ++y) {
      delete umarix[x][y];
      umarix[x][y] = nullptr;
    }
  }
  umarix.clear();
}