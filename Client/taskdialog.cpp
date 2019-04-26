#include "taskdialog.h"
#include "../library/json/jsonfunction.h"
#include "ui_taskdialog.h"
#include <QTime>

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent), ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    header << "user"
           << "index"
           << "result"
           << "time"
           << "memory"
           << "language"
           << "start_time"
           << "end_time"
           << "cost_time";
    ui->taskTable->setColumnCount(header.size());
    ui->taskTable->setHorizontalHeaderLabels(header);
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

void TaskDialog::addTask(const QJsonObject& json)
{
    //    for (auto& i : header) {
    //        Q_ASSERT(JsonFunction::hasKey(json, i));
    //    };
    auto row = ui->taskTable->rowCount();
    ui->taskTable->setRowCount(row + 1);
    for (int i = 0; i < header.size() - 1; i++) {
        const auto& obj = json[header[i]];
        QString     item;
        if (obj.type() == QJsonValue::Type::String) {
            item = obj.toString();
        } else {
            item = QString::number(obj.toInt());
        }
        ui->taskTable->setItem(row, i, new QTableWidgetItem(item));
    }
    auto submitTime = QTime::fromString(ui->taskTable->item(row, header.size() - 3)->text());
    auto endTime    = QTime::fromString(ui->taskTable->item(row, header.size() - 2)->text());
    auto timeCost   = submitTime.secsTo(endTime);
    ui->taskTable->setItem(row, header.size() - 1, new QTableWidgetItem(QString::number(timeCost)));
}

void TaskDialog::closeEvent(class QCloseEvent* e)
{
    Q_UNUSED(e);
    this->hide();
}
