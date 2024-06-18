#include "RegionOfInterestDialog.h"
#include "../../build/ui_regionofinterestdialog.h"
#include "../FrameData/Data/RegionOfInterest.h"
#include "../main/MainWindow.h"
#include <QSettings>
RegionOfInterestDialog::RegionOfInterestDialog(Config* a_current_config, const Image& a_background_image, QWidget* a_parent)
    : QDialog(a_parent), m_ui(new Ui::RegionOfInterestDialog) {
    m_ui->setupUi(this);
    m_roi_to_show = new QVector<RegionOfInterest*>();
    foreach(RegionOfInterest* roi, a_current_config->get_config_detector().get_rois()) {
        RegionOfInterest* roi_to_add = new RegionOfInterest(roi->get_name(), roi->get_id(), roi->get_polygon(), roi->is_inside(), roi->get_opacity());
        m_roi_to_show->append(roi_to_add);
    }
    m_painter_scene = nullptr;
    m_current_config = a_current_config;
    m_current_background_image = a_background_image;
    this->create_q_polygon_painter();
    if(a_current_config->get_config_detector().get_rois().size() > 0 &&
       !a_current_config->get_config_detector().get_rois().at(0)->is_inside()) {
        m_ui->interestIsOutside->setChecked(true);
    } else {
        m_ui->interestIsInside->setChecked(true);
    }

    foreach(RegionOfInterest* roi, *m_roi_to_show) {
        add_region_to_table(roi);
    }
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    resize(1500, 800);
}

RegionOfInterestDialog::~RegionOfInterestDialog() {
    delete m_ui;
}

void RegionOfInterestDialog::on_btnAddRegion_clicked() {
    if(m_is_adding) {
        m_ui->interestIsInside->setVisible(true);
        m_ui->interestIsOutside->setVisible(true);
        m_ui->btnAddRegion->setText(new_region);
        m_new_region = new RegionOfInterest();
        m_config_dialog = new ConfigOfRegionOfInterestDialog(m_new_region);
        m_config_dialog->setAttribute(Qt::WA_DeleteOnClose); 
        connect(m_config_dialog, &ConfigOfRegionOfInterestDialog::sig_config_finished, this, &RegionOfInterestDialog::slot_config_finished);
        m_config_dialog->exec();
        m_is_adding = false;
    } else {
        m_ui->interestIsInside->setVisible(false);
        m_ui->interestIsOutside->setVisible(false);
        m_ui->btnAddRegion->setText(add_region);
        m_is_adding = true;
        m_painter_scene->new_polygon();
    }
}

int RegionOfInterestDialog::get_highest_id() {
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);

    int highest_id = settings.value("region_of_interest/max_id").toInt();
    settings.beginGroup("region_of_interest");
    settings.setValue("max_id", highest_id+1);
    settings.endGroup();
    return highest_id;
}

bool RegionOfInterestDialog::is_interest_inside() {
    return m_ui->interestIsInside->isChecked();
}

void RegionOfInterestDialog::slot_config_finished() {
    delete m_config_dialog;
    m_new_region->set_id(get_highest_id() + 1);
    m_new_region->set_is_inside(is_interest_inside());
    m_new_region->set_polygon(m_painter_scene->get_new_polygon());
    m_roi_to_show->append(m_new_region);
    m_painter_scene->add_region(m_new_region);
    if(m_roi_to_show->size() == 1) {
        create_q_polygon_painter();
    }
    add_region_to_table(m_new_region);
}

void RegionOfInterestDialog::on_interestIsInside_clicked() {
    foreach(RegionOfInterest* roi, *m_roi_to_show) {
        roi->set_is_inside(true);
    }
    create_q_polygon_painter();
}

void RegionOfInterestDialog::on_interestIsOutside_clicked() {
    foreach(RegionOfInterest* roi, *m_roi_to_show) {
        roi->set_is_inside(false);
    }
    create_q_polygon_painter();
}

void RegionOfInterestDialog::add_region_to_table(RegionOfInterest* a_region_to_add) {
    int current_row_count = m_ui->roiTable->rowCount();
    m_ui->roiTable->insertRow(current_row_count);

    QTableWidgetItem* id_item = new QTableWidgetItem(QString::number(a_region_to_add->get_id()));
    id_item->setFlags(id_item->flags() & ~Qt::ItemIsEditable);
    m_ui->roiTable->setItem(current_row_count, 0, id_item);

    QTableWidgetItem* name_item = new QTableWidgetItem(a_region_to_add->get_name());
    name_item->setFlags(name_item->flags() & ~Qt::ItemIsEditable);
    m_ui->roiTable->setItem(current_row_count, 1, name_item);

    QTableWidgetItem* opacity_item = new QTableWidgetItem(QString::number(a_region_to_add->get_opacity(), 'f', 2));
    opacity_item->setFlags(opacity_item->flags() & ~Qt::ItemIsEditable);
    m_ui->roiTable->setItem(current_row_count, 2, opacity_item);

    QPushButton* delete_button = new QPushButton("Delete");
    m_ui->roiTable->setCellWidget(current_row_count, 3, delete_button);
    connect(delete_button, &QPushButton::clicked, this, [this, current_row_count]() {
        on_Delete_clicked(current_row_count);
    });
}

void RegionOfInterestDialog::on_Delete_clicked(int a_row) {
    if (a_row < 0 || a_row >= m_roi_to_show->size()) return;
    delete m_roi_to_show->at(a_row);
    m_roi_to_show->removeAt(a_row);
    m_ui->roiTable->removeRow(a_row);
    create_q_polygon_painter();
}



void RegionOfInterestDialog::create_q_polygon_painter(){
    if (m_painter_scene != nullptr) {
        delete m_painter_scene;
    }
    bool is_inside = !m_roi_to_show->isEmpty() && m_roi_to_show->first()->is_inside();
    m_painter_scene = new RegionOfInterestPainterScene(is_inside, m_current_background_image, *m_roi_to_show);
    m_ui->graphicsView->setScene(m_painter_scene);
}


void RegionOfInterestDialog::on_btnSave_clicked() {
    m_current_config->get_config_detector().set_rois(*m_roi_to_show);
    emit sig_update_config();
    this->close();
}

void RegionOfInterestDialog::on_pushButton_clicked() {
    this->close();
}

