#include "BoundingBox.h"
BoundingBox::BoundingBox(const std::vector<int>& a_coords,
                         const float a_confidence, const QString& a_label,
                         const cv::Size2i& a_image_size, const Format& a_format,
                         const int a_rotation)
    : m_confidence(a_confidence),
      m_label(a_label),
      m_image_wh(a_image_size),
      m_rotation(a_rotation) {
  m_bbox = std::vector<int>(4);
  if (!is_format_valid(a_format)) {
    std::cerr << "Error: Format is invalid!" << std::endl;
    throw std::invalid_argument("Error: Invalid parameters!");
  }

  m_bbox = convert_from(a_coords, a_format);

  if (!is_coords_valid(m_bbox) || !is_valid()) {
    std::cerr << to_string() << std::endl;
    throw std::invalid_argument("Error: Invalid parameters!");
  }
}

BoundingBox::~BoundingBox() {}

bool BoundingBox::operator==(const BoundingBox& a_other) const {
  return this->m_bbox[0] == a_other.m_bbox[0] &&
         this->m_bbox[1] == a_other.m_bbox[1] &&
         this->m_bbox[2] == a_other.m_bbox[2] &&
         this->m_bbox[3] == a_other.m_bbox[3] &&
         this->m_confidence == a_other.m_confidence &&
         this->m_label.compare(a_other.m_label) == 0 &&
         this->m_image_wh.width == a_other.m_image_wh.width &&
         this->m_image_wh.height == a_other.m_image_wh.height &&
         this->m_rotation == a_other.m_rotation;
}

BoundingBox& BoundingBox::operator=(const BoundingBox& a_other) {
  if (this == &a_other) {
    return *this;
  }

  m_bbox = a_other.m_bbox;
  m_confidence = a_other.m_confidence;
  m_label = a_other.m_label;
  m_rotation = a_other.m_rotation;
  m_image_wh = a_other.m_image_wh;

  return *this;
}

int BoundingBox::str_to_format_index(const QString& a_str) {
  if (a_str.compare("TLWH") == 0) {
    return static_cast<int>(Format::TLWH);
  } else if (a_str.compare("TLBR") == 0) {
    return static_cast<int>(Format::TLBR);
  } else if (a_str.compare("CWH") == 0) {
    return static_cast<int>(Format::CWH);
  }

  return static_cast<int>(Format::UNKNOWN);
}

std::vector<int> BoundingBox::get_coords(const Format a_format) const {
  if (m_bbox.empty()) return std::vector<int>();
  std::vector<int> output(4);
  switch (a_format) {
    case Format::TLWH:
      output = m_bbox;
      break;
    case Format::CWH:
      output[0] = m_bbox[0] + std::round(m_bbox[2] / 2);
      output[1] = m_bbox[1] + std::round(m_bbox[3] / 2);
      output[2] = m_bbox[2];
      output[3] = m_bbox[3];
      break;
    case Format::TLBR:
      output = {m_bbox[0], m_bbox[1], m_bbox[0] + m_bbox[2],
                m_bbox[1] + m_bbox[3]};
      break;
  }
  return output;
}

bool BoundingBox::set_coords(const std::vector<int>& a_new_coords,
                             const Format& a_format) {
  m_bbox = convert_from(a_new_coords, a_format);

  if (is_coords_valid(m_bbox)) {
    return true;
  }

  std::cerr << "Error: Coords is invalid!" << std::endl;
  return false;
}

cv::Size2i BoundingBox::get_image_wh() const { return m_image_wh; }

bool BoundingBox::set_image_wh(const cv::Size2i& a_new_image_wh,
                               const Image_Resize_Type& a_resize_type,
                               const bool a_scale_box) {
  if (is_image_wh_valid(a_new_image_wh)) {
    if (a_scale_box) {
      scale_bbox(a_new_image_wh, a_resize_type);
      m_image_wh = a_new_image_wh;
      m_bbox = clip_to_image_size(m_bbox);
    }
    m_image_wh = a_new_image_wh;
    return true;
  }

  std::cerr << "Error: Image wh is invalid!" << std::endl;
  return false;
}

int BoundingBox::get_rotation() const { return m_rotation; }

void BoundingBox::set_rotation(const int a_rotation) {
  m_rotation = (a_rotation % 360 + 360) % 360;  // [0, 360)
}

float BoundingBox::get_confidence() const { return m_confidence; }

void BoundingBox::set_confidence(const float a_new_confidence) {
  m_confidence = a_new_confidence;
}

QString BoundingBox::get_label() const { return m_label; }

void BoundingBox::set_label(const QString& a_new_label) {
  m_label = a_new_label;
}

bool BoundingBox::is_valid() const {
  if (!is_image_wh_valid(m_image_wh)) {
    std::cerr << "Error: Image size is invalid!" << std::endl;
    return false;
  }

  if (!is_coords_valid(m_bbox)) {
    std::cerr << "Error: Coords is invalid!" << std::endl;
    return false;
  }

  return true;
}

