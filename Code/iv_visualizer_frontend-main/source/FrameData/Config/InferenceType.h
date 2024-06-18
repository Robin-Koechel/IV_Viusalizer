#ifndef INFERENCE_TYPE_H
#define INFERENCE_TYPE_H

#include <QString>

/**
 * @brief Enumeration defining different types of inference.
 */
enum InferenceType {
    ONNX_CPU,      ///< ONNX model inference on CPU.
    ONNX_GPU,      ///< ONNX model inference on GPU.
    TRT_FLOAT16,   ///< TensorRT model inference with FLOAT16 precision.
    TRT_FLOAT32    ///< TensorRT model inference with FLOAT32 precision.
};

#endif // INFERENCE_TYPE_H