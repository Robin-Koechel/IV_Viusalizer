#include "ColorDict.h"

#include <QColor>
#include <QString>
#include <QFileInfo>
#include <QSettings>
#include "../main/MainWindow.h"
ColorDict* ColorDict::m_dict = nullptr;  // Initialisierung der statischen Variable

ColorDict::ColorDict() {

  QString a_settings_path = MainWindow::m_config_file_path;
  if (!QFileInfo::exists(a_settings_path)) {
    QSettings settings(a_settings_path, QSettings::IniFormat);
    if (!settings.contains("color_dict")) {
      // Begin the "color_dict" group
      settings.beginGroup("color_dict");

      // Get the keys in the "color_dict" group
      QStringList keys = settings.childKeys();

      // Iterate over the keys and add them to the map
      foreach (const QString& key, keys) {
        int value = settings.value(key).toInt();
        m_label_to_color_map.insert(key, value);
      }

      // End the "color_dict" group
      settings.endGroup();
    }
  }
}

ColorDict* ColorDict::get_color_dict() {
  if (m_dict == nullptr) {
    m_dict = new ColorDict();
  }
  return m_dict;
}

QColor ColorDict::get_color(const QString& label) {
  QString a_label_lowercase = label.toLower();
  if (m_label_to_color_map.contains(a_label_lowercase)) {
    int h = m_label_to_color_map.value(a_label_lowercase);
    return QColor::fromHsl(h, m_saturation, m_lightness);
  } else {
    int a_hue = get_new_color(m_number_colors);
    m_label_to_color_map.insert(a_label_lowercase, a_hue);

    // write to file
      QString a_settings_path = MainWindow::m_config_file_path;
  QSettings settings(a_settings_path, QSettings::IniFormat);

  // Begin the "color_dict" group
  settings.beginGroup("color_dict");

  // Iterate over each key-value pair in the map
  QMap<QString, int>::const_iterator it;
  for (it = m_label_to_color_map.constBegin(); it != m_label_to_color_map.constEnd(); ++it) {
    // Save key-value pair in the settings
    settings.setValue(it.key(), it.value());
  }

  // End the "color_dict" group
  settings.endGroup();


    return QColor::fromHsl(a_hue, m_saturation, m_lightness);
  }
}

int ColorDict::get_new_color(int step) {
  if (step != 0) {
    for (int i = 0; i * step <= m_number_colors; i++) {
      if (!m_label_to_color_map.values().contains((i * step) % m_number_colors)) {
        return (i * step) % m_number_colors;
      }
    }
    return get_new_color(step / 2);
  } else {
    return 0;
  }
}

ColorDict::~ColorDict() {  // TOTO: change path
  delete this->m_dict;
}
