#ifndef GAUSSIANFILTERDIALOG_H
#define GAUSSIANFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class GaussianFilterDialog;
}

class GaussianFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianFilterDialog(QWidget *parent = 0);
    ~GaussianFilterDialog();
    int GetSpinBoxValue();
    double GetDoubleSpinBoxValue();

private slots:
    void Test();

signals:
    void Signal3();


private:
    Ui::GaussianFilterDialog *ui;
};

#endif // GAUSSIANFILTERDIALOG_H
