#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QTimer>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "meanfilterdialog.h"
#include "mediumfilterdialog.h"
#include "gaussianfilterdialog.h"

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
    void on_originalButton_clicked();

    void on_meanfilterButton_clicked();
    void on_mediumfilterButton_clicked();
    void on_gaussianfilterButton_clicked();

    void on_robertsoperatorButton_clicked();
    void on_prewittoperatorButton_clicked();
    void on_sobeloperatorButton_clicked();

    void ShowMeanFilterPic();
    void ShowMediumFilterPic();
    void ShowGaussianFilterPic();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    cv::Mat originalimg; //store the original img
    QImage qimg;
    QImage imgScaled;
    cv::Mat rgb;

    MeanFilterDialog *meanfilterDialog;
    MediumFilterDialog *mediumfilterDialog;
    GaussianFilterDialog *gaussianfilterDialog;


};
#endif // MAINWINDOW_H
