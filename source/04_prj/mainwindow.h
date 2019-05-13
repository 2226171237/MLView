#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include "classifier.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setNumLabel(int num,int x,int y);
    QString method;
    QMap<QString,QString> method_map;
    QPushButton *run_button;
    QPushButton *clear_button;
    QLabel *num_label;
    QList<QPoint> test_data;//label全局左边网格
    QColor color_class_bk[5]={QColor(255,106,106),QColor(127,255,0),
            QColor(72,118,255),QColor(238,221,130),QColor(100,0,100)};
    void run_classifer();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
