#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include <iostream>
#include <vector>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void setValues(class myProcess **array, DWORD cProcesses);

    ~MainWindow();
private slots:
    void update();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();

    void handleContextMenu(const QPoint& pos);

signals:
    void clicked();
private:
    Ui::MainWindow *ui;

    void prosessDlgExec();
    myProcess** proc_buffer {nullptr}; //array with processes
    DWORD cProcesses {0};   //number of processes
};
#endif // MAINWINDOW_H
