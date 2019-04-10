#include "taskdialog.h"
#include "../library/json/jsonfunction.h"
#include "ui_taskdialog.h"

TaskDialog::TaskDialog(QWidget* parent) : QDialog(parent), ui(new Ui::TaskDialog)
{
    ui->setupUi(this);
    header << "user"
           << "index"
           << "result"
           << "time"
           << "memory"
           << "language"
           << "start_time";
    ui->taskTable->setColumnCount(header.size());
    ui->taskTable->setHorizontalHeaderLabels(header);
}

TaskDialog::~TaskDialog()
{
    delete ui;
}

void TaskDialog::addTask(const QJsonObject& json)
{
    for (auto& i : header) {
        Q_ASSERT(JsonFunction::hasKey(json, i));
    };
    auto row = ui->taskTable->rowCount();
    ui->taskTable->setRowCount(row + 1);
    for (int i = 0; i < header.size(); i++) {
        const auto& obj = json[header[i]];
        QString     item;
        if (obj.type() == QJsonValue::Type::String) {
            item = obj.toString();
        } else {
            item = QString::number(obj.toInt());
        }
        ui->taskTable->setItem(row, i, new QTableWidgetItem(item));
    }
}

void TaskDialog::closeEvent(class QCloseEvent* e)
{
    Q_UNUSED(e);
    this->hide();
}
