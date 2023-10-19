#ifndef PROCESS_DIALOG_H
#define PROCESS_DIALOG_H

#include "myprocess.h"

#include <QDialog>

namespace Ui {
class process_dialog;
}

class Process_dialog : public QDialog
{
   Q_OBJECT

public:
   explicit Process_dialog(QWidget *parent = nullptr, myProcess * process = nullptr);
   ~Process_dialog();

private:
   Ui::process_dialog *ui;

   myProcess * process;
   void updatePrivillegiesTable();

private slots:
   void privsEdited(int row);
};

#endif // PROCESS_DIALOG_H
