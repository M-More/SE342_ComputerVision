#ifndef RECONSTRUCTIONDIALOG_H
#define RECONSTRUCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ReconstructionDialog;
}

class ReconstructionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReconstructionDialog(QWidget *parent = 0);
    ~ReconstructionDialog();
    int GetSizeValue();
    int GetSeValue(int row, int col);


private slots:
    void ConfirmSize();
    void ConditionalDilation();
    void GeodesicDilation();
    void GeodesicErosion();
    void OpeningByReconstruction();
    void ClosingByReconstruction();


signals:
    void ConditionalDilationSignal();
    void GeodesicDilationSignal();
    void GeodesicErosionSignal();
    void OpeningByReconstructionSignal();
    void ClosingByReconstructionSignal();

private:
    Ui::ReconstructionDialog *ui;
};

#endif // RECONSTRUCTIONDIALOG_H
