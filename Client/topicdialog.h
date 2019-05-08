#ifndef TOPICDIALOG_H
#define TOPICDIALOG_H

#include <QDialog>
#include <QJsonArray>
#include <QMap>

namespace Ui
{
class TopicDialog;
}

class TopicDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopicDialog(QWidget* parent = nullptr);
    ~TopicDialog() override;

public slots:
    void updateTopic(const QJsonArray& array);

protected:
    void closeEvent(class QCloseEvent* e) override;

private:
    void setTopicIndex(int index);
    void setSampleIndex(int index);

private:
    Ui::TopicDialog*       ui;
    QJsonArray             topicArray;
    QMap<int, QJsonObject> topicMap;
};

#endif  // TOPICDIALOG_H
