#include "energyclassifier.h"
#include <QDebug>
#include <iostream>
#include <QTime>
using namespace std;
QList<int> randdata(int max);
EnergyClassifier::EnergyClassifier()
{
    beta=0.5;
}
VectorXi EnergyClassifier::fit(QList<QPointF> X_1,QList<QPointF> X_2,float beta,int max_iters)
{
    this->beta=beta;
    int x1_num=X_1.length();
    int x2_num=X_2.length();
    int sample_num=x1_num+x2_num;
    VectorXi x_get_num_temp=VectorXi::Zero(sample_num);
    VectorXi x_get_num=VectorXi::Zero(sample_num);

    QList<int> randDataIndex=randdata(sample_num);
    QList<QPointF> X_temp;
    X_temp.append(X_1);
    X_temp.append(X_2);
    MatrixXf X(sample_num,2);
    for(int i=0;i<sample_num;i++)
    {
        X(i,0)=X_temp[randDataIndex[i]].x();
        X(i,1)=X_temp[randDataIndex[i]].y();
    }
    data=X;
    int Nc=0;
    int k=0;
    float K_x=0;
    while(k<max_iters)//k<max_iters
    {
        for(int i=0;i<sample_num;i++)
        {
            for(int n=0;n<x_get_num_temp.rows();n++)
            {
                int m=x_get_num_temp(n);
                K_x+=m*exp(-beta*(X.row(i)-X.row(n)).squaredNorm());
            }
            if(randDataIndex[i]<x1_num)
            {
                if(K_x>0)
                    Nc++;
                else
                   {
                        x_get_num_temp(i)++;
                        x_get_num(randDataIndex[i])++;
                        Nc=0;
                    }
            }
            else
            {
                if (K_x<0)
                    Nc++;
                else
                {
                    x_get_num_temp(i)--;
                    x_get_num(randDataIndex[i])--;
                    Nc=0;
                }
            }
            K_x=0;
            if (!(Nc<sample_num))
                break;
        }
        //qDebug()<<Nc;
        if (!(Nc<sample_num))
           break;
        k++;
    }
    this->x_times=x_get_num;
    return x_get_num;
}
QList<int> EnergyClassifier::predictor(QList<QPointF> X_data)
{
    int sample_num=X_data.length();
    MatrixXf X(sample_num,2);
    QList<int> y_hat;
    for(int i=0;i<sample_num;i++)
    {
        X(i,0)=X_data[i].x();
        X(i,1)=X_data[i].y();
    }
    float K_x=0;
    for(int i=0;i<sample_num;i++)
    {
        for(int j=0;j<this->x_times.rows();j++)
        {
           int m=this->x_times[j];
           K_x+=m*exp(-beta*(X.row(i)-this->data.row(j)).squaredNorm());
        }
        if(K_x>0)
            y_hat.append(1);
        else
            y_hat.append(-1);
        K_x=0.;
    }

    return y_hat;
}
VectorXd EnergyClassifier::predictor(QList<QPointF> X_data,MatrixXf x_train,VectorXi x_times,int class1,int class2)
{
    int sample_num=X_data.length();
    MatrixXf X(sample_num,2);
    VectorXd y_hat(sample_num);
    for(int i=0;i<sample_num;i++)
    {
        X(i,0)=X_data[i].x();
        X(i,1)=X_data[i].y();
    }
    float K_x=0;
    for(int i=0;i<sample_num;i++)
    {
        for(int j=0;j<x_times.rows();j++)
        {
           int m=x_times[j];
           K_x+=m*exp(-beta*(X.row(i)-x_train.row(j)).squaredNorm());
        }
        //std::cout<<"K_x"<<K_x<<std::endl;
        if(K_x>0)
            y_hat(i)=class1;
        else
            y_hat(i)=class2;
        K_x=0.;
    }
    //std::cout<<"X \n"<<X<<std::endl;
    //std::cout<<"y_hat\n"<<y_hat<<std::endl;
    return y_hat;
}
QList<int> randdata(int max)
{
    int i,j;
    QList<int> numbersList;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    for(i=0;i<max;i++)
    {
        numbersList.append(qrand()%max);
        bool flag=true;
        while(flag)
        {
            for(j=0;j<i;j++)
            {
                if(numbersList[i]==numbersList[j])
                {
                    break;
                }
            }
            if(j<i)
            {
                numbersList[i]=rand()%max;
            }
            if(j==i)
            {
                flag=!flag;
            }
        }
    }
    return numbersList;
}
