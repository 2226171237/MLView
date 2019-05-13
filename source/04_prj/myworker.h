#ifndef MYWORKER_H
#define MYWORKER_H

#include <QObject>
#include "linearclassifier.h"

class MyWorker : public QObject
{
    Q_OBJECT
public:
    explicit MyWorker(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYWORKER_H
