#include "CamFrame.h"
#include <QToolBar>
#include "../../build/ui_camframe.h"
#include "RegionOfInterestDialog.h"
#include "../FrameData/Config/ConfigFrame.h"
#include "../Export/ExportDialog.h"
CamFrame::CamFrame(QWidget *a_parent)
    : QFrame(a_parent), m_ui(new Ui::CamFrame) 
{
    m_ui->setupUi(this);
    QToolBar* a_toolbar = new QToolBar();

    QAction* a_config_action = a_toolbar->addAction(m_name_of_config_toolbar_entry); 
    connect(a_config_action, &QAction::triggered, this, &CamFrame::slot_on_config_clicked);

    QAction* a_roi_action = a_toolbar->addAction(m_name_of_region_of_interest_toolbar_entry); 
    connect(a_roi_action, &QAction::triggered, this, &CamFrame::slot_on_roi_clicked);

    QAction* a_export_action = a_toolbar->addAction(m_name_of_export_toolbar_entry); 
    connect(a_export_action, &QAction::triggered, this, &CamFrame::slot_on_export_clicked);

    a_toolbar->setStyleSheet(m_toolbar_stylesheet);

    m_video_player_widget.setStyleSheet(m_video_player_stylesheet); 

    QVBoxLayout* a_layout = new QVBoxLayout();
    a_layout->setMenuBar(a_toolbar);
    a_layout->addWidget(&m_video_player_widget); 
    a_layout->setContentsMargins(0, 0, 0, 0);

    QWidget* dock_content = new QWidget(); 
    dock_content->setLayout(a_layout);
    m_ui->verticalLayout->addWidget(dock_content);
    disable_all_childs();
   
}

CamFrame::~CamFrame()
{
}


void CamFrame::disable_all_childs(){
    const QObjectList &children = this->children();
    for (QObject *a_child : children) {
        QWidget *a_widget = qobject_cast<QWidget *>(a_child);
        if (a_widget) {
            a_widget->setEnabled(false);
        }
    }
}

void CamFrame::change_stream(Stream* a_new_stream) {
    a_new_stream->read_config_from_backend();
    m_current_stream = a_new_stream;
    m_video_player_widget.change_stream(a_new_stream); 
    const QObjectList &a_children = this->children();
    for (QObject *a_child : a_children) {
        QWidget *a_widget = qobject_cast<QWidget *>(a_child);
        if (a_widget) {
            a_widget->setEnabled(true);
        }
    }
}



void CamFrame::slot_config_changed()
{
    m_current_stream->update_config();
}

void CamFrame::mousePressEvent(QMouseEvent* a_event)
{
    emit sig_cam_selected(this);
}

void CamFrame::slot_on_config_clicked()
{
    ConfigFrame* a_config = new ConfigFrame(nullptr, m_current_stream->get_config());
    connect(a_config, &ConfigFrame::sig_update_config, this, &CamFrame::slot_config_changed); 
    a_config->setAttribute(Qt::WA_DeleteOnClose);
    a_config->show(); 

}

void CamFrame::slot_on_roi_clicked()
{
    OnlyVideo type_of_range; 
    DataFrameStream* a_stream = m_current_stream->get_live_stream(type_of_range.get_enum()); 

    Image image_to_add = a_stream->get_next_data_frame(true)->get_image(); 

    delete a_stream;
    RegionOfInterestDialog* a_roi_dialog = new RegionOfInterestDialog(m_current_stream->get_config(), image_to_add); 
    connect(a_roi_dialog, &RegionOfInterestDialog::sig_update_config, this, &CamFrame::slot_config_changed); 
    a_roi_dialog->setAttribute(Qt::WA_DeleteOnClose);
    a_roi_dialog->exec(); 
}

void CamFrame::slot_on_export_clicked()
{
    ExportDialog* exportDialog = new ExportDialog(m_current_stream);
    exportDialog->setAttribute(Qt::WA_DeleteOnClose);
    exportDialog->exec(); 
}

bool CamFrame::is_showing_stream(Stream* a_stream_to_check){
    return a_stream_to_check == m_current_stream;
}

void CamFrame::stop_stream(){
    m_video_player_widget.stop_stream();
    disable_all_childs();
}
