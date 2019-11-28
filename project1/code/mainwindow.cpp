#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <vector>
#include <math.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                ".",tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    qDebug()<<"filenames:"<<fileName;
    image = cv::imread(fileName.toLatin1().data());

    originalimg = image.clone();        //clone the img
    qimg = MainWindow::Mat2QImage(image);
    display(qimg);                      //display by the label
    if(image.data)
    {
        ui->originalButton->setEnabled(true);
        ui->meanfilterButton->setEnabled(true);
        ui->gaussianfilterButton->setEnabled(true);
        ui->mediumfilterButton->setEnabled(true);
    }
}

QImage MainWindow::Mat2QImage(const cv::Mat &mat)
{
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for (int i = 0; i < 256; i++){
            image.setColor(i, qRgb(i,i,i));
        }
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row++){
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if(mat.type() == CV_8UC3){
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4){
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

void MainWindow::display(QImage img)
{
    QImage imgScaled;
    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
    ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));
}

void MainWindow::on_originalButton_clicked()
{
    qimg = MainWindow::Mat2QImage(originalimg);
    display(qimg);
}

void MainWindow::on_meanfilterButton_clicked(){
    meanfilterDialog = new MeanFilterDialog(this);
    meanfilterDialog->setModal(false);
    meanfilterDialog->show();
    connect(meanfilterDialog, SIGNAL(Signal1()), this, SLOT(ShowMeanFilterPic()));
}

void MainWindow::ShowMeanFilterPic(){
    image = originalimg.clone();
    int kernal_size = meanfilterDialog->GetSpinBoxValue();  // get value of kernalsize
    int center = (kernal_size-1)/2;
    int height = image.rows;
    int width = image.cols;
    int channels = image.channels();

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC3);

    for (int x = 0; x < channels; x++)  // extend border
    {
        for (int i = 0; i < height+2*center; i++)
        {
            for (int j = 0; j < width+2*center; j++)
            {
                if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
                {
                    tmp.at<cv::Vec3b>(i,j)[x] = 0;
                }
                else
                {
                    if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                    {
                        tmp.at<cv::Vec3b>(i,j)[x] = 0;
                    }
                    else
                    {
                        tmp.at<cv::Vec3b>(i,j)[x]=image.at<cv::Vec3b>(i-center,j-center)[x];
                    }
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int x = 0; x < channels; x++)  // mean filter
    {
        for (int i = center; i < height - center; i++)
        {
            for (int j = center; j < width - center; j++)
            {
                int sum = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        sum += tmp.at<cv::Vec3b>(i+m,j+n)[x];
                    }
                }
                tmp.at<cv::Vec3b>(i,j)[x]=sum/(kernal_size*kernal_size);
            }
        }
    }

    for (int x = 0; x < channels; x++)
    {
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                image.at<cv::Vec3b>(i,j)[x] = tmp.at<cv::Vec3b>(i+center,j+center)[x];
            }
        }
    }

    qimg = MainWindow::Mat2QImage(image);
    display(qimg);
}


void MainWindow::on_mediumfilterButton_clicked(){
    mediumfilterDialog = new MediumFilterDialog(this);
    mediumfilterDialog->setModal(false);
    mediumfilterDialog->show();
    connect(mediumfilterDialog, SIGNAL(Signal2()), this, SLOT(ShowMediumFilterPic()));
}

