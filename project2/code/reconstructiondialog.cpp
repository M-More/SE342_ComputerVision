#include "reconstructiondialog.h"
#include "ui_reconstructiondialog.h"

ReconstructionDialog::ReconstructionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReconstructionDialog)
{
    ui->setupUi(this);
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(ConfirmSize()));
    connect(ui->conditionaldilationButton,SIGNAL(clicked()),this,SLOT(ConditionalDilation()));
    connect(ui->geodesicdilationButton,SIGNAL(clicked()),this,SLOT(GeodesicDilation()));
    connect(ui->geodesicerosionButton,SIGNAL(clicked()),this,SLOT(GeodesicErosion()));
    connect(ui->openingButton,SIGNAL(clicked()),this,SLOT(OpeningByReconstruction()));
    connect(ui->closingButton,SIGNAL(clicked()),this,SLOT(ClosingByReconstruction()));
}

ReconstructionDialog::~ReconstructionDialog()
{
    delete ui;
}

int ReconstructionDialog::GetSizeValue()
{
    int a=ui->spinBox->value();
    return a;
}

int ReconstructionDialog::GetSeValue(int row, int col){
    int a=ui->tableWidget->item(row, col)->text().toInt();
    return a;
}

void ReconstructionDialog::ConfirmSize()
{
    ui->tableWidget->setRowCount(GetSizeValue());
    ui->tableWidget->setColumnCount(GetSizeValue());
    for (int i=0; i<GetSizeValue(); i++){
        ui->tableWidget->setColumnWidth(i,50);
    }
    for (int i=0; i<GetSizeValue(); i++){
        for (int j=0; j<GetSizeValue(); j++){
            ui->tableWidget->setItem(i,j,new QTableWidgetItem("0"));
        }
    }
}

void ReconstructionDialog::ConditionalDilation(){
    emit ConditionalDilationSignal();
}

void ReconstructionDialog::GeodesicDilation(){
    emit GeodesicDilationSignal();
}

void ReconstructionDialog::GeodesicErosion(){
    emit GeodesicErosionSignal();
}

void ReconstructionDialog::OpeningByReconstruction(){
    emit OpeningByReconstructionSignal();
}

void ReconstructionDialog::ClosingByReconstruction(){
    emit ClosingByReconstructionSignal();
}
