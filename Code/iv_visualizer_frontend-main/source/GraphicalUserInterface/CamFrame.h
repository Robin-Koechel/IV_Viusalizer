#ifndef CAMFRAME_H
#define CAMFRAME_H

#include "../VideoPlayer/VideoPlayerWidget.h"
#include <QFrame>
#include <memory> // Include for smart pointers

namespace Ui {
class CamFrame; // Forward declaration of Ui::CamFrame
}

/**
 * @class CamFrame
 * @brief The CamFrame class extends QFrame to manage camera frame display and user interactions.
 *
 * This class is responsible for displaying video streams within the application, handling user interactions
 * such as mouse events, and managing configuration changes through a dedicated user interface. It utilizes
 * the VideoPlayerWidget for playing video streams and provides slots for handling clicks on various UI elements
 * like configuration, region of interest (ROI), and export options.
 */
class CamFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a CamFrame object with an optional parent widget.
     * 
     * Initializes the UI components and sets up actions and slots for toolbar buttons including
     * configuration, region of interest (ROI), and export. Sets the style and layout for the video player widget.
     *
     * @param parent The parent widget to this frame, defaulted to nullptr if not specified.
     */
    explicit CamFrame(QWidget* parent = nullptr);

    /**
     * @brief Destroys the CamFrame object, ensuring proper resource management.
     * 
     * Cleans up the UI components allocated during construction.
     */
    ~CamFrame();

    /**
     * @brief Changes the video stream displayed within the camera frame.
     * 
     * Updates the video player widget to display a new video stream and enables the camera frame.
     * 
     * @param new_stream Pointer to the new Stream object to be displayed.
     */
    void change_stream(Stream* a_new_stream);


    /**
     * @brief stops the showing stream
    */
    void stop_stream();

    /**
     * @brief checks if the stream gets showed 
     * @param a_stream_to_check the stream to check
     * @return true if the stream is showing, false  otherwise
    */
    bool is_showing_stream(Stream* a_stream_to_check);

private:
    /**
     * @brief disables every child
    */
    void disable_all_childs();

protected:
    /**
     * @brief Handles mouse press events on the camera frame.
     * 
     * Emits a signal when the camera frame is clicked, indicating a camera selection.
     * 
     * @param event Details of the mouse press event.
     */
    void mousePressEvent(QMouseEvent* event) override;

public slots:
    /**
     * @brief Slot to handle configuration changes.
     * 
     * Updates the video stream configuration based on user modifications from the configuration dialog.
     */
    void slot_config_changed();

    /**
     * @brief Slot to handle clicks on the configuration option.
     * 
     * Opens the configuration dialog for the current video stream.
     */
    void slot_on_config_clicked();

    /**
     * @brief Slot to handle clicks on the ROI (Region of Interest) option.
     * 
     * Opens the ROI dialog, allowing users to select and update regions of interest within the video stream.
     */
    void slot_on_roi_clicked();

    /**
     * @brief Slot to handle clicks on the export option.
     * 
     * Initiates the export process for the current video stream or selected region of interest.
     */
    void slot_on_export_clicked();

signals:
    /**
     * @brief Signal emitted when a camera is selected via mouse click on the camera frame.
     * @param a_selected_cam the cam that was selected
     */
    void sig_cam_selected(CamFrame* a_selected_cam);

private:
    const QString m_name_of_config_toolbar_entry = "Config"; ///< name of config in toolbar
    const QString m_name_of_region_of_interest_toolbar_entry = "Region Of Interest"; ///< name of region of interest in toolbar
    const QString m_name_of_export_toolbar_entry = "Export"; ///< name of export in toolbar
    const QString m_toolbar_stylesheet = "QToolBar {border: 1px solid black}"; ///< stylesheet of tool bar
    const QString m_video_player_stylesheet = "border: 1px solid black";    ///< stylesheet of video player
    std::unique_ptr<Ui::CamFrame> m_ui; ///< Smart pointer to the UI, ensuring automatic cleanup.
    VideoPlayerWidget m_video_player_widget; ///< Widget for playing video streams within the frame.
    Stream* m_current_stream; ///< Stream that is currently playing
};

#endif // CAMFRAME_H
