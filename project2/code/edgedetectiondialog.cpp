#include "edgedetectiondialog.h"
#include "ui_edgedetectiondialog.h"

EdgeDetectionDialog::EdgeDetectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdgeDetectionDialog)
{
    ui->setupUi(this);
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(ConfirmSize()));
    connect(ui->standardButton,SIGNAL(clicked()),this,SLOT(Standard()));
    connect(ui->externalButton,SIGNAL(clicked()),this,SLOT(External()));
    connect(ui->internalButton,SIGNAL(clicked()),this,SLOT(Internal()));
}

EdgeDetectionDialog::~EdgeDetectionDialog()
{
    delete ui;
}

int EdgeDetectionDialog::GetSizeValue()
{
    int a=ui->spinBox->value();
    return a;
}

int EdgeDetectionDialog::GetSeValue(int row, int col){
    int a=ui->tableWidget->item(row, col)->text().toInt();
    return a;
}

void EdgeDetectionDialog::ConfirmSize()
{
    ui->tableWidget->setRowCount(GetSizeValue());
    ui->tableWidget->setColumnCount(GetSizeValue());
    for (int i=0; i<GetSizeValue(); i++){
        ui->tableWidget->setColumnWidth(i,50);
    }
}

void EdgeDetectionDialog::Standard(){
    emit StandardSignal();
}

void EdgeDetectionDialog::External(){
    emit ExternalSignal();
}

void EdgeDetectionDialog::Internal(){
    emit InternalSignal();
}
