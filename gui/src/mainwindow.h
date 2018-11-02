#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QtWidgets>
#include <vector>
#include "stdlib.h"
#include <fstream>
#include "globalconstants.h"
#include "doubleslider.h"
#include "doublespinbox.h"
#include "doublependulum.h"
#include "drawingarea.h"
#include "grapharea.h"
#include "error.h"
class MainWindow : public QMainWindow
{
    /*This class is the main window, it creates and stores most objects
     * and handles interaction between the elements of the program.*/
    Q_OBJECT
public:
    MainWindow();
    void changeColorButtonColor();
private:
    bool m_isPlaying;
    double m_playbackSpeed;
    int m_frameCountSinceLastSpeedChange;
    double m_simulationTimeSinceLastSpeedChange;
    enum{m_sampleSize = 25};
    int m_frameCountModSampleSize;
    double m_timePerFrame[m_sampleSize];
    double m_averageFPS;
    bool m_isFirstCycle;

    DoublePendulum* m_currentPendulum;
    std::array<DoublePendulum*, Limitations::maxNumberOfPendula> m_pendula;
    unsigned short m_currentPendulumIndex; //Stores the index of the current pendulum

//tab widget
    QTabWidget* m_tabWidget;
//tabs
    //pendulum drawing
    DrawingArea* m_drawingArea;
    //graph drawing
    GraphArea* m_graphArea;
//actions
    QAction* m_rewindAction;
    QAction* m_playAction;
    QAction* m_pauseAction;
    QAction* m_stopAction;
    QAction* m_fastForwardAction;
    QAction* m_zoomInAction;
    QAction* m_zoomOutAction;
//parameter widgets
    QComboBox* m_pendulaComboBox;
    QPushButton* m_addPendulumPushButton;
    QPushButton* m_deleteCurrentPendulumPushButton;
    QPushButton* m_colorPushButton;
    bool m_deleteWarning;
    void connectPendulum(DoublePendulum* pendulum);
    void connectCurrentPendulumToWidgets(); /*private method, it must only be called when the first pendulum
                                            is created or when the current pendulum is changed, because
                                            the created connections must be unique.*/
    void deleteCurrentPendulum(); /*private method, it is not a slot, because we need
                                  the confirmation of the user before deleting the pendulum*/

    void switchWidgetsValues();

    QLabel* m_x1Label;
    DoubleSpinBox* m_x1DoubleSpinBox;
    DoubleSlider* m_x1Slider;
    QLabel* m_y1Label;
    DoubleSpinBox* m_y1DoubleSpinBox;
    DoubleSlider* m_y1Slider;

    QLabel* m_x2Label;
    DoubleSpinBox* m_x2DoubleSpinBox;
    DoubleSlider* m_x2Slider;
    QLabel* m_y2Label;
    DoubleSpinBox* m_y2DoubleSpinBox;
    DoubleSlider* m_y2Slider;

    QComboBox* m_angleComboBox;
    DoubleSpinBox* m_angleDoubleSpinBox;
    DoubleSpinBox* m_angleUncertaintyDoubleSpinBox;
    QComboBox* m_angleUnitComboBox;
    DoubleSlider* m_angleSlider;

    QComboBox* m_angVelComboBox;
    DoubleSpinBox* m_angVelDoubleSpinBox;
    DoubleSpinBox* m_angVelUncertaintyDoubleSpinBox;
    QComboBox* m_angVelUnitComboBox;
    DoubleSlider* m_angVelSlider;

    QComboBox* m_radiusComboBox;
    DoubleSpinBox* m_radiusDoubleSpinBox;
    DoubleSpinBox* m_radiusUncertaintyDoubleSpinBox;
    QComboBox* m_radiusUnitComboBox;
    DoubleSlider* m_radiusSlider;

    QComboBox* m_massComboBox;
    DoubleSpinBox* m_massDoubleSpinBox;
    DoubleSpinBox* m_massUncertaintyDoubleSpinBox;
    QComboBox* m_massUnitComboBox;
    DoubleSlider* m_massSlider;

