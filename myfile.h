#ifndef MYFILE_H
#define MYFILE_H

#include <windows.h>

#include <QDialog>

namespace Ui {
class myFile;
}

class myFile : public QDialog
{
    Q_OBJECT

public:
    explicit myFile(QWidget *parent = nullptr);
    ~myFile();
    void getUsers();

private slots:
    void on_lineEdit_returnPressed();
    void on_pushButton_clicked();
    void on_comboBox_currentTextChanged(const QString &arg1);


    void on_pushButton_2_clicked();

private:
    Ui::myFile *ui;

    void updateIntegrity(PACL casl);
    void changeIntegrity();
};

#endif // MYFILE_H
