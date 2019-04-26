#include "submitdialog.h"
#include "ui_submitdialog.h"
#include <QMessageBox>
#include <QPushButton>

SubmitDialog::SubmitDialog(QWidget* parent) : QDialog(parent), ui(new Ui::SubmitDialog)
{
    ui->setupUi(this);
    defaultText = ui->codeTextEdit->toPlainText();
    connect(ui->pushButton, &QPushButton::clicked, this, &SubmitDialog::onSubmit);
    ui->timeText->hide();
    ui->timeLabel->hide();
}

SubmitDialog::~SubmitDialog()
{
    delete ui;
}

void SubmitDialog::start()
{
    if (this->isHidden()) {
        this->ui->codeTextEdit->setPlainText(this->defaultText);
        this->show();
    }
}

void SubmitDialog::onSubmit()
{
    const auto& code = ui->codeTextEdit->toPlainText();
    if (!checkCode(code)) {
        QMessageBox::information(this, "error", "code cannot be empty");
    } else {
        auto time = ui->timeText->text().toInt();
        Q_ASSERT(time > 0);
        for (int i = 0; i < time; i++) {
            emit submit(1, code, "CPP");
        }
        this->hide();
    }
}

bool SubmitDialog::checkCode(const QString& code)
{
    return code.length() > 0;
}
