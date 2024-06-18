#ifndef REGIONOFINTERESTDIALOG_H
#define REGIONOFINTERESTDIALOG_H

#include <QDialog>
#include "../FrameData/Config/Config.h"
#include "RegionOfInterestPainterScene.h"
#include "ConfigOfRegionOfInterestDialog.h"

namespace Ui {
class RegionOfInterestDialog;
}
/**
 * @class RegionOfInterestDialog
 * @brief The RegionOfInterestDialog class provides a dialog for configuring regions of interest on an image.
 *
 * This dialog allows users to add, configure, and delete regions of interest (ROIs) on a given background image.
 * It supports defining whether an ROI is inside or outside a certain area and saving these configurations for later use.
 */
class RegionOfInterestDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a RegionOfInterestDialog with a given configuration, background image, and parent widget.
     * @param a_current_config Pointer to the current configuration settings.
     * @param a_background_image Reference to the background image on which ROIs will be drawn.
     * @param a_parent Pointer to the parent widget, default is nullptr.
     */
    explicit RegionOfInterestDialog(Config* a_current_config, const Image &a_background_image, QWidget *a_parent = nullptr);
    ~RegionOfInterestDialog();

public slots:
    /**
     * @brief Slot to handle the completion of ROI configuration.
     */
    void slot_config_finished();

signals:
    /**
     * @brief Signal emitted to update the configuration after changes have been made.
     */
    void sig_update_config();
private:
    /**
     * @brief Gets the highest ID among the current regions of interest.
     * @return The highest ID as an integer.
     */
    int get_highest_id();

    /**
     * @brief Checks if the current interest is marked as inside.
     * @return True if the interest is inside, false otherwise.
     */
    bool is_interest_inside();

    /**
     * @brief Adds a given region of interest to the UI table.
     * @param a_region_to_add Pointer to the region of interest to add.
     */
    void add_region_to_table(RegionOfInterest *a_region_to_add);

    /**
     * @brief Creates and updates the polygon painter scene with the current regions of interest.
     */
    void create_q_polygon_painter();
private slots:
    /**
     * @brief Slot triggered by clicking the 'Add Region' button.
     *        Toggles the state between adding a new region and finalizing it.
     */
    void on_btnAddRegion_clicked();
    /**
     * @brief Slot for handling the selection indicating interest is inside.
     *        Sets all regions of interest to 'inside'.
     */
    void on_interestIsInside_clicked();

    /**
     * @brief Slot for handling the selection indicating interest is outside.
     *        Sets all regions of interest to 'outside'.
     */
    void on_interestIsOutside_clicked();

    /**
     * @brief Slot triggered by clicking the 'Delete' button for a region.
     *        Removes the selected region from the table and internal list.
     * @param a_row The row index in the table corresponding to the region to delete.
     */
    void on_Delete_clicked(int a_row);

    /**
     * @brief Slot triggered by clicking the 'Save' button.
     *        Saves the current configuration of regions of interest.
     */
    void on_btnSave_clicked();

    /**
     * @brief Slot triggered by clicking the generic 'Push' button (e.g., Cancel or Close).
     *        Closes the dialog without saving changes.
     */
    void on_pushButton_clicked();


private:
    const QString add_region = "Add Region";
    const QString new_region = "New Region";

    Ui::RegionOfInterestDialog *m_ui;
    bool m_is_adding = false;
    RegionOfInterestPainterScene* m_painter_scene;
    Config* m_current_config;
    ConfigOfRegionOfInterestDialog* m_config_dialog;
    RegionOfInterest* m_new_region;
    Image m_current_background_image;
    QVector<RegionOfInterest*>* m_roi_to_show;
};

#endif // REGIONOFINTERESTDIALOG_H
