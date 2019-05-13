#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDebug>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedWidth(800);
    this->setFixedHeight(620);
    for(int i=0;i<ui->label->width();i++)
       for(int j=0;j<ui->label->height();j++)
           test_data.append(QPoint(i,j));
    this->setWindowTitle("分类器可视化");
    this->method="logits";
    this->method_map.insert("logits","逻辑回归算法");
    this->method_map.insert("H-K","H-K算法");
    this->method_map.insert("energy","位势函数算法");

    QMenuBar *mBar=menuBar();
    QMenu *pClass=mBar->addMenu("类别选择");
    QAction *pClass_1=pClass->addAction("类别1");
    QAction *pClass_2=pClass->addAction("类别2");
    QAction *pClass_3=pClass->addAction("类别3");
    QAction *pClass_4=pClass->addAction("类别4");
    QAction *pClass_5=pClass->addAction("类别5");

    connect(pClass_1,&QAction::triggered,ui->label,&MyLabel::addClass1);
    connect(pClass_2,&QAction::triggered,ui->label,&MyLabel::addClass2);
    connect(pClass_3,&QAction::triggered,ui->label,&MyLabel::addClass3);
    connect(pClass_4,&QAction::triggered,ui->label,&MyLabel::addClass4);
    connect(pClass_5,&QAction::triggered,ui->label,&MyLabel::addClass5);

    QMenu *pMethod=mBar->addMenu("分类方法");
    QMenu *pLinear=pMethod->addMenu("线性分类器");
    QAction *pLogits=pLinear->addAction("逻辑回归算法(默认)");
    QAction *pHk=pLinear->addAction("H-K算法");
    QMenu *pNoLinear=pMethod->addMenu("非线性分类器");
    QAction *pEnergy=pNoLinear->addAction("位势函数算法");

    connect(pLogits,&QAction::triggered,[=]() mutable {this->method="logits";
    ui->textEdit->setText(QString("分类结果信息框\n分类方法：%1").arg("逻辑回归算法"));});
    connect(pHk,&QAction::triggered,[=]() mutable {this->method="H-K";
    ui->textEdit->setText(QString("分类结果信息框\n分类方法：%1").arg("H-K算法"));});
    connect(pEnergy,&QAction::triggered,[=]() mutable {this->method="energy";
    ui->textEdit->setText(QString("分类结果信息框\n分类方法：%1").arg("位势函数算法"));});

    QToolBar *toolBar=addToolBar("toolbar");
    toolBar->addAction(pClass_1);
    toolBar->addAction(pClass_2);
    toolBar->addAction(pClass_3);

    run_button=new QPushButton(this);
    run_button->setText("运行");
    toolBar->addWidget(run_button);

    clear_button=new QPushButton(this);
    clear_button->setText("清除");
    toolBar->addWidget(clear_button);
    //清空按键响应，字典清空，并label窗口重绘。
    connect(clear_button,&QPushButton::pressed,
            [=]() mutable{ui->label->map.clear();
                          ui->label->list_len=0;
                          num_label->setText("0");
                          ui->label->draw_img=false;
                          ui->label->update();
                          ui->textEdit->setText(QString("分类结果信息框\n分类方法："
                            "%1\n").arg(this->method_map[this->method]));});

    //添加状态栏
    QStatusBar *sBar=statusBar();
    QLabel *label=new QLabel(this);
    label->setText("样本数");
    sBar->addWidget(label);
    //从左往右依次添加
    num_label=new QLabel(QString("%1").arg(ui->label->list_len));
    sBar->addWidget(num_label);
    connect(ui->label,&MyLabel::total_num_signal,this,&MainWindow::setNumLabel);

    ui->textEdit->setText("分类结果信息框\n分类方法：逻辑回归算法");

    connect(run_button,&QPushButton::clicked,this,&MainWindow::run_classifer);
    connect(run_button,&QPushButton::pressed,
    [=]() mutable {ui->textEdit->setText(QString("分类结果信息框\n分类方法：%1\n请稍候正在进行训练。。"
                                                 "。。。。\n").arg(this->method_map[this->method]));
                  });
}
void MainWindow::setNumLabel(int num,int x,int y)
{
    num_label->setText(QString("%1，坐标:(%2,%3)").arg(num).arg(x).arg(y));
}
void MainWindow::run_classifer()
{
    if (ui->label->map.keys().length()<2)
    {
        QMessageBox::critical(this,"错误","请至少输入两个类别的数据");
        return;
    }

    //qDebug()<<"map: "<<ui->label->map;
    Classifier clf(ui->label->map,ui->label->width(),ui->label->height());
    clf.fit(this->method);
    ui->label->draw_img=true;
    QImage img(ui->label->width(),ui->label->height(),QImage::Format_RGB32);

    // ////////////
    QList<int> class_list=ui->label->map.keys();
    int total_num=0;
    int current_num=0;
    QList<int> result;
    for(int i=0;i<class_list.length();i++)
    {
        result=clf.predicter(ui->label->map[class_list[i]]);
        for(int j=0;j<result.length();j++)
        {
            if(result[j]==class_list[i])
                current_num++;
        }
        total_num+=result.length();
    }
    //定义图形颜色类别。
    QList<int> img_class=clf.predicter(test_data);
    for(int i=0;i<ui->label->width();i++)
    {
        for(int j=0;j<ui->label->height();j++)
        {
            int index=i*ui->label->width()+j;
            img.setPixelColor(i,j,this->color_class_bk[img_class[index]-1]);
        }
    }
    ui->label->img=img;
    ui->label->update();
    ui->textEdit->setText(QString("类别数：%1\n样本总数：%2\n分类方法：%3\n正确率：%4%\n"
    ).arg(class_list.length()).arg(total_num).arg(this->method_map[this->method]).arg(100.0*current_num/total_num));
}

MainWindow::~MainWindow()
{
    delete ui;
}
