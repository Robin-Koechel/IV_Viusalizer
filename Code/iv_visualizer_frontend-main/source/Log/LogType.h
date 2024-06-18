#ifndef LOGTYPE_H
#define LOGTYPE_H

/**
 * @brief Enum representing different types of logs.
 */
enum LogType{
    DETECTOR,       /**< Detector log type. */
    PREPROCESSOR,   /**< Preprocessor log type. */
    POSTPROCESSOR,  /**< Postprocessor log type. */
    ERROR,          /**< Error log type. */
    WARNING,        /**< Warning log type. */
    DEBUG           /**< Debug log type. */
};

#endif // LOGTYPE_H
