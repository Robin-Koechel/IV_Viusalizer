#ifndef TWOSLIDERNAVIGATOR_H
#define TWOSLIDERNAVIGATOR_H

#include "VideoNavigator.h"
#include <QFrame>
#include "PainterSlider.h"

namespace Ui {
class TwoSliderNavigator;
}

/**
 * @class TwoSliderNavigator
 * @brief Provides dual-slider navigation controls for precise and coarse video scrubbing.
 *
 * Inherits from VideoNavigator to offer enhanced navigation capabilities with two sliders:
 * a fine slider for precise control within a defined time span, and a rough slider for broader
 * navigation across the entire video duration. This class allows for detailed control over video
 * playback position with the additional feature of span-specific navigation.
 */
class TwoSliderNavigator : public VideoNavigator
{
    Q_OBJECT

public:
    /**
     * Constructor with time navigation parameters.
     * @param a_time_to_navigate Total time available for navigation.
     * @param a_time_span_of_fine_slider Time span covered by the fine slider.
     * @param a_parent Parent widget.
     */
    explicit TwoSliderNavigator(uint64_t a_time_to_navigate, uint64_t a_time_span_of_fine_slider, QWidget *a_parent = nullptr);

    /**
     * Constructor with a specified time span for the fine slider.
     * @param a_time_span_of_fine_slider Time span covered by the fine slider.
     * @param a_parent Parent widget.
     */
    explicit TwoSliderNavigator(uint64_t a_time_span_of_fine_slider, QWidget *a_parent = nullptr);

    /**
     * Destructor.
     */
    ~TwoSliderNavigator();

    /**
     * Overrides VideoNavigator's method to change the current video position.
     * @param a_new_position The new position to navigate to in the video.
     */
    void change_current_position(uint64_t a_new_position) override;

    /**
     * Sets the total navigable time for the navigator.
     * @param a_new_time_to_navigate The new total time available for navigation.
     */
    void set_time_to_navigate(uint64_t a_new_time_to_navigate) override;

    /**
     * Retrieves the current position within the video.
     * @return The current video position.
     */
    uint64_t get_current_position() override;

    /**
     * @brief marks the positions
     * @param a_positions marks to mark
     * 
    */

    void set_marks(const QList<uint64_t> &a_positions);


protected:
    /**
     * Handles updates to the navigation position, adjusting UI elements accordingly.
     */
    void position_changed();

    /**
     * Sets the starting position of the fine slider for detailed navigation.
     * @param a_new_position New start position for the fine slider.
     */
    void change_start_position_of_fine_slider(uint64_t a_new_position);

    /**
     * Resets the sliders to their default positions.
     */
    void reset_slider();

    /**
     * Updates the navigable time span based on the video's duration or other criteria.
     */
    void update_time_to_navigate();

private slots:
    /**
     * Handles the event when the fine slider's selection is released.
     */
    void on_fineSlider_sliderReleased();

    /**
     * Handles the event when the rough slider's selection is released.
     */
    void on_roughSlider_sliderReleased();

    /**
     * Manages actions to be taken when the fine slider is pressed, such as preparation for fine navigation.
     */
    void on_fineSlider_sliderPressed();

    /**
     * Manages actions to be taken when the rough slider is pressed, typically involving broader navigational adjustments.
     */
    void on_roughSlider_sliderPressed();

private:
    Ui::TwoSliderNavigator *m_ui; ///< User interface for the TwoSliderNavigator.
    uint64_t m_start_position_of_fine_slider; ///< Start position of the fine slider for detailed navigation.
    uint64_t m_time_span_of_fine_slider; ///< Time span that the fine slider covers for precise control.
    bool m_slider_is_moving = false; ///< Flag to track if a slider is currently being moved.
    PainterSlider* m_paint_slider; ///< Slider where marks can be set
};

#endif // TWOSLIDERNAVIGATOR_H
