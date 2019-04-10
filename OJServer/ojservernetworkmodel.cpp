#include "ojservernetworkmodel.h"
#include "../library/file/dataaccess.h"
#include "../library/global_config/globalconfig.h"
#include "../library/json/jsonfunction.h"
#include "../library/network/networkidfunction.h"
#include "judger.h"
#include <QTimer>

OJServerNetworkModel::OJServerNetworkModel(QObject* parent)
    : NetworkModel(parent), judger(new Judger(this)), timer(new QTimer(this))
{
}

void OJServerNetworkModel::newPushEvent(const QJsonObject& json)
{
    Q_ASSERT(JsonFunction::hasKey(json, "data"));
    const auto& type = json["type"].toString();
    if (type == "task") {
        const auto& data = json["data"].toObject();
        if (JsonFunction::hasKey(data, "error")) {
            // not valid data, discard
        } else {
            onTaskReady(data);
        }
    } else if (type == "topic") {
        this->topic = json["data"].toObject();
        DataAccess().exportTopicTest(topic["array"].toArray(),
                                     Global::config["compiler_path"].toString() + R"(\Test)");

    } else {
        Q_UNREACHABLE();
    }
}

void OJServerNetworkModel::start()
{
    QJsonObject json;
    json.insert("action", "pull");
    json.insert("type", "topic");
    emit sendJsonToBCServer(json);
    json.insert("action", "push");
    json.insert("type", "ready");
    emit sendJsonToBCServer(json);
}

void OJServerNetworkModel::onTaskReady(const QJsonObject& task)
{
    QJsonObject json;
    json.insert("action", "push");
    json.insert("type", "task_result");
    auto result = judger->judge(task);
    JsonFunction::copy(json, result, { "time", "memory", "result", "task_id" });
    emit sendJsonToBCServer(json);
}
