## 1. 2D Plot
source: `qcustomplot`
1. initial_gragh---`Initialize_Plot`
2. live plotting(sinx)--- ``
3. define the shape of gragh from input button
4. deinfe the obstacles generator

`Plotting`button clicked---`Printing_Timer`start---`Realtime_Plot` start 

`clear all`button clicked---`Printing_Timer`stop---`MapPlot`clear

## 2. debug

1. 20240508 
**Could not find the 'ui'file in QTCreator**
![alt text](assets/software_document/image.png)
**solution:**
I forgot to define it in the CMakeLists.txt, adding it now.
>set(PROJECT_SOURCES src/qt/pathplanning_platform.ui)

2. 20240508
when I setting the `clear all`button, the original objective:1. stop the Printing_Timer;2. clear the graph;
Objective1 is achieved,but Objective2 can not achieve, as shown below
![alt text](assets/software_document/image-1.png)
**solution:**

before:
```
void PathPlanning_Platform::on_Plotting_Clear_clicked()
{
    Printing_Timer->stop();
    ui->output_txt->setText("stopping plotting");
    MapPlot->graph(0)->data()->clear();
}
```
after:
```
void PathPlanning_Platform::on_Plotting_Clear_clicked()
{
    Printing_Timer->stop();
    ui->output_txt->setText("stopping plotting");
    MapPlot->graph(0)->data()->clear(); // 清除数据
    MapPlot->replot(); // 重要：调用 replot 来更新图表显示
}
```
3. 20240508
另一个问题，当我按下了Plotting，开始实时画图，然后按下Plotting_Clear，可以正常停下timer，清除graph(0)内容。但是，当我再按下Plotting，它是从上一次停下的点开始继续画，这不是我想要的。我想要它每次Plotting，都是重头开始.

solution:
```
rt_location.x = 0; 
错误:
放在Realtime_Plot()里面。不行，因为Realtime_Plot是每次timer都会调用触发的，
它会一直触发x=0,导致图只会画第一个点

正确：放入`on_Plotting_clicked`因为目标就是按钮被按下，重新开始，
它在这里只会执行一次

```

