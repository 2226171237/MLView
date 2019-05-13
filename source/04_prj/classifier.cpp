#include "classifier.h"
#include <QDebug>
#include <iostream>
using namespace std;
Classifier::Classifier(QMap<int,QList<QPoint> > x_data,int w,int h)
{
    transformInputdata(x_data,w,h);//归一化数据到[-1,1]
    this->wide=w;
    this->high=h;
    this->linear_method=NULL;
    this->non_linear_method=NULL;
}
void Classifier::fit(QString method)
{   this->method=method;
    if((method=="logits")||(method=="H-K"))
    {
        QList<float> linearWeights;//线性分类器的权值
        linear_method=new LinearClassifier(method);
        QList<int> key_list=x_train.keys();
        for(int i=0;i<key_list.length()-1;i++)
        {
            for(int j=i+1;j<key_list.length();j++)
            {
                linearWeights=linear_method->fit(x_train[key_list[i]],x_train[key_list[j]]);
                QString str=QString("%1:%2").arg(key_list[i]).arg(key_list[j]);
                this->weight_map.insert(str,linearWeights);
            }
        }
    }
    else
    {
        VectorXi x_times;
        non_linear_method =new EnergyClassifier;
        QList<int> key_list=x_train.keys();
        for(int i=0;i<key_list.length()-1;i++)
        {
            for(int j=i+1;j<key_list.length();j++)
            {
                x_times=non_linear_method->fit(x_train[key_list[i]],x_train[key_list[j]]);
                QString str=QString("%1:%2").arg(key_list[i]).arg(key_list[j]);
                this->x_times_map.insert(str,x_times);
            }
        }
        //std::cout<<"fit 1:2 "<<this->x_times_map["1:2"]<<std::endl;
        //std::cout<<"1:3 "<<this->x_times_map["1:3"]<<std::endl;
        //std::cout<<"2:3 "<<this->x_times_map["2:3"]<<std::endl;
    }

}
QList<int> Classifier::predicter(QList<QPoint> x_data)
{
    QList<QPointF> X;
    QList<int> y_hat;
    float x,y;
    for(int i=0;i<x_data.length();i++)
    {
        x=x_data[i].x();
        y=x_data[i].y();
        X.append(QPointF((2.0*x-wide)/wide,(2.0*y-high)/high));
    }
    MatrixXd vote;
    if ((this->method=="logits")||(this->method=="H-K"))
    {
        QList<QString> key_list=this->weight_map.keys();
        MatrixXd vote1(key_list.length(),X.length());
        for(int i=0;i<key_list.length();i++)
        {
            int class1=key_list[i].split(':')[0].toInt();
            int class2=key_list[i].split(':')[1].toInt();
            vote1.row(i)=this->linear_method->predictor(X,this->weight_map[key_list[i]],class1,class2);
        }
        vote=vote1;
    }
    else
    {
        QList<QString> key_list=this->x_times_map.keys();
        //qDebug()<<"key_list"<<key_list;
        //std::cout<<"pre 1:2 "<<this->x_times_map["1:2"]<<std::endl;
        MatrixXd vote2(key_list.length(),X.length());
        MatrixXf x_store;
        for(int i=0;i<key_list.length();i++)
        {
            int class1=key_list[i].split(':')[0].toInt();
            int class2=key_list[i].split(':')[1].toInt();
            x_store=MatrixXf(x_train[class1].length()+x_train[class2].length(),2);
            for(int k=0;k<x_train[class1].length();k++)
            {
                x_store(k,0)=x_train[class1][k].x();
                x_store(k,1)=x_train[class1][k].y();
            }
            for(int k=0;k<x_train[class2].length();k++)
            {
                x_store(k+x_train[class1].length(),0)=x_train[class2][k].x();
                x_store(k+x_train[class1].length(),1)=x_train[class2][k].y();
            }
            vote2.row(i)=this->non_linear_method->predictor(X,x_store,this->x_times_map[key_list[i]],class1,class2);
        }
        vote=vote2;
    }
    //std::cout<<"vote ："<<vote<<std::endl;
    for(int i=0;i<vote.cols();i++)
    {
        QList<int> t;
        for(int j=0;j<vote.rows();j++)
        {
            t.append(vote(j,i));
        }
        int max=0;
        int c=1;
        QList<int> c_l=x_train.keys();
        for(int k=0;k<c_l.length();k++)
        {
            int n=t.count(c_l[k]);
            if(!(n<max))
            {
                max=n;
                c=c_l[k];
            }
        }
        y_hat.append(c);
    }
    return y_hat;
}

void Classifier::transformInputdata(QMap<int, QList<QPoint> > x_data,int w,int h)
{
    QList<int> key_list;
    float x,y;
    key_list=x_data.keys();
    class_num=key_list.length();
    for(int i=0;i<class_num;i++)
    {
        this->x_train.insert(key_list[i],QList<QPointF>());
        for(int j=0;j<x_data[key_list[i]].length();j++)
        {
            x=x_data[key_list[i]][j].x();
            y=x_data[key_list[i]][j].y();
            x_train[key_list[i]].append(QPointF((2.0*x-w)/w,(2.0*y-h)/h));
        }
    }
}
Classifier::~Classifier()
{
    if(this->linear_method)
    {
        delete this->linear_method;
        this->linear_method=NULL;
     }
    if(this->non_linear_method)
    {
        delete this->non_linear_method;
        this->non_linear_method=NULL;
    }
}
