#include "MainWindow.h"
#include "../../build/ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include "../ExportSettings/JsonSettingsSerializer.h"
#include "../Log/LogFrame.h"
#include <QSettings>
#include "../GraphicalUserInterface/ClusterTreeWidget.h"
#include <QFileDialog>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>
#include <QMap>

QString MainWindow::m_config_file_path = "../source/main/IvVisualizerConf.conf";
QString MainWindow::m_settings_group_grpc = "Grpc/";
QString MainWindow::m_settings_grpc_url = "url";
QString MainWindow::use_mock = "grpc_mock/use_mock";
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    is_in_constructor = true;
    ui->setupUi(this);
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    this->m_max_number_cam_frames = settings.value("gui/max_cam_frames").toInt();
    this->m_max_number_streams = settings.value("gui/max_streams").toInt();
    this->m_max_number_cluster_frames = settings.value("gui/max_cluster_frames").toInt();
    this->m_max_number_log_frames = settings.value("gui/max_log_frames").toInt();
    this->m_use_mock = settings.value(use_mock).toBool();
    m_streams = new QVector<Stream*>();
    m_last_selected_cam_frame = nullptr;
    setDockOptions(DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks);

    this->m_serializer = new JsonSettingsSerializer();
    GRPCReader reader(settings.value(m_settings_group_grpc + m_settings_grpc_url).toString());
    QVector<QString>* available_urls = reader.read_available_urls();
    for(QString url : *available_urls){
        m_streams->append(new Stream(url, settings.value(m_settings_group_grpc + m_settings_grpc_url).toString()));
        m_streams->at(m_streams->size() - 1)->set_name(url);
    }
    load_cams();
    load_cluster();
    load_logs();
    restoreGeometry(settings.value(m_geometry).toByteArray());
    restoreState(settings.value(m_windowstate).toByteArray());

    
    is_in_constructor = false;
}

void MainWindow::add_cam(bool is_in_constructor){
    if(get_number_active_cams(is_in_constructor) < m_max_number_cam_frames){
        CamFrame* cam_frame = new CamFrame();
        cam_frame->setVisible(true);
        QDockWidget* dock_widget = new QDockWidget(m_name_of_cam + QString::number(m_dock_to_cam_map.size() ), this);
        dock_widget->setContextMenuPolicy(Qt::PreventContextMenu);
        dock_widget->setObjectName(m_name_of_cam + QString::number(m_dock_to_cam_map.size() - 1));
        dock_widget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dock_widget->setWidget(cam_frame);
        this->addDockWidget(Qt::LeftDockWidgetArea, dock_widget);
        m_dock_to_cam_map.insert(dock_widget, cam_frame);
        connect(cam_frame, &CamFrame::sig_cam_selected, this, &MainWindow::slot_camframe_selected);
    } else{
        QMessageBox msg;
        msg.setText("max number of cam frames reached");
        msg.exec();
    }
}

int MainWindow::get_number_active_cams(bool is_in_constructor){
    int counter = 0;
    for (auto it = m_dock_to_cam_map.begin(); it != m_dock_to_cam_map.end(); ++it) {
        if(!it.value()->isVisible() && !is_in_constructor){
            delete it.value();
            m_dock_to_cam_map.erase(it);
        }else{
            counter++;
        }

    }
    return counter;    
}

int MainWindow::get_number_active_clusters(bool is_in_constructor){
    int counter = 0;
    for (auto it = m_dock_to_cluster_map.begin(); it != m_dock_to_cluster_map.end(); ++it) {
        if(!it.value()->isVisible() && !is_in_constructor){
            delete it.value();
            m_dock_to_cluster_map.erase(it);
        }else{
            counter++;
        }

    }
    return counter;    
}

int MainWindow::get_number_active_logs(bool is_in_constructor){
    int counter = 0;
    for (auto it = m_dock_to_log_map.begin(); it != m_dock_to_log_map.end(); ++it) {
        if(!it.value()->isVisible() && !is_in_constructor){
            delete it.value();
            m_dock_to_log_map.erase(it);
        }else{
            counter++;
        }

    }
    return counter;    
}

void MainWindow::load_cams(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_cam);
    int dock_widget_count = settings.value(m_name_of_number_of_cams, 0).toInt();
    for (int i = 0; i < dock_widget_count; ++i) {
        add_cam(true);
    }
    settings.endGroup();
}

void MainWindow::load_logs(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_log);
    int dock_widget_count = settings.value(m_name_of_number_of_logs, 0).toInt();
    for (int i = 0; i < dock_widget_count; ++i) {
        on_actionshow_logs_triggered(true); // almost like add_log 
    }
    settings.endGroup();
}

