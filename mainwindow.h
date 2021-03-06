#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qgleswidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QTimer *timer;
//    QGLESWIDGET widget;
protected slots:
    void onTimeOut();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
