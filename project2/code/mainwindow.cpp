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
}

void MainWindow::on_openButton2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                ".",tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    qDebug()<<"filenames:"<<fileName;
    image = cv::imread(fileName.toLatin1().data());

    originalimg = image.clone();        //clone the img
    qimg = MainWindow::Mat2QImage(image);
    display(qimg);                      //display by the label
}


void MainWindow::on_markerButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                ".",tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    qDebug()<<"filenames:"<<fileName;
    marker = cv::imread(fileName.toLatin1().data());

    qimg = MainWindow::Mat2QImage(marker);
    displaymarker(qimg);                      //display by the label
}

void MainWindow::on_maskButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),
                                ".",tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
    qDebug()<<"filenames:"<<fileName;
    mask = cv::imread(fileName.toLatin1().data());

    qimg = MainWindow::Mat2QImage(mask);
    displaymask(qimg);                      //display by the label
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

void MainWindow::displaymarker(QImage img)
{
    QImage imgScaled;
    imgScaled = img.scaled(ui->imagelabel->size(),Qt::KeepAspectRatio);
    ui->imagelabel->setPixmap(QPixmap::fromImage(imgScaled));
}

void MainWindow::displaymask(QImage img)
{
    QImage imgScaled;
    imgScaled = img.scaled(ui->masklabel->size(),Qt::KeepAspectRatio);
    ui->masklabel->setPixmap(QPixmap::fromImage(imgScaled));
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

void MainWindow::on_originalButton_clicked()
{
    qimg = MainWindow::Mat2QImage(originalimg);
    display(qimg);
}

void MainWindow::on_originalButton2_clicked()
{
    qimg = MainWindow::Mat2QImage(originalimg);
    display(qimg);
}

void MainWindow::on_edgedetectionButton_clicked(){
    edgedetectionDialog = new EdgeDetectionDialog(this);
    edgedetectionDialog->setModal(false);
    edgedetectionDialog->show();
    connect(edgedetectionDialog, SIGNAL(StandardSignal()), this, SLOT(ShowStandardEdgeDetectionPic()));
    connect(edgedetectionDialog, SIGNAL(ExternalSignal()), this, SLOT(ShowExternalEdgeDetectionPic()));
    connect(edgedetectionDialog, SIGNAL(InternalSignal()), this, SLOT(ShowInternalEdgeDetectionPic()));
}


void MainWindow::ShowStandardEdgeDetectionPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::Mat srcBinary;

    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    cv::threshold(srcGray, srcBinary, 145, 255, cv::THRESH_BINARY); // convert the image to binary image

    int height = srcBinary.rows;
    int width = srcBinary.cols;

    int se_size = edgedetectionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = edgedetectionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcBinary.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // dilation
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            int flag = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    if (array[numcount] == 1 && tmp.at<uchar>(i+m,j+n) == 255){
                        flag = 1;
                    }
                    numcount++;
                }
            }
            tmp2.at<uchar>(i,j)= flag * 255;
        }
    }

    cv::Mat tmp3 = srcBinary.clone();   // tmp3 = F(+)K

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            tmp3.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    for (int i = center; i < height - center; i++)  // erosion
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            int flag = 1;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    if ((array[numcount] == 0 && tmp.at<uchar>(i+m,j+n) == 255) || (array[numcount] == 1 && tmp.at<uchar>(i+m,j+n) == 0)){
                        flag = 0;
                    }
                    numcount++;
                }
            }
            tmp2.at<uchar>(i,j)= flag * 255;
        }
    }

    cv::Mat tmp4 = srcBinary.clone();   // tmp4 = F(-)K

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            tmp4.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcBinary.clone(); // result = (F(+)K) - (F(-)K)  standard

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            result.at<uchar>(i,j) = tmp3.at<uchar>(i,j) - tmp4.at<uchar>(i,j);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::ShowExternalEdgeDetectionPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::Mat srcBinary;

    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    cv::threshold(srcGray, srcBinary, 145, 255, cv::THRESH_BINARY); // convert the image to binary image

    int height = srcBinary.rows;
    int width = srcBinary.cols;

    int se_size = edgedetectionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = edgedetectionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcBinary.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // dilation
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            int flag = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    if (array[numcount] == 1 && tmp.at<uchar>(i+m,j+n) == 255){
                        flag = 1;
                    }
                    numcount++;
                }
            }
            tmp2.at<uchar>(i,j)= flag * 255;
        }
    }

    cv::Mat tmp3 = srcBinary.clone();   // tmp3 = F+K

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            tmp3.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcBinary.clone(); // result = (F+K) - F  external

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            result.at<uchar>(i,j) = tmp3.at<uchar>(i,j) - srcBinary.at<uchar>(i,j);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::ShowInternalEdgeDetectionPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::Mat srcBinary;

    cv::cvtColor(image, srcGray, CV_BGR2GRAY);
    cv::threshold(srcGray, srcBinary, 145, 255, cv::THRESH_BINARY); // convert the image to binary image

    int height = srcBinary.rows;
    int width = srcBinary.cols;

    int se_size = edgedetectionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = edgedetectionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcBinary.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // erosion
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            int flag = 1;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    if ((array[numcount] == 0 && tmp.at<uchar>(i+m,j+n) == 255) || (array[numcount] == 1 && tmp.at<uchar>(i+m,j+n) == 0)){
                        flag = 0;
                    }
                    numcount++;
                }
            }
            tmp2.at<uchar>(i,j)= flag * 255;
        }
    }

    cv::Mat tmp4 = srcBinary.clone();   // tmp4 = F(-)K

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            tmp4.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcBinary.clone(); // result = F - (F(-)K)  internal

    for (int i = 0; i < srcBinary.rows; i++)
    {
        for (int j = 0; j < srcBinary.cols; j++)
        {
            result.at<uchar>(i,j) = srcBinary.at<uchar>(i,j) - tmp4.at<uchar>(i,j);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::on_gradientButton_clicked(){
    gradientDialog = new GradientDialog(this);
    gradientDialog->setModal(false);
    gradientDialog->show();
    connect(gradientDialog, SIGNAL(StandardGradientSignal()), this, SLOT(ShowStandardGradientPic()));
    connect(gradientDialog, SIGNAL(ExternalGradientSignal()), this, SLOT(ShowExternalGradientPic()));
    connect(gradientDialog, SIGNAL(InternalGradientSignal()), this, SLOT(ShowInternalGradientPic()));
}


void MainWindow::ShowStandardGradientPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);  // convert to gray scale image

    int height = srcGray.rows;
    int width = srcGray.cols;

    int se_size = gradientDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = gradientDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // dilation
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = array[numcount] + tmp.at<uchar>(i+m,j+n);
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[se_size*se_size-1] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[se_size*se_size-1] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[se_size*se_size-1];
            }
        }
    }

    cv::Mat tmp3 = srcGray.clone();   // tmp3 = F(+)K

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            tmp3.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    for (int i = center; i < height - center; i++)  // erosion
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = tmp.at<uchar>(i+m,j+n) - array[numcount];
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[0] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[0] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[0];
            }
        }
    }

    cv::Mat tmp4 = srcGray.clone();   // tmp4 = F(-)K

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            tmp4.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcGray.clone(); // result = 0.5((F+K) - (F-K))  standard

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            int tg = (tmp3.at<uchar>(i,j) - tmp4.at<uchar>(i,j))/2;
            if (tg > 255){
                result.at<uchar>(i,j) = 255;
            }
            else if (tg < 0){
                result.at<uchar>(i,j) = 0;
            }
            else{
                result.at<uchar>(i,j) = tg;
            }
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::ShowExternalGradientPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);  // convert to gray scale image

    int height = srcGray.rows;
    int width = srcGray.cols;

    int se_size = gradientDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = gradientDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // dilation
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = array[numcount] + tmp.at<uchar>(i+m,j+n);
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[se_size*se_size-1] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[se_size*se_size-1] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[se_size*se_size-1];
            }
        }
    }

    cv::Mat tmp3 = srcGray.clone();   // tmp3 = F(+)K

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            tmp3.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcGray.clone(); // result = 0.5((F+K) - F)  external

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            int tg = (tmp3.at<uchar>(i,j) - srcGray.at<uchar>(i,j))/2;
            if (tg > 255){
                result.at<uchar>(i,j) = 255;
            }
            else if (tg < 0){
                result.at<uchar>(i,j) = 0;
            }
            else{
                result.at<uchar>(i,j) = tg;
            }
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::ShowInternalGradientPic(){
    image = originalimg.clone();
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);  // convert to gray scale image

    int height = srcGray.rows;
    int width = srcGray.cols;

    int se_size = gradientDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = gradientDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // erosion
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = tmp.at<uchar>(i+m,j+n) - array[numcount];
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[0] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[0] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[0];
            }
        }
    }

    cv::Mat tmp4 = srcGray.clone();   // tmp4 = F(-)K

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            tmp4.at<uchar>(i,j) = tmp2.at<uchar>(i+center,j+center);
        }
    }

    cv::Mat result = srcGray.clone(); // result = 0.5(F - (F-K))  internal

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            int tg = (srcGray.at<uchar>(i,j) - tmp4.at<uchar>(i,j))/2;
            if (tg > 255){
                result.at<uchar>(i,j) = 255;
            }
            else if (tg < 0){
                result.at<uchar>(i,j) = 0;
            }
            else{
                result.at<uchar>(i,j) = tg;
            }
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    display(qimg);
}


