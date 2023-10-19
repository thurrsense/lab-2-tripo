/********************************************************************************
** Form generated from reading UI file 'process_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROCESS_DIALOG_H
#define UI_PROCESS_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_process_dialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *pivillegiesTitle;
    QTableWidget *privillegiesTable;

    void setupUi(QDialog *process_dialog)
    {
        if (process_dialog->objectName().isEmpty())
            process_dialog->setObjectName("process_dialog");
        process_dialog->resize(800, 300);
        verticalLayout = new QVBoxLayout(process_dialog);
        verticalLayout->setObjectName("verticalLayout");
        pivillegiesTitle = new QLabel(process_dialog);
        pivillegiesTitle->setObjectName("pivillegiesTitle");

        verticalLayout->addWidget(pivillegiesTitle);

        privillegiesTable = new QTableWidget(process_dialog);
        privillegiesTable->setObjectName("privillegiesTable");
        privillegiesTable->horizontalHeader()->setStretchLastSection(true);
        privillegiesTable->verticalHeader()->setStretchLastSection(false);

        verticalLayout->addWidget(privillegiesTable);


        retranslateUi(process_dialog);

        QMetaObject::connectSlotsByName(process_dialog);
    } // setupUi

    void retranslateUi(QDialog *process_dialog)
    {
        process_dialog->setWindowTitle(QCoreApplication::translate("process_dialog", "Dialog", nullptr));
        pivillegiesTitle->setText(QCoreApplication::translate("process_dialog", "\320\237\321\200\320\270\320\262\320\265\320\273\320\265\320\263\320\270\320\270 \320\277\321\200\320\276\321\206\320\265\321\201\321\201\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class process_dialog: public Ui_process_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROCESS_DIALOG_H