    QLabel* m_gLabel;
    DoubleSpinBox* m_gDoubleSpinBox;
    DoubleSpinBox* m_gUncertaintyDoubleSpinBox;
    QComboBox* m_gUnitComboBox;
    DoubleSlider* m_gSlider;

    QLabel* m_dtLabel;
    DoubleSpinBox* m_dtDoubleSpinBox;
    QComboBox* m_dtUnitComboBox;
    DoubleSlider* m_dtSlider;

    QLabel* m_integrationMethodLabel;
    QComboBox* m_integrationMethodComboBox;

    QLabel* m_qualityLabel;
    QComboBox* m_qualityComboBox;

    QCheckBox* m_drawPathCheckBox;

    QLabel* m_xAxisLabel;
    QComboBox* m_xAxisComboBox;

    QLabel* m_yAxisLabel;
    QComboBox* m_yAxisComboBox;

    QCheckBox* m_autoScrollCheckBox;

    QCheckBox* m_exportDataCheckBox;
    QPushButton* m_chooseFilePushButton;
    QLabel* m_outputsPerSecondLabel;
    DoubleSpinBox* m_outputsPerSecondDoubleSpinBox;
    double m_outputsPerSecond;
    int m_outputCountSinceLastSpeedChange;
    int m_dataBlockCountSinceLastSpeedChange;
    QString m_lastDataDirectory;
    QString m_dataPath;
    std::ofstream m_dataStream;
    void openDataFile(); //private method, opens the chosen data file and prepares to write into it
    void closeDataFile(); //private method, closes the data file and deletes all objects related to it
    void warningNoFileChosen(); //private method, displays a qmessagebox telling the user that he must choose a file to export data
    bool m_writeData;
    bool m_fileIsChosen;
    bool m_fileIsOpened;

    QCheckBox* m_renderOffscreenCheckBox;
    bool m_renderOffscreen;

    QPushButton* m_clearPushButton;

    QLCDNumber* m_FPSlCDNumber;

    QLabel* m_playbackSpeedLabel;
    DoubleSpinBox* m_playbackSpeedDoubleSpinBox;

//other attributes
    std::array<unsigned short, ArraySize::currentIndex> m_currentIndex {{0}};
    std::array<unsigned short, ArraySize::currentUnit> m_currentUnit {{0}};
public slots:
//action slots
    void rewindSimulation();
    void playSimulation(bool isChecked = true);
    void pauseSimulation(bool isChecked = true);
    void stopSimulation();
    void fastForwardSimulation();
    void setPlaybackSpeed(double playbackSpeed);

    void zoomIn();
    void zoomOut();

//widget slots
    void displayColorDialog();
    void changeCurrentPendulum(int pendulumPlace);
    void disconnectCurrentPendulum();
    void createNewPendulum();
    void askToDeleteCurrentPendulum();
    void onTabChanged(int tabIndex);
    void switchParameter(QString newParameter);
    void changeAngUnit(int unit);
    void changeAngVelUnit(int unit);
    void changeRadiusUnit(int unit);
    void changeMassUnit(int unit);
    void changeGUnit(int unit);
    void changeDtUnit(int unit);
    void onExportDataClicked(bool isChecked);
    void onConfigureClicked();
    void onWarningButtonClicked(QAbstractButton* button);
    void setOutputsPerSecond(double outputsPerSecond);
    void adjustOutputsPerSecond(double dt);
    void setRenderOffscreen(bool isEnabled);
    void clearCurrentTab();
    void clearAll();
    void clearAll(int pendulumNumber);
    void clearCurrentPendulum();
    void updateDoubleSpinBoxes();
    void resetFPS();

//loop
    void startDrawing();



    void closeEvent(QCloseEvent*);
signals:
    void playbackSpeedChanged(double playbackSpeed);
};

#endif // MAINWINDOW_H
