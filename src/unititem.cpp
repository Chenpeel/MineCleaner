#include "unititem.h"

#include <QString>

int UnitItem::_refCount_ = 0;

QPixmap *UnitItem::_init_ = nullptr;
QPixmap *UnitItem::_flag_ = nullptr;
QPixmap *UnitItem::_puzzle_ = nullptr;
QPixmap *UnitItem::_empty_ = nullptr;
QPixmap *UnitItem::_num_ = nullptr;
QPixmap *UnitItem::_digit_[9] = {nullptr};
QPixmap *UnitItem::_explosive_ = nullptr;

UnitItem::UnitItem(int x, int y, enum unitState state, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent) {
  unit_x = x;
  unit_y = y;
  this->unit_state = state;
  if (!_refCount_) {
    // first init
    QString prefix = "../resource/img/";
    QString suffix = ".png";
    QString num;

    _init_ = new QPixmap(prefix + "init" + suffix);
    _flag_ = new QPixmap(prefix + "flag" + suffix);
    _puzzle_ = new QPixmap(prefix + "puzzle" + suffix);
    _empty_ = new QPixmap(prefix + "empty" + suffix);
    _num_ = new QPixmap(prefix + "num" + suffix);
    _explosive_ = new QPixmap(prefix + "explosive" + suffix);
    for (int i = 0; i < 9; i++) {
      QString name;
      name += prefix;
      name += num.setNum(i);
      name += suffix;
      _digit_[i] = new QPixmap(name);
    }
    setPixmap(*_init_);
    // end first init
  }
}

UnitItem::~UnitItem() {
  _refCount_--;
  if (!_refCount_) {
    delete _init_;
    delete _flag_;
    delete _puzzle_;
    delete _empty_;
    delete _num_;
    delete _explosive_;
    for (int i = 0; i < 9; i++) delete _digit_[i];
  }
}

enum unitState UnitItem::getState() { return unit_state; }

void UnitItem::setState(enum unitState state, int digit) {
  this->unit_state = state;
  switch (state) {
    case INIT:
      setPixmap(*_init_);
      break;
    case FLAG:
      setPixmap(*_flag_);
      break;
    case PUZZLE:
      setPixmap(*_puzzle_);
      break;
    case EMPTY:
      setPixmap(*_empty_);
      break;
    case NUM:
      setPixmap(*_num_);
      break;
    case DIGIT:
      setPixmap(*_digit_[digit]);
      break;
    case EXPLOSIVE:
      setPixmap(*_explosive_);
      break;

    default:
      break;
  }
  update();
}
