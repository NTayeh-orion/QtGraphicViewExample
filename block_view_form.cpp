#include "block_view_form.h"
#include "ui_block_view_form.h"

BlockViewForm::BlockViewForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BlockViewForm)
{
    ui->setupUi(this);
}

BlockViewForm::~BlockViewForm()
{
    delete ui;
}
