#include "InputDialog.h"

#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent), width(0), height(0) {
  widthEdit = new QLineEdit(this);
  heightEdit = new QLineEdit(this);

  QFormLayout *formLayout = new QFormLayout;
  formLayout->addRow(new QLabel("格宽[6,100]:"), widthEdit);
  formLayout->addRow(new QLabel("格高[4, 80]:"), heightEdit);

  QPushButton *okButton = new QPushButton("OK", this);
  connect(okButton, &QPushButton::clicked, this, &InputDialog::onOkClicked);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(formLayout);
  mainLayout->addWidget(okButton);

  setLayout(mainLayout);
}

InputDialog::~InputDialog() {}

int InputDialog::getWidth() const { return width; }

int InputDialog::getHeight() const { return height; }

void InputDialog::onOkClicked() {
  width = widthEdit->text().toInt();
  height = heightEdit->text().toInt();
  accept();
}