void MainWindow::on_seButton1_clicked(){
    reconstructionDialog = new ReconstructionDialog(this);
    reconstructionDialog->setModal(false);
    reconstructionDialog->show();
    connect(reconstructionDialog, SIGNAL(ConditionalDilationSignal()), this, SLOT(ShowConditionalDilationPic()));
    connect(reconstructionDialog, SIGNAL(GeodesicDilationSignal()), this, SLOT(ShowGeodesicDilationPic()));
    connect(reconstructionDialog, SIGNAL(GeodesicErosionSignal()), this, SLOT(ShowConditionalDilationPic()));
    connect(reconstructionDialog, SIGNAL(OpeningByReconstructionSignal()), this, SLOT(ShowOpeningByReconstructionPic()));
    connect(reconstructionDialog, SIGNAL(ClosingByReconstructionSignal()), this, SLOT(ShowClosingByReconstructionPic()));
}

void MainWindow::on_seButton2_clicked(){
    reconstructionDialog = new ReconstructionDialog(this);
    reconstructionDialog->setModal(false);
    reconstructionDialog->show();
    connect(reconstructionDialog, SIGNAL(ConditionalDilationSignal()), this, SLOT(ShowConditionalDilationPic()));
    connect(reconstructionDialog, SIGNAL(GeodesicDilationSignal()), this, SLOT(ShowGeodesicDilationPic()));
    connect(reconstructionDialog, SIGNAL(GeodesicErosionSignal()), this, SLOT(ShowConditionalDilationPic()));
    connect(reconstructionDialog, SIGNAL(OpeningByReconstructionSignal()), this, SLOT(ShowOpeningByReconstructionPic()));
    connect(reconstructionDialog, SIGNAL(ClosingByReconstructionSignal()), this, SLOT(ShowClosingByReconstructionPic()));
}


