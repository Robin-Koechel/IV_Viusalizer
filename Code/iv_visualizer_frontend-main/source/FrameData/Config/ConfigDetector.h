#ifndef CONFIG_DETECTOR_H
#define CONFIG_DETECTOR_H

#include "InferenceType.h"
#include "../Data/RegionOfInterest.h"
#include <QVector>

/**
 * @class ConfigDetector
 * @brief Holds configuration settings for the detection system.
 *
 * This class encapsulates the configuration settings necessary for setting up
 * and controlling the detection system. It includes settings for the CUDA device,
 * Non-Maximum Suppression (NMS) threshold, confidence threshold, regions of interest,
 * and the type of inference to perform.
 */
class ConfigDetector
{


public:
    /**
     * @brief Constructs a ConfigDetector object with default settings.
     */
    ConfigDetector();


    /**
     * @brief Constructs a ConfigDetector object with the specified settings.
     *
     * This constructor initializes a ConfigDetector object with the given CUDA device,
     * non-maximum suppression (NMS) threshold, confidence threshold, inference type,
     * and regions of interest (ROIs).
     *
     * @param a_cuda_device The CUDA device used for inference.
     * @param a_nms_threshold The threshold for non-maximum suppression.
     * @param a_confidence_threshold The confidence threshold for object detection.
     * @param a_inference_type The type of inference used for object detection.
     * @param a_rois The regions of interest (ROIs) for object detection.
     */
    ConfigDetector(int a_cuda_device, double a_nms_threshold, int a_confidence_threshold, InferenceType a_inference_type, QVector<RegionOfInterest*> a_rois);


    // Assignment operator
    ConfigDetector& operator=(const ConfigDetector& other) {
        if (this != &other) {
            // Copy members from 'other' to 'this'
            m_cuda_device = other.m_cuda_device;
            m_nms_threshold = other.m_nms_threshold;
            m_confidence_threshold = other.m_confidence_threshold;

            // Copy regions of interest (ROIs)
            m_rois.clear();
            for (const auto& roi : other.m_rois) {
                m_rois.push_back(new RegionOfInterest(*roi));
            }

            m_inference_type = other.m_inference_type;
        }
        return *this;
    }

    /**
     * @brief Gets the CUDA device ID.
     * @return The CUDA device ID.
     */
    int get_cuda_device() const;

    /**
     * @brief Sets the CUDA device ID.
     * @param a_new_cuda_device The new CUDA device ID.
     */
    void set_cuda_device(int a_new_cuda_device);

    /**
     * @brief Gets the NMS threshold.
     * @return The NMS threshold.
     */
    double get_nms_threshold() const;

    /**
     * @brief Sets the NMS threshold.
     * @param a_new_nms_threshold The new NMS threshold.
     */
    void set_nms_threshold(double a_new_nms_threshold);

    /**
     * @brief Gets the confidence threshold for detections.
     * @return The confidence threshold.
     */
    int get_confidence_threshold() const;

    /**
     * @brief Sets the confidence threshold for detections.
     * @param a_new_confidence_threshold The new confidence threshold.
     */
    void set_confidence_threshold(int a_new_confidence_threshold);

    /**
     * @brief Gets the regions of interest.
     * @return A constant reference to the vector of regions of interest.
     */
    const QVector<RegionOfInterest*> get_rois() const;

    /**
     * @brief Sets the regions of interest.
     * @param a_new_rois The new vector of regions of interest.
     */
    void set_rois(const QVector<RegionOfInterest*>& a_new_rois);

    /**
     * @brief Gets the inference type.
     * @return The inference type.
     */
    InferenceType get_inference_type() const;

    /**
     * @brief Sets the inference type.
     * @param a_new_inference_type The new inference type.
     */
    void set_inference_type(const InferenceType& a_new_inference_type);
    
private:
    int m_cuda_device;                ///< CUDA device ID for GPU acceleration.
    double m_nms_threshold;           ///< Threshold for Non-Maximum Suppression.
    int m_confidence_threshold;       ///< Confidence threshold for detections.
    QVector<RegionOfInterest*> m_rois;///< Regions of interest for detection.
    InferenceType m_inference_type;   ///< Type of inference to be used.
    
/**
 * @brief Default inference type.
 * 
 * The default type of inference to be used.
 */
const InferenceType m_default_inf_type = InferenceType::ONNX_CPU; ///< the default inference type.

/**
 * @brief Default CUDA device index.
 * 
 * The default CUDA device index to be used.
 */
const int m_default_cuda_device = 0;

/**
 * @brief Default non-maximum suppression (NMS) threshold.
 * 
 * The default threshold value for non-maximum suppression.
 */
const double m_default_nms_threshold = 0.0;

/**
 * @brief Default confidence threshold.
 * 
 * The default threshold value for confidence.
 */
const int m_default_confidence_threshold = 0;
};

#endif // CONFIG_DETECTOR_H
