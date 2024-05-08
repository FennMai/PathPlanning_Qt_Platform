/**
 *
* basic 1.0, date:07/05/2024:
* 1.basic qt window
* 2.reconstruct the project
*
*
**/

#include "pathplanning_platform.h"
#include "./ui_pathplanning_platform.h"
#include <cmath>

PathPlanning_Platform::PathPlanning_Platform(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PathPlanning_Platform)
{
    ui->setupUi(this);

    /***** qcustomplot structure *****/
    MapPlot = new QCustomPlot();
    ui->QT_MapPlot->addWidget(MapPlot);

    Initialize_Plot();

    /***** Timer *****/
    // 定义一个定时器，作为客观的实时绘图的工具
    // 注意！只判断输入数据结构是否有效，并对输入数据进行绘图，不做其他数据处理！
    // 与数据处理解耦
    Printing_Timer = new QTimer(this);
    connect(Printing_Timer, &QTimer::timeout, this, &PathPlanning_Platform::Realtime_Plot);

}

PathPlanning_Platform::~PathPlanning_Platform()
{
    delete ui;
    delete MapPlot;
}

/**********************
** Initialize graph **
**********************/
void PathPlanning_Platform::Initialize_Plot() {
    // 设置 MapPlot 的显示范围，定义了图形的视觉边界
    MapPlot->xAxis->setRange(-0.1, 10);  // X轴显示范围从-0.1到10
    MapPlot->yAxis->setRange(-1, 2.5);   // Y轴显示范围从-1到2.5

    // 下面的代码用于配置轴标题，暂时被注释
    // LidarPlot->xAxis->setLabel("X Axis / m ");
    // LidarPlot->yAxis->setLabel("Y Axis / m ");

    // 创建一个新图形用于绘制动态数据
    MapPlot->addGraph();
    MapPlot->graph(0)->setName("Dynamic Graph Data");  // 为图形设置名称

    // 设置数据点的绘图风格：不使用线条，只使用散点标记
    MapPlot->graph(0)->setLineStyle(QCPGraph::lsNone);

    // 配置散点的样式，包括形状、大小、边框颜色和填充颜色
    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssSquare);  // 设置散点形状为正方形
    // ssSquare 圆形
    scatterStyle.setSize(3);  // 散点大小
    scatterStyle.setPen(QPen(Qt::red));  // 散点边框颜色为红色
    scatterStyle.setBrush(Qt::yellow);  // 散点填充颜色为黄色
    MapPlot->graph(0)->setScatterStyle(scatterStyle);  // 应用散点样式
}
/**********************
** Real-Time Plotting **
**********************/
void PathPlanning_Platform::Realtime_Plot(){

    rt_location.y =sin(rt_location.x);

    MapPlot->graph(0)->addData(rt_location.x, rt_location.y);
    MapPlot->graph(0)->rescaleAxes(true); //自动调整图表的轴范围以适应所有绘制的数据点。
    MapPlot->replot(); // 将新加入的点，绘制到graph(0)里

    rt_location.x +=0.1;

    // 当完成所有点的绘制后，自动停下
    if (rt_location.x>3){
        Printing_Timer->stop();
        ui->output_txt->setText("stopping plotting");
    }

}
/**********************
** Real-Time Plotting Button**
**********************/
void PathPlanning_Platform::on_Plotting_clicked()
{
    rt_location.x = 0;
    ui->output_txt->setText("starting plotting");
    Printing_Timer->start(500);// 1000ms
}

/**********************
** Plotting clear all**
**********************/
void PathPlanning_Platform::on_Plotting_Clear_clicked()
{
    Printing_Timer->stop();
    ui->output_txt->setText("stopping plotting");
    MapPlot->graph(0)->data()->clear(); // 清除数据
    MapPlot->replot(); // 重要：调用 replot 来更新图表显示
}