void MainWindow::ShowConditionalDilationPic(){
    cv::Mat markerGray;
    cv::Mat maskGray;
    cv::Mat markerBinary;
    cv::Mat maskBinary;

    cv::cvtColor(marker, markerGray, CV_BGR2GRAY);
    cv::threshold(markerGray, markerBinary, 145, 255, cv::THRESH_BINARY); // convert the marker to binary image

    cv::cvtColor(mask, maskGray, CV_BGR2GRAY);
    cv::threshold(maskGray, maskBinary, 145, 255, cv::THRESH_BINARY); // convert the mask to binary image

    int height = markerBinary.rows;
    int width = markerBinary.cols;

    int se_size = reconstructionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = reconstructionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat marker_extmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat mask_extmp(height+2*center, width+2*center, CV_8UC1);

    for (int i = 0; i < height+2*center; i++)       // extend border of marker
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                marker_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    marker_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    marker_extmp.at<uchar>(i,j)=markerBinary.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    for (int i = 0; i < height+2*center; i++)       // extend border of mask
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                mask_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    mask_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    mask_extmp.at<uchar>(i,j)=maskBinary.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    cv::Mat tmp = marker_extmp.clone();
    cv::Mat tmp2 = marker_extmp.clone();
    cv::Mat tmp3 = marker_extmp.clone();

    height = tmp.rows;
    width = tmp.cols;

    bool eq;

    while (eq == false){
        for (int i = center; i < height - center; i++)  // dilation
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                int flag = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        if (array[numcount] == 1 && tmp.at<uchar>(i+m,j+n) == 255){
                            flag = 1;
                        }
                        numcount++;
                    }
                }
                tmp2.at<uchar>(i,j)= flag * 255;
            }
        }

        for (int i = 0; i < height; i++){   // intersection
            for (int j = 0; j < width; j++){
                if (tmp2.at<uchar>(i,j) == 255 && mask_extmp.at<uchar>(i,j) == 255){
                    tmp3.at<uchar>(i,j) = 255;
                }
            }
        }

        eq = true;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (tmp3.at<uchar>(i,j) != tmp.at<uchar>(i,j)){
                    eq = false;
                    goto outloop1;
                }
            }
        }
        outloop1:
        tmp = tmp3.clone();
    }

    cv::Mat result = markerBinary.clone();

    for (int i = 0; i < markerBinary.rows; i++)
    {
        for (int j = 0; j < markerBinary.cols; j++)
        {
            result.at<uchar>(i,j) = tmp3.at<uchar>(i+center,j+center);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    displaymarker(qimg);
}


void MainWindow::ShowGeodesicDilationPic(){
    cv::Mat markerGray;
    cv::Mat maskGray;

    cv::cvtColor(marker, markerGray, CV_BGR2GRAY);
    cv::cvtColor(mask, maskGray, CV_BGR2GRAY);

    int height = markerGray.rows;
    int width = markerGray.cols;

    int se_size = reconstructionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = reconstructionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat marker_extmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat mask_extmp(height+2*center, width+2*center, CV_8UC1);

    for (int i = 0; i < height+2*center; i++)       // extend border of marker
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                marker_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    marker_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    marker_extmp.at<uchar>(i,j)=markerGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    for (int i = 0; i < height+2*center; i++)       // extend border of mask
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                mask_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    mask_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    mask_extmp.at<uchar>(i,j)=maskGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    cv::Mat tmp = marker_extmp.clone();
    cv::Mat tmp2 = marker_extmp.clone();
    cv::Mat tmp3 = marker_extmp.clone();

    height = tmp.rows;
    width = tmp.cols;

    bool eq;

    while (eq == false){
        for (int i = center; i < height - center; i++)  // dilation
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        sort_array[numcount] = array[numcount] + tmp.at<uchar>(i+m,j+n);
                        numcount++;
                    }
                }
                bubble_sort(sort_array, se_size*se_size);

                if (sort_array[se_size*se_size-1] > 255){
                    tmp2.at<uchar>(i,j) = 255;
                }
                else if (sort_array[se_size*se_size-1] < 0){
                    tmp2.at<uchar>(i,j) = 0;
                }
                else{
                    tmp2.at<uchar>(i,j) = sort_array[se_size*se_size-1];
                }
            }
        }

        for (int i = 0; i < height; i++){   // find min
            for (int j = 0; j < width; j++){
                tmp3.at<uchar>(i,j) = min(tmp2.at<uchar>(i,j), mask_extmp.at<uchar>(i,j));
            }
        }

        eq = true;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (tmp3.at<uchar>(i,j) != tmp.at<uchar>(i,j)){
                    eq = false;
                    goto outloop2;
                }
            }
        }
        outloop2:
        tmp = tmp3.clone();
    }

    cv::Mat result = markerGray.clone();

    for (int i = 0; i < markerGray.rows; i++)
    {
        for (int j = 0; j < markerGray.cols; j++)
        {
            result.at<uchar>(i,j) = tmp3.at<uchar>(i+center,j+center);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    displaymarker(qimg);
}

void MainWindow::ShowGeodesicErosionPic(){
    cv::Mat markerGray;
    cv::Mat maskGray;

    cv::cvtColor(marker, markerGray, CV_BGR2GRAY);
    cv::cvtColor(mask, maskGray, CV_BGR2GRAY);

    int height = markerGray.rows;
    int width = markerGray.cols;

    int se_size = reconstructionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = reconstructionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat marker_extmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat mask_extmp(height+2*center, width+2*center, CV_8UC1);

    for (int i = 0; i < height+2*center; i++)       // extend border of marker
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                marker_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    marker_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    marker_extmp.at<uchar>(i,j)=markerGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    for (int i = 0; i < height+2*center; i++)       // extend border of mask
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                mask_extmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    mask_extmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    mask_extmp.at<uchar>(i,j)=maskGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    cv::Mat tmp = marker_extmp.clone();
    cv::Mat tmp2 = marker_extmp.clone();
    cv::Mat tmp3 = marker_extmp.clone();

    height = tmp.rows;
    width = tmp.cols;

    bool eq;

    while (eq == false){
        for (int i = center; i < height - center; i++)  // erosion
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        sort_array[numcount] = tmp.at<uchar>(i+m,j+n) - array[numcount];
                        numcount++;
                    }
                }
                bubble_sort(sort_array, se_size*se_size);

                if (sort_array[0] > 255){
                    tmp2.at<uchar>(i,j) = 255;
                }
                else if (sort_array[0] < 0){
                    tmp2.at<uchar>(i,j) = 0;
                }
                else{
                    tmp2.at<uchar>(i,j) = sort_array[0];
                }
            }
        }

        for (int i = 0; i < height; i++){   // find max
            for (int j = 0; j < width; j++){
                tmp3.at<uchar>(i,j) = max(tmp2.at<uchar>(i,j), mask_extmp.at<uchar>(i,j));
            }
        }

        eq = true;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (tmp3.at<uchar>(i,j) != tmp.at<uchar>(i,j)){
                    eq = false;
                    goto outloop3;
                }
            }
        }
        outloop3:
        tmp = tmp3.clone();
    }

    cv::Mat result = markerGray.clone();

    for (int i = 0; i < markerGray.rows; i++)
    {
        for (int j = 0; j < markerGray.cols; j++)
        {
            result.at<uchar>(i,j) = tmp3.at<uchar>(i+center,j+center);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    displaymarker(qimg);

}

void MainWindow::ShowOpeningByReconstructionPic(){
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);  // convert to gray scale image
    int height = srcGray.rows;
    int width = srcGray.cols;

    int se_size = reconstructionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = reconstructionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // erosion
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = tmp.at<uchar>(i+m,j+n) - array[numcount];
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[0] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[0] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[0];
            }
        }
    }

    cv::Mat mask_extmp = tmp.clone();

    cv::Mat tmp_ = tmp2.clone();
    cv::Mat tmp_2 = tmp2.clone();
    cv::Mat tmp_3 = tmp2.clone();

    height = tmp_.rows;
    width = tmp_.cols;

    bool eq = false;

    while (eq == false){
        for (int i = center; i < height - center; i++)  // dilation
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        sort_array[numcount] = array[numcount] + tmp_.at<uchar>(i+m,j+n);
                        numcount++;
                    }
                }
                bubble_sort(sort_array, se_size*se_size);

                if (sort_array[se_size*se_size-1] > 255){
                    tmp_2.at<uchar>(i,j) = 255;
                }
                else if (sort_array[se_size*se_size-1] < 0){
                    tmp_2.at<uchar>(i,j) = 0;
                }
                else{
                    tmp_2.at<uchar>(i,j) = sort_array[se_size*se_size-1];
                }
            }
        }

        for (int i = 0; i < height; i++){   // find min
            for (int j = 0; j < width; j++){
                tmp_3.at<uchar>(i,j) = min(tmp_2.at<uchar>(i,j), mask_extmp.at<uchar>(i,j));
            }
        }

        eq = true;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (tmp_3.at<uchar>(i,j) != tmp_.at<uchar>(i,j)){
                    eq = false;
                    goto outloop4;
                }
            }
        }
        outloop4:
        tmp_ = tmp_3.clone();
    }

    cv::Mat result = srcGray.clone();

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            result.at<uchar>(i,j) = tmp_3.at<uchar>(i+center,j+center);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    displaymarker(qimg);
}

