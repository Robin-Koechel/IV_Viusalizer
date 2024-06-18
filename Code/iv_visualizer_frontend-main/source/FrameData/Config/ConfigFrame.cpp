#include "ConfigFrame.h"
#include "../../../build/ui_configframe.h"
#include "Config.h"

ConfigFrame::ConfigFrame(QWidget* a_parent, Config* a_config)
    : QFrame(a_parent), m_ui(new Ui::ConfigFrame), m_config(a_config) {
    m_ui->setupUi(this);
    m_submit_button = m_ui->SubmitPushButton;
    connect(m_submit_button, &QPushButton::released, this, &ConfigFrame::handle_button);

    this->update_gui();
}

ConfigFrame::~ConfigFrame() {
    delete m_ui;
}

QString ConfigFrame::inference_type_to_string(InferenceType a_type) {
    switch (a_type) {
        case InferenceType::ONNX_CPU:
            return m_name_onnx_cpu;
        case InferenceType::ONNX_GPU:
            return m_name_onnx_gpu;
        case InferenceType::TRT_FLOAT16:
            return m_name_trt_float_16;
        case InferenceType::TRT_FLOAT32:
            return m_name_trt_float_32;
        default:
            return m_name_unknown;
    }
}

InferenceType ConfigFrame::string_to_inference_type(const QString& a_str) {
    if (a_str == m_name_onnx_cpu) {
        return InferenceType::ONNX_CPU;
    } else if (a_str == m_name_onnx_gpu) {
        return InferenceType::ONNX_GPU;
    } else if (a_str == m_name_trt_float_16) {
        return InferenceType::TRT_FLOAT16;
    } else if (a_str == m_name_trt_float_32) {
        return InferenceType::TRT_FLOAT32;
    } else {
        return InferenceType::ONNX_CPU; // Default value for unknown strings
    }
}

void ConfigFrame::update_gui() {
    m_ui->cudaDeviceSpinBox->setValue(m_config->get_config_detector().get_cuda_device());
    m_ui->nmsTresholdDoubleSpinBox->setValue(m_config->get_config_detector().get_nms_threshold());
    m_ui->confidenceThresholdSpinBox->setValue(m_config->get_config_detector().get_confidence_threshold());

    int a_index = m_ui->inferenceTypeComboBox->findText(inference_type_to_string(m_config->get_config_detector().get_inference_type()));
    m_ui->inferenceTypeComboBox->setCurrentIndex(a_index);
    
    m_ui->logInfoCheckBox->setChecked(m_config->get_config_container().get_log_info());
    m_ui->logDebugCheckBox->setChecked(m_config->get_config_container().get_log_debug());
    
    m_ui->StreamUrlLineEdit->setText(m_config->get_config_gstreamer().get_stream_url());
    m_ui->targetFpsSpinBox->setValue(m_config->get_config_gstreamer().get_target_fps());
    m_ui->imageWidthSpinBox->setValue(m_config->get_config_gstreamer().get_custom_size_width());
    m_ui->imageHeightSpinBox->setValue(m_config->get_config_gstreamer().get_custom_size_height());
}

void ConfigFrame::handle_button() {
    m_config->get_config_detector().set_cuda_device(m_ui->cudaDeviceSpinBox->value());
    m_config->get_config_detector().set_nms_threshold(m_ui->nmsTresholdDoubleSpinBox->value());
    m_config->get_config_detector().set_confidence_threshold(m_ui->confidenceThresholdSpinBox->value());

    InferenceType a_type = string_to_inference_type(m_ui->inferenceTypeComboBox->currentText());
    m_config->get_config_detector().set_inference_type(a_type);

    m_config->get_config_container().set_log_info(m_ui->logInfoCheckBox->isChecked());
    m_config->get_config_container().set_log_debug(m_ui->logDebugCheckBox->isChecked());

    m_config->get_config_gstreamer().set_stream_url(m_ui->StreamUrlLineEdit->text());
    m_config->get_config_gstreamer().set_target_fps(m_ui->targetFpsSpinBox->value());
    m_config->get_config_gstreamer().set_custom_size_width(m_ui->imageWidthSpinBox->value());
    m_config->get_config_gstreamer().set_custom_size_height(m_ui->imageHeightSpinBox->value());
    emit sig_update_config();
    this->close();

}
