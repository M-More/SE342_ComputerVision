#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "edgedetectiondialog.h"
#include "gradientdialog.h"
#include "reconstructiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    QImage Mat2QImage(const cv::Mat &mat);
    void display(QImage image);
    void bubble_sort(int *arr, int num);

    void on_openButton_clicked();
    void on_openButton2_clicked();
    void on_originalButton_clicked();
    void on_originalButton2_clicked();

    void on_edgedetectionButton_clicked();
    void ShowStandardEdgeDetectionPic();
    void ShowExternalEdgeDetectionPic();
    void ShowInternalEdgeDetectionPic();

    void on_gradientButton_clicked();
    void ShowStandardGradientPic();
    void ShowExternalGradientPic();
    void ShowInternalGradientPic();

    //
    
    void on_markerButton_clicked();
    void on_maskButton_clicked();
    void displaymarker(QImage image);
    void displaymask(QImage image);
    
    void on_seButton1_clicked();
    void on_seButton2_clicked();
    void ShowConditionalDilationPic();
    void ShowGeodesicDilationPic();
    void ShowGeodesicErosionPic();
    void ShowOpeningByReconstructionPic();
    void ShowClosingByReconstructionPic();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    cv::Mat originalimg; //store the original img
    QImage qimg;
    QImage imgScaled;
    cv::Mat rgb;

    cv::Mat marker;
    cv::Mat mask;

    EdgeDetectionDialog *edgedetectionDialog;
    GradientDialog *gradientDialog;
    ReconstructionDialog *reconstructionDialog;
};
#endif // MAINWINDOW_H