QString MainWindow::get_export_stream_settings_file_location(){
    QString filter = "Setting files (*.json)";
    QString default_filename = QDir::homePath() + "/strem_settings.json"; 
    QString file_name = QFileDialog::getSaveFileName(this, tr("Select file to export settings"), default_filename, filter);
    return file_name;

}

QString MainWindow::get_import_stream_settings_file_location(){
    QString filter = "Setting files (*.json)";
    QString file_name = QFileDialog::getOpenFileName(this,  QObject::tr("Select File"), "", filter);
    return file_name;

}


void MainWindow::add_cluster(bool is_in_constructor){
    if(get_number_active_clusters(is_in_constructor) < this->m_max_number_cluster_frames){
        ClusterTreeWidget* cluster_widget = new ClusterTreeWidget(m_streams, m_use_mock);
        QDockWidget* dockWidget = new QDockWidget(m_name_of_cluster + QString::number(m_dock_to_cluster_map.size()), this);
        dockWidget->setContextMenuPolicy(Qt::PreventContextMenu);
        dockWidget->setObjectName(m_name_of_cluster + QString::number(m_dock_to_cluster_map.size() - 1));
        dockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidget->setWidget(cluster_widget);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
        m_dock_to_cluster_map.insert(dockWidget, cluster_widget);
        connect(cluster_widget, &ClusterTreeWidget::sig_change_stream, this, &MainWindow::slot_show_stream);
        connect(cluster_widget, &ClusterTreeWidget::sig_delete_stream, this, &MainWindow::slot_delete_stream);
    }else{
        QMessageBox msg;
        msg.setText("max number of cam cluster tree frames reached");
        msg.exec();
    }
}



void MainWindow::load_cluster(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_cluster);
    int dock_widget_count = settings.value(m_name_of_number_of_cluster, 0).toInt();
    for (int i = 0; i < dock_widget_count; ++i) {
        add_cluster(true);
    }
    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    save_gui();

    QMainWindow::closeEvent(event);
}

void MainWindow::save_cams(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_cam);
    int number_of_visible_cams = 0;
    QMap<QDockWidget*, CamFrame*>::iterator it;
    for (it = m_dock_to_cam_map.begin(); it != m_dock_to_cam_map.end(); ++it) {
        if (it.value()->isVisible()) {
            number_of_visible_cams++;
        }
    }
    settings.setValue(m_name_of_number_of_cams, number_of_visible_cams);
    settings.endGroup();
}

void MainWindow::save_logs(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_log);
    int number_of_visible_logs = 0;
    QMap<QDockWidget*, LogFrame*>::iterator it;
    for (it = m_dock_to_log_map.begin(); it != m_dock_to_log_map.end(); ++it) {
        if (it.value()->isVisible()) {
            number_of_visible_logs++;
        }
    }
    settings.setValue(m_name_of_number_of_logs, number_of_visible_logs);
    settings.endGroup();
}

void MainWindow::save_cluster(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    settings.beginGroup(m_name_of_group_cluster);
    int number_of_visible_cluster = 0;
    QMap<QDockWidget*, ClusterTreeWidget*>::iterator it;
    for (it = m_dock_to_cluster_map.begin(); it != m_dock_to_cluster_map.end(); ++it) {
        if (it.value()->isVisible()) {
            number_of_visible_cluster++;
        }
    }
    settings.setValue(m_name_of_number_of_cluster, number_of_visible_cluster);
    settings.endGroup();
    
}

void MainWindow::save_gui(){
    QSettings settings(m_config_file_path, QSettings::IniFormat);
    save_cams();
    save_logs();
    save_cluster();
    settings.setValue(m_geometry, saveGeometry());
    settings.setValue(m_windowstate, saveState());
}

MainWindow::~MainWindow()
{
    delete ui;
    QMap<QDockWidget*, ClusterTreeWidget*>::iterator it;
    for (it = m_dock_to_cluster_map.begin(); it != m_dock_to_cluster_map.end(); ++it) {
        delete it.value();  
    }
    m_dock_to_cluster_map.clear();
    QMap<QDockWidget*, CamFrame*>::iterator it_cam;
    for (it_cam = m_dock_to_cam_map.begin(); it_cam != m_dock_to_cam_map.end(); ++it_cam) {
        delete it_cam.value();
    }
    m_dock_to_cam_map.clear();
    for(Stream* testStream:*this->m_streams){
        delete testStream;
    }
    for(int i = 0; i < m_dock_cam_frame_widgets.size(); i++){
        delete this->m_dock_cam_frame_widgets.at(i);
    }
    delete this->m_serializer;

    for (auto it = m_dock_to_log_map.begin(); it != m_dock_to_log_map.end(); ++it) {
        delete it.value();
    }

    m_dock_to_log_map.clear();
}

