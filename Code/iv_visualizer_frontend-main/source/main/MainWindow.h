#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qboxlayout.h"
#include <QMainWindow>
#include "../GraphicalUserInterface/CamFrame.h"
#include <QVector>
#include "../ExportSettings/JsonSettingsSerializer.h"
#include "../Log/LogFrame.h"
#include <QDockWidget>
#include "../GraphicalUserInterface/ClusterTreeWidget.h"
#include "../../build/build/ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void save_cams();
    void save_cluster();
    void load_cams();
    void load_cluster();
    void add_cam(bool is_in_constructor);
    void add_cluster(bool is_in_constructor);
    void save_gui();
    QString get_export_stream_settings_file_location();
    QString get_import_stream_settings_file_location();
    void find_streams_to_import(QVector<Stream*> streams_to_add);
    int get_number_active_cams(bool is_in_constructor);
    int get_number_active_logs(bool is_in_constructor);
    int get_number_active_clusters(bool is_in_constructor);
    void load_logs();
    void save_logs();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void slot_show_stream(Stream* a_stream_to_show);
    void slot_camframe_selected(CamFrame* a_selected_cam_frame);
    void slot_delete_stream(Stream* a_stream_to_delete);

private slots:
    void on_actionexport_settings_triggered();
    void on_actionimport_settings_triggered();
    void on_actionshow_logs_triggered(bool a_in_constructor);
    void on_actionopen_camera_triggered();
    void on_actionopen_cluster_triggered();

public:
    static QString m_config_file_path;
    static QString m_settings_group_grpc;
    static QString m_settings_grpc_url;
    static QString use_mock;

private:
    const QString m_name_of_number_of_cams = "CamWidgetCount";   
    const QString m_name_of_group_cam = "CamWidget";
    const QString m_name_of_cam = "Cam";
    const QString m_name_of_number_of_cluster = "ClusterWidgetCount";    
    const QString m_name_of_group_cluster = "ClusterWidget";    
    const QString m_name_of_cluster = "Cluster";
    const QString m_name_of_number_of_logs = "LogWidgetCount";  
    const QString m_name_of_group_log = "LogWidget";
    const QString m_name_of_log = "Log";
    const QString m_geometry = "Geometry";
    const QString m_windowstate = "WindowState";
    

    Ui::MainWindow *ui;
    QMap<QDockWidget*, CamFrame*> m_dock_to_cam_map;
    QMap<QDockWidget*, ClusterTreeWidget*> m_dock_to_cluster_map;
    CamFrame* m_last_selected_cam_frame = nullptr;

    int m_max_number_streams;
    int m_max_number_cam_frames;
    int m_max_number_log_frames;
    int m_max_number_cluster_frames;
    bool is_in_constructor;
    QVector<QDockWidget*> m_dock_cam_frame_widgets;
    QVector<VideoPlayerWidget*> m_video_player_widgets;
    QVector<Stream*>* m_streams;

    bool m_use_mock = true; // change if you want to use real grpc connection

    // import and export
    JsonSettingsSerializer* m_serializer;

    // Log Frame
    QMap<QDockWidget*, LogFrame*> m_dock_to_log_map;
};
#endif // MAINWINDOW_H
