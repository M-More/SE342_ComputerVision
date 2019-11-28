#include "gaussianfilterdialog.h"
#include "ui_gaussianfilterdialog.h"

GaussianFilterDialog::GaussianFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussianFilterDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(3,25);
    ui->horizontalSlider->setSingleStep(2);
    ui->spinBox->setRange(3,25);
    ui->spinBox->setSingleStep(2);
    ui->doubleSpinBox->setRange(0,10);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Test()));
}

GaussianFilterDialog::~GaussianFilterDialog()
{
    delete ui;
}

int GaussianFilterDialog::GetSpinBoxValue()
{
    int a=ui->spinBox->value();
    return a;
}

double GaussianFilterDialog::GetDoubleSpinBoxValue()
{
    double sigma=ui->doubleSpinBox->value();
    return sigma;
}

void GaussianFilterDialog::Test()
{
    emit Signal3();
}