void MainWindow::bubble_sort(int *arr, int num)
{
    for (int i = 0; i < num - 1; i++)
    {
        for (int j = 0; j < num - i - 1; j++)
        {
            if (arr[j] > arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void MainWindow::ShowMediumFilterPic(){
    image = originalimg.clone();
    int kernal_size = mediumfilterDialog->GetSpinBoxValue();  // get value of kernalsize
    int center = (kernal_size-1)/2;
    int height = image.rows;
    int width = image.cols;
    int channels = image.channels();
    int *array;
    array = (int*)malloc(sizeof(int)*kernal_size*kernal_size);

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC3);

    for (int x = 0; x < channels; x++)  // extend border
    {
        for (int i = 0; i < height+2*center; i++)
        {
            for (int j = 0; j < width+2*center; j++)
            {
                if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
                {
                    tmp.at<cv::Vec3b>(i,j)[x] = 0;
                }
                else
                {
                    if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                    {
                        tmp.at<cv::Vec3b>(i,j)[x] = 0;
                    }
                    else
                    {
                        tmp.at<cv::Vec3b>(i,j)[x]=image.at<cv::Vec3b>(i-center,j-center)[x];
                    }
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int x = 0; x < channels; x++)  // medium filter
    {
        for (int i = center; i < height - center; i++)
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        array[numcount] = tmp.at<cv::Vec3b>(i+m,j+n)[x];
                        numcount++;
                    }
                }
                bubble_sort(array, kernal_size*kernal_size);
                tmp.at<cv::Vec3b>(i,j)[x]=array[(kernal_size*kernal_size-1)/2];
            }
        }
    }

    for (int x = 0; x < channels; x++)
    {
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                image.at<cv::Vec3b>(i,j)[x] = tmp.at<cv::Vec3b>(i+center,j+center)[x];
            }
        }
    }

    qimg = MainWindow::Mat2QImage(image);
    display(qimg);
}


void MainWindow::on_gaussianfilterButton_clicked(){
    gaussianfilterDialog = new GaussianFilterDialog(this);
    gaussianfilterDialog->setModal(false);
    gaussianfilterDialog->show();
    connect(gaussianfilterDialog, SIGNAL(Signal3()), this, SLOT(ShowGaussianFilterPic()));
}

void MainWindow::ShowGaussianFilterPic(){
    image = originalimg.clone();
    int kernal_size = gaussianfilterDialog->GetSpinBoxValue();  // get value of kernalsize
    int center = (kernal_size-1)/2;
    double sigma = gaussianfilterDialog->GetDoubleSpinBoxValue(); // get value of sigma
    int height = image.rows;
    int width = image.cols;
    int channels = image.channels();

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC3);

    for (int x = 0; x < channels; x++)  // extend border
    {
        for (int i = 0; i < height+2*center; i++)
        {
            for (int j = 0; j < width+2*center; j++)
            {
                if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
                {
                    tmp.at<cv::Vec3b>(i,j)[x] = 0;
                }
                else
                {
                    if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                    {
                        tmp.at<cv::Vec3b>(i,j)[x] = 0;
                    }
                    else
                    {
                        tmp.at<cv::Vec3b>(i,j)[x]=image.at<cv::Vec3b>(i-center,j-center)[x];
                    }
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    double *kernal = new double[kernal_size*kernal_size];
    double kernalsum = 0;
    for (int i = 0; i < kernal_size; i++)
    {
        for (int j = 0; j < kernal_size; j++)
        {
            kernal[i*kernal_size+j] = exp(-(i - center)*(i - center) / (2*sigma*sigma) - (j - center)*(j - center) / (2*sigma*sigma));
            kernalsum += kernal[i*kernal_size+j];
        }
    }
    for (int i = 0; i < kernal_size; i++)
    {
        for (int j = 0; j < kernal_size; j++)
        {
            kernal[i*kernal_size+j] /= kernalsum;
        }
    }


    for (int x = 0; x < channels; x++)  // gaussian filter
    {
        for (int i = center; i < height - center; i++)
        {
            for (int j = center; j < width - center; j++)
            {
                double t = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        t += tmp.at<cv::Vec3b>(i+m,j+n)[x] * kernal[(center+m)*kernal_size+n+center];
                    }
                }
                tmp.at<cv::Vec3b>(i,j)[x]=static_cast<int>(t);
            }
        }
    }

    for (int x = 0; x < channels; x++)
    {
        for (int i = 0; i < image.rows; i++)
        {
            for (int j = 0; j < image.cols; j++)
            {
                image.at<cv::Vec3b>(i,j)[x] = tmp.at<cv::Vec3b>(i+center,j+center)[x];
            }
        }
    }

    qimg = MainWindow::Mat2QImage(image);
    display(qimg);
}

void MainWindow::on_robertsoperatorButton_clicked(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    int height = srcGray.rows;
    int width = srcGray.cols;

    cv::Mat tmp(height+1, width+1, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+1; i++)      // extend border
    {
        for (int j = 0; j < width+1; j++)
        {
            if (i==height+1)
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if (j==width+1)
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i,j);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = 0; i < height - 1; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            int t1 = tmp.at<uchar>(i,j) - tmp.at<uchar>(i+1,j+1);
            int t2 = tmp.at<uchar>(i+1,j) - tmp.at<uchar>(i,j+1);
            tmp2.at<uchar>(i, j) = (uchar)sqrt(t1*t1 + t2*t2);
        }
    }

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            srcGray.at<uchar>(i,j)= tmp2.at<uchar>(i,j);
        }
    }

    qimg = MainWindow::Mat2QImage(srcGray);
    display(qimg);
}

