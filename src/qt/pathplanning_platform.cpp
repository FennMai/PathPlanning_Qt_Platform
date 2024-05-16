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
*********************************
* now:
* @version   1.2
* @author    Fennmai
* @date      16/05/2024
* @brief
* 1. obstacles generators mode1 - custom input obstacles
*
********************************/

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
    MapPlot->xAxis->setRange(0, 10);  // X轴显示范围从-0.1到10
    MapPlot->yAxis->setRange(0, 10);   // Y轴显示范围从-1到2.5

    // 下面的代码用于配置轴标题，暂时被注释
     MapPlot->xAxis->setLabel("X Axis / m ");
     MapPlot->yAxis->setLabel("Y Axis / m ");

    // 创建一个新图形用于绘制动态数据
    MapPlot->addGraph();
    MapPlot->graph(0)->setName("Dynamic Graph Data");  // 为图形设置名称

    // 设置数据点的绘图风格：不使用线条，只使用散点标记
    MapPlot->graph(0)->setLineStyle(QCPGraph::lsNone);

    // 配置散点的样式，包括形状、大小、边框颜色和填充颜色
    QCPScatterStyle scatterStyle;
    scatterStyle.setShape(QCPScatterStyle::ssSquare);  // 设置散点形状为圆形

    scatterStyle.setSize(3);  // 散点大小
    scatterStyle.setPen(QPen(Qt::red));  // 散点边框颜色为红色
    scatterStyle.setBrush(Qt::yellow);  // 散点填充颜色为黄色
    MapPlot->graph(0)->setScatterStyle(scatterStyle);  // 应用散点样式


    // 暂时将 obs_input_table 的初始化也放在这里
    // set the column
    ui->obs_input_table->setColumnCount(3);
    // set the header name
    ui->obs_input_table->setHorizontalHeaderLabels(QStringList()<<"obs_x"<<"obs_y"<<"obs_size");


}
/**********************
** Real-Time Plotting **
**********************/
void PathPlanning_Platform::Realtime_Plot(){
    // 数据导入
    rt_location.y =sin(rt_location.x);
    // 数据格式判断

    // 数据使用-->绘图
    MapPlot->graph(0)->addData(rt_location.x, rt_location.y);
    MapPlot->graph(0)->rescaleAxes(true); //自动调整图表的轴范围以适应所有绘制的数据点。
    MapPlot->replot(); // 将新加入的点，绘制到graph(0)里

    rt_location.x +=0.1;

    // 检测是否完成-->当完成所有点的绘制后，自动停下
    if (rt_location.x>6){
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
    Printing_Timer->start(50);// 1000ms，定时器时间可以自行设置，此处想在ui做一个滑动条，关联到这里
}

/**********************
** Plotting clear all**
**********************/
void PathPlanning_Platform::on_Plotting_Clear_clicked()
{
    Printing_Timer->stop();
    ui->output_txt->setText("stopping plotting");
    MapPlot->graph(0)->data()->clear(); // 清除数据
    MapPlot->clearItems();
    MapPlot->replot(); // 重要：调用 replot 来更新图表显示
}

/**********************
** Obstacles info input**
**********************/
void PathPlanning_Platform::on_obs_data_get_clicked()
{
    // get the nums of obstacles
    obs_nums = ui->obs_nums_txt->value();
    ui->output_txt->setText(QString::number(obs_nums));

    // define the table by nums ( nums*3)
    // set the column
    // ui->obs_input_table->setColumnCount(3);
    // set the header name
    // ui->obs_input_table->setHorizontalHeaderLabels(QStringList()<<"obs_x"<<"obs_y"<<"obs_size");

    // set the row
    ui->obs_input_table->setRowCount(obs_nums);



}
/*************************************************
**Function: Obstacles info input getting
**Description: 函数功能描述
**Input: Obs_Info_get -- button clicked
**obs_nums -- obstacles numbers -- from on_obs_data_get_clicked()
**Output: obs_info -- all obstacles infos in global
**Return:
**Others: 这里得做个安全处理，如果nums有值，table没有值/ nums设定值大于 table输入的行数，QT会直接crashed！
*************************************************/
void PathPlanning_Platform::on_Obs_Info_get_clicked()
{
    // clean the data in vector before
    obs_info.clear();
    PasteFromClipboard();

    for(int row=0;row < obs_nums; ++row){
        // getting the each row info
        double obs_x = ui->obs_input_table->item(row,0)->text().toDouble();
        double obs_y = ui->obs_input_table->item(row,1)->text().toDouble();
        double obs_size = ui->obs_input_table->item(row,2)->text().toDouble();
        //
        Obs_Plot(obs_x, obs_y, obs_size);
        //
        obs_info.push_back({obs_x, obs_y, obs_size});
    }
    // 输出obs_info的大小，以验证数据是否正确存入
    ui->output_txt->setText(QString("Number of obstacles: %1").arg(obs_info.size()));

    // obs_info showing in the MapPlot

}
/*************************************************
**Function: Obstacles info input getting
**Description: 粘贴剪贴板中的数据到表格
**Input:
**Output:
**Return:
**Others:
*************************************************/
void PathPlanning_Platform::PasteFromClipboard() {
    // 获取剪贴板
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    // 检查剪贴板中是否有文本数据
    if (mimeData->hasText()) {
        // 从剪贴板中获取文本数据
        QString textData = mimeData->text();

        // 将文本数据按行分割
        QStringList lines = textData.split('\n');

        // 遍历行数据
        for (int i = 0; i < lines.size(); ++i) {
            // 分割每一行的数据
            QStringList rowData = lines[i].split('\t');

            // 检查数据是否足够填充到表格中
            if (rowData.size() == ui->obs_input_table->columnCount()) {
                // 填充数据到表格中
                for (int j = 0; j < rowData.size(); ++j) {
                    QTableWidgetItem *item = new QTableWidgetItem(rowData[j]);
                    ui->obs_input_table->setItem(i, j, item);
                }
            }
        }
    }
}

/*************************************************
** Function: Obs_Plot
** Description: 在地图中绘制障碍物
** Input: obs_x - 障碍物 x 坐标
**        obs_y - 障碍物 y 坐标
**        obs_size - 障碍物圆的半径
** Output:
** Return:
** Others:
*************************************************/
void PathPlanning_Platform::Obs_Plot(double obs_x, double obs_y, double obs_size) {
    // 创建圆形障碍物
    QCPItemEllipse *obstacle = new QCPItemEllipse(MapPlot);
    // 设置圆心坐标和半径
    obstacle->topLeft->setCoords(obs_x - obs_size, obs_y + obs_size);
    obstacle->bottomRight->setCoords(obs_x + obs_size, obs_y - obs_size);
    // 设置圆的样式
    obstacle->setPen(QPen(Qt::red));  // 设置边框颜色为红色
    obstacle->setBrush(QBrush(Qt::yellow));  // 设置填充颜色为黄色
    // 将障碍物添加到图表中
    //MapPlot->addItem(obstacle);
    // 重新绘制图表
    MapPlot->graph(0)->rescaleAxes(true);
    MapPlot->replot();
}


void PathPlanning_Platform::Obs_Plot_Format(){

}
