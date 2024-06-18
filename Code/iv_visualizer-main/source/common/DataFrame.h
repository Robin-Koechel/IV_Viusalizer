#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QString>
#include "Image.h"
#include <QVector>
#include <memory>
#include <RegionOfInterest.h>
#include <BoundingBox.h>

class DataFrame
{
private:
    // Data members
    QString url;
    std::shared_ptr<Image> image;
    QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest;
    QVector<std::shared_ptr<iv_annotation::BoundingBox>> listOfBoundingBoxes;
    int fps;
    uint64_t timestamp;

public:
    // Constructors
    DataFrame();
    DataFrame(const QString &url, std::shared_ptr<Image> image,
              QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest,
              QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxen);
    DataFrame(const QString &url, std::shared_ptr<Image> image,
              QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest,
              QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxen,
              int fps);

    // Accessor methods for data members
    const QString &getUrl() const;
    void setUrl(const QString &newUrl);

    std::shared_ptr<Image> getImage() const;
    void setImage(std::shared_ptr<Image> newImage);

    const QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> &getRegionsOfInterest() const;
    void setRegionsOfInterest(QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> newRegionsOfInterest);

    const QVector<std::shared_ptr<iv_annotation::BoundingBox>> &getBoundingBoxen() const;
    void setBoundingBoxen(QVector<std::shared_ptr<iv_annotation::BoundingBox>> newListOfBoundingBoxes);

    int getFps() const;
    void setFps(int new_fps);

    uint64_t getTimestamp() const;
    void setTimestamp(uint64_t new_timestamp);
};

#endif // DATAFRAME_H
