#ifndef __FIELD_DATA_H__
#define __FIELD_DATA_H__
#include <vector>

#define Field (FieldData::getInstance())

using std::vector;
typedef vector<int> Column;
typedef vector<Column> Matrix;

class FieldData {
private:
  static FieldData *instance;
  Matrix matrix;

protected:
  int width;
  int height;
  int mines_num; // mines' number
  void updateSurrounding(int x, int y);

public:
  FieldData();
  ~FieldData();

  Matrix visited;
  static FieldData *getInstance();
  inline int getWidth() { return width; }
  inline int getHeight() { return height; }
  inline int getMinesNum() { return mines_num; }
  const Matrix &getMatrix() { return matrix; }

  void initMatrix();
  void initVisited();
  void deployMines();
  void reset();
  void setWidthHeight(int width, int height);
  // 添加辅助方法获取指定位置的值，包含边界检查
  int getValueAt(int row, int col);
};

#endif
