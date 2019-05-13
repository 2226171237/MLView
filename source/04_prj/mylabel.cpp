#include "mylabel.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPen>

#include <QDebug>
MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    list_len=0;
    current_class=1;
    draw_img=false;
    this->setBackgroundRole(QPalette::Light);
    this->setFixedWidth(570);
    this->setFixedHeight(570);
}
void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    int x,y;
    if (ev->button()==Qt::LeftButton)
    {
        x=ev->pos().x();
        y=ev->pos().y();
        //保存当前原始点
        current_p.setX(x);
        current_p.setY(y);

        list_len++; //数据量加1
        map[current_class].append(current_p);
        this->update();//发出重绘事件
        emit total_num_signal(list_len,x,y);
    }
}

void MyLabel::paintEvent(QPaintEvent *f)
{
    QLabel::paintEvent(f);
    QPainter p(this);
    QList<int> key_list;
    key_list=map.keys();
    //qDebug()<<"in panitEvent"<<key_list;
    if(draw_img)
        p.drawImage(QPoint(0,0),img);
    for(int i=0;i<key_list.length();i++)
    {
       p.setBrush(brush_list[key_list[i]-1]);
       for(int j=0;j<map[key_list[i]].length();j++)
       {
           p.drawEllipse(map[key_list[i]][j],5,5);
       }
    }
}


void MyLabel::addClass1()
{
    if(map.keys().count(1)==0)
    {
        map.insert(1,QList<QPoint>());
    }
    current_class=1;
}
void MyLabel::addClass2()
{
    if(map.keys().count(2)==0)
    {
        map.insert(2,QList<QPoint>());
    }
    current_class=2;
}
void MyLabel::addClass3()
{
    if(map.keys().count(3)==0)
    {
        map.insert(3,QList<QPoint>());
    }
    current_class=3;
}
void MyLabel::addClass4()
{
    if(map.keys().count(4)==0)
    {
        map.insert(4,QList<QPoint>());
    }
    current_class=4;
}
void MyLabel::addClass5()
{
    if(map.keys().count(5)==0)
    {
        map.insert(5,QList<QPoint>());
    }
    current_class=5;
}
