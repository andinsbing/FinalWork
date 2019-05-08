#include "topicdialog.h"
#include "../library/json/jsonfunction.h"
#include "ui_topicdialog.h"
#include <QJsonObject>

TopicDialog::TopicDialog(QWidget* parent) : QDialog(parent), ui(new Ui::TopicDialog)
{
    ui->setupUi(this);
    connect(this->ui->topicIndexCombox,
            QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            [this](const QString& index) {
                if (!index.isEmpty()) {
                    this->setTopicIndex(index.toInt());
                }
            });
    connect(this->ui->sampleIndexComboBox,
            QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            [this](const QString& index) {
                if (!index.isEmpty()) {
                    this->setSampleIndex(index.toInt());
                }
            });
}

TopicDialog::~TopicDialog()
{
    delete ui;
}

void TopicDialog::updateTopic(const QJsonArray& array)
{
    topicArray = array;
    for (const auto& i : array) {
        const auto topic = i.toObject();
        Q_ASSERT(JsonFunction::hasKey(
            topic, { "title", "description", "time_limit", "memory_limit", "sample", "test" }));
        auto index = i["index"].toInt();
        topicMap.insert(index, topic);
    }
    this->ui->topicIndexCombox->clear();
    for (int i = 1; i <= array.size(); i++) {
        this->ui->topicIndexCombox->addItem(QString::number(i));
        //        this->ui->indexCombox->setItemText(i - 1, QString::number(i));
    }
    setTopicIndex(1);
}

void TopicDialog::closeEvent(QCloseEvent* e)
{
    Q_UNUSED(e);
    this->hide();
}

void TopicDialog::setTopicIndex(int index)
{
    this->ui->topicIndexCombox->setCurrentIndex(index - 1);
    Q_ASSERT(topicMap.contains(index));
    auto topic        = topicMap[index];
    auto title        = topic["title"].toString();
    auto time_limit   = topic["time_limit"].toInt();
    auto memory_limit = topic["memory_limit"].toInt();
    auto description  = topic["description"].toString();
    auto sampleArray  = topic["sample"].toArray();

    this->ui->titleLineEdit->setText(title);
    this->ui->timeLimitLineEdit->setText(QString::number(time_limit));
    this->ui->memoryLimitLineEdit->setText(QString::number(memory_limit));
    this->ui->decriptionTextEdit->setPlainText(description);

    this->ui->sampleIndexComboBox->clear();
    for (int i = 1; i <= sampleArray.size(); i++) {
        this->ui->sampleIndexComboBox->addItem(QString::number(i));
        //        this->ui->indexCombox->setItemText(i - 1, QString::number(i));
    }
    if (sampleArray.size() > 0) {
        this->setSampleIndex(1);
    } else {
        this->ui->inputTextEdit->clear();
        this->ui->outputTextEdit->clear();
    }
}

void TopicDialog::setSampleIndex(int index)
{
    this->ui->sampleIndexComboBox->setCurrentIndex(index - 1);
    auto topic       = topicMap[this->ui->topicIndexCombox->currentText().toInt()];
    auto sampleArray = topic["sample"].toArray();
    Q_ASSERT(sampleArray.size() >= index);
    auto sample = sampleArray.at(index - 1).toObject();
    Q_ASSERT(JsonFunction::hasKey(sample, { "input", "output" }));
    const auto& input  = sample["input"].toString();
    const auto& output = sample["output"].toString();
    this->ui->inputTextEdit->setText(input);
    this->ui->outputTextEdit->setText(output);
}
