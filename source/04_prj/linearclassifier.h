#ifndef LINEARCLASSIFIER_H
#define LINEARCLASSIFIER_H
#include <QList>
#include <QPointF>
#include <Eigen>
//H-K算法实现线性分类器。
using namespace Eigen;
class LinearClassifier
{
public:
    LinearClassifier(QString method="H-K");
    ~LinearClassifier();

    //训练函数
    QList<float> fit(QList<QPointF>,QList<QPointF>,float learning_rate=0.1,float eps=1e-5,int max_iters=3000);
    //预测函数
    QList<int> predictor(QList<QPointF>);
    VectorXd predictor(QList<QPointF>,QList<float>,int class1=1,int class2=2);

private:
    float generateRand(float,float);//生成随机数[min,max]
    QList<float> H_K(MatrixXf,float,float,int);//H-K算法
    QList<float> Logits_reg(MatrixXf,VectorXf,float,float,int);//逻辑回归

public:
    QList<float> weights;//存放权值
    QString method;//分类方法
};

#endif // LINEARCLASSIFIER_H
