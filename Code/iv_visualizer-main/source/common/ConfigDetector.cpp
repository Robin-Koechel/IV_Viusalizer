#include "ConfigDetector.h"

ConfigDetector::ConfigDetector(InferenceType type, int device, float nms, float confidence, const std::vector<iv_annotation::RegionOfInterest>& list)
        : inference_type(type), cuda_device(device), nms_threshold(nms),
          confidence_threshold(confidence), rois(list) {}

// Getter and setter for inference_type
InferenceType ConfigDetector::getInferenceType() const {
    return inference_type;
}

void ConfigDetector::setInferenceType(InferenceType value) {
    inference_type = value;
}

// Getter and setter for cuda_device
int ConfigDetector::getCudaDevice() const {
    return cuda_device;
}

void ConfigDetector::setCudaDevice(int value) {
    cuda_device = value;
}

// Getter and setter for nms_threshold
float ConfigDetector::getNmsThreshold() const {
    return nms_threshold;
}

void ConfigDetector::setNmsThreshold(float value) {
    nms_threshold = value;
}

// Getter and setter for confidence_threshold
float ConfigDetector::getConfidenceThreshold() const {
    return confidence_threshold;
}

void ConfigDetector::setConfidenceThreshold(float value) {
    confidence_threshold = value;
}

// Getter and setter for rois
std::vector<iv_annotation::RegionOfInterest> ConfigDetector::getRois() const {
    return rois;
}

void ConfigDetector::setRois(const std::vector<iv_annotation::RegionOfInterest>& value) {
    rois = value;
}