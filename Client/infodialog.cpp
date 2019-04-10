#include "infodialog.h"
#include "ui_infodialog.h"
#include <QMessageBox>

InfoDialog::InfoDialog(QWidget* parent) : QDialog(parent), ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &InfoDialog::onInfo);
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::start()
{
    this->ui->InfoTextEdit->clear();
    this->show();
}

void InfoDialog::onInfo()
{
    const auto& text = ui->InfoTextEdit->toPlainText();
    if (!checkInfoText(text)) {
        QMessageBox::information(this, "info", "information fromat error");
    } else {
        emit info(text);
        this->hide();
        QMessageBox::information(this, "info", "submit info successfully");
    }
}

bool InfoDialog::checkInfoText(const QString& text) const
{
    return text.length() > 0;
}
