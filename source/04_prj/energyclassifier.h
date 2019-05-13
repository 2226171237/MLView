#ifndef ENERGYCLASSIFIER_H
#define ENERGYCLASSIFIER_H
#include <QList>
#include <QPointF>
#include <Eigen>
//基于位势函数非线性分类算法
using namespace Eigen;
class EnergyClassifier
{
public:
    EnergyClassifier();
    VectorXi fit(QList<QPointF>,QList<QPointF>,float beta=0.5,int max_iters=5000);
    QList<int> predictor(QList<QPointF>);
    VectorXd predictor(QList<QPointF>,MatrixXf,VectorXi,int class1=1,int class2=2);
    VectorXi x_times;
    MatrixXf data;
    float beta;
private:



};

#endif // ENERGYCLASSIFIER_H
