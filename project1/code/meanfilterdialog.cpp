#include "meanfilterdialog.h"
#include "ui_meanfilterdialog.h"

MeanFilterDialog::MeanFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeanFilterDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(3,25);
    ui->horizontalSlider->setPageStep(2);
    ui->spinBox->setRange(3,25);
    ui->spinBox->setSingleStep(2);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Test()));
}

MeanFilterDialog::~MeanFilterDialog()
{
    delete ui;
}

int MeanFilterDialog::GetSpinBoxValue()
{
    int a=ui->spinBox->value();
    return a;
}

void MeanFilterDialog::Test()
{
    emit Signal1();
}
