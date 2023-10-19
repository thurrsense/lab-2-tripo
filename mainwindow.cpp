#include "mainwindow.h"
#include "process_dialog.h"
#include "ui_mainwindow.h"

//#include "infoProcess.cpp"
#include "myprocess.h"

#include <psapi.h>
#include <tlhelp32.h>
#include <processthreadsapi.h>
#include <sddl.h>
#include <wow64apiset.h>

#include "myFile.h"

#include <QMessageBox>
#include <QDebug>

void setPidNamePPID(class myProcess **array, DWORD cProcesses){

    DWORD now = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0 );
    PROCESSENTRY32 pe32;
    std::wstring tempPrev;
    if( hSnapshot == INVALID_HANDLE_VALUE )
        return;
        ZeroMemory( &pe32, sizeof( pe32 ) );
        pe32.dwSize = sizeof( pe32 );

        Process32First( hSnapshot, &pe32 );
        do{
            array[now]->PID = pe32.th32ProcessID;
            array[now]->PIDPArent = pe32.th32ParentProcessID;
            array[now]->name.append(pe32.szExeFile);

            int i = 0;
            for(; array[i]->PIDPArent != pe32.th32ParentProcessID; i++){}
            array[now]->nameParent.append(array[i]->name);

            now++;
        }while( Process32Next( hSnapshot, &pe32 ) );
    if( hSnapshot != INVALID_HANDLE_VALUE )
        CloseHandle( hSnapshot );

    for(DWORD i = 0; i < cProcesses; i++){
        array[i]->setProcessInfo();
    }

}

void MainWindow::setValues(class myProcess **array, DWORD cProcesses)
{
    ui->tableWidget->setRowCount(cProcesses); // указываем количество строк
    ui->tableWidget->setColumnCount(12);

    for(int stroka = 0; stroka < ui->tableWidget->rowCount(); stroka++)
      for(int stolbeu = 0; stolbeu < ui->tableWidget->columnCount(); stolbeu++)
      {
           QTableWidgetItem *item = new QTableWidgetItem(); // выделяем память под ячейку

           if(stolbeu == 0){
                item->setText(QString("%1").arg(array[stroka]->PID));
           }
           if(stolbeu == 1){
                item->setText(QString("%1").arg(array[stroka]->name));
           }
           if(stolbeu == 2){
                item->setText(QString("%1").arg(array[stroka]->PATH));
           }
           if(stolbeu == 3){
                item->setText(QString("%1").arg(array[stroka]->PIDPArent));
           }
           if(stolbeu == 4){
                item->setText(QString("%1").arg(array[stroka]->nameParent));
           }
           if(stolbeu == 5){
                item->setText(QString("%1").arg(array[stroka]->nameOwner));
           }
           if(stolbeu == 6){
                item->setText(QString("%1").arg(array[stroka]->SID));
           }
           if(stolbeu == 7){
                item->setText(QString("%1").arg(array[stroka]->x));
           }
           if(stolbeu == 8){
                item->setText(QString("%1").arg(array[stroka]->env));
           }
           if(stolbeu == 9){
                item->setText(QString("%1").arg(array[stroka]->y));
           }
           if(stolbeu == 10){
                item->setText(QString("%1").arg(array[stroka]->infDLL));
           }
           if(stolbeu == 11){
                item->setText(QString("%1").arg(array[stroka]->addInfo));
           }

           ui->tableWidget->setItem(stroka, stolbeu, item); // вставляем ячейку
           ui->tableWidget->setColumnWidth(0, 50);
           ui->tableWidget->setColumnWidth(2, 300);
           ui->tableWidget->setColumnWidth(10, 200);

      }
}

void MainWindow::update()
{

    DWORD aProcesses[1024], cbNeeded;
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        //return 1;
    }

    if (proc_buffer)
    {
        for(DWORD i = 0; i < cProcesses; i++)
        {
            delete proc_buffer[i];
        }
        delete proc_buffer;
    }
    cProcesses = cbNeeded / sizeof(DWORD);
    proc_buffer = new myProcess* [cProcesses];
    for(DWORD i = 0; i < cProcesses; i++){
        proc_buffer[i] = new myProcess(aProcesses[i]);
    }

    setPidNamePPID(proc_buffer, cProcesses);
    setValues(proc_buffer, cProcesses);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DWORD aProcesses[1024], cbNeeded;
    EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded );
    cProcesses = cbNeeded / sizeof(DWORD);
    proc_buffer = new myProcess* [cProcesses];
    for(DWORD i = 0; i < cProcesses; i++){
        proc_buffer[i] = new myProcess(aProcesses[i]);
    }

    setPidNamePPID(proc_buffer, cProcesses);

    // выделяем память под все ячейки таблицы
    for(int stroka = 0; stroka <   ui->tableWidget->rowCount(); stroka++)
    for(int stolbeu = 0; stolbeu <   ui->tableWidget->columnCount(); stolbeu++)
        ui->tableWidget->setItem(stroka, stolbeu, new QTableWidgetItem());

    setValues(proc_buffer, cProcesses);


    QStringList horzHeaders;
    horzHeaders << "PID" << "Name" << "Path" << "PPID" << "PName" << "OName" << "SID" << "x32/x64" << "Environment" << "DEP/ASLR" << "DLL" << "AddInfo";
    ui->tableWidget->setHorizontalHeaderLabels( horzHeaders );

    //Установка прерывания для кнопки обновления

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::handleContextMenu);
}

void MainWindow::handleContextMenu(const QPoint& /*pos*/)
{
    prosessDlgExec();
}

MainWindow::~MainWindow()
{
    if (proc_buffer)
    {
        for(DWORD i = 0; i < cProcesses; i++)
        {
            delete proc_buffer[i];
        }
        delete proc_buffer;
    }
    delete ui;
}

void MainWindow::on_action_2_triggered()
{
    update();
}

void MainWindow::on_action_3_triggered()
{
   prosessDlgExec();
}

void MainWindow::on_action_4_triggered()
{
    myFile newFileObj;
    newFileObj.setModal(true);
    newFileObj.exec();
}

void MainWindow::prosessDlgExec()
{
    int cur_row = ui->tableWidget->currentRow();
    if (cur_row >= 0)
    {
        //check that such process is existing now
        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, proc_buffer[cur_row]->PID);
        if (NULL == hProc)
        {
            QMessageBox::critical(this, windowTitle(),
                               "Process does not exist.\n Please try update list of processes");
            return;
        }
        CloseHandle(hProc);

        Process_dialog dlg (nullptr, proc_buffer[cur_row]);
        dlg.setModal(true);
        dlg.exec();
    }
}

