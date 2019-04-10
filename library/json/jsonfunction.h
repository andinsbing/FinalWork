#ifndef JSONFUNCTION_H
#define JSONFUNCTION_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <initializer_list>

namespace JsonFunction
{

bool hasKey(const QJsonObject& json, const QString& key);
bool hasKey(const QJsonObject& json, const std::initializer_list<QString>& keyList);
bool obeyConstraint(const QJsonObject& json, const QString& key);
bool obeyConstraint(const QJsonObject& json, const std::initializer_list<QString>& keyList);
void copy(QJsonObject& des, const QJsonObject& src, const QString& key);
void copy(QJsonObject& des, const QJsonObject& src, const std::initializer_list<QString>& keyList);
void remove(QJsonObject& json, const QString& key);
void remove(QJsonObject& json, const std::initializer_list<QString>& keyList);
QJsonArray  parseJsonArray(const QString& str);
QJsonObject parseJsonObject(const QString& str);

}  // namespace JsonFunction

#endif  // JSONFUNCTION_H
