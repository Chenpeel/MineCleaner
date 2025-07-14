#include "fielddata.h"
#include <iostream>
#include <random>

FieldData *FieldData::instance = nullptr;
FieldData *FieldData::getInstance() {
  if (instance == nullptr)
    instance = new FieldData();
  return instance;
}

// 修复析构函数中的递归删除问题
FieldData::~FieldData() {
  // 避免递归删除
  if (this == instance) {
    instance = nullptr;
  }
}

FieldData::FieldData() : width(0), height(0), mines_num(0) { reset(); }

void FieldData::reset() {
  matrix.clear();
  initMatrix();
  deployMines();
  visited.clear();
  initVisited();
}

void FieldData::initMatrix() {
  matrix.resize(height); // 注意这里修改为height，表示行数
  for (int i = 0; i < height; i++) {
    matrix[i].resize(width); // 每行有width个元素（列数）
  }
}

void FieldData::deployMines() {
  // 初始化所有格子为0
  for (int i = 0; i < height; i++) {  // 行
    for (int j = 0; j < width; j++) { // 列
      matrix[i][j] = 0;
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> pos_x(0, width - 1);
  std::uniform_int_distribution<> pos_y(0, height - 1);
  // 地雷比例 0.08 - 0.15
  std::uniform_int_distribution<> num((int)width * height * 0.08,
                                      (int)width * height * 0.15);
  mines_num = num(gen);
  int n = mines_num;
  while (n > 0) {
    int x = pos_x(gen);       // 列
    int y = pos_y(gen);       // 行
    if (matrix[y][x] != -1) { // 注意这里用[y][x]，与坐标系保持一致
      matrix[y][x] = -1;
      n--;
      updateSurrounding(x, y);
    } else {
      continue;
    }
  }
}

void FieldData::initVisited() {
  visited.resize(height); // 注意这里修改为height，表示行数
  for (int i = 0; i < height; i++) {
    visited[i].resize(width); // 每行有width个元素（列数）
  }
  for (int i = 0; i < height; i++) {  // 行
    for (int j = 0; j < width; j++) { // 列
      visited[i][j] = 0;
    }
  }
}

void FieldData::setWidthHeight(int width, int height) {
  this->width = width;
  this->height = height;
}

void FieldData::updateSurrounding(int x, int y) {
  // Define the 8 directions
  int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                          {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  // Iterate over all directions
  for (int i = 0; i < 8; ++i) {
    int nx = x + directions[i][0];
    int ny = y + directions[i][1];

    // Check if (nx, ny) is within bounds
    if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
      if (matrix[ny][nx] != -1) { // 注意这里用[ny][nx]，与坐标系保持一致
        matrix[ny][nx]++;
      }
    }
  }
}

// 添加新方法，安全地获取矩阵值，包含边界检查
int FieldData::getValueAt(int row, int col) {
  if (row >= 0 && row < height && col >= 0 && col < width) {
    return matrix[row][col];
  }
  return -2; // 返回一个特殊值表示越界
}
