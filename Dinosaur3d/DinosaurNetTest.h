#ifndef DINOSAURNETTEST_H
#define DINOSAURNETTEST_H

#include <QObject>
#include <QMap>

class DinosaurNetTest : public QObject
{
    Q_OBJECT
public:
    explicit DinosaurNetTest(QObject *parent = 0);


signals:

public slots:
    bool getNetworkState();
};

#endif // DINOSAURNETTEST_H
