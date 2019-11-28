#ifndef MEANFILTERDIALOG_H
#define MEANFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class MeanFilterDialog;
}

class MeanFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeanFilterDialog(QWidget *parent = 0);
    ~MeanFilterDialog();
    int GetSpinBoxValue();

private slots:
    void Test();

signals:
    void Signal1();

private:
    Ui::MeanFilterDialog *ui;
};

#endif // MEANFILTERDIALOG_H
