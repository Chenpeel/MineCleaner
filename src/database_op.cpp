#include "database_op.h"

#include <QRegularExpression>
struct connectionInfo {
  const QString hostName = "localhost";
  const QString userName = "root";
  const QString password = "12345678";
  const QString databaseName = "minecleaner";
  const int port = 3306;
};

connectionInfo const connect;
DatabaseOp::DatabaseOp() {}

bool DatabaseOp::connectDatabase() {
  qDebug() << "Available drivers:" << QSqlDatabase::drivers();

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName(connect.hostName);
  db.setUserName(connect.userName);
  db.setPassword(connect.password);
  db.setPort(connect.port);
  db.setDatabaseName("minecleaner");

  if (!db.open()) {
    qDebug() << "Cannot open database";
    qDebug() << db.lastError().text();
    return false;
  } else {
    qDebug() << "Database opened successfully";
    return true;
  }
}

bool DatabaseOp::registerUser(User register_user) {
  if (connectDatabase()) {
    QSqlQuery query;
    if (isMail(register_user.email)) {
      query.prepare(
          "INSERT INTO User (username, email, password) VALUES (:username, "
          ":email, :password)");
      query.bindValue(":username", register_user.userName);
      query.bindValue(":email", register_user.email);
      query.bindValue(":password", register_user.password);
      if (query.exec()) {
        return true;
      } else {
        qDebug() << "User registration failed";
        qDebug() << query.lastError().text();
        return false;
      }
    } else {
      qDebug() << "Invalid email address";
      return false;
    }
    QSqlDatabase::removeDatabase("minecleaner");
  }
  return false;
}

bool DatabaseOp::loginUser(User login_user) {
  if (connectDatabase()) {
    QSqlQuery query;
    bool isEmail = isMail(login_user.email);
    if (isEmail) {
      query.prepare(
          "SELECT * FROM User WHERE email = :email AND password = :password");
      query.bindValue(":email", login_user.email);
      query.bindValue(":password", login_user.password);
    } else {
      query.prepare(
          "SELECT * FROM User WHERE username = :username AND password = "
          ":password");
      query.bindValue(":username", login_user.userName);
      query.bindValue(":password", login_user.password);
    }

    if (query.exec()) {
      if (query.next()) {
        // User found
        QSqlDatabase::removeDatabase("minecleaner");
        return true;
      } else {
        // User not found
        qDebug() << "User not found";
      }
    } else {
      // Query execution failed
      qDebug() << "Query failed:" << query.lastError().text();
    }

    QSqlDatabase::removeDatabase("minecleaner");
  } else {
    // Database connection failed
    qDebug() << "Database connection failed";
  }

  return false;
}

bool DatabaseOp::insertGameRecord(User to_record) {
  if (connectDatabase()) {
    if (loginUser(to_record)) {
      QSqlQuery query_user;
      query_user.prepare("SELECT * FROM User WHERE username = :username AND "
                         "password = :password");
      query_user.bindValue(":username", to_record.userName);
      query_user.bindValue(":password", to_record.password);
      if (query_user.exec()) {
        if (query_user.next()) {
          QSqlQuery query_record;
          query_record.prepare(
              "INSERT INTO GameRecord (username, levelInfo, completedTime, "
              "elapsedTime) VALUES (:username, :levelInfo, :completedTime, "
              ":elapsedTime)");
          query_record.bindValue(":username", to_record.userName);
          query_record.bindValue(":levelInfo", to_record.levelInfo);
          query_record.bindValue(":completedTime", to_record.completedTime);
          query_record.bindValue(":elapsedTime", to_record.elapsedTime);
          if (query_record.exec()) {
            return true;
          } else {
            qDebug() << "Game record insertion failed";
            qDebug() << query_record.lastError().text();
            return false;
          }
        } else {
          qDebug() << "User not found";
          return false;
        }
      } else {
        qDebug() << "User login failed";
        qDebug() << query_user.lastError().text();
        return false;
      }
    }
    QSqlDatabase::removeDatabase("minecleaner");
  }
  return false;
}

QVector<User> DatabaseOp::getGameRecord(User to_get) {
  QVector<User> game_records;
  if (connectDatabase()) {
    if (loginUser(to_get)) {
      QSqlQuery query;
      query.prepare("SELECT * FROM GameRecord WHERE username = :username AND "
                    "levelInfo = :levelInfo");
      query.bindValue(":username", to_get.userName);
      query.bindValue(":levelInfo", to_get.levelInfo);
      if (query.exec()) {
        while (query.next()) {
          User record;
          record.userName = query.value(0).toString();
          record.levelInfo = query.value(1).toString();
          record.completedTime = query.value(2).toString();
          record.elapsedTime = query.value(3).toInt();
          game_records.push_back(record);
        }
      } else {
        qDebug() << "Game record retrieval failed";
        qDebug() << query.lastError().text();
      }
    }
    QSqlDatabase::removeDatabase("minecleaner");
  }
  return game_records;
}

bool DatabaseOp::isMail(const QString &email) {
  QRegularExpression regex(
      R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
  QRegularExpressionMatch match = regex.match(email);
  return match.hasMatch();
}
