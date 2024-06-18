#ifndef GRPC_WRITER_H
#define GRPC_WRITER_H

#include "Writer.h"
#include "IvBackendClient.h"
/**
 * grpc_writer - Class for writing data using gRPC protocol.
 */
class GRPCWriter : public Writer {
public:
    /**
     * Constructor for GRPCWriter.
     *
     * @param a_url_grpc The URL for gRPC communication.
     */
    GRPCWriter(QString a_url_grpc);
    
    /**
     * write_config - Writes configuration data.
     *
     * @param a_url_stream The URL of the stream.
     * @param a_config_to_write The configuration to write.
     */
    void write_config(const QString a_url_stream, Config& a_config_to_write);
    
    /**
     * add_cam - Adds a camera.
     *
     * @param a_url_stream The URL of the stream.
     * @return true if connection was established.
     */
    bool add_cam(const QString a_url_stream);
    
    /**
     * @brief stops the stream
     * @param url_stream the url to stop
    */
    void stop_cam(const QString url_stream) override;

private:
    IvBackendClient* m_backend; ///<source to aks
    const int m_wait_time = 3000; ///<says how long the waoting time for the processing tools is while connecting

};

#endif // GRPC_WRITER_H
