#ifndef CLUSTERCONFIGDIALOG_H
#define CLUSTERCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class ClusterConfigDialog;
}

class ClusterConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClusterConfigDialog(QVector<QString> a_currentInput, QWidget *a_parent = nullptr);
    ~ClusterConfigDialog();
    QVector<QString> get_input();

private:
    Ui::ClusterConfigDialog *m_ui;
};

#endif // CLUSTERCONFIGDIALOG_H