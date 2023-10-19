/********************************************************************************
** Form generated from reading UI file 'myfile.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFILE_H
#define UI_MYFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_myFile
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QLineEdit *lineEdit_4;
    QSpacerItem *horizontalSpacer_5;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *pushButton_2;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_5;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_8;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_9;
    QLabel *labelIntegrityTitle;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_10;
    QLineEdit *lineEditCurIntegrity;
    QSpacerItem *horizontalSpacer_9;
    QComboBox *comboBoxIntegrity;
    QSpacerItem *verticalSpacer_4;

    void setupUi(QDialog *myFile)
    {
        if (myFile->objectName().isEmpty())
            myFile->setObjectName("myFile");
        myFile->resize(728, 774);
        verticalLayout = new QVBoxLayout(myFile);
        verticalLayout->setSpacing(15);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(myFile);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        lineEdit = new QLineEdit(myFile);
        lineEdit->setObjectName("lineEdit");

        verticalLayout->addWidget(lineEdit);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_3 = new QLabel(myFile);
        label_3->setObjectName("label_3");

        horizontalLayout_2->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        lineEdit_3 = new QLineEdit(myFile);
        lineEdit_3->setObjectName("lineEdit_3");

        horizontalLayout_3->addWidget(lineEdit_3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        comboBox = new QComboBox(myFile);
        comboBox->setObjectName("comboBox");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(210, 0));
        comboBox->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_3->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        label_4 = new QLabel(myFile);
        label_4->setObjectName("label_4");

        horizontalLayout_4->addWidget(label_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_4);

        label_2 = new QLabel(myFile);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        lineEdit_4 = new QLineEdit(myFile);
        lineEdit_4->setObjectName("lineEdit_4");

        horizontalLayout_5->addWidget(lineEdit_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_5);

        listWidget = new QListWidget(myFile);
        listWidget->setObjectName("listWidget");

        verticalLayout->addWidget(listWidget);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        pushButton_2 = new QPushButton(myFile);
        pushButton_2->setObjectName("pushButton_2");

        horizontalLayout_6->addWidget(pushButton_2);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        verticalLayout->addLayout(horizontalLayout_6);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        label_5 = new QLabel(myFile);
        label_5->setObjectName("label_5");

        horizontalLayout_7->addWidget(label_5);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(25);
        horizontalLayout_8->setObjectName("horizontalLayout_8");
        lineEdit_5 = new QLineEdit(myFile);
        lineEdit_5->setObjectName("lineEdit_5");

        horizontalLayout_8->addWidget(lineEdit_5);

        lineEdit_6 = new QLineEdit(myFile);
        lineEdit_6->setObjectName("lineEdit_6");

        horizontalLayout_8->addWidget(lineEdit_6);

        lineEdit_7 = new QLineEdit(myFile);
        lineEdit_7->setObjectName("lineEdit_7");

        horizontalLayout_8->addWidget(lineEdit_7);

        pushButton = new QPushButton(myFile);
        pushButton->setObjectName("pushButton");

        horizontalLayout_8->addWidget(pushButton);


        verticalLayout->addLayout(horizontalLayout_8);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName("horizontalLayout_9");
        labelIntegrityTitle = new QLabel(myFile);
        labelIntegrityTitle->setObjectName("labelIntegrityTitle");

        horizontalLayout_9->addWidget(labelIntegrityTitle);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName("horizontalLayout_10");
        lineEditCurIntegrity = new QLineEdit(myFile);
        lineEditCurIntegrity->setObjectName("lineEditCurIntegrity");
        lineEditCurIntegrity->setReadOnly(true);

        horizontalLayout_10->addWidget(lineEditCurIntegrity);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_9);

        comboBoxIntegrity = new QComboBox(myFile);
        comboBoxIntegrity->setObjectName("comboBoxIntegrity");
        sizePolicy.setHeightForWidth(comboBoxIntegrity->sizePolicy().hasHeightForWidth());
        comboBoxIntegrity->setSizePolicy(sizePolicy);
        comboBoxIntegrity->setMinimumSize(QSize(210, 0));
        comboBoxIntegrity->setMaximumSize(QSize(210, 16777215));

        horizontalLayout_10->addWidget(comboBoxIntegrity);


        verticalLayout->addLayout(horizontalLayout_10);

        verticalSpacer_4 = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);


        retranslateUi(myFile);

        QMetaObject::connectSlotsByName(myFile);
    } // setupUi

    void retranslateUi(QDialog *myFile)
    {
        myFile->setWindowTitle(QCoreApplication::translate("myFile", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("myFile", "\320\243\320\272\320\260\320\266\320\270\321\202\320\265 \320\277\321\203\321\202\321\214 \320\264\320\276 \321\204\320\260\320\271\320\273\320\260:", nullptr));
        label_3->setText(QCoreApplication::translate("myFile", "\320\222\320\273\320\260\320\264\320\265\320\273\320\265\321\206 \321\204\320\260\320\271\320\273\320\260:", nullptr));
        label_4->setText(QCoreApplication::translate("myFile", "SID \320\262\320\273\320\260\320\264\320\265\320\273\321\214\321\206\320\260:", nullptr));
        label_2->setText(QString());
        pushButton_2->setText(QCoreApplication::translate("myFile", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214 \320\277\321\200\320\260\320\262\320\270\320\273\320\276 ACE", nullptr));
        label_5->setText(QCoreApplication::translate("myFile", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214 \320\275\320\276\320\262\321\213\320\271 ACE:", nullptr));
        pushButton->setText(QCoreApplication::translate("myFile", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        labelIntegrityTitle->setText(QCoreApplication::translate("myFile", "\320\243\321\200\320\276\320\262\320\265\320\275\321\214 \321\206\320\265\320\273\320\276\321\201\321\202\320\275\320\276\321\201\321\202\320\270 (mandatory label)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class myFile: public Ui_myFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFILE_H
