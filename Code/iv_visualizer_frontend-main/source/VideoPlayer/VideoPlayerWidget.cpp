#include "VideoPlayerWidget.h"
#include "qdatetime.h"
#include "TwoSliderNavigator.h"
#include "../../build/ui_videoplayerwidget.h"
#include <iostream>
#include <QSettings>
#include "../main/MainWindow.h"
#include <QSettings>
#include "qdatetime.h"
VideoPlayerWidget::VideoPlayerWidget(QWidget *a_parent) :
    QFrame(a_parent),
    m_ui(new Ui::VideoPlayerWidget)
{
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    m_ui->setupUi(this);
    m_ui->speedSelector->setCurrentIndex(3);
    this->m_use_mock = settings.value(MainWindow::use_mock).toBool();
    m_view_type_frame = new ViewTypeFrameWidget(this);
    m_navigator = new TwoSliderNavigator(0,settings.value(settings_group_name + m_settings_time_of_fine_slider_in_sec).toInt(), this);
    uint64_t ms_to_go_back = settings.value(settings_group_name + m_settings_go_back_in_ms).toInt();
    uint64_t ms_to_go_to_the_future = settings.value(settings_group_name + m_settings_go_forward_in_ms).toInt();

    m_thread_of_asking_stream = new AskingAfterStreamThread(m_current_stream, ms_to_go_to_the_future*convert_ms_to_ns,  ms_to_go_back*convert_ms_to_ns);

    connect( m_navigator, &VideoNavigator::sig_pos_changed, this, &VideoPlayerWidget::slot_pos_changed);
    m_player = new VideoPlayer(buffer_size*convert_ms_to_ns);
    connect(this, &VideoPlayerWidget::sig_ask_for_new_data_frame_stream,  m_thread_of_asking_stream, &AskingAfterStreamThread::slot_ask_for_new_data_frame_stream);
    connect(this, &VideoPlayerWidget::sig_ask_for_new_live_data_frame_stream,  m_thread_of_asking_stream, &AskingAfterStreamThread::slot_ask_for_new_live_data_frame_stream);
    connect(this, &VideoPlayerWidget::sig_new_stream,  m_thread_of_asking_stream, &AskingAfterStreamThread::slot_change_stream);

    connect(m_thread_of_asking_stream, &AskingAfterStreamThread::sig_change_data_frame_stream, this, &VideoPlayerWidget::slot_send_new_data_frame_stream);

    connect(this, &VideoPlayerWidget::sig_change_data_frame_stream,  m_player,&VideoPlayer::slot_change_data_frame_stream);
    connect(this, &VideoPlayerWidget::sig_change_speed,  m_player, &VideoPlayer::slot_speed_changed);
    connect(this, &VideoPlayerWidget::sig_play_or_pause,  m_player, &VideoPlayer::slot_play_or_pause);
    connect(this, &VideoPlayerWidget::sig_position_changed,  m_player, &VideoPlayer::slot_pos_changed);
    connect(this, &VideoPlayerWidget::sig_one_frame_backwards, m_player, &VideoPlayer::slot_one_frame_backwards);
    connect(this, &VideoPlayerWidget::sig_one_frame_forwards, m_player, &VideoPlayer::slot_one_frame_forwards);

    connect( m_player, &VideoPlayer::sig_ask_for_new_data_frame_stream, this, &VideoPlayerWidget::slot_ask_for_new_data_frame_stream);
    connect( m_player, &VideoPlayer::sig_pos_changed, this, &VideoPlayerWidget::slot_change_current_position);
    connect( m_view_type_frame, &ViewTypeFrameWidget::sig_view_type_changed, this, &VideoPlayerWidget::slot_view_type_changed);
    connect( m_player, &VideoPlayer::sig_show_data_frame, this, &VideoPlayerWidget::slot_show_data_frame);
    m_ui->camFrameVLayout->addWidget(m_navigator);
    m_ui->viewTypeSelectorLayout->addWidget(m_view_type_frame);
    m_is_live = false;
    m_thread_of_video_player = new QThread();
    m_player->moveToThread(m_thread_of_video_player);
    m_thread_of_video_player->start();
    m_thread_of_asking_stream->moveToThread(m_thread_of_asking_stream);
    m_thread_of_asking_stream->start();

    m_ui->graphicsView->setScene(&m_pipe_data.get_scene());
    m_blocking_pos_changed = false;
    m_ui->btnPlay->setText(m_play);
    m_live_changed = false;
    m_current_version = 0;
}

VideoPlayerWidget::~VideoPlayerWidget()
{
    delete m_ui;
    delete m_navigator;
    delete m_player;
    delete m_view_type_frame;
    m_thread_of_video_player->quit();
    m_thread_of_video_player->wait();
    delete m_thread_of_video_player;
    m_thread_of_asking_stream->quit();
    m_thread_of_asking_stream->wait();
    delete m_thread_of_asking_stream;
  
}
void VideoPlayerWidget::slot_view_type_changed(const ViewType& a_type){
    m_pipe_data.clear_scene();
    m_pipe_object = &a_type.create_pipeline();

}
void VideoPlayerWidget::slot_change_current_position(uint64_t a_new_position){
    if(!m_blocking_pos_changed && !m_is_live){
        m_navigator->change_current_position(a_new_position / convert_sec_to_ns - m_current_time_range_of_accessible_data.get_start_time() / convert_sec_to_ns);
        emit sig_navigator_moved(a_new_position);
    }
}

void VideoPlayerWidget::on_btnPlay_clicked()
{
    if( m_ui->btnPlay->text() == m_play){
        m_ui->btnPlay->setText(m_stop);
        emit sig_play_or_pause(false);

    }
    else{
        m_ui->btnPlay->setText(m_play);
        emit sig_play_or_pause(true);

    }
}