void MainWindow::ShowClosingByReconstructionPic(){
    cv::Mat srcGray;
    cv::cvtColor(image, srcGray, CV_BGR2GRAY);  // convert to gray scale image
    int height = srcGray.rows;
    int width = srcGray.cols;

    int se_size = reconstructionDialog->GetSizeValue();  // get size of se
    int center = (se_size-1)/2;
    int *array;
    array = (int*)malloc(sizeof(int)*se_size*se_size);
    int *sort_array;
    sort_array = (int*)malloc(sizeof(int)*se_size*se_size);

    for (int i=0; i<se_size*se_size; i++){
        array[i] = reconstructionDialog->GetSeValue(i/se_size,i-i/se_size*se_size);
    }

    cv::Mat tmp(height+2*center, width+2*center, CV_8UC1);
    cv::Mat tmp2 = tmp.clone();

    for (int i = 0; i < height+2*center; i++)       // extend border
    {
        for (int j = 0; j < width+2*center; j++)
        {
            if ((i>=0&&i<=center-1)||(i>=height+center&&i<=height+2*center-1))
            {
                tmp.at<uchar>(i,j) = 0;
            }
            else
            {
                if ((j>=0&&j<=center-1)||(j>=width+center&&j<=width+2*center-1))
                {
                    tmp.at<uchar>(i,j) = 0;
                }
                else
                {
                    tmp.at<uchar>(i,j)=srcGray.at<uchar>(i-center,j-center);
                }
            }
        }
    }

    height = tmp.rows;
    width = tmp.cols;

    for (int i = center; i < height - center; i++)  // dilation
    {
        for (int j = center; j < width - center; j++)
        {
            int numcount = 0;
            for (int m = -center; m <= center; m++)
            {
                for (int n = -center; n <= center; n++)
                {
                    sort_array[numcount] = array[numcount] + tmp.at<uchar>(i+m,j+n);
                    numcount++;
                }
            }
            bubble_sort(sort_array, se_size*se_size);

            if (sort_array[se_size*se_size-1] > 255){
                tmp2.at<uchar>(i,j) = 255;
            }
            else if (sort_array[se_size*se_size-1] < 0){
                tmp2.at<uchar>(i,j) = 0;
            }
            else{
                tmp2.at<uchar>(i,j) = sort_array[se_size*se_size-1];
            }
        }
    }

    cv::Mat mask_extmp = tmp.clone();

    cv::Mat tmp_ = tmp2.clone();
    cv::Mat tmp_2 = tmp2.clone();
    cv::Mat tmp_3 = tmp2.clone();

    height = tmp_.rows;
    width = tmp_.cols;

    bool eq = false;

    while (eq == false){
        for (int i = center; i < height - center; i++)  // erosion
        {
            for (int j = center; j < width - center; j++)
            {
                int numcount = 0;
                for (int m = -center; m <= center; m++)
                {
                    for (int n = -center; n <= center; n++)
                    {
                        sort_array[numcount] = tmp_.at<uchar>(i+m,j+n) - array[numcount];
                        numcount++;
                    }
                }
                bubble_sort(sort_array, se_size*se_size);

                if (sort_array[0] > 255){
                    tmp_2.at<uchar>(i,j) = 255;
                }
                else if (sort_array[0] < 0){
                    tmp_2.at<uchar>(i,j) = 0;
                }
                else{
                    tmp_2.at<uchar>(i,j) = sort_array[0];
                }
            }
        }

        for (int i = 0; i < height; i++){   // find max
            for (int j = 0; j < width; j++){
                tmp_3.at<uchar>(i,j) = max(tmp_2.at<uchar>(i,j), mask_extmp.at<uchar>(i,j));
            }
        }

        eq = true;
        for (int i = 0; i < height; i++){
            for (int j = 0; j < width; j++){
                if (tmp_3.at<uchar>(i,j) != tmp_.at<uchar>(i,j)){
                    eq = false;
                    goto outloop5;
                }
            }
        }
        outloop5:
        tmp_ = tmp_3.clone();
    }

    cv::Mat result = srcGray.clone();

    for (int i = 0; i < srcGray.rows; i++)
    {
        for (int j = 0; j < srcGray.cols; j++)
        {
            result.at<uchar>(i,j) = tmp_3.at<uchar>(i+center,j+center);
        }
    }

    qimg = MainWindow::Mat2QImage(result);
    displaymarker(qimg);
}


