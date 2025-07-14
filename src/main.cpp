#include <QApplication>
#include <QLocale>
#include <Qtranslator>

#include "menuscene.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "Mine Sweeper" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      a.installTranslator(&translator);
      break;
    }
  }
  MenuScene ms;
  QString iconPath = "../resource/img/icon/icon.png";
  a.setWindowIcon(QIcon(iconPath));
  ms.setWindowIcon(QIcon(iconPath));
  ms.show();
  return a.exec();
}