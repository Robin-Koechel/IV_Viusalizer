
#ifndef CLUSTERTREEWIDGET_H
#define CLUSTERTREEWIDGET_H

#include "qtreewidget.h"
#include "../BackendConnector/Stream.h"
class ClusterTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ClusterTreeWidget(QVector<Stream*>* a_current_streams, bool a_create_mock_streams = false);
    void contextMenuEvent(QContextMenuEvent *a_event)override;
    void update_streams();

private:
    Stream* find_stream(QString a_name);
    bool name_is_valid(QString a_name);
    bool url_is_valid(QString a_url);
    void add_cam(QVector<QString> a_input);
    void add_mock_cam(QVector<QString> a_input);
signals:
    void sig_change_stream(Stream* a_new_stream);
    void sig_delete_stream(Stream* a_stream_to_delete);
private slots:
    void slots_rename_stream();
    void slots_delete_stream();
    void slot_add_cam();
    void slot_on_item_clicked(QTreeWidgetItem *a_item, int a_column);


private:

    QVector<Stream*>* m_all_streams;
    QTreeWidgetItem *m_current_selected_item;
    QString m_url_grpc;
    bool m_create_mock_streams;
    uint64_t m_number_of_frames = 18000ULL * 1000000000ULL;
};

#endif // CLUSTERTREEWIDGET_H