#ifndef COLORDICT_H
#define COLORDICT_H

#include <QMap>
#include <QString>
#include <QColor>

class ColorDict{

private:
    ColorDict();
    QMap<QString, int> m_label_to_color_map; // the value stores the hue(h) value for hsl-color format
    int m_number_colors = 360;
    int m_saturation = 255;
    int m_lightness = 128;
    
    int get_new_color(int step);
    static ColorDict* m_dict;

public:
    ~ColorDict();
    static ColorDict* get_color_dict();
    QColor get_color(const QString &label);
};
#endif