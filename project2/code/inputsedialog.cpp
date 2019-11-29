#include "inputsedialog.h"
#include "ui_inputsedialog.h"

inputseDialog::inputseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputseDialog)
{
    ui->setupUi(this);
    connect(ui->confirmButton,SIGNAL(clicked()),this,SLOT(Confirm()));
}

inputseDialog::~inputseDialog()
{
    delete ui;
}

int inputseDialog::GetRowValue()
{
    int a=ui->rowspinBox->value();
    return a;
}

int inputseDialog::GetColumnValue()
{
    int a=ui->colspinBox->value();
    return a;
}

void inputseDialog::Confirm()
{
    ui->tableWidget->setRowCount(GetRowValue());
    ui->tableWidget->setColumnCount(GetColumnValue());
}
