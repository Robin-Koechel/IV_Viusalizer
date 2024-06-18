
#include "ConfigOfRegionOfInterestDialog.h"
#include "../../build/ui_configofregionofinterestdialog.h"

ConfigOfRegionOfInterestDialog::ConfigOfRegionOfInterestDialog(RegionOfInterest* a_to_configure, QWidget* a_parent)
    : QDialog(a_parent), m_ui(new Ui::ConfigOfRegionOfInterestDialog) {
    m_ui->setupUi(this);
    m_to_configure = a_to_configure;
}

ConfigOfRegionOfInterestDialog::~ConfigOfRegionOfInterestDialog() {
    delete m_ui;
}

void ConfigOfRegionOfInterestDialog::on_btnSave_clicked() {
    float value_of_box = m_ui->opacitySpinBox->value();
    m_to_configure->set_opacity(value_of_box);
    m_to_configure->set_name(m_ui->nameTextEdit->text());
    emit sig_config_finished();
}
