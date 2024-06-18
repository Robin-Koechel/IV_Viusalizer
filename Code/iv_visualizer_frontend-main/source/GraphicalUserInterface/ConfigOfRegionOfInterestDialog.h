#ifndef CONFIG_OF_REGION_OF_INTEREST_DIALOG_H
#define CONFIG_OF_REGION_OF_INTEREST_DIALOG_H

#include "../FrameData/Data/RegionOfInterest.h"
#include <QDialog>

namespace Ui {
class ConfigOfRegionOfInterestDialog;
}

/**
 * @class ConfigOfRegionOfInterestDialog
 * @brief Dialog for configuring properties of a RegionOfInterest.
 *
 * This class provides a user interface for adjusting the properties
 * of a given RegionOfInterest object, such as its opacity and name.
 * It emits a signal upon the completion of the configuration.
 */
class ConfigOfRegionOfInterestDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a configuration dialog for a RegionOfInterest.
     * @param a_to_configure Pointer to the RegionOfInterest to configure.
     * @param a_parent Pointer to the parent widget, default is nullptr.
     */
    explicit ConfigOfRegionOfInterestDialog(RegionOfInterest* a_to_configure, QWidget *a_parent = nullptr);
    
    /**
     * @brief Destructor.
     */
    ~ConfigOfRegionOfInterestDialog();

signals:
    /**
     * @brief Signal emitted when the configuration is finished.
     */
    void sig_config_finished();

private slots:
    /**
     * @brief Slot to handle the save button click event.
     *
     * Saves the current UI values to the RegionOfInterest object and emits sig_config_finished.
     */
    void on_btnSave_clicked();

private:
    Ui::ConfigOfRegionOfInterestDialog *m_ui; ///< Pointer to the UI components.
    RegionOfInterest* m_to_configure; ///< Pointer to the RegionOfInterest being configured.
};

#endif // CONFIG_OF_REGION_OF_INTEREST_DIALOG_H
