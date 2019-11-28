#ifndef MEDIUMFILTERDIALOG_H
#define MEDIUMFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class MediumFilterDialog;
}

class MediumFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MediumFilterDialog(QWidget *parent = 0);
    ~MediumFilterDialog();
    int GetSpinBoxValue();

private slots:
    void Test2();

signals:
    void Signal2();

private:
    Ui::MediumFilterDialog *ui;
};



#endif // MEDIUMFILTERDIALOG_H
