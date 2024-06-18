#ifndef VIEWTYPEFRAME_H
#define VIEWTYPEFRAME_H

#include <QFrame>

namespace Ui {
class ViewTypeFrame;
}

/**
 * @class ViewTypeFrame
 * @brief Frame widget for selecting view types.
 *
 * This class represents a frame widget used for selecting different view types.
 * It provides a graphical interface for users to choose the type of view they want
 * to visualize.
 */
class ViewTypeFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ViewTypeFrame object.
     * @param a_parent The parent widget.
     */
    explicit ViewTypeFrame(QWidget *a_parent = nullptr);

    /**
     * @brief Destroys the ViewTypeFrame object.
     */
    ~ViewTypeFrame();

private:
    Ui::ViewTypeFrame *m_ui; ///< User interface pointer.
};

#endif // VIEWTYPEFRAME_H
