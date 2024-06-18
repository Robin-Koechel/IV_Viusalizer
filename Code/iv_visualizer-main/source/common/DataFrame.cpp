#include "DataFrame.h"

// Constructor
DataFrame::DataFrame(const QString &url, std::shared_ptr<Image> image,
                     QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest,
                     QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxen)
    : url(url), image(std::move(image)), regionsOfInterest(std::move(regionsOfInterest)),
      listOfBoundingBoxes(std::move(boundingBoxen))
{
}

DataFrame::DataFrame(const QString &url, std::shared_ptr<Image> image,
                     QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest,
                     QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxen, int fps)
    : url(url), image(std::move(image)), regionsOfInterest(std::move(regionsOfInterest)),
      listOfBoundingBoxes(std::move(boundingBoxen)), fps(fps)
{
}


DataFrame::DataFrame() : image(std::make_shared<Image>()) {}

// Return the url attribute
const QString &DataFrame::getUrl() const
{
    return url;
}

// Set the url attribute
void DataFrame::setUrl(const QString &newUrl)
{
    url = newUrl;
}

// Return the image attribute
std::shared_ptr<Image> DataFrame::getImage() const
{
    return image;
}

// Set the image attribute
void DataFrame::setImage(std::shared_ptr<Image> newImage)
{
    image = std::move(newImage);
}

// Return the set of regions of interest
const QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> &DataFrame::getRegionsOfInterest() const
{
    return regionsOfInterest;
}

// Set the set of regions of interest
void DataFrame::setRegionsOfInterest(QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> newRegionsOfInterest)
{
    regionsOfInterest = std::move(newRegionsOfInterest);
}

// Return the set of annotations
const QVector<std::shared_ptr<iv_annotation::BoundingBox>> &DataFrame::getBoundingBoxen() const
{
    return listOfBoundingBoxes;
}

// Set the set of annotations
void DataFrame::setBoundingBoxen(QVector<std::shared_ptr<iv_annotation::BoundingBox>> newListOfBoundingBoxes)
{
    listOfBoundingBoxes = std::move(newListOfBoundingBoxes);
}

int DataFrame::getFps() const
{
    return fps;
}

void DataFrame::setFps(int new_fps)
{
    fps = new_fps;
}

uint64_t DataFrame::getTimestamp() const
{
    return image->get_timestamp();
}

void DataFrame::setTimestamp(uint64_t new_timestamp)
{
    timestamp = new_timestamp;
}