void MainWindow::on_sobeloperatorButton_clicked(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    int height = srcGray.rows;
    int width = srcGray.cols;

    cv::Mat tmp(height+2, width+2, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2; i++)      // extend border
    {
        for (int j = 0; j < width+2; j++)
        {
            if (i==0||i==height+1)
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if (j==0||j==width+1)
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-1,j-1);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int gx = (-1)*tmp.at<uchar>(i-1,j-1) + (-2)*tmp.at<uchar>(i-1,j) + (-1)*tmp.at<uchar>(i-1,j+1)
                    + 1*tmp.at<uchar>(i+1,j-1) + 2*tmp.at<uchar>(i+1,j) + 1*tmp.at<uchar>(i+1,j+1);
            int gy = (-1)*tmp.at<uchar>(i-1,j-1) + (-2)*tmp.at<uchar>(i,j-1) + (-1)*tmp.at<uchar>(i+1,j-1)
                    + 1*tmp.at<uchar>(i-1,j+1) + 2*tmp.at<uchar>(i,j+1) + 1*tmp.at<uchar>(i+1,j+1);
            tmp2.at<uchar>(i, j) = (uchar)sqrt(gx*gx + gy*gy);
        }
    }

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            srcGray.at<uchar>(i,j)= tmp2.at<uchar>(i+1,j+1);
        }
    }

    qimg = MainWindow::Mat2QImage(srcGray);
    display(qimg);
}

void MainWindow::on_prewittoperatorButton_clicked(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    int height = srcGray.rows;
    int width = srcGray.cols;

    cv::Mat tmp(height+2, width+2, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2; i++)      // extend border
    {
        for (int j = 0; j < width+2; j++)
        {
            if (i==0||i==height+1)
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if (j==0||j==width+1)
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-1,j-1);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int gx = (-1)*tmp.at<uchar>(i-1,j-1) + (-1)*tmp.at<uchar>(i-1,j) + (-1)*tmp.at<uchar>(i-1,j+1)
                    + 1*tmp.at<uchar>(i+1,j-1) + 1*tmp.at<uchar>(i+1,j) + 1*tmp.at<uchar>(i+1,j+1);
            int gy = (-1)*tmp.at<uchar>(i-1,j-1) + (-1)*tmp.at<uchar>(i,j-1) + (-1)*tmp.at<uchar>(i+1,j-1)
                    + 1*tmp.at<uchar>(i-1,j+1) + 1*tmp.at<uchar>(i,j+1) + 1*tmp.at<uchar>(i+1,j+1);
            tmp2.at<uchar>(i, j) = (uchar)sqrt(gx*gx + gy*gy);
        }
    }

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            srcGray.at<uchar>(i,j)= tmp2.at<uchar>(i+1,j+1);
        }
    }

    qimg = MainWindow::Mat2QImage(srcGray);
    display(qimg);
}
