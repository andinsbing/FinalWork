#include "sqlcontroller.h"
#include "../library/file/dataaccess.h"
#include "../library/json/jsonfunction.h"
#include "sqlconnector.h"
#include <QBitArray>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlResult>
#include <QtDebug>

SQLController::SQLController(QObject* parent) : QObject(parent), connector(new SQLConnector(this))
{
}

void SQLController::addTopic(const QJsonObject& json)
{
    Q_ASSERT(JsonFunction::hasKey(
        json, { "title", "description", "time_limit", "memory_limit", "test", "sample" }));
    auto title        = json["title"].toString();
    auto description  = json["description"].toString();
    auto time_limit   = json["time_limit"].toInt();
    auto memory_limit = json["memory_limit"].toInt();
    auto test         = json["test"].toArray();
    auto sample       = json["sample"].toArray();

    auto command =
        QString(
            R"(INSERT INTO acm.topic (title, description, time_limit,memory_limit,test, sample) VALUES ('%1','%2', %3, %4, '%5', '%6'))")
            .arg(title, description, QString::number(time_limit), QString::number(memory_limit),
                 QString::fromUtf8(QJsonDocument(test).toJson(QJsonDocument::JsonFormat::Compact)),
                 QString::fromUtf8(
                     QJsonDocument(sample).toJson(QJsonDocument::JsonFormat::Compact)));

    QSqlQuery query;
    query.prepare(command);
    bool ok = query.exec();
    if (!ok) {
        qDebug() << query.lastQuery();
        qDebug() << query.lastError();
    }
    Q_ASSERT(ok);
}

void SQLController::getContest()
{
    auto contestList = getTable("acm.contest", "`enable`=1");
    Q_ASSERT(contestList.size() == 1);
    auto contest      = contestList[0].toObject();
    bool ok           = contest["topic"].isUndefined();
    ok                = contest["topic"].isString();
    ok                = contest["topic"].isArray();
    const auto& topic = JsonFunction::parseJsonArray(contest["topic"].toString());

    QJsonArray topicArray;
    for (int i = 0; i < topic.size(); i++) {
        const auto& obj   = topic[i].toObject();
        auto        index = obj["index"].toInt();
        auto        id    = obj["id"].toInt();
        auto        temp  = getTopic(id);
        temp.insert("index", index);
        temp["test"]   = JsonFunction::parseJsonArray(temp["test"].toString());
        temp["sample"] = JsonFunction::parseJsonArray(temp["sample"].toString());
        topicArray.append(temp);
    }
    Q_ASSERT(topicArray.size() != 0);
    emit updateContest(contest);
    emit updateTopic({ { "array", topicArray } });
    emit updateUser(getUser());
}

QJsonObject SQLController::getTopic(int id)
{
    auto result = getTable("acm.topic", "`id` = " + QString::number(id));
    Q_ASSERT(result.size() == 1);
    return result[0].toObject();
}

QJsonArray SQLController::readTable(const QString& command)
{
    QJsonObject json;
    QSqlQuery   query;
    query.prepare(command);
    bool ok = query.exec();
    Q_ASSERT(ok);
    const auto&    record = query.record();
    QList<QString> field;
    int            fieldCount = record.count();
    for (int i = 0; i < fieldCount; i++) {
        field.push_back(record.fieldName(i));
    }
    QJsonArray subArray;
    while (query.next()) {
        QJsonObject subJson;
        for (int i = 0; i < fieldCount; i++) {
            switch (record.field(i).type()) {
            case QVariant::Type::Int:
            case QVariant::Type::UInt:
                subJson.insert(field[i], query.value(i).toInt());
                break;
            case QVariant::Type::Bool:
                subJson.insert(field[i], query.value(i).toBool());
                break;
            case QVariant::Type::Double:
                subJson.insert(field[i], query.value(i).toDouble());
                break;
            case QVariant::Type::String:
                subJson.insert(field[i], query.value(i).toString());
                break;
            case QVariant::Type::DateTime:
                subJson.insert(field[i], query.value(i).toDateTime().toString());
                break;
            default:
                qDebug() << record.field(i).type() << "not found";
                Q_UNREACHABLE();
            }
        }
        subArray.append(subJson);
    }
    return subArray;
}

QJsonArray SQLController::getTable(const QString& tableName)
{
    return readTable("SELECT * from " + tableName);
}

QJsonObject SQLController::getUser()
{
    return { { "array", getTable("acm.team") } };
}

QJsonArray SQLController::getTable(const QString& tableName, const QString& where)
{
    return readTable("SELECT * from " + tableName + " where " + where);
}

void SQLController::start()
{
    connector->start();
}
