#include "NumDialog.h"

#include <QMessageBox>
#include <random>
NumDialog::NumDialog(int correctNumber, QWidget *parent)
    : QDialog(parent), correctNumber(correctNumber), result(false) {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);

  // 创建一个 QLabel 来显示图片
  imageLabel = new QLabel(this);
  int rand_start = correctNumber * 10;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> code(rand_start, rand_start + 1);
  int res_pic_code = code(gen);
  QString prefix = "../resource/img/math_pic/";
  QString suffix = ".png";
  QString pic_path = prefix + QString::number(res_pic_code) + suffix;
  QPixmap pixmap(pic_path);
  // QDebug()<<"path: "<<pic_path;
  imageLabel->setPixmap(pixmap);
  mainLayout->addWidget(imageLabel);
  // imageLabel.setpos
  // 创建一个 QLineEdit 来输入数字
  inputField = new QLineEdit(this);
  inputField->setPlaceholderText("Enter the number");
  mainLayout->addWidget(inputField);

  // 创建确认按钮
  confirmButton = new QPushButton("Confirm", this);
  mainLayout->addWidget(confirmButton);

  connect(confirmButton, &QPushButton::clicked, this,
          &NumDialog::onConfirmButtonClicked);
}

void NumDialog::onConfirmButtonClicked() {
  bool ok;
  int userInput = inputField->text().toInt(&ok);
  if (ok && userInput == correctNumber) {
    result = true;
    accept();
  } else {
    QMessageBox::warning(this, "Error", "Incorrect number, please try again.");
  }
}
