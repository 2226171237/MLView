#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QList>
#include <QPoint>
#include <QPointF>
#include <QBrush>
#include <QMap>
class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);
    void addClass1();
    void addClass2();
    void addClass3();
    void addClass4();
    void addClass5();

signals:
    void total_num_signal(int num,int x,int y);

public slots:

public:
    int list_len;
    int current_class;//当前类
    QBrush brush_list[6]={QBrush(QColor(255,0,0)),QBrush(QColor(0,255,0)),
                          QBrush(QColor(0,0,255)),QBrush(QColor(255,255,0)),
                          QBrush(QColor(255,0,255)),QBrush(QColor(0,0,0))};
    QPoint current_p; //当前点击的坐标
    //QPoint line_begin,line_end;//得到的直线
    QImage img;//得到的图片,分类结果图
    bool draw_img;
    QMap<int,QList<QPoint>> map;//字典映射：（类别：样本点）
protected:
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *f);
};

#endif // MYLABEL_H
