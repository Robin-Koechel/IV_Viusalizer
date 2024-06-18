#include "DbConfig.h"

DbConfig::DbConfig(uint64_t timeToLiveAnnotations, uint64_t timeToLiveVideos, QString url) :  timeToLiveAnnotations(timeToLiveAnnotations), timeToLiveVideos(timeToLiveVideos), url(url)
{

}

uint64_t DbConfig::getTimeToLiveAnnotations(){
    return timeToLiveAnnotations;
}

uint64_t DbConfig::getTimeToLiveVideos(){
    return timeToLiveVideos;
}

QString DbConfig::getUrl(){
    return url;
}
