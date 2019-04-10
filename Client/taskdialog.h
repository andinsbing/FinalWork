#ifndef TASKDIALOG_H
#define TASKDIALOG_H

#include <QDialog>

namespace Ui
{
class TaskDialog;
}

class TaskDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TaskDialog(QWidget* parent = nullptr);
    ~TaskDialog() override;

public slots:
    void addTask(const QJsonObject& json);

protected:
    void closeEvent(class QCloseEvent* e) override;

private:
    Ui::TaskDialog* ui;
    QStringList     header;
};

#endif  // TASKDIALOG_H
