#ifndef EDGEDETECTIONDIALOG_H
#define EDGEDETECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class EdgeDetectionDialog;
}

class EdgeDetectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EdgeDetectionDialog(QWidget *parent = 0);
    ~EdgeDetectionDialog();
    int GetSizeValue();
    int GetSeValue(int row, int col);

private slots:
    void ConfirmSize();
    void Standard();
    void External();
    void Internal();

signals:
    void StandardSignal();
    void ExternalSignal();
    void InternalSignal();

private:
    Ui::EdgeDetectionDialog *ui;
};

#endif // EDGEDETECTIONDIALOG_H
