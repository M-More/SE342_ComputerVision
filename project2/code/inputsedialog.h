#ifndef INPUTSEDIALOG_H
#define INPUTSEDIALOG_H

#include <QDialog>

namespace Ui {
class inputseDialog;
}

class inputseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputseDialog(QWidget *parent = 0);
    ~inputseDialog();
    int GetRowValue();
    int GetColumnValue();

private slots:
    void Confirm();

private:
    Ui::inputseDialog *ui;
};

#endif // INPUTSEDIALOG_H
