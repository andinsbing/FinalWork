#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui
{
class InfoDialog;
}

class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDialog(QWidget* parent = nullptr);
    ~InfoDialog();
signals:
    void info(const QString& text);
public slots:
    void start();
private slots:
    void onInfo();

private:
    bool checkInfoText(const QString& text) const;

private:
    Ui::InfoDialog* ui;
};

#endif  // INFODIALOG_H
