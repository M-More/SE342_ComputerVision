#include "mediumfilterdialog.h"
#include "ui_mediumfilterdialog.h"

MediumFilterDialog::MediumFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MediumFilterDialog)
{
    ui->setupUi(this);
    ui->horizontalSlider->setRange(3,25);
    ui->horizontalSlider->setPageStep(2);
    ui->spinBox->setRange(3,25);
    ui->spinBox->setSingleStep(2);
    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Test2()));
}

MediumFilterDialog::~MediumFilterDialog()
{
    delete ui;
}

int MediumFilterDialog::GetSpinBoxValue()
{
    int a=ui->spinBox->value();
    return a;
}

void MediumFilterDialog::Test2()
{
    emit Signal2();
}
