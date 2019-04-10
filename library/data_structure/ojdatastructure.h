#ifndef OJDATASTRUCTURE_H
#define OJDATASTRUCTURE_H

#include <QByteArray>
#include <QString>
#include <QVector>

struct Topic
{
    QVector<qint32>  id;
    QVector<QString> title;
    QVector<QString> content;
    QVector<qint32>  time;    // seconds
    QVector<qint32>  memory;  // MB
    QVector<QString> exampleInput;
    QVector<QString> exampleOutput;
    QByteArray       toByte() const;
    static Topic     fromByte(const QByteArray& ba);
};

struct Test
{
    QVector<QString> input;
    QVector<QString> output;
    QByteArray       toByte() const;
    static Test      fromByte(const QByteArray& ba);
};

struct Exam
{
    QVector<Topic> topic;
    QVector<Test>  test;
    QByteArray     toByte() const;
    static Exam    fromByte(const QByteArray& ba);
};

struct TopicScore
{
    QVector<bool>     accept;
    QVector<qint32>   errorTime;
    QVector<qint32>   seconds;
    QByteArray        toByte() const;
    static TopicScore fromByte(const QByteArray& ba);
};

struct ExamRank
{
    QVector<QString> name;
    QVector<qint32>  rank;
    QVector<qint32>  time;
    QByteArray       toByte() const;
    static ExamRank  fromByte(const QByteArray& ba);
};

struct Task
{
    QString code;
    qint32  timeLimit;
    qint32  memoryLImit;
    QString language;
    qint32  index;
    qint32  time;
    double  memory;
    bool    isFinished;
};

// enum class Language : qint8
//{
//    Cpp,
//    Java,
//    C,
//    Python,
//};

// struct Task
//{
//    QString  code;
//    int      timeLimit;
//    int      memoryLImit;
//    Language language;
//    int      index;
//};

// enum class ResultType : qint8
//{
//    Accept,
//    WrongAnswer,
//    PresentationError,
//    TimeLimitError,
//    OutputLimitExceed,
//    MemoryLimitExceed,
//    RuntimeError,
//    CompileError,
//    CompileTimeLimitExceed,
//    JudgerInnerErrror,
//};

// struct Result
//{
//    ResultType type;
//    int        timeCost;
//    int        memoryCost;
//};

#endif  // OJDATASTRUCTURE_H
