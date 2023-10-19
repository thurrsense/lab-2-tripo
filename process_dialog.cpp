#include "winapi_shared.h"

#include "process_dialog.h"
#include "ui_process_dialog.h"

#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>

Process_dialog::Process_dialog(QWidget *parent, myProcess *process) :
   QDialog(parent),
   ui(new Ui::process_dialog),
   process(process)
{
   ui->setupUi(this);

   updatePrivillegiesTable();
}

Process_dialog::~Process_dialog()
{
   delete ui;
}

void Process_dialog::updatePrivillegiesTable()
{

   ui->privillegiesTable->setColumnCount(5);
   ui->privillegiesTable->setHorizontalHeaderLabels( {"Name", "Enabled/Disabled", "Enabled by Default", "Used for Access", "Change"});
   ui->privillegiesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

   if (!process)
      return;

   std::map<std::wstring, DWORD> all_privs = process->allPrivelegies();
   ui->privillegiesTable->setRowCount(all_privs.size()); // указываем количество строк
   int row_current = 0;
   for (auto && [name, privs] : all_privs)
   {
      auto item = new QTableWidgetItem(QString::fromStdWString(name));
      item->setFlags(item->flags() & !Qt::ItemIsEditable);
      ui->privillegiesTable->setItem(row_current, 0, item);

      QPushButton * button = new QPushButton(this);
      button->setEnabled(false);
      auto privs_lambda = privs;
      auto func_add_checkbox = [this, row_current, privs_lambda, button](int status, int column, bool enabled)
      {
         QCheckBox * box = new QCheckBox(this);
         box->setChecked(privs_lambda & status);
         box->setEnabled(enabled);
         if (enabled)
         {
             connect(box, &QCheckBox::stateChanged,
                     [button]()
             {
                button->setEnabled(true);
             });
         }
         ui->privillegiesTable->setCellWidget(row_current, column, box);
      };

      func_add_checkbox(SE_PRIVILEGE_ENABLED, 1, true);
      func_add_checkbox(SE_PRIVILEGE_ENABLED_BY_DEFAULT, 2, false);
      func_add_checkbox(SE_PRIVILEGE_USED_FOR_ACCESS, 3, false);

      button->setText("Change");
      connect(button, &QPushButton::clicked,
              [row_current, this]()
      {
         privsEdited( row_current);
      });
      ui->privillegiesTable->setCellWidget(row_current, 4, button);

      row_current++;
   }
}

void Process_dialog::privsEdited(int row)
{
    auto priv_name_string = ui->privillegiesTable->item(row, 0)->text().toStdWString();
    LPCTSTR priv_name = priv_name_string.c_str();

    bool isEnabled = qobject_cast<QCheckBox *>(ui->privillegiesTable->cellWidget(row,1))->isChecked();

    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process->PID);

    if (NULL == hProc)
    {
        QMessageBox::critical(this, "Operation canceled", "Can not open process");
        updatePrivillegiesTable();
        return;
    }
    HANDLE hToken;

    if (!OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        QMessageBox::critical(this, "Operation canceled", "Can not open token");
        updatePrivillegiesTable();
        return;
    }

    if (!SetPrivilege(hToken, priv_name, isEnabled))
    {
        QMessageBox::critical(this, "Operation canceled", "Can not set privilege");
        updatePrivillegiesTable();
        return;
    }

    updatePrivillegiesTable();
}
