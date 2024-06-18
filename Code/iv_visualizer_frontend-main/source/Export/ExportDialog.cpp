#include "ExportDialog.h"
#include "../BackendConnector/TimeRange.h"
#include "ExportDialog.h"
#include "ImageToVideoConverter.h"
#include "../../build/ui_exportdialog.h"
#include "../BackendConnector/Stream.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "../main/MainWindow.h"
ExportDialog::ExportDialog(Stream* a_stream_to_export, QWidget *a_parent)
    : QDialog(a_parent)
    , m_ui(new Ui::ExportDialog)
{
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    m_ns_to_go_to_the_future = settings.value(settings_group_name + m_settings_go_forward_in_ms).toInt();
    m_ns_to_go_to_the_future = m_ns_to_go_to_the_future * convert_ms_to_ns;
    m_ui->setupUi(this);
    this->m_video_player_widget = new VideoPlayerWidget();
    m_video_player_widget->disablelive();
    m_video_player_widget->setStyleSheet(m_video_player_stylesheet); 
    this->m_video_player_widget->change_stream(a_stream_to_export);
    m_ui->videoPlayerLayout->addWidget(this->m_video_player_widget);
    this->m_marks.append(0);
    this->m_marks.append(0);
    connect(this->m_video_player_widget, &VideoPlayerWidget::sig_navigator_moved, this, &ExportDialog::slot_position_changed);
    this->m_stream = a_stream_to_export;
    m_export_thread = new QThread();

    connect(&converter, &ImageToVideoConverter::sig_finished, this, &ExportDialog::slot_finished);
    connect(&converter, &ImageToVideoConverter::sig_progress, this, &ExportDialog::slot_progress);
    connect(this, &ExportDialog::sig_dataFrameStreamToVideo, &converter, &ImageToVideoConverter::slot_dataFrameStreamToVideo);
    converter.moveToThread(m_export_thread);
    m_export_thread->start();
}

ExportDialog::~ExportDialog()
{
    m_export_thread->quit();
    m_export_thread->wait();
    delete m_export_thread;
    delete m_ui;
    delete m_video_player_widget;
}

void ExportDialog::slot_position_changed(uint64_t a_position_changed){
    if(m_ui->rbtnEnd->isChecked()){
        m_marks[1] = a_position_changed;
    }
    else{
        m_marks[0] = a_position_changed;
    }
    this->m_video_player_widget->set_marks(m_marks);
}

void ExportDialog::on_btnExportSelectedArea_clicked()
{
    if(m_marks[0] > m_marks[1]){
        QMessageBox msg;
        msg.setText("End is before start");
        msg.exec();
    }
    else{
        QString file = get_path();
        if(!file.isEmpty()){
            
            const ViewType &viewType = this->m_video_player_widget->get_view_type();
            TimeRange* range  = new TimeRange(m_marks[0], m_marks[1]);
            TimeRange timeRangeToUse = TimeRange(range->get_start_time(),std::min(range->get_start_time() + m_ns_to_go_to_the_future, range->get_end_time()));

            DataFrameStream *dataFrameStream = this->m_stream->get_image_stream(timeRangeToUse, viewType.get_enum());            
            m_progress_dialog = new QProgressDialog("Export", "Cancel", 0, 100, this);
            connect(m_progress_dialog, &QProgressDialog::canceled, this, &ExportDialog::slot_canceled_export);
            m_progress_dialog->setWindowModality(Qt::WindowModal);
            m_progress_dialog->show();
            emit sig_dataFrameStreamToVideo(dataFrameStream, file, &viewType.create_pipeline(), m_stream->get_lists_of_time_range_of_accessible_data(viewType.get_enum()), m_stream, m_ns_to_go_to_the_future, range, viewType.get_enum());          
        }  
    }
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);  
    if(settings.value(MainWindow::use_mock).toBool()){
        m_stream->update_config();
    }
}
void ExportDialog::slot_finished(){
    delete m_progress_dialog;
    QMessageBox msg;
    msg.setText("Exported");
    msg.exec();

}

QString ExportDialog::get_path(){
    QString filter = "Video files (*.mp4 *.avi *.mov);;All files (*.*)";
    QString default_filename = QDir::homePath() + "/untitled"; 
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save Video"), default_filename, filter);
    return file_name;
}

void ExportDialog::slot_progress(double prog){
    m_progress_dialog->setValue(prog * 100);
}


void ExportDialog::on_btnExportWholeVideo_clicked()
{
    QString file = get_path();
    if(!file.isEmpty()){
        
        const ViewType &viewType = this->m_video_player_widget->get_view_type();
        TimeRange timeRange = m_stream->get_time_range_of_accessible_data(viewType.get_enum());
        TimeRange timeRangeToUse = TimeRange(timeRange.get_start_time(), std::min(timeRange.get_start_time() + m_ns_to_go_to_the_future, timeRange.get_end_time()));

        DataFrameStream *dataFrameStream = this->m_stream->get_image_stream(timeRangeToUse, viewType.get_enum());            
        
        m_progress_dialog = new QProgressDialog("Export", "Cancel", 0, 100, this);
        connect(m_progress_dialog, &QProgressDialog::canceled, this, &ExportDialog::slot_canceled_export);
        m_progress_dialog->setWindowModality(Qt::WindowModal);
        m_progress_dialog->show();
        emit sig_dataFrameStreamToVideo(dataFrameStream, file, &viewType.create_pipeline(), m_stream->get_lists_of_time_range_of_accessible_data(viewType.get_enum()), m_stream, m_ns_to_go_to_the_future, &timeRange, viewType.get_enum());          
    }
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);  
    if(settings.value(MainWindow::use_mock).toBool()){
        m_stream->update_config();
    }
}

void ExportDialog::slot_canceled_export(){
    converter.request_stop();
}