void MainWindow::on_actionexport_settings_triggered() {
    QString a_file_name = get_export_stream_settings_file_location();
    if(!a_file_name.isEmpty()){
        this->m_serializer->export_streams(*this->m_streams, a_file_name);
    }
}

void MainWindow::on_actionimport_settings_triggered() {
    QString a_file_name = get_import_stream_settings_file_location();
    if(!a_file_name.isEmpty()){
        QVector<Stream*> streams = this->m_serializer->import_streams(a_file_name, m_use_mock);
        if(streams.size() <= m_max_number_streams){
            find_streams_to_import(streams);
            QMap<QDockWidget*, ClusterTreeWidget*>::iterator it;
            for (it = m_dock_to_cluster_map.begin(); it != m_dock_to_cluster_map.end(); ++it) {
                it.value()->update_streams();  
            }
        }else{
            QMessageBox msgBox;
            msgBox.setText("invalid file - contains to many streams");
            msgBox.exec();
        }

    }
}

void MainWindow::find_streams_to_import(QVector<Stream*> streams_to_add){
    for(Stream* stream_to_add : streams_to_add){
        bool found = false;
        for(int i = 0; i < m_streams->size(); i++){
            if(m_streams->at(i)->get_url() == stream_to_add->get_url()){
                delete m_streams->at(i);
                (*m_streams)[i] = stream_to_add;
                found = true;
            }
        }
        if(!found){
            m_streams->append(stream_to_add);
        }
        stream_to_add->update_config();

    }
}

void MainWindow::on_actionshow_logs_triggered(bool a_in_constructor=false) {
    if(get_number_active_logs(a_in_constructor)<this->m_max_number_log_frames){
        LogFrame* log_frame = new LogFrame();
        QDockWidget* dock_widget = new QDockWidget(m_name_of_log + QString::number(m_dock_to_log_map.size() - 1), this);
        dock_widget->setContextMenuPolicy(Qt::PreventContextMenu);
        dock_widget->setAllowedAreas(Qt::AllDockWidgetAreas);
        dock_widget->setWidget(log_frame);
        dock_widget->setObjectName(m_name_of_log + QString::number(m_dock_to_log_map.size() - 1));
        dock_widget->setVisible(true);
        this->addDockWidget(Qt::LeftDockWidgetArea, dock_widget);
        m_dock_to_log_map.insert(dock_widget, log_frame);
    }else{
        QMessageBox msg;
        msg.setText("max number of log frames reached.");
        msg.exec();
    }
}

void MainWindow::on_actionopen_camera_triggered() {
    if(get_number_active_cams(false) < m_max_number_cam_frames){
        add_cam(false);
    }else{
        QMessageBox msgBox;
        msgBox.setText("Maximum number of camera frames reached. No more streams can be added");
        msgBox.exec();
    }
}

void MainWindow::on_actionopen_cluster_triggered() {
    add_cluster(false);
}


void MainWindow::slot_camframe_selected(CamFrame* a_selected_cam_frame){
    if(m_last_selected_cam_frame != nullptr){
        m_last_selected_cam_frame->setGraphicsEffect(nullptr);
    }
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(10);
    effect->setColor(Qt::black);
    effect->setOffset(0);
    a_selected_cam_frame->setGraphicsEffect(effect);
    m_last_selected_cam_frame = a_selected_cam_frame;

}


void MainWindow::slot_delete_stream(Stream* a_stream_to_delete){ 
    QMap<QDockWidget*, CamFrame*>::iterator it;
    for (it = m_dock_to_cam_map.begin(); it != m_dock_to_cam_map.end(); ++it) {
        if(it.value()->is_showing_stream(a_stream_to_delete)){
            it.value()->stop_stream();
        }
    }
    int index = m_streams->indexOf(a_stream_to_delete); 
    if (index != -1) { 
        delete (*m_streams)[index];
        m_streams->remove(index);
    }
}

void MainWindow::slot_show_stream(Stream* a_stream_to_show){
    foreach (QDockWidget* dock_widget, m_dock_to_cam_map.keys()) {
        if (m_dock_to_cam_map[dock_widget] == m_last_selected_cam_frame) {
            dock_widget->setWindowTitle(a_stream_to_show->get_name());
        }
    }
    if(m_last_selected_cam_frame != nullptr){
        m_last_selected_cam_frame->change_stream(a_stream_to_show);
    }

}
