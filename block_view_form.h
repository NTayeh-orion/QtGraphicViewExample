#ifndef BLOCK_VIEW_FORM_H
#define BLOCK_VIEW_FORM_H

#include <QWidget>

namespace Ui {
class BlockViewForm;
}

class BlockViewForm : public QWidget
{
    Q_OBJECT

public:
    void extracted();
    explicit BlockViewForm(QWidget *parent = nullptr);
    ~BlockViewForm();
    Ui::BlockViewForm *ui;

private:
};

#endif // BLOCK_VIEW_FORM_H
