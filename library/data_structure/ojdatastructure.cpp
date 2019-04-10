//#include "ojdatastructure.h"
//#include <QDataStream>

// const auto dsVersion = QDataStream::Qt_5_2;

// QByteArray Topic::toByte() const
//{
//    QByteArray  ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);
//    ds.setVersion(dsVersion);
//    ds << id << title << content << time << memory << exampleInput << exampleOutput;
//    return ba;
//}

// Topic Topic::fromByte(const QByteArray& ba)
//{
//    QDataStream ds(ba);
//    ds.setVersion(dsVersion);
//    Topic ret;
//    ds >> ret.id >> ret.time >> ret.content >> ret.time >> ret.memory >> ret.exampleInput
//        >> ret.exampleOutput;
//    return ret;
//}

// QByteArray Test::toByte() const
//{
//    QByteArray  ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);
//    ds.setVersion(dsVersion);
//    ds << input << output;
//    return ba;
//}

// Test Test::fromByte(const QByteArray& ba)
//{
//    QDataStream ds(ba);
//    ds.setVersion(dsVersion);
//    Test ret;
//    ds >> ret.input >> ret.output;
//    return ret;
//}

// QByteArray Exam::toByte() const
//{
//    QByteArray  ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);
//    ds.setVersion(dsVersion);
//    ds << topic << test;
//    return ba;
//}

// Exam Exam::fromByte(const QByteArray& ba)
//{
//    QDataStream ds(ba);
//    ds.setVersion(dsVersion);
//    Exam ret;
//    ds >> ret.topic >> ret.test;
//    return ret;
//}

// QByteArray TopicScore::toByte() const
//{
//    QByteArray  ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);
//    ds.setVersion(dsVersion);
//    ds << accept << errorTime << seconds;
//    return ba;
//}

// TopicScore TopicScore::fromByte(const QByteArray& ba)
//{
//    QDataStream ds(ba);
//    ds.setVersion(dsVersion);
//    TopicScore ret;
//    ds >> ret.accept >> ret.errorTime >> ret.seconds;
//    return ret;
//}

// QByteArray ExamRank::toByte() const
//{
//    QByteArray  ba;
//    QDataStream ds(&ba, QIODevice::WriteOnly);
//    ds.setVersion(dsVersion);
//    ds << name << rank << time;
//    return ba;
//}

// ExamRank ExamRankfromByte(const QByteArray& ba)
//{
//    QDataStream ds(ba);
//    ds.setVersion(dsVersion);
//    ExamRank ret;
//    ds >> ret.name >> ret.rank >> ret.time;
//    return ret;
//}
