#include "jsonfunction.h"
#include <QJsonDocument>

bool JsonFunction::hasKey(const QJsonObject& json, const std::initializer_list<QString>& keyList)
{
    for (const auto& i : keyList) {
        if (!hasKey(json, i)) {
            return false;
        }
    }
    return true;
}

bool JsonFunction::hasKey(const QJsonObject& json, const QString& key)
{
    return json.find(key) != json.end();
}

bool JsonFunction::obeyConstraint(const QJsonObject& json, const QString& key)
{
    return json.size() == 1 && hasKey(json, key);
}

bool JsonFunction::obeyConstraint(const QJsonObject&                    json,
                                  const std::initializer_list<QString>& keyList)
{
    return static_cast<size_t>(json.size()) == keyList.size() && hasKey(json, keyList);
}

void JsonFunction::copy(QJsonObject& des, const QJsonObject& src, const QString& key)
{
    Q_ASSERT(hasKey(src, key));
    des[key] = src[key];
}

void JsonFunction::copy(QJsonObject& des, const QJsonObject& src,
                        const std::initializer_list<QString>& keyList)
{
    for (const auto& i : keyList) {
        copy(des, src, i);
    }
}

void JsonFunction::remove(QJsonObject& json, const QString& key)
{
    Q_ASSERT(hasKey(json, key));
    json.remove(key);
}

void JsonFunction::remove(QJsonObject& json, const std::initializer_list<QString>& keyList)
{
    for (const auto& i : keyList) {
        remove(json, i);
    }
}

QJsonArray JsonFunction::parseJsonArray(const QString& str)
{
    const auto& doc = QJsonDocument::fromJson(str.toUtf8());
    Q_ASSERT(doc.isArray());
    return doc.array();
}

QJsonObject JsonFunction::parseJsonObject(const QString& str)
{
    const auto& doc = QJsonDocument::fromJson(str.toUtf8());
    Q_ASSERT(doc.isObject());
    return doc.object();
}
