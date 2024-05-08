/**
* basic 1.0, date:07/05/2024:
* 1.basic qt window
* 2.reconstruct the project
*
*
**/

#ifndef PATHPLANNING_PLATFORM_H
#define PATHPLANNING_PLATFORM_H

#include <QMainWindow>
#include<QTimer>
#include"qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PathPlanning_Platform; }
QT_END_NAMESPACE

/***** basci structure *****/
struct RT_Location {
    float x = 0.0;
    float y = 0.0;
    float orientation = 0.0;
};

class PathPlanning_Platform : public QMainWindow
{
    Q_OBJECT

public:

    PathPlanning_Platform(QWidget *parent = nullptr);
    ~PathPlanning_Platform();

    /***** qcustomplot plot *****/
    // 对绘图背景参数初始化
    void Initialize_Plot();
    // 连接Timer，实时接受数据数据并绘图
    void Realtime_Plot();


private slots:
    void on_Plotting_clicked();

    void on_Plotting_Clear_clicked();

private:
    Ui::PathPlanning_Platform *ui;
    /***** qcustomplot define *****/
    QCustomPlot *MapPlot;
    /***** QTimer define *****/
    QTimer *Printing_Timer;
    /***** Vector *****/
//    float key =0;  // 用于绘制的 X 轴变量
    RT_Location rt_location;

};
#endif // PATHPLANNING_PLATFORM_H
