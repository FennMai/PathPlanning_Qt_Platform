/********************************
* history:
*
* basic 1.0, date:07/05/2024:
* 1.basic qt window
* 2.reconstruct the project
*
* basic 1.1, date:08/05/2024:
* 1.UI design
* 2.add qcustomplot package, and basic build it
* 3.define a simple sinx graph
* 4.debug the real-time plotting
*
* @version   1.2
* @author    Fennmai
* @date      16/05/2024
* @brief
* 1. obstacles generators mode1 - custom input obstacles
*
*********************************
* now:
* @version   1.2.2
* @author    Fennmai
* @date      18/05/2024
* @brief
* 1.feat: obstacles generators mode 2 - Random Generator
*
********************************/


#ifndef PATHPLANNING_PLATFORM_H
#define PATHPLANNING_PLATFORM_H

#include <QMainWindow>
#include<QTimer>
#include<vector>
#include <QRandomGenerator>
#include <QClipboard>
#include <QApplication>

#include"qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PathPlanning_Platform; }
QT_END_NAMESPACE

/***** basic structure *****/
struct RT_Location {
    float x = 0.0;
    float y = 0.0;
    float orientation = 0.0;
};

struct Obstacle_info {
    double obs_x;
    double obs_y;
    double obs_size;
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

    /***** Vector *****/
    std::vector<Obstacle_info>obs_info;
    std::vector<double>obs_apf_rep_f; //对障碍物 --> APF --> 斥力

    /***** Variables *****/
    int obs_nums; // 障碍物数量
    float MapWeight; // 地图的长度
    float MapHeight; // 地图的宽度

private slots:
    void on_Plotting_clicked();

    void on_Plotting_Clear_clicked();

    void on_obs_data_get_clicked();

    void on_Obs_Info_get_clicked();

    void UdMapWight(const QString &text);

    void UdMapHight(const QString &text);

    void on_ObsRndGen_clicked();

private:
    Ui::PathPlanning_Platform *ui;
    /***** qcustomplot define *****/
    QCustomPlot *MapPlot;
    /***** QTimer define *****/
    QTimer *Printing_Timer;
    /***** Vector *****/
//    float key =0;  // 用于绘制的 X 轴变量
    RT_Location rt_location;
    /***** function *****/
    void PasteFromClipboard();
    void Obs_Plot(double obs_x, double obs_y, double obs_size);
    void Obs_Plot_Format();


};
#endif // PATHPLANNING_PLATFORM_H
