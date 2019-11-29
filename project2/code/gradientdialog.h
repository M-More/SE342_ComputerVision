#ifndef GRADIENTDIALOG_H
#define GRADIENTDIALOG_H

#include <QDialog>

namespace Ui {
class GradientDialog;
}

class GradientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GradientDialog(QWidget *parent = 0);
    ~GradientDialog();
    int GetSizeValue();
    int GetSeValue(int row, int col);

private slots:
    void ConfirmSize();
    void Standard();
    void External();
    void Internal();

signals:
    void StandardGradientSignal();
    void ExternalGradientSignal();
    void InternalGradientSignal();

private:
    Ui::GradientDialog *ui;
};

#endif // GRADIENTDIALOG_H
