#ifndef LIBRARY_HEADER_INCLUDE_BOUNDINGBOX_H_
#define LIBRARY_HEADER_INCLUDE_BOUNDINGBOX_H_

#include <QString>

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

// From Types.h of iv_detector
enum class Image_Resize_Type { UNKNOWN, DIRECTLY, KEEP_RATIO };

/**
 * @brief Represents a bounding box.
 *
 * This class is used to define a bounding box with various properties such as
 * the coords, confidence, image size, and rotation.
 */
class BoundingBox {
 public:
  /**
   * @brief The format of the bounding box coordinates.
   *
   */
  enum class Format { UNKNOWN, TLWH, TLBR, CWH };

  BoundingBox() {}
  /**
   * @brief Construct a new BoundingBox object
   *
   * @param a_coords The coords defining the bounding box.
   * @param a_confidence The confidence score of the detection.
   * @param a_label The label of the detected object.
   * @param a_image_size The size of the image in which the bounding box is
   * detected.
   * @param a_format The format of the bounding box coordinates.
   * @param a_rotation The rotation of the bounding box in degrees.
   */
  BoundingBox(const std::vector<int>& a_coords, const float a_confidence,
              const QString& a_label, const cv::Size2i& a_image_size,
              const Format& a_format = Format::TLWH, const int a_rotation = 0);
  ~BoundingBox();

  /**
   * @brief Overloads the equality operator for BoundingBox.
   *
   * @param a_other Another BoundingBox to compare with.
   * @return true If both Bounding Boxes are equal.
   * @return false If both Bounding Boxes are not equal.
   */
  bool operator==(const BoundingBox& a_other) const;

  /**
   * @brief Overloads the assignment operator for BoundingBox
   *
   * @param a_other The BoundingBox instance to assign from.
   * @return BoundingBox& Reference to the updated BoundingBox instance.
   */
  BoundingBox& operator=(const BoundingBox& a_other);

  /**
   * @brief Static method to convert a string to a format index.
   *
   * @param str The string representation of the format.
   * @return int The index of the format corresponding to the input string.
   */
  static int str_to_format_index(const QString& str);

  /**
   * @brief Get the coords of the bounding box.
   * @param a_format desired format of the returned coordinates
   * @return std::vector<int> coordinates in the required format.
   */
  std::vector<int> get_coords(const Format a_format) const;

  /**
   * @brief Set the coords of the bounding box.
   *
   * @param a_new_coords The new coords to define the bounding box.
   * @param a_format The format of the new coords.
   * @return true If the coords was set successfully.
   * @return false If the coords was not set.
   */
  bool set_coords(const std::vector<int>& a_new_coords, const Format& a_format);

  /**
   * @brief Get the image size of the image in which the bounding box is
   * detected.
   *
   * @return cv::Size2i The size of the image.
   */
  cv::Size2i get_image_wh() const;

  /**
   * @brief Set the image width and height
   *
   * @param a_new_image_wh The new width and height of the image.
   * @param a_resize_type The type of resizing applied to the bounding box, if
   * any.
   * @param a_scale_box If true, scales the bounding box according to the new
   * image dimensions.
   * @return true If operations is successful.
   * @return false Otherwise.
   */
  bool set_image_wh(
      const cv::Size2i& a_new_image_wh,
      const Image_Resize_Type& a_resize_type = Image_Resize_Type::UNKNOWN,
      bool a_scale_box = true);

  /**
   * @brief Get the current rotation of the bounding box.
   *
   * @return int The rotation angle in degrees.
   */
  int get_rotation() const;

  /**
   * @brief Set the rotation of the bounding box.
   *
   * @param a_rotation The new rotation angle in degrees.
   */
  void set_rotation(const int a_rotation);

  /**
   * @brief Get the confidence score of the bounding box.
   *
   * @return float The confidence score.
   */
  float get_confidence() const;

  /**
   * @brief Set the confidence score of the bounding box.
   *
   * @param a_new_confidence The new confidence score.
   */
  void set_confidence(const float a_new_confidence);

  /**
   * @brief Get the label of the bounding box.
   *
   * @return QString The label.
   */
  QString get_label() const;

  /**
   * @brief Set the label of the bounding box.
   *
   * @param a_new_label The new label.
   */
  void set_label(const QString& a_new_label);

  /**
   * @brief Checks if the bounding box is valid.
   *
   * @return true If the bounding box is valid.
   * @return false Otherwise.
   */
  bool is_valid() const;

  /**
   * @brief Returns a string representation of the bounding box.
   *
   * @return std::string A string representation of the bounding box.
   */
  std::string to_string() const;

 private:
  std::vector<int> m_bbox;  ///< The coords defining the bounding box.
  int m_rotation;  ///< The rotation angle of the bounding box in degrees.
  /**
   * @brief The confidence score of the bounding box, typically between 0.f
   * and 1.f.
   *
   */
  float m_confidence;
  QString m_label;  ///< The label associated with the bounding box.

  /**
   * @brief The width and height of the image associated with the bounding box.
   *
   */
  cv::Size2i m_image_wh;

  /**
   * @brief Adjusts the coords of the Bounding Box to fit within image size
   *
   * @return std::vector<int> The coords clip to image size.
   */
  std::vector<int> clip_to_image_size(const std::vector<int>& a_coords) const;

  /**
   * @brief Calculates the bounding box coords (in tlwh) from the given coords
   * of a differnt format.
   *
   * @param a_coords The coords to convert.
   * @param a_format The format to which the bounding box should be
   * converted.
   * @return std::vector<int> The calculated coords in the new format.
   */
  std::vector<int> convert_from(const std::vector<int>& a_coords,
                                const Format& a_format) const;

  /**
   * @brief Scales the bounding box to a new image size.
   *
   * @param a_new_image_wh The new width and height of the image.
   * @param a_resize_type The resize type
   * @return true If the scaling operation is successful.
   * @return false Otherwise.
   */
  bool scale_bbox(
      const cv::Size2i& a_new_image_wh,
      const Image_Resize_Type& a_resize_type = Image_Resize_Type::UNKNOWN);

  /**
   * @brief Checks if the provided image dimensions are valid.
   *
   * @param a_image_wh The image dimensions to check.
   * @return true If the image dimensions are valid.
   * @return false Otherwise.
   */
  bool is_image_wh_valid(const cv::Size2i& a_image_wh) const;

  /**
   * @brief Checks if the provided format is valid.
   *
   * @param a_format The format to check.
   * @return true If the format is valid.
   * @return false Otherwise.
   */
  bool is_format_valid(const Format& a_format) const;

  /**
   * @brief Checks if the provided coords is valid.
   *
   * @param a_coords The coords to check
   * @return true If the coords is valid.
   * @return false Otherwise.
   */
  bool is_coords_valid(const std::vector<int>& a_coords) const;
};

#endif  // LIBRARY_HEADER_INCLUDE_BOUNDINGBOX_H_
