#ifndef __NUM_DIALOG_H__
#define __NUM_DIALOG_H__

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class NumDialog : public QDialog {
    Q_OBJECT

public:
    explicit NumDialog(int correctNumber, QWidget *parent = nullptr);

    bool getResult() const { return result; }

private slots:
    void onConfirmButtonClicked();

private:
    int correctNumber;
    bool result;
    QLineEdit *inputField;
    QLabel *imageLabel;
    QPushButton *confirmButton;
};

#endif