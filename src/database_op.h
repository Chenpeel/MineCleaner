#ifndef __DATABASE_OP_H__
#define __DATABASE_OP_H__

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QVector>

struct User {
  QString userName;
  QString email;
  QString password;
  QString levelInfo;
  QString completedTime;
  int elapsedTime;
};
class DatabaseOp {
 public:
  DatabaseOp();
  bool isMail(const QString &email);
  bool connectDatabase();
  bool registerUser(User register_user);
  bool loginUser(User login_user);
  bool insertGameRecord(User to_record);
  QVector<User> getGameRecord(User to_get);

 private:
  User user;
  QSqlDatabase db;
  QSqlQuery query;
};

#endif