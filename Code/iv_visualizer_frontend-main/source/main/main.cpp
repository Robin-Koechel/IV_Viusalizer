#include "MainWindow.h"
#include "../BackendConnector/IvBackendClient.h"
#include "ivbackend.grpc.pb.h"
#include <QApplication>
#include <QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Initialize and use the gRPC client
    IvBackendClient client;
    QSettings a_settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    QString m_url_grpc = a_settings.value(MainWindow::m_settings_group_grpc + MainWindow::m_settings_grpc_url).toString();
    //client.SetServerAddress(m_url_grpc.toStdString());    
    //client.GetTestDataFrame();

    return a.exec();
}