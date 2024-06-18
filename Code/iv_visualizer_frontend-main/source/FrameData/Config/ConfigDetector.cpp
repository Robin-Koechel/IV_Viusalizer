#include "ConfigDetector.h"

ConfigDetector::ConfigDetector() 
    : m_cuda_device(m_default_cuda_device), m_nms_threshold(m_default_nms_threshold), m_confidence_threshold(m_default_confidence_threshold), m_inference_type(m_default_inf_type) {
        m_cuda_device = m_default_cuda_device;
        m_nms_threshold = m_default_nms_threshold;
        m_confidence_threshold = m_default_confidence_threshold;
        m_inference_type = m_default_inf_type;
    }

ConfigDetector::ConfigDetector(int a_cuda_device, double a_nms_threshold, int a_confidence_threshold, InferenceType a_inference_type, QVector<RegionOfInterest*> a_rois):
    m_cuda_device(a_cuda_device),
    m_nms_threshold(a_nms_threshold),
    m_confidence_threshold(a_confidence_threshold),
    m_inference_type(a_inference_type),
    m_rois(a_rois)
{}

int ConfigDetector::get_cuda_device() const {
    return m_cuda_device;
}

void ConfigDetector::set_cuda_device(int a_new_cuda_device) {
    m_cuda_device = a_new_cuda_device;
}

double ConfigDetector::get_nms_threshold() const {
    return m_nms_threshold;
}

void ConfigDetector::set_nms_threshold(double a_new_nms_threshold) {
    m_nms_threshold = a_new_nms_threshold;
}

int ConfigDetector::get_confidence_threshold() const {
    return m_confidence_threshold;
}

void ConfigDetector::set_confidence_threshold(int a_new_confidence_threshold) {
    m_confidence_threshold = a_new_confidence_threshold;
}

const QVector<RegionOfInterest*> ConfigDetector::get_rois() const {
    return m_rois;
}

void ConfigDetector::set_rois(const QVector<RegionOfInterest*>& a_new_rois) {
    m_rois = a_new_rois;
}

InferenceType ConfigDetector::get_inference_type() const {
    return m_inference_type;
}

void ConfigDetector::set_inference_type(const InferenceType& a_new_inference_type) {
    m_inference_type = a_new_inference_type;
}

