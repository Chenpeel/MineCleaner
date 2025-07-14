#ifndef __UNITITEM_H__
#define __UNITITEM_H__

/**
 * 单位格子的图片映射
 */
#include <QGraphicsPixmapItem>
#include <QPixmap>

enum unitState {
  INIT,
  FLAG,
  PUZZLE,
  EMPTY,
  NUM,
  DIGIT,
  EXPLOSIVE
};

class UnitItem : public QGraphicsPixmapItem {
 protected:
  int unit_x;
  int unit_y;
  enum unitState unit_state;

 public:
  UnitItem(int x, int y, enum unitState state= INIT,
           QGraphicsItem* parent = Q_NULLPTR);
  ~UnitItem();

  unitState getState();
  void setState(enum unitState state, int digit = -2);

  static QPixmap* _init_;
  static QPixmap* _flag_;
  static QPixmap* _puzzle_;
  static QPixmap* _empty_;
  static QPixmap* _num_;
  static QPixmap* _digit_[9];
  static QPixmap* _explosive_;

  static int _refCount_;
  
};


#endif  // __UNITITEM_H__