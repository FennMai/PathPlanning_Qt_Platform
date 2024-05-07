/**
* basic 1.0, date:07/05/2024:
* 1.basic qt window
* 2.reconstruct the project
*
*
**/

#include "pathplanning_platform.h"
#include "./ui_pathplanning_platform.h"


PathPlanning_Platform::PathPlanning_Platform(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PathPlanning_Platform)
{
    ui->setupUi(this);
}

PathPlanning_Platform::~PathPlanning_Platform()
{
    delete ui;
}

