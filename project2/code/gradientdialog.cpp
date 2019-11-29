#include "gradientdialog.h"
#include "ui_gradientdialog.h"

GradientDialog::GradientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(ConfirmSize()));
    connect(ui->standardButton,SIGNAL(clicked()),this,SLOT(Standard()));
    connect(ui->externalButton,SIGNAL(clicked()),this,SLOT(External()));
    connect(ui->internalButton,SIGNAL(clicked()),this,SLOT(Internal()));
}

GradientDialog::~GradientDialog()
{
    delete ui;
}

int GradientDialog::GetSizeValue()
{
    int a=ui->spinBox->value();
    return a;
}

int GradientDialog::GetSeValue(int row, int col){
    int a=ui->tableWidget->item(row, col)->text().toInt();
    return a;
}

void GradientDialog::ConfirmSize()
{
    ui->tableWidget->setRowCount(GetSizeValue());
    ui->tableWidget->setColumnCount(GetSizeValue());
    for (int i=0; i<GetSizeValue(); i++){
        ui->tableWidget->setColumnWidth(i,50);
    }
}

void GradientDialog::Standard(){
    emit StandardGradientSignal();
}

void GradientDialog::External(){
    emit ExternalGradientSignal();
}

void GradientDialog::Internal(){
    emit InternalGradientSignal();
}
