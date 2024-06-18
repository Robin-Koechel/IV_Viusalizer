
#include "ClusterConfigDialog.h"
#include "../../build/ui_clusterconfigdialog.h"

ClusterConfigDialog::ClusterConfigDialog( QVector<QString> a_current_input, QWidget *a_parent)
    : QDialog(a_parent)
    , m_ui(new Ui::ClusterConfigDialog)
{
    m_ui->setupUi(this);
    if(!a_current_input.isEmpty()){
        m_ui->leUrl->setText(a_current_input.at(0));
        m_ui->lEName->setText(a_current_input.at(1));
        m_ui->leUrl->setReadOnly(true);
    }

}

QVector<QString> ClusterConfigDialog::get_input(){
    QVector<QString> a_input;
    a_input.append(m_ui->leUrl->text());
    a_input.append(m_ui->lEName->text());
    return a_input;
}

ClusterConfigDialog::~ClusterConfigDialog()
{
    delete m_ui;
}