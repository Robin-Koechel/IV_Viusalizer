#ifndef DBCONFIG_H
#define DBCONFIG_H
#include <cstdint>
#include <QString>

class DbConfig
{
public:
    DbConfig(uint64_t timeToLiveAnnotations, uint64_t timeToLiveVideos, QString url);
    uint64_t getTimeToLiveAnnotations();
    uint64_t getTimeToLiveVideos();
    QString getUrl();
private:
    uint64_t timeToLiveAnnotations;
    uint64_t timeToLiveVideos;
    QString url;
};

#endif // DBCONFIG_H
