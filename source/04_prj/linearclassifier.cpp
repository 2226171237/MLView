#include "linearclassifier.h"
#include <QTime>
#include <iostream>
#include <QDebug>
using namespace std;
bool isStop(VectorXf e_res,float eps);

LinearClassifier::LinearClassifier(QString method)
{
    this->method=method;
    this->weights.append(generateRand(-0.5,0.5));//初始化w1
    this->weights.append(generateRand(-0.5,0.5)); //初始化w2
    this->weights.append(0.0);//初始化b

}
QList<float> LinearClassifier::fit(QList<QPointF> X_1, QList<QPointF> X_2,float learning_rate,float eps,int max_iters)
{
    int x1_num=X_1.length();
    int x2_num=X_2.length();
    int sample_nums=x1_num+x2_num;
    MatrixXf X(sample_nums,3);
    VectorXf label(sample_nums);
    for(int i=0;i<x1_num;i++)
    {
        X(i,0)=X_1[i].x();
        X(i,1)=X_1[i].y();
        X(i,2)=1.0;
        label(i)=1.0;
    }
    if (this->method=="H-K")
    {
        for(int i=0;i<x2_num;i++)
        {
            X(i+x1_num,0)=-X_2[i].x();
            X(i+x1_num,1)=-X_2[i].y();
            X(i+x1_num,2)=-1.0;
            label(i+x1_num)=-1.0;
        } 
        this->weights=this->H_K(X,learning_rate,eps,max_iters);
    }
    else if(this->method=="logits")
    {
        for(int i=0;i<x2_num;i++)
        {
            X(i+x1_num,0)=X_2[i].x();
            X(i+x1_num,1)=X_2[i].y();
            X(i+x1_num,2)=1.0;
            label(i+x1_num)=-1.0;
        }
        this->weights=this->Logits_reg(X,label,learning_rate,eps,max_iters);
    }

    return this->weights;
}
QList<int> LinearClassifier::predictor(QList<QPointF> X_data)
{
    int x_num=X_data.length();
    MatrixXf X(x_num,3);
    VectorXf y_hat(x_num);
    Vector3f w(weights[0],weights[1],weights[2]);
    QList<int> y;
    for(int i=0;i<x_num;i++)
    {
        X(i,0)=X_data[i].x();
        X(i,1)=X_data[i].y();
        X(i,2)=1.0;
    }
    y_hat=X*w;
    for(int i=0;i<x_num;i++)
    {
        if (y_hat(i)>0)
           y.append(1);
        else
           y.append(-1);
    }
    return y;
}
VectorXd LinearClassifier::predictor(QList<QPointF> X_data,QList<float> weights,int class1,int class2)
{
    int x_num=X_data.length();
    MatrixXf X(x_num,3);
    VectorXf y_hat(x_num);
    Vector3f w(weights[0],weights[1],weights[2]);
    VectorXd y(x_num);
    for(int i=0;i<x_num;i++)
    {
        X(i,0)=X_data[i].x();
        X(i,1)=X_data[i].y();
        X(i,2)=1.0;
    }
    y_hat=X*w;
    for(int i=0;i<x_num;i++)
    {
        if (y_hat(i)>0 ||y_hat(i)==0)
           y(i)=class1;
        else
           y(i)=class2;
    }
    return y;
}
QList<float> LinearClassifier::H_K(MatrixXf X,float learning_rate,float eps,int max_iters)
{
    MatrixXf inv_X=(X.transpose()*X).inverse()*X.transpose();
    int k=0;
    int cols=inv_X.cols();
    VectorXf b(cols);
    VectorXf e(cols);
    VectorXf e_res(cols);
    VectorXf W(3);
    for(int i=0;i<cols;i++)
    {
        b(i)=0.1;
    }
    W=inv_X*b;
    while(true)
    {
        if (k>max_iters) break;
        e=X*W-b;
        if(isStop(e,eps)) break;//各分量全接近为0，算法结束，线性可分
        if (e.minCoeff()>eps) break;//各分量全大于等于0，算法收敛，线性不可分，得到最优解
        e_res=e+VectorXf(e.array().abs());
        if(isStop(e_res,eps)) break; //各分量停止变为正值，算法结束，线性不可分。
        W+=learning_rate*inv_X*e_res;
        b+=learning_rate*e_res;
        k++;
    }
    QList<float> weight;
    weight.append((float)W(0));
    weight.append((float)W(1));
    weight.append((float)W(2));
    return weight;
}
bool isStop(VectorXf e_res,float eps)
{
    for(int i=0;i<e_res.rows();i++)
    {
        if (fabs(e_res(i))>eps)
        {
            return false;
        }
    }
    return true;
}
QList<float> LinearClassifier::Logits_reg(MatrixXf X, VectorXf label,float learning_rate, float eps,int max_iters)
{
    Vector3f grad_w(0.,0.,0.);
    Vector3f W=Vector3f::Random();
    int k=0;
    while(k<max_iters)
    {
       ArrayXf temp=(1.0/(1.0+(label.array()*(X*W).array()).exp()))*label.array();
       for(int i=0;i<3;i++)
       {
           grad_w(i)=(temp*X.col(i).array()).sum()/X.rows();
       }

       if (grad_w.norm()<100*eps) break;
       W+=learning_rate*grad_w;
       k++;
    }
    QList<float> weight;
    weight.append((float)W(0));
    weight.append((float)W(1));
    weight.append((float)W(2));
    return weight;
}

float LinearClassifier::generateRand(float min, float max)
{
    static bool seedStatus;
    if (!seedStatus)
    {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    if(min>max)
    {
        float temp=min;
        min=max;
        max=temp;
    }
    float diff = fabs(max-min);
    float m1=(float)(qrand()%100)/100;
    float retval=min+m1*diff;
    return retval;
}
LinearClassifier::~LinearClassifier()
{
}
