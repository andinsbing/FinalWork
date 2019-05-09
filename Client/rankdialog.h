#ifndef RANKDIALOG_H
#define RANKDIALOG_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QQueue>
#include <set>

struct RankData
{
    explicit RankData(int topicSize)
    {
        for (int i = 0; i <= topicSize; i++) {
            submitCount.append(0);
            isAC.append(false);
            ErrorCount.append(0);
        }
    }
    qint32        id;
    QString       name;
    QList<qint32> submitCount;
    QList<qint32> ErrorCount;
    QList<bool>   isAC;
    qint32        totalTime = 0;
    qint32        ACCount   = 0;
    bool          operator<(const RankData& r) const
    {
        if (this->ACCount != r.ACCount) {
            return this->ACCount > r.ACCount;
        }
        if (this->totalTime != r.totalTime) {
            return this->totalTime < r.totalTime;
        }
        return this->id < r.id;
    }
};

namespace Ui
{
class RankDialog;
}

class RankDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RankDialog(QWidget* parent = nullptr);
    ~RankDialog() override;

public slots:
    void updateTopic(const QJsonArray& array);
    void addTask(const QJsonObject& json);

protected:
    void closeEvent(class QCloseEvent* e) override;

private slots:
    void updataTable();

private:
    Ui::RankDialog*    ui;
    qint32             topicCount;
    QMap<int, int>     map;
    std::set<RankData> rankSet;
};

#endif  // RANKDIALOG_H
