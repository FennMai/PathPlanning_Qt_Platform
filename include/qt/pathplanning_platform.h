#ifndef PATHPLANNING_PLATFORM_H
#define PATHPLANNING_PLATFORM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PathPlanning_Platform; }
QT_END_NAMESPACE

class PathPlanning_Platform : public QMainWindow
{
    Q_OBJECT

public:
    PathPlanning_Platform(QWidget *parent = nullptr);
    ~PathPlanning_Platform();

private:
    Ui::PathPlanning_Platform *ui;
};
#endif // PATHPLANNING_PLATFORM_H