std::string BoundingBox::to_string() const {
  return "BBox:\n\tFormat: TLWH"
         "\n\tCoords: at (" +
         std::to_string(m_bbox[0]) + ", " + std::to_string(m_bbox[1]) + ") " +
         std::to_string(m_bbox[2]) + " x " + std::to_string(m_bbox[3]) +
         "\n\tRotation: " + std::to_string(m_rotation) +
         "\n\tConfidence: " + std::to_string(m_confidence) +
         "\n\tLabel: " + m_label.toStdString() +
         "\n\tImage Size: " + std::to_string(m_image_wh.width) + " x " +
         std::to_string(m_image_wh.height);
}

// Private methods:
std::vector<int> BoundingBox::clip_to_image_size(
    const std::vector<int>& a_coords) const {
  std::vector<int> output(4);

  output[0] = std::max(std::min(a_coords[0], m_image_wh.width), 0);
  output[1] = std::max(std::min(a_coords[1], m_image_wh.height), 0);

  output[2] = std::max(
      std::min(a_coords[0] + a_coords[2], m_image_wh.width) - a_coords[0], 0);
  output[3] = std::max(
      std::min(a_coords[1] + a_coords[3], m_image_wh.height) - a_coords[1], 0);
  return output;
}

std::vector<int> BoundingBox::convert_from(const std::vector<int>& a_coords,
                                           const Format& a_format) const {
  int x, y, width, height;
  switch (a_format) {
    case Format::TLWH:
      x = a_coords[0];
      y = a_coords[1];
      width = a_coords[2];
      height = a_coords[3];
      break;
    case Format::CWH:
      width = a_coords[2];
      height = a_coords[3];
      x = a_coords[0] - std::round(width / 2);
      y = a_coords[1] - std::round(height / 2);
      break;
    case Format::TLBR:
      y = a_coords[1];
      x = a_coords[0];
      width = a_coords[2] - x;
      height = a_coords[3] - y;
      break;
  }

  return clip_to_image_size({x, y, width, height});
}

bool BoundingBox::scale_bbox(const cv::Size2i& a_new_image_wh,
                             const Image_Resize_Type& a_resize_type) {
  if (a_resize_type == Image_Resize_Type::KEEP_RATIO) {
    int width = static_cast<int>(
        m_image_wh.height *
        (static_cast<float>(a_new_image_wh.width) / a_new_image_wh.height));
    int height = static_cast<int>(
        m_image_wh.width *
        (static_cast<float>(a_new_image_wh.height) / a_new_image_wh.width));

    const bool horizontal = (height <= m_image_wh.height);

    width = horizontal ? m_image_wh.width : width;
    height = horizontal ? height : m_image_wh.height;

    const int x = horizontal
                      ? 0
                      : static_cast<int>(
                            static_cast<float>(m_image_wh.width - width) / 2.f);
    const int y =
        horizontal ? static_cast<int>(
                         static_cast<float>(m_image_wh.height - height) / 2.f)
                   : 0;

    cv::Rect2i roi(x, y, width, height);

    float aspect_ratio =
        static_cast<float>(m_image_wh.width) / m_image_wh.height;
    float aspect_ratio_new =
        static_cast<float>(a_new_image_wh.width) / a_new_image_wh.height;

    cv::Size2f scale;
    int offset_x = 0;
    int offset_y = 0;
    if (aspect_ratio_new > aspect_ratio) {
      scale.width = scale.height =
          static_cast<float>(a_new_image_wh.width) / roi.width;
      offset_y = ((m_image_wh.height - roi.height) * scale.height) / 2;
    } else {
      scale.width = scale.height =
          static_cast<float>(a_new_image_wh.height) / roi.height;
      offset_x = ((m_image_wh.width - roi.width) * scale.width) / 2;
    }

    m_bbox[0] = std::floor((m_bbox[0] * scale.width) - offset_x);
    m_bbox[1] = std::floor((m_bbox[1] * scale.height) - offset_y);
    m_bbox[2] = std::floor(m_bbox[2] * scale.width);
    m_bbox[3] = std::floor(m_bbox[3] * scale.height);
  } else {
    cv::Size2f scale;
    scale.width = a_new_image_wh.width / static_cast<float>(m_image_wh.width);
    scale.height =
        a_new_image_wh.height / static_cast<float>(m_image_wh.height);

    m_bbox[0] = std::floor(m_bbox[0] * scale.width);
    m_bbox[1] = std::floor(m_bbox[1] * scale.height);
    m_bbox[2] = std::floor(m_bbox[2] * scale.width);
    m_bbox[3] = std::floor(m_bbox[3] * scale.height);
  }

  return true;
}

bool BoundingBox::is_image_wh_valid(const cv::Size2i& a_image_wh) const {
  return a_image_wh.area() > 0;
}

bool BoundingBox::is_format_valid(const Format& a_format) const {
  return a_format == Format::TLWH || a_format == Format::TLBR ||
         a_format == Format::CWH;
}

bool BoundingBox::is_coords_valid(const std::vector<int>& a_coords) const {
  if (a_coords.empty()) return false;

  return ((a_coords[0] >= 0 && a_coords[0] < m_image_wh.width) &&
          (a_coords[1] >= 0 && a_coords[1] < m_image_wh.height) &&
          (a_coords[2] > 0 && a_coords[3] > 0) &&
          ((a_coords[0] + a_coords[2]) <= m_image_wh.width) &&
          ((a_coords[1] + a_coords[3]) <= m_image_wh.height));

  return false;
}
