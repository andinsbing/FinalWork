#include "rankdialog.h"
#include "../library/json/jsonfunction.h"
#include "ui_rankdialog.h"

RankDialog::RankDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RankDialog)
{
    ui->setupUi(this);
}

RankDialog::~RankDialog()
{
    delete ui;
}

void RankDialog::updateTopic(const QJsonArray& array)
{
    QStringList list;
    list << "User";
    for (int i = 1; i <= array.size(); i++) {
        list << QString("Topic ") + QString::number(i);
    }
    topicCount = array.size();
    list << "Total Time";
    list << "AC Count";
    ui->tableWidget->setColumnCount(list.size());
    ui->tableWidget->setHorizontalHeaderLabels(list);
}

void RankDialog::addTask(const QJsonObject& json)
{
    Q_ASSERT(JsonFunction::hasKey(json, { "user_id", "user", "time", "index", "result" }));
    auto user_id = json["user_id"].toInt();
    auto user    = json["user"].toString();
    auto time    = json["time"].toInt();
    auto result  = json["result"].toString();
    bool isAC    = result == "Accept";
    auto index   = json["index"].toInt();

    auto it = rankSet.begin();
    while (it != rankSet.end() && it->id != user_id) {
        it++;
    }
    if (it == rankSet.end()) {
        RankData data(topicCount);
        data.id          = user_id;
        data.name        = user;
        data.isAC[index] = isAC;
        data.submitCount[index] += 1;
        if (isAC) {
            data.totalTime += time;
            data.ACCount += 1;
            data.totalTime += data.ErrorCount[index] * 20 * 60 * 1000;  // add 20 minutes per error
        } else {
            data.ErrorCount[index] += 1;
        }
        rankSet.insert(data);
    } else {
        auto data = *it;
        rankSet.erase(it);
        data.submitCount[index] += 1;
        if (isAC && !data.isAC[index]) {
            data.totalTime += time;
            data.ACCount += 1;
            data.isAC[index] = true;
            data.totalTime += data.ErrorCount[index] * 20 * 60 * 1000;  // add 20 minutes per error
        } else if (!isAC) {
            data.ErrorCount[index] += 1;
        }
        rankSet.insert(data);
    }
    updataTable();
}

void RankDialog::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e);
    this->hide();
}

void RankDialog::updataTable()
{
    ui->tableWidget->setRowCount(static_cast<int>(this->rankSet.size()));
    int row = 0;
    for (auto& data : rankSet) {
        int col = 0;
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(data.name));
        for (int i = 1; i <= topicCount; i++) {
            auto it = new QTableWidgetItem(QString::number(data.submitCount[i]));
            ui->tableWidget->setItem(row, col++, it);
            if (data.isAC[i]) {
                it->setTextColor(QColor(200, 20, 20));
            } else {
                it->setTextColor(QColor(20, 30, 200));
            }
        }
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(data.totalTime)));
        ui->tableWidget->setItem(row, col++, new QTableWidgetItem(QString::number(data.ACCount)));
        row++;
    }
}
