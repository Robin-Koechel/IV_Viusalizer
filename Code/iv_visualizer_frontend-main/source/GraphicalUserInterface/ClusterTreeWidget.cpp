#include "ClusterTreeWidget.h"
#include "ClusterConfigDialog.h"
#include "qevent.h"
#include "qmenu.h"
#include <QMessageBox>
#include "../BackendStub/MockReader.h"
#include <QHeaderView>
#include "../main/MainWindow.h"
#include <QSettings>

ClusterTreeWidget::ClusterTreeWidget(QVector<Stream *>* a_current_streams,bool a_create_mock_streams) : m_create_mock_streams(a_create_mock_streams){
    m_all_streams = a_current_streams;
    QHeaderView* header = this->header();
    header->hide();    
    update_streams();
    QSettings a_settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    m_url_grpc = a_settings.value(MainWindow::m_settings_group_grpc + MainWindow::m_settings_grpc_url).toString();
}

void ClusterTreeWidget::update_streams(){
    this->clear();
    QTreeWidgetItem *a_parent_item = new QTreeWidgetItem(this, QStringList("Cams"));
    for (int i = 0; i < m_all_streams->size(); ++i) {
            new QTreeWidgetItem(a_parent_item, QStringList(m_all_streams->at(i)->get_name()));
    }
    connect(this, &QTreeWidget::itemClicked, this, &ClusterTreeWidget::slot_on_item_clicked);
}

void ClusterTreeWidget::contextMenuEvent(QContextMenuEvent *event) {
    QTreeWidgetItem* item_at_pos = itemAt(event->pos());
    if (!item_at_pos) {
        return;
    }

    m_current_selected_item = item_at_pos;

    QMenu menu(this);
    if (m_current_selected_item && m_current_selected_item->parent() != nullptr) {
        QAction *rename_action = menu.addAction("Rename");
        QAction *delete_action = menu.addAction("Delete");

        disconnect(rename_action, &QAction::triggered, this, &ClusterTreeWidget::slots_rename_stream);
        disconnect(delete_action, &QAction::triggered, this, &ClusterTreeWidget::slots_delete_stream);

        connect(rename_action, &QAction::triggered, this, &ClusterTreeWidget::slots_rename_stream);
        connect(delete_action, &QAction::triggered, this, &ClusterTreeWidget::slots_delete_stream);
    } else {
        QAction *add_cam_action = menu.addAction("Add cam");

        disconnect(add_cam_action, &QAction::triggered, this, &ClusterTreeWidget::slot_add_cam); 
        connect(add_cam_action, &QAction::triggered, this, &ClusterTreeWidget::slot_add_cam);
    }

    menu.exec(event->globalPos());
}

void ClusterTreeWidget::slot_add_cam(){
    if(!m_create_mock_streams){
        QVector<QString> current_input;
        ClusterConfigDialog dialog(current_input);
        if (dialog.exec() == QDialog::Accepted) {
            QVector<QString> user_input = dialog.get_input();
            add_cam(user_input);
        }
    }
    else{
        QVector<QString> current_input;
        ClusterConfigDialog dialog(current_input);
        if (dialog.exec() == QDialog::Accepted) {
            QVector<QString> user_input = dialog.get_input();
            add_mock_cam(user_input);
        }
    }
    
}
void ClusterTreeWidget::add_mock_cam(QVector<QString> a_input){
    if(name_is_valid(a_input.at(1))){
        if(url_is_valid(a_input.at(0))){
            
            TimeRange* time = new TimeRange(0,m_number_of_frames);
            MockReader* mockreader = new MockReader("test", time);
            MockWriter* mockwriter = new MockWriter("test", time, mockreader);
            Stream* stream_to_add = new Stream(mockreader, mockwriter);
            stream_to_add->set_name(a_input.at(1));
            stream_to_add->get_config()->get_config_gstreamer().set_stream_url(a_input.at(0));
            m_all_streams->append(stream_to_add);
            new QTreeWidgetItem(m_current_selected_item, QStringList(a_input.at(1)));
        }
        else{
            QMessageBox::critical(nullptr, "Error", "Url: " +  a_input.at(0) + " is alredy used");
        }
    }
    else{
        QMessageBox::critical(nullptr, "Error", "Name: " +  a_input.at(1) + " is alredy used");
    }
}

void ClusterTreeWidget::add_cam(QVector<QString> a_user_input){
    if(name_is_valid(a_user_input.at(1))){
        if(url_is_valid(a_user_input.at(0))){
            Stream* stream_to_add = new Stream(a_user_input.at(0), this->m_url_grpc);
            if(!stream_to_add->is_connected()){
                delete stream_to_add;
                QMessageBox::critical(nullptr, "Error", "Could not establish a connection");
                return;
            }
            stream_to_add->set_name(a_user_input.at(1));
            m_all_streams->append(stream_to_add);
            new QTreeWidgetItem(m_current_selected_item, QStringList(a_user_input.at(1)));
        }
        else{
            QMessageBox::critical(nullptr, "Error", "Url: " +  a_user_input.at(0) + " is alredy used");
        }
    }
    else{
        QMessageBox::critical(nullptr, "Error", "Name: " +  a_user_input.at(1) + " is alredy used");
    }
}

void ClusterTreeWidget::slot_on_item_clicked(QTreeWidgetItem *a_item, int a_column) {
    if (a_item && a_item->parent() != nullptr) {
        Stream *clicked_stream = find_stream(a_item->text(0));
        if(clicked_stream) {
            emit sig_change_stream(clicked_stream);
        }
    }

}

void ClusterTreeWidget::slots_rename_stream(){
    QVector<QString> current_input;
    Stream* selected_stream = find_stream(this->m_current_selected_item->text(0));
    current_input.append(selected_stream->get_url());
    current_input.append(selected_stream->get_name());

    ClusterConfigDialog dialog(current_input);
    if (dialog.exec() == QDialog::Accepted) {
        QVector<QString> user_input = dialog.get_input();
        if(name_is_valid(user_input.at(1))){
            selected_stream->set_name(user_input.at(1));
            m_current_selected_item->setText(0, user_input.at(1));

        }
        else{
            QMessageBox::critical(nullptr, "Error", "Name: " +  user_input.at(1) + " is alredy used");
        }
    }

}
bool ClusterTreeWidget::url_is_valid(QString a_url){
    foreach(Stream* stream, *m_all_streams){
        if(stream->get_url() == a_url){
            return false;
        }
    }
    return true;
}

bool ClusterTreeWidget::name_is_valid(QString a_name){
    foreach(Stream* stream, *m_all_streams){
        if(stream->get_name() == a_name){
            return false;
        }
    }
    return true;
}
void ClusterTreeWidget::slots_delete_stream(){
    find_stream(m_current_selected_item->text(0))->stop_stream();
    sig_delete_stream(find_stream(m_current_selected_item->text(0)));
    delete m_current_selected_item;
}
Stream* ClusterTreeWidget::find_stream(QString a_name){
    for(int i = 0; i < m_all_streams->size(); i++){
        if(a_name == m_all_streams->at(i)->get_name()){
            return m_all_streams->at(i);
        }
    }
    return nullptr;
}