void VideoPlayerWidget::on_speedSelector_currentTextChanged(const QString &a_arg1)
{
    emit sig_change_speed(a_arg1.toDouble());
    slot_ask_for_new_data_frame_stream();   
}

void VideoPlayerWidget::slot_show_data_frame(const DataFrame* a_data_frame_to_show, bool a_blocking_pos, int a_version){
    QTime time = QTime::currentTime();
    if(m_current_version <= a_version){
        if(a_blocking_pos && m_blocking_pos_changed){
            m_blocking_pos_changed = false;
        }       
        m_pipe_data.clear_regions_of_interest();
        m_pipe_data.set_data_frame(*a_data_frame_to_show);
        m_pipe_object->run(m_pipe_data);
        m_pipe_data.hide_rects();
    }
    int elapsedTime = time.msecsTo(QTime::currentTime());
    int a= 0;
}


void VideoPlayerWidget::change_stream(Stream* a_stream){

    emit sig_new_stream(a_stream);
    m_current_stream = a_stream;
    m_pipe_object = &m_view_type_frame->get_selected_view_type().create_pipeline();
    slot_ask_for_new_data_frame_stream();
}

const ViewType& VideoPlayerWidget::get_view_type(){
    return this->m_view_type_frame->get_selected_view_type();
}


void VideoPlayerWidget::on_checkBoxLive_stateChanged(int a_arg1)
{
    m_live_changed = true;
    if (a_arg1 == Qt::Unchecked) {
        m_navigator->setVisible(true);
        m_ui->btnPlay->setVisible(true);
        m_ui->speedSelector->setVisible(true);
        if( m_ui->btnPlay->text() == m_play){
            emit sig_play_or_pause(true);
        }
        else{
            emit sig_play_or_pause(false);
        }
        emit sig_change_speed(m_ui->speedSelector->currentText().toDouble());

    } else if (a_arg1 == Qt::Checked) {
        m_navigator->setVisible(false);
        m_ui->btnPlay->setVisible(false);
        m_ui->speedSelector->setVisible(false);
        emit sig_play_or_pause(true);
        emit sig_change_speed(1);
    }
    slot_ask_for_new_data_frame_stream();
}

void VideoPlayerWidget::slot_pos_changed(uint64_t a_new_position){
    m_blocking_pos_changed = true;
    emit sig_position_changed(a_new_position *convert_sec_to_ns + m_current_time_range_of_accessible_data.get_start_time());
    emit sig_navigator_moved(a_new_position *convert_sec_to_ns + m_current_time_range_of_accessible_data.get_start_time());
}

void VideoPlayerWidget::slot_ask_for_new_data_frame_stream(){
    m_current_time_range_of_accessible_data = m_current_stream->get_time_range_of_accessible_data(m_view_type_frame->get_selected_view_type().get_enum());
    m_navigator->set_time_to_navigate(m_current_time_range_of_accessible_data.get_end_time() / convert_sec_to_ns - m_current_time_range_of_accessible_data.get_start_time() / convert_sec_to_ns);
    uint64_t position_of_nav = m_navigator->get_current_position();

    if((!m_is_live && !m_live_changed) || (m_is_live && m_live_changed)){
        emit sig_ask_for_new_data_frame_stream(position_of_nav * convert_sec_to_ns, &this->m_view_type_frame->get_selected_view_type());
    }
    else{
        emit sig_ask_for_new_live_data_frame_stream(position_of_nav * convert_sec_to_ns, &m_view_type_frame->get_selected_view_type());
    }

}

void VideoPlayerWidget::slot_send_new_data_frame_stream(DataFrameStream* a_new_stream){
    uint64_t position_of_nav = m_navigator->get_current_position();
    if((m_is_live && !m_live_changed) || (!m_is_live && m_live_changed)){
        emit sig_change_data_frame_stream(a_new_stream, a_new_stream->get_time_of_stream()->get_start_time());
    }
    else{
        emit sig_change_data_frame_stream(a_new_stream, m_current_time_range_of_accessible_data.get_start_time() + position_of_nav * convert_sec_to_ns);
    }
    m_pipe_data.clear_scene_without_image();
    m_current_version++;
    if(m_use_mock){
        m_current_stream->update_config();
    }
    if(m_live_changed){
        m_is_live = !m_is_live;
        m_live_changed = false;
    }
}
Stream* VideoPlayerWidget::get_stream(){
    return m_current_stream;
}


void VideoPlayerWidget::keyPressEvent(QKeyEvent *a_event) {
    if (a_event->key() == Qt::Key_Period) {
        emit sig_one_frame_forwards();
    } else if (a_event->key() == Qt::Key_Comma) {
        emit sig_one_frame_backwards();
    } else {
        QWidget::keyPressEvent(a_event);
    }
}

void VideoPlayerWidget::stop_stream(){
    if( m_ui->btnPlay->text() == m_play){
        emit sig_play_or_pause(false);
        m_ui->btnPlay->setText(m_stop);
    }
    m_pipe_data.clear_scene();
}

void VideoPlayerWidget::disablelive(){
    m_ui->checkBoxLive->setVisible(false);
    m_is_live = false;
}

void VideoPlayerWidget::set_marks(QList<uint64_t> &a_positions){
    QList<uint64_t> marks; // Deklariert eine QList namens marks
    for(int i = 0; i < a_positions.size(); i++){    
        if(a_positions[i] == 0){
            marks.append(0);
        }         
        else{
            marks.append((a_positions[i] - m_current_time_range_of_accessible_data.get_start_time())/ convert_sec_to_ns );
        }  

    }

    m_navigator->set_marks(marks);

}

