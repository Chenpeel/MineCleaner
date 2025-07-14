#ifndef __MY_GRAPHICS_Scene__
#define __MY_GRAPHICS_Scene__

#include <QGraphicsScene>
#include <vector>
using std::vector;
#include "fielddata.h"
#include "unititem.h"

typedef vector<UnitItem *> UnitColumn;
typedef vector<UnitColumn> UnitMatrix;

class MyGraphicsScene : public QGraphicsScene {
 public:
  MyGraphicsScene(QObject *parent = Q_NULLPTR);
  ~MyGraphicsScene();
  UnitMatrix umarix;
  void initMap();
  void clearMap();
  void addItemToScene(QGraphicsItem *item);
};

#endif