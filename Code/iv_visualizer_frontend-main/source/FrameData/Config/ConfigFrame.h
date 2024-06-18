#ifndef CONFIG_FRAME_H
#define CONFIG_FRAME_H

#include <QFrame>
#include <QPushButton>

#include "Config.h"

namespace Ui {
class ConfigFrame;
}

/**
 * @class ConfigFrame
 * @brief GUI component for displaying and editing configuration settings.
 *
 * ConfigFrame provides a graphical interface for displaying the current
 * configuration settings and allows the user to modify these settings.
 */
class ConfigFrame : public QFrame {
  Q_OBJECT

 public:
    /**
     * @brief Constructs a ConfigFrame with the given parent and configuration.
     * @param a_parent The parent widget of this frame.
     * @param a_config Pointer to the configuration object to be used.
     */
    explicit ConfigFrame(QWidget* a_parent = nullptr, Config* a_config = nullptr);

    /**
     * @brief Destructor for ConfigFrame.
     */
    ~ConfigFrame();

    /**
     * @brief Updates the GUI elements to reflect the current configuration settings.
     */
    void update_gui();
 signals:
    /**
     * @brief Emits signal to update the configuration object based on the GUI inputs.
     */
    void sig_update_config();

 private slots:
    /**
     * @brief Handles the submit button click event.
     */
    void handle_button();

private:
  QString inference_type_to_string(InferenceType a_type);
  InferenceType string_to_inference_type(const QString& a_str);

 private:
    Ui::ConfigFrame* m_ui;         ///< Pointer to the UI setup class.
    Config* m_config;              ///< Pointer to the configuration object.
    QPushButton* m_submit_button;  ///< Pointer to the submit button.
    /**
     * @brief Default name for ONNX CPU inference type.
     *
     * The default name for ONNX CPU inference type.
     */
    const QString m_name_onnx_cpu = "ONNX CPU";

    /**
     * @brief Default name for ONNX GPU inference type.
     *
     * The default name for ONNX GPU inference type.
     */
    const QString m_name_onnx_gpu = "ONNX GPU";

    /**
     * @brief Default name for TensorRT FLOAT16 inference type.
     *
     * The default name for TensorRT FLOAT16 inference type.
     */
    const QString m_name_trt_float_16 = "TRT FLOAT16";

    /**
     * @brief Default name for TensorRT FLOAT32 inference type.
     *
     * The default name for TensorRT FLOAT32 inference type.
     */
    const QString m_name_trt_float_32 = "TRT FLOAT32";

    /**
     * @brief Default name for unknown inference type.
     *
     * The default name for an unknown inference type.
     */
    const QString m_name_unknown = "UNKNOWN";

    /**
     * @brief Default inference type.
     *
     * The default type of inference to be used.
     */
    const InferenceType m_default_inf_type =
        InferenceType::ONNX_CPU;  ///< the default inference type.

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

#endif  // CONFIG_FRAME_H
