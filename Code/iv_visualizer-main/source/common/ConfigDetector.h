#ifndef CONFIGDETECTOR_H
#define CONFIGDETECTOR_H

#include <vector>
#include <RegionOfInterest.h>

/**
 * @brief Enum class defining the types of inference supported.
 */
enum class InferenceType {
    ONNX_CPU,     /**< CPU. */
    ONNX_GPU,     /**< GPU Cuda. */
    TRT_FLOAT16,  /**< GPU TensorRT via trt_model_handler. */
    TRT_FLOAT32   /**< GPU TensorRT via trt_model_handler. */
};

/**
 * @brief Class representing configuration settings for the detector.
 */
class ConfigDetector {
private:
    InferenceType inference_type;        /**< Type of inference. */
    int cuda_device;                     /**< CUDA device ID. */
    float nms_threshold;                 /**< Non-Maximum Suppression threshold. */
    float confidence_threshold;          /**< Confidence threshold. */
    std::vector<iv_annotation::RegionOfInterest> rois;  /**< List of regions of interest. */

public:
    /**
     * @brief Constructor.
     * @param type Type of inference.
     * @param device CUDA device ID.
     * @param nms Non-Maximum Suppression threshold.
     * @param confidence Confidence threshold.
     * @param list List of regions of interest.
     */
    ConfigDetector(InferenceType type, int device, float nms, float confidence, const std::vector<iv_annotation::RegionOfInterest>& list);

    // Getter and setter for inference_type
    InferenceType getInferenceType() const;
    void setInferenceType(InferenceType value);

    // Getter and setter for cuda_device
    int getCudaDevice() const;
    void setCudaDevice(int value);

    // Getter and setter for nms_threshold
    float getNmsThreshold() const;
    void setNmsThreshold(float value);

    // Getter and setter for confidence_threshold
    float getConfidenceThreshold() const;
    void setConfidenceThreshold(float value);

    // Getter and setter for rois
    std::vector<iv_annotation::RegionOfInterest> getRois() const;
    void setRois(const std::vector<iv_annotation::RegionOfInterest>& value);
};

#endif // CONFIGDETECTOR_H
