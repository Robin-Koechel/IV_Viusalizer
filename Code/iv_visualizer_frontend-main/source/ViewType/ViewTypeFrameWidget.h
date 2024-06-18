#ifndef VIEWTYPEFRAMEWIDGET_H
#define VIEWTYPEFRAMEWIDGET_H

#include <QWidget>
#include "ViewType.h"
#include "AnnotationWithoutVideo.h"
#include "AnnotationAndVideo.h"
#include "OnlyVideo.h"

namespace Ui {
class ViewTypeFrameWidget;
}

/**
 * @class ViewTypeFrameWidget
 * @brief Widget for selecting view types.
 *
 * This class represents a widget used for selecting different view types.
 * It provides a graphical interface for users to choose the type of view they want
 * to visualize.
 */
class ViewTypeFrameWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a ViewTypeFrameWidget object.
     * @param a_parent The parent widget.
     */
    explicit ViewTypeFrameWidget(QWidget *a_parent = nullptr);

    /**
     * @brief Destroys the ViewTypeFrameWidget object.
     */
    ~ViewTypeFrameWidget();

    /**
     * @brief Retrieves the selected view type.
     * @return A reference to the selected view type object.
     */
    const ViewType& get_selected_view_type();

signals:
    /**
     * @brief Signal emitted when the selected view type changes.
     * @param a_type The new selected view type.
     */
    void sig_view_type_changed(const ViewType& a_type);

private slots:
    void on_rbtnOnlyVideo_clicked();
    void on_rbtnAnnotationAndVideo_clicked();
    void on_rbtnAnnotationWithoutVideo_clicked();

private:
    Ui::ViewTypeFrameWidget *m_ui; ///< User interface pointer.
    AnnotationWithoutVideo m_annotation_without_video; ///< Annotation without video view type object.
    AnnotationAndVideo m_annotation_with_video; ///< Annotation and video view type object.
    OnlyVideo m_only_video; ///< Only video view type object.
};

#endif // VIEWTYPEFRAMEWIDGET_H
