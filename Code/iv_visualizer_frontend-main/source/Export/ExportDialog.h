#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "../VideoPlayer/VideoPlayerWidget.h"
#include <QDialog>
#include <QThread>
#include <QProgressDialog>
#include "ImageToVideoConverter.h"
namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(Stream* a_stream_to_export, QWidget *a_parent = nullptr);
    QString get_path();
    ~ExportDialog();
signals:
    void sig_dataFrameStreamToVideo(DataFrameStream *dataFrameStreamFirst, QString outputFilename, PipelineObject *pipelineoObject, QVector<TimeRange*> ranges, Stream* currentStream, uint64_t askingTimeInNS, TimeRange* rangeToExport, ViewTypeEnum typeToExport);

    void sig_cancle_export();
public slots:
    void slot_position_changed(uint64_t positionChanged);
    void slot_finished();
    void slot_progress(double prog);
    void slot_canceled_export();
private slots:
    void on_btnExportSelectedArea_clicked();
    
    void on_btnExportWholeVideo_clicked();


private:
    const QString m_settings_go_forward_in_ms = "AskingIntoTheFutureInMs"; 
    const QString settings_group_name = "VideoPlayerWidget/"; ///<Groupname of settings
    const int convert_ms_to_ns = 1000000; ///<factor to convert sec to ms
    const QString m_video_player_stylesheet = "border: 1px solid black";
    Ui::ExportDialog *m_ui;
    VideoPlayerWidget* m_video_player_widget;
    QList<uint64_t> m_marks;
    Stream* m_stream;
    QThread* m_export_thread;
    QProgressDialog *m_progress_dialog;
    ImageToVideoConverter converter;

    uint64_t m_ns_to_go_to_the_future;



};

#endif // EXPORTDIALOG_H
