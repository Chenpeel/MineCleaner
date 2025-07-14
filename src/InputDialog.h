#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class InputDialog : public QDialog {
    Q_OBJECT

public:
    InputDialog(QWidget *parent = nullptr);
    ~InputDialog();

    int getWidth() const;
    int getHeight() const;

private slots:
    void onOkClicked();

private:
    QLineEdit *widthEdit;
    QLineEdit *heightEdit;
    int width;
    int height;
};

#endif // INPUTDIALOG_H
