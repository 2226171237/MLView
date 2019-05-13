#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <QMap>
#include <QList>
#include <QPointF>
#include <QPoint>
#include "linearclassifier.h"
#include <Eigen>
#include "energyclassifier.h"
using namespace Eigen;

class Classifier
{
public:
    Classifier(QMap<int,QList<QPoint> >,int,int);
    ~Classifier();
    QMap<int,QList<QPointF>> x_train;//训练数据
    QString method;//方法
    int class_num;//类别数
    int wide,high;
    LinearClassifier *linear_method;
    QMap<QString,QList<float>> weight_map;//不同分类器的权重，字典 key:class1:class2->value:weight.
    EnergyClassifier *non_linear_method;
    QMap<QString,VectorXi> x_times_map;
public:
    void transformInputdata(QMap<int,QList<QPoint>>,int,int);//及那个输入数据归一化到[-1,1]

    void fit(QString method="logits");//训练

    QList<int> predicter(QList<QPoint>);//推断
};

#endif // CLASSIFIER_H
