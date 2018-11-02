#include "mainwindow.h"

MainWindow::MainWindow(): m_isPlaying(false), m_playbackSpeed(1), m_frameCountSinceLastSpeedChange(1), m_simulationTimeSinceLastSpeedChange(0), m_deleteWarning(true), m_outputCountSinceLastSpeedChange(1), m_dataBlockCountSinceLastSpeedChange(1)
{
    setContextMenuPolicy(Qt::NoContextMenu);

    for(int i(0); i<ArraySize::currentIndex; i++)
    {
        m_currentIndex[i] = 0;
    }

    for(int i(0); i<ArraySize::currentUnit; i++)
    {
        m_currentUnit[i] = 0;
    }

    m_pendula[0] = new DoublePendulum(m_currentIndex, m_currentUnit, QString(TranslatedStrings::pendulum + " " + QString::number(1)), Color::pendulumColor[0]);
    m_currentPendulumIndex = 0;

    //creation of the tool bar and the actions which go into it
    QToolBar* toolBar = addToolBar(tr("tool bar"));

    m_rewindAction = toolBar->addAction(tr("rewind"));
    m_rewindAction->setIcon(QIcon(path+"Icons/rewind.png"));
    m_rewindAction->setCheckable(true);
    QObject::connect(m_rewindAction, SIGNAL(triggered(bool)), this, SLOT(rewindSimulation()));

    m_playAction = toolBar->addAction(tr("play"));
    m_playAction->setIcon(QIcon(path+"Icons/play.png"));
    m_playAction->setCheckable(true);
    m_playAction->setChecked(m_isPlaying);
    QObject::connect(m_playAction, SIGNAL(triggered(bool)), this, SLOT(playSimulation(bool)));

    m_pauseAction = toolBar->addAction(tr("pause"));
    m_pauseAction->setIcon(QIcon(path+"Icons/pause.png"));
    m_pauseAction->setCheckable(true);
    m_pauseAction->setChecked(!m_isPlaying);
    QObject::connect(m_pauseAction, SIGNAL(triggered(bool)), this, SLOT(pauseSimulation(bool)));

    m_stopAction = toolBar->addAction(tr("stop"));
    m_stopAction->setIcon(QIcon(path+"Icons/stop.png"));
    QObject::connect(m_stopAction, SIGNAL(triggered(bool)), this, SLOT(stopSimulation()));

    m_fastForwardAction = toolBar->addAction(tr("fast forward"));
    m_fastForwardAction->setIcon(QIcon(path+"Icons/fast forward.png"));
    m_fastForwardAction->setCheckable(true);
    QObject::connect(m_fastForwardAction, SIGNAL(triggered(bool)), this, SLOT(fastForwardSimulation()));

    m_zoomInAction = toolBar->addAction(tr("zoom in"));
    m_zoomInAction->setIcon(QIcon(path+"Icons/zoom in.png"));
    QObject::connect(m_zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));

    m_zoomOutAction = toolBar->addAction(tr("zoom out"));
    m_zoomOutAction->setIcon(QIcon(path+"Icons/zoom out.png"));
    QObject::connect(m_zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));



    m_tabWidget = new QTabWidget;
    QObject::connect(m_tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));

    //drawing tab
    m_drawingArea = new DrawingArea(m_pendula);

    //graph tab
    m_graphArea = new GraphArea(m_pendula, m_currentPendulumIndex, this, m_isPlaying);


    //manage pendula
    m_pendulaComboBox = new QComboBox;
    m_pendulaComboBox->addItem(TranslatedStrings::pendulum + QString(" ")+QString::number(m_currentPendulumIndex+1));
    m_addPendulumPushButton = new QPushButton(tr("add pendulum"));
    m_deleteCurrentPendulumPushButton = new QPushButton(tr("delete current pendulum"));
    m_deleteCurrentPendulumPushButton->setEnabled(false);
    m_colorPushButton = new QPushButton(QString());
    m_colorPushButton->setToolTip(tr("Color of the pendulum"));
    m_colorPushButton->setFixedSize(40, 22);
    m_colorPushButton->setAutoFillBackground(true);
    changeColorButtonColor();
    QObject::connect(m_pendulaComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCurrentPendulum(int)));
    QObject::connect(m_addPendulumPushButton, SIGNAL(clicked(bool)), this, SLOT(createNewPendulum()));
    QObject::connect(m_deleteCurrentPendulumPushButton, SIGNAL(clicked(bool)), this, SLOT(askToDeleteCurrentPendulum()));
    QObject::connect(m_colorPushButton, SIGNAL(clicked(bool)), this, SLOT(displayColorDialog()));

    //position
    //x1
    m_x1Label = new QLabel;
    m_x1Label->setText("x" + QString(SpecChar::indice1)+":");
    m_x1DoubleSpinBox = new DoubleSpinBox;
    m_x1DoubleSpinBox->setRange(-sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    m_x1DoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_x1DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosX1());
    m_x1DoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_x1Slider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->initPosX1(), -sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    //connection of the signals with the slots
    QObject::connect(m_x1DoubleSpinBox, SIGNAL(valueChanged(double)), m_x1Slider, SLOT(changeValue(double)));

    QObject::connect(m_x1Slider, SIGNAL(doubleValueChanged(double)), m_x1DoubleSpinBox, SLOT(setValue(double))); //connects the converted slider value with the double spin box

    //y1
    m_y1Label = new QLabel;
    m_y1Label->setText("y" + QString(SpecChar::indice1)+":");
    m_y1DoubleSpinBox = new DoubleSpinBox;
    m_y1DoubleSpinBox->setRange(-sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    m_y1DoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_y1DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosY1());
    m_y1DoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_y1Slider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->initPosY1(), -sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    //connection of the signals with the slots
    QObject::connect(m_y1DoubleSpinBox, SIGNAL(valueChanged(double)), m_y1Slider, SLOT(changeValue(double)));

    QObject::connect(m_y1Slider, SIGNAL(doubleValueChanged(double)), m_y1DoubleSpinBox, SLOT(setValue(double)));

    //x2
    m_x2Label = new QLabel;
    m_x2Label->setText("x" + QString(SpecChar::indice2)+":");
    m_x2DoubleSpinBox = new DoubleSpinBox;
    m_x2DoubleSpinBox->setRange(-sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    m_x2DoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_x2DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosX2());
    m_x2DoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_x2Slider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->initPosX2(), -sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    //connection of the signals with the slots
    QObject::connect(m_x2DoubleSpinBox, SIGNAL(valueChanged(double)), m_x2Slider, SLOT(changeValue(double)));

    QObject::connect(m_x2Slider, SIGNAL(doubleValueChanged(double)), m_x2DoubleSpinBox, SLOT(setValue(double)));

    //y2
    m_y2Label = new QLabel;
    m_y2Label->setText("y" + QString(SpecChar::indice2)+":");
    m_y2DoubleSpinBox = new DoubleSpinBox;
    m_y2DoubleSpinBox->setRange(-sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    m_y2DoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_y2DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosY2());
    m_y2DoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_y2Slider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->initPosY2(), -sqrt(DefaultParameterValue::maxRadius), sqrt(DefaultParameterValue::maxRadius));
    //connection of the signals with the slots
    QObject::connect(m_y2DoubleSpinBox, SIGNAL(valueChanged(double)), m_y2Slider, SLOT(changeValue(double)));

    QObject::connect(m_y2Slider, SIGNAL(doubleValueChanged(double)), m_y2DoubleSpinBox, SLOT(setValue(double)));


    //angle
    m_angleComboBox = new QComboBox;
    m_angleComboBox->addItem(QString(SpecChar::theta) + QString(SpecChar::indice1));
    m_angleComboBox->addItem(QString(SpecChar::theta) + QString(SpecChar::indice2));
    m_angleDoubleSpinBox = new DoubleSpinBox;
    m_angleDoubleSpinBox->setMinimum(-DefaultParameterValue::maxAng);
    m_angleDoubleSpinBox->setMaximum(DefaultParameterValue::maxAng);
    m_angleDoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_angleDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAng1()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
    m_angleDoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_angleUncertaintyDoubleSpinBox = new DoubleSpinBox; //Note: here and throughout the entire class, uncertainty means percent uncertainty
    m_angleUncertaintyDoubleSpinBox->setRange(0, DefaultParameterValue::maxRelativeUncertainty*100);
    m_angleUncertaintyDoubleSpinBox->setDecimals(DefaultParameterValue::uncertaintyDecimals-2);
    m_angleUncertaintyDoubleSpinBox->setSingleStep(DefaultParameterValue::uncertaintyStep);
    m_angleUncertaintyDoubleSpinBox->setPrefix(SpecChar::plusMinus);
    m_angleUncertaintyDoubleSpinBox->setValue(0);
    m_angleUncertaintyDoubleSpinBox->setSuffix(SpecChar::percent);
    m_angleUnitComboBox = new QComboBox;
    m_angleUnitComboBox->addItems(UnitList::unitsOfAngle); //adds the units (rad, deg, grad, tr)
    m_angleSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->measuredInitAng1(), -DefaultParameterValue::maxAng, DefaultParameterValue::maxAng);
    //connection of the signals with the slots
    QObject::connect(m_angleDoubleSpinBox, SIGNAL(valueChanged(double)), m_angleSlider, SLOT(changeValue(double)));

    QObject::connect(m_angleSlider, SIGNAL(doubleValueChanged(double)), m_angleDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_angleComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchParameter(QString)));

    QObject::connect(m_angleUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeAngUnit(int)));


    //angular velocity
    m_angVelComboBox = new QComboBox;
    m_angVelComboBox->addItems(QStringList() << QString(SpecChar::omega) + QString(SpecChar::indice1) << QString(SpecChar::omega)+QString(SpecChar::indice2)); //adds v1, v2, omega1, omega2
    m_angVelDoubleSpinBox = new DoubleSpinBox;
    m_angVelDoubleSpinBox->setMinimum(-DefaultParameterValue::defaultMax);
    m_angVelDoubleSpinBox->setMaximum(DefaultParameterValue::defaultMax);
    m_angVelDoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_angVelDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAngVel1()/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]]);
    m_angVelDoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_angVelUncertaintyDoubleSpinBox = new DoubleSpinBox; //Note: here and throughout the entire class, uncertainty means percent uncertainty
    m_angVelUncertaintyDoubleSpinBox->setRange(0, DefaultParameterValue::maxRelativeUncertainty*100);
    m_angVelUncertaintyDoubleSpinBox->setDecimals(DefaultParameterValue::uncertaintyDecimals-2);
    m_angVelUncertaintyDoubleSpinBox->setSingleStep(DefaultParameterValue::uncertaintyStep);
    m_angVelUncertaintyDoubleSpinBox->setPrefix(SpecChar::plusMinus);
    m_angVelUncertaintyDoubleSpinBox->setValue(0);
    m_angVelUncertaintyDoubleSpinBox->setSuffix(SpecChar::percent);
    m_angVelUnitComboBox = new QComboBox;
    m_angVelUnitComboBox->addItems(UnitList::unitsOfAngularVelocity);
    m_angVelSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->measuredInitAngVel1(), -DefaultParameterValue::defaultMax, DefaultParameterValue::defaultMax);
    //connection of the signals with the slots
    QObject::connect(m_angVelDoubleSpinBox, SIGNAL(valueChanged(double)), m_angVelSlider, SLOT(changeValue(double)));

    QObject::connect(m_angVelSlider, SIGNAL(doubleValueChanged(double)), m_angVelDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_angVelComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchParameter(QString)));

    QObject::connect(m_angVelUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeAngVelUnit(int)));


    //radius (=length of the rod)
    m_radiusComboBox = new QComboBox;
    m_radiusComboBox->addItem("r" + QString(SpecChar::indice1));
    m_radiusComboBox->addItem("r" + QString(SpecChar::indice2));
    m_radiusDoubleSpinBox = new DoubleSpinBox;
    m_radiusDoubleSpinBox->setMinimum(DefaultParameterValue::minRadius);
    m_radiusDoubleSpinBox->setMaximum(DefaultParameterValue::maxRadius);
    m_radiusDoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_radiusDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredR1()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
    m_radiusDoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_radiusUncertaintyDoubleSpinBox = new DoubleSpinBox; //Note: here and throughout the entire class, uncertainty means percent uncertainty
    m_radiusUncertaintyDoubleSpinBox->setRange(0, DefaultParameterValue::maxRelativeUncertainty*100);
    m_radiusUncertaintyDoubleSpinBox->setDecimals(DefaultParameterValue::uncertaintyDecimals-2);
    m_radiusUncertaintyDoubleSpinBox->setSingleStep(DefaultParameterValue::uncertaintyStep);
    m_radiusUncertaintyDoubleSpinBox->setPrefix(SpecChar::plusMinus);
    m_radiusUncertaintyDoubleSpinBox->setValue(0);
    m_radiusUncertaintyDoubleSpinBox->setSuffix(SpecChar::percent);
    m_radiusUnitComboBox = new QComboBox;
    m_radiusUnitComboBox->addItems(UnitList::unitsOfLength);
    m_radiusSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->measuredR1(), DefaultParameterValue::minRadius, DefaultParameterValue::maxRadius);
    //connection of the signals with the slots
    QObject::connect(m_radiusDoubleSpinBox, SIGNAL(valueChanged(double)), m_radiusSlider, SLOT(changeValue(double)));

    QObject::connect(m_radiusSlider, SIGNAL(doubleValueChanged(double)), m_radiusDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_radiusComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchParameter(QString)));

    QObject::connect(m_radiusUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeRadiusUnit(int)));

    //mass
    m_massComboBox = new QComboBox;
    m_massComboBox->addItem("m" + QString(SpecChar::indice1));
    m_massComboBox->addItem("m" + QString(SpecChar::indice2));
    m_massDoubleSpinBox = new DoubleSpinBox;
    m_massDoubleSpinBox->setMinimum(DefaultParameterValue::defaultMin);
    m_massDoubleSpinBox->setMaximum(DefaultParameterValue::defaultMax);
    m_massDoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_massDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredM1()/UnitConversion::toKg[m_currentUnit[UnitOf::mass]]);
    m_massDoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_massUncertaintyDoubleSpinBox = new DoubleSpinBox; //Note: here and throughout the entire class, uncertainty means percent uncertainty
    m_massUncertaintyDoubleSpinBox->setRange(0, DefaultParameterValue::maxRelativeUncertainty*100);
    m_massUncertaintyDoubleSpinBox->setDecimals(DefaultParameterValue::uncertaintyDecimals-2);
    m_massUncertaintyDoubleSpinBox->setSingleStep(DefaultParameterValue::uncertaintyStep);
    m_massUncertaintyDoubleSpinBox->setPrefix(SpecChar::plusMinus);
    m_massUncertaintyDoubleSpinBox->setValue(0);
    m_massUncertaintyDoubleSpinBox->setSuffix(SpecChar::percent);
    m_massUnitComboBox = new QComboBox;
    m_massUnitComboBox->addItems(UnitList::unitsOfMass);
    m_massSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->measuredM1(), DefaultParameterValue::defaultMin, DefaultParameterValue::defaultMax);
    //connection of the signals with the slots
    QObject::connect(m_massDoubleSpinBox, SIGNAL(valueChanged(double)), m_massSlider, SLOT(changeValue(double)));

    QObject::connect(m_massSlider, SIGNAL(doubleValueChanged(double)), m_massDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_massComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(switchParameter(QString)));

    QObject::connect(m_massUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeMassUnit(int)));


    //gravity
    m_gLabel = new QLabel("g:");
    m_gDoubleSpinBox = new DoubleSpinBox;
    m_gDoubleSpinBox->setMinimum(0);
    m_gDoubleSpinBox->setMaximum(DefaultParameterValue::defaultMax);
    m_gDoubleSpinBox->setDecimals(DefaultParameterValue::defaultDecimals);
    m_gDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredG()/UnitConversion::toMps2[m_currentUnit[UnitOf::acc]]);
    m_gDoubleSpinBox->setSingleStep(DefaultParameterValue::defaultStep);
    m_gUncertaintyDoubleSpinBox = new DoubleSpinBox; //Note: here and throughout the entire class, uncertainty means percent uncertainty
    m_gUncertaintyDoubleSpinBox->setRange(0, DefaultParameterValue::maxRelativeUncertainty*100);
    m_gUncertaintyDoubleSpinBox->setDecimals(DefaultParameterValue::uncertaintyDecimals-2);
    m_gUncertaintyDoubleSpinBox->setSingleStep(DefaultParameterValue::uncertaintyStep);
    m_gUncertaintyDoubleSpinBox->setPrefix(SpecChar::plusMinus);
    m_gUncertaintyDoubleSpinBox->setValue(0);
    m_gUncertaintyDoubleSpinBox->setSuffix(SpecChar::percent);
    m_gUnitComboBox = new QComboBox;
    m_gUnitComboBox->addItems(UnitList::unitsOfAcceleration);
    m_gSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->measuredG(), 0, DefaultParameterValue::defaultMax);
    //connection of the signals with the slots
    QObject::connect(m_gDoubleSpinBox, SIGNAL(valueChanged(double)), m_gSlider, SLOT(changeValue(double)));

    QObject::connect(m_gSlider, SIGNAL(doubleValueChanged(double)), m_gDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_gUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeGUnit(int)));


    //time interval
    m_dtLabel = new QLabel("dt:");
    m_dtDoubleSpinBox = new DoubleSpinBox;
    m_dtDoubleSpinBox->setMinimum(DefaultParameterValue::minDt);
    m_dtDoubleSpinBox->setMaximum(DefaultParameterValue::maxDt);
    m_dtDoubleSpinBox->setDecimals(DefaultParameterValue::dtDecimals);
    m_dtDoubleSpinBox->setSingleStep(0.00001);
    m_dtDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->dt()/UnitConversion::toS[m_currentUnit[UnitOf::time]]);
    m_dtDoubleSpinBox->setSingleStep(DefaultParameterValue::dtStep);
    m_dtUnitComboBox = new QComboBox;
    m_dtUnitComboBox->addItems(UnitList::unitsOfTime);
    m_dtSlider = new DoubleSlider(m_pendula[m_currentPendulumIndex]->dt(), DefaultParameterValue::minDt, DefaultParameterValue::maxDt);
    //connection of the signals with the slots
    QObject::connect(m_dtDoubleSpinBox, SIGNAL(valueChanged(double)), m_dtSlider, SLOT(changeValue(double)));

    QObject::connect(m_dtSlider, SIGNAL(doubleValueChanged(double)), m_dtDoubleSpinBox, SLOT(setValue(double)));

    QObject::connect(m_dtUnitComboBox, SIGNAL(currentIndexChanged(int)), this,  SLOT(changeDtUnit(int)));

    //now that the dt widgets have been created, we can connect the pendulum
    connectPendulum(m_pendula[0]); //Note: the other connections will be created after



    //integration method
    m_integrationMethodLabel = new QLabel(tr("Integration method:"));
    m_integrationMethodComboBox = new QComboBox;
    m_integrationMethodComboBox->addItems(QStringList() << tr("Modified Euler") << tr("Störmer-Verlet") << tr("RK4") << tr("Modified RK4") << tr("Leapfrog") << tr("4th-order Ruth"));


    //connection of the widgets with the pendulum
    connectCurrentPendulumToWidgets();


    //quality
    m_qualityLabel = new QLabel(tr("Render mode:"));
    m_qualityComboBox = new QComboBox;
    m_qualityComboBox->addItems(QStringList() << tr("High performance") << tr("Balanced") << tr("High Quality"));
    QObject::connect(m_qualityComboBox, SIGNAL(currentIndexChanged(int)), m_drawingArea, SLOT(setQualityLevel(int)));
    QObject::connect(m_qualityComboBox, SIGNAL(currentIndexChanged(int)), m_graphArea, SLOT(setQualityLevel(int)));
    m_qualityComboBox->setCurrentIndex(Quality::balanced);


    //draw path
    m_drawPathCheckBox = new QCheckBox(tr("draw path"));
    QObject::connect(m_drawPathCheckBox, SIGNAL(toggled(bool)), m_drawingArea, SLOT(setDrawPath(bool)));

    //graph parameters
    m_xAxisLabel = new QLabel(tr("x-axis parameter:"));
    m_xAxisLabel->setEnabled(false);
    m_xAxisComboBox = new QComboBox;
    m_xAxisComboBox->setEnabled(false);
    m_xAxisComboBox->addItems(Parameter::namesList);
    QObject::connect(m_xAxisComboBox, SIGNAL(currentIndexChanged(int)), m_graphArea, SLOT(setXParameter(int)));
    m_xAxisComboBox->setCurrentIndex(Parameter::time);
    m_graphArea->setXParameter(m_xAxisComboBox->currentIndex()); //I have to do that manually, because Parameter::time == 0, thus the combobox doesn't emit the signal currentIndexChanged(int)

    m_yAxisLabel = new QLabel(tr("y-axis parameter:"));
    m_yAxisLabel->setEnabled(false);
    m_yAxisComboBox = new QComboBox;
    m_yAxisComboBox->setEnabled(false);
    m_yAxisComboBox->addItems(Parameter::namesList);
    QObject::connect(m_yAxisComboBox, SIGNAL(currentIndexChanged(int)), m_graphArea, SLOT(setYParameter(int)));
    m_yAxisComboBox->setCurrentIndex(Parameter::ang1);

    //auto scroll
    m_autoScrollCheckBox = new QCheckBox(tr("auto scroll"));
    m_autoScrollCheckBox->setChecked(true);
    m_autoScrollCheckBox->setEnabled(false);
    QObject::connect(m_autoScrollCheckBox, SIGNAL(toggled(bool)), m_graphArea, SLOT(setAutoScroll(bool)));

    //export data in a file
    m_exportDataCheckBox = new QCheckBox(tr("Export data"));
    m_exportDataCheckBox->setChecked(false);
    m_chooseFilePushButton = new QPushButton(TranslatedStrings::chooseFile);
    m_chooseFilePushButton->setEnabled(false);
    m_outputsPerSecondLabel = new QLabel(tr("Outputs per second:"));
    m_outputsPerSecondLabel->setEnabled(false);
    m_outputsPerSecondDoubleSpinBox = new DoubleSpinBox;
    m_outputsPerSecondDoubleSpinBox->setDecimals(1);
    m_outputsPerSecondDoubleSpinBox->setRange(0.1, 1000);
    m_outputsPerSecondDoubleSpinBox->setValue(Screen::refreshRate);
    m_outputsPerSecondDoubleSpinBox->setEnabled(false);
    m_outputsPerSecond = m_outputsPerSecondDoubleSpinBox->value();
    m_writeData = false;
    m_fileIsChosen = false;
    m_fileIsOpened = false;
    QObject::connect(m_exportDataCheckBox, SIGNAL(toggled(bool)), m_chooseFilePushButton, SLOT(setEnabled(bool)));
    QObject::connect(m_exportDataCheckBox, SIGNAL(toggled(bool)), m_outputsPerSecondLabel, SLOT(setEnabled(bool)));
    QObject::connect(m_exportDataCheckBox, SIGNAL(toggled(bool)), m_outputsPerSecondDoubleSpinBox, SLOT(setEnabled(bool)));
    QObject::connect(m_exportDataCheckBox, SIGNAL(toggled(bool)), this, SLOT(onExportDataClicked(bool)));
    QObject::connect(m_chooseFilePushButton, SIGNAL(clicked(bool)), this, SLOT(onConfigureClicked()));
    QObject::connect(m_outputsPerSecondDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setOutputsPerSecond(double)));

    //render offscreen
    m_renderOffscreenCheckBox = new QCheckBox(tr("Offscreen rendering"));
    m_renderOffscreenCheckBox->setToolTip(tr("Renders both the simulation and the graph, at the cost of performance."));
    m_renderOffscreenCheckBox->setChecked(true);
    QObject::connect(m_renderOffscreenCheckBox, SIGNAL(toggled(bool)), this, SLOT(setRenderOffscreen(bool)));

    //playback speed
    QString playBackToolTip(tr("This box allows you to choose at which speed the simulation plays. You can also use the fast forward and rewind buttons."));
    m_playbackSpeedLabel = new QLabel(tr("Playback speed:"));
    m_playbackSpeedLabel->setToolTip(playBackToolTip);
    m_playbackSpeedDoubleSpinBox = new DoubleSpinBox;
    m_playbackSpeedDoubleSpinBox->setToolTip(playBackToolTip);
    m_playbackSpeedDoubleSpinBox->setDecimals(2);
    m_playbackSpeedDoubleSpinBox->setSingleStep(0.05);
    m_playbackSpeedDoubleSpinBox->setRange(-20, 20);
    m_playbackSpeedDoubleSpinBox->setValue(1);
    QObject::connect(m_playbackSpeedDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setPlaybackSpeed(double)));
    QObject::connect(this, SIGNAL(playbackSpeedChanged(double)), m_playbackSpeedDoubleSpinBox, SLOT(setValue(double)));

    //clear
    m_clearPushButton = new QPushButton(tr("clear"));
    QObject::connect(m_clearPushButton, SIGNAL(clicked(bool)), this, SLOT(clearCurrentTab()));


    //now that all the connections are established we can safely interact with the widgets
    m_integrationMethodComboBox->setCurrentIndex(DoublePendulum::IntegrationMethod::Leapfrog);

    //adds everything to a layout
    QHBoxLayout* pendulaLayout = new QHBoxLayout;
    pendulaLayout->addWidget(m_pendulaComboBox, 2);
    pendulaLayout->addWidget(m_colorPushButton, 1);
    pendulaLayout->addWidget(m_addPendulumPushButton, 1);
    pendulaLayout->addWidget(m_deleteCurrentPendulumPushButton, 1);

    QGridLayout* pendulumParametersLayout = new QGridLayout;

    pendulumParametersLayout->addWidget(m_x1Label, 0, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_x1DoubleSpinBox, 0, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_x1Slider, 0, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_y1Label, 0, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_y1DoubleSpinBox, 0, 4, 1, 1);
    pendulumParametersLayout->addWidget(m_y1Slider, 0, 5, 1, 1);

    pendulumParametersLayout->addWidget(m_x2Label, 1, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_x2DoubleSpinBox, 1, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_x2Slider, 1, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_y2Label, 1, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_y2DoubleSpinBox, 1, 4, 1, 1);
    pendulumParametersLayout->addWidget(m_y2Slider, 1, 5, 1, 1);

    pendulumParametersLayout->addWidget(m_angleComboBox, 3, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_angleDoubleSpinBox, 3, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_angleUncertaintyDoubleSpinBox, 3, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_angleUnitComboBox, 3, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_angleSlider, 3, 4, 1, 2);

    pendulumParametersLayout->addWidget(m_angVelComboBox, 4, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_angVelDoubleSpinBox, 4, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_angVelUncertaintyDoubleSpinBox, 4, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_angVelUnitComboBox, 4, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_angVelSlider, 4, 4, 1, 2);

    pendulumParametersLayout->addWidget(m_radiusComboBox, 5, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_radiusDoubleSpinBox, 5, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_radiusUncertaintyDoubleSpinBox, 5, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_radiusUnitComboBox, 5, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_radiusSlider, 5, 4, 1, 2);

    pendulumParametersLayout->addWidget(m_massComboBox, 6, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_massDoubleSpinBox,6, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_massUncertaintyDoubleSpinBox, 6, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_massUnitComboBox, 6, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_massSlider, 6, 4, 1, 2);

    pendulumParametersLayout->addWidget(m_gLabel, 7, 0, 1, 1);
    pendulumParametersLayout->addWidget(m_gDoubleSpinBox, 7, 1, 1, 1);
    pendulumParametersLayout->addWidget(m_gUncertaintyDoubleSpinBox, 7, 2, 1, 1);
    pendulumParametersLayout->addWidget(m_gUnitComboBox, 7, 3, 1, 1);
    pendulumParametersLayout->addWidget(m_gSlider, 7, 4, 1, 2);

    pendulumParametersLayout->addWidget(m_integrationMethodLabel, 9, 0, 1, 2);
    pendulumParametersLayout->addWidget(m_integrationMethodComboBox, 9, 2, 1, 2);

    QGroupBox* pendulumParametersGroupBox = new QGroupBox(tr("Current pendulum parameters"));
    pendulumParametersGroupBox->setLayout(pendulumParametersLayout);


    QGridLayout* renderingOptionsLayout = new QGridLayout;
    renderingOptionsLayout->addWidget(m_qualityLabel, 0, 0, 1, 1);
    renderingOptionsLayout->addWidget(m_qualityComboBox, 0, 1, 1, 1);
    renderingOptionsLayout->addWidget(m_renderOffscreenCheckBox, 1, 0, 1, 2);

    QGroupBox* renderingOptionsGroupBox = new QGroupBox(tr("Rendering options"));
    renderingOptionsGroupBox->setLayout(renderingOptionsLayout);


    QGridLayout* dataExportationLayout = new QGridLayout;
    dataExportationLayout->addWidget(m_exportDataCheckBox, 0, 0, 1, 2);
    dataExportationLayout->addWidget(m_chooseFilePushButton, 0, 2, 1, 2);
    dataExportationLayout->addWidget(m_outputsPerSecondLabel, 1, 0, 1, 2);
    dataExportationLayout->addWidget(m_outputsPerSecondDoubleSpinBox, 1, 2, 1, 1);

    QGroupBox* dataExportationGroupBox = new QGroupBox(tr("Data exportation"));
    dataExportationGroupBox->setLayout(dataExportationLayout);

    QGridLayout* bottomLayout = new QGridLayout;

    bottomLayout->addWidget(m_dtLabel, 0, 0, 1, 1);
    bottomLayout->addWidget(m_dtDoubleSpinBox, 0, 1, 1, 2);
    bottomLayout->addWidget(m_dtUnitComboBox, 0, 3, 1, 1);
    bottomLayout->addWidget(m_dtSlider, 0, 4, 1, 3);

    bottomLayout->addWidget(m_drawPathCheckBox, 2, 0, 1, 2);
    bottomLayout->addWidget(m_autoScrollCheckBox, 2, 2, 1, 2);

    bottomLayout->addWidget(renderingOptionsGroupBox, 2, 5, 3, 2);

    bottomLayout->addWidget(m_xAxisLabel, 3, 0, 1, 2);
    bottomLayout->addWidget(m_xAxisComboBox, 3, 2, 1, 2);

    bottomLayout->addWidget(m_yAxisLabel, 4, 0, 1, 2);
    bottomLayout->addWidget(m_yAxisComboBox, 4, 2, 1, 2);

    bottomLayout->addWidget(m_playbackSpeedLabel, 5, 0, 1, 2);
    bottomLayout->addWidget(m_playbackSpeedDoubleSpinBox, 5, 2, 1, 1);

    bottomLayout->addWidget(dataExportationGroupBox, 5, 4, 2, 3);

    bottomLayout->addWidget(m_clearPushButton, 6, 0, 1, 2);


    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addLayout(pendulaLayout);
    rightLayout->addWidget(pendulumParametersGroupBox);
    rightLayout->addLayout(bottomLayout);

    //----------------------------------------------------------------------------

    //FPS
    m_FPSlCDNumber = new QLCDNumber(5, m_drawingArea);
    m_FPSlCDNumber->setToolTip(tr("FPS Counter"));
    m_FPSlCDNumber->setSegmentStyle(QLCDNumber::Flat);
    m_FPSlCDNumber->setAutoFillBackground(true);
    m_FPSlCDNumber->setBackgroundRole(QPalette::Base);
    m_FPSlCDNumber->display(QString("0.0"));
#ifndef __linux__
    QObject::connect(m_drawingArea, SIGNAL(viewportUpdated()), m_FPSlCDNumber, SLOT(raise())); //prevents the QOpenGLWidget of the drawing area from hidind the FPS counter
    QObject::connect(m_graphArea, SIGNAL(viewportUpdated()), m_FPSlCDNumber, SLOT(raise()));
#endif


    //adds tabs to the tab widget
    m_tabWidget->addTab(m_drawingArea, tr("simulation"));
    m_tabWidget->addTab(m_graphArea, tr("graph"));


    QWidget *rightWidget = new QWidget;
    rightWidget->setLayout(rightLayout);

    QDockWidget* dock = new QDockWidget;
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dock->setWidget(rightWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock, Qt::Vertical);
    rightWidget->show();

    //sets the central widget
    setCentralWidget(m_tabWidget);


    //fits window to screen
    showMaximized();
    setMinimumSize(width(), height());


    //displays the current pendulum
    m_drawingArea->refresh();
}

void MainWindow::connectPendulum(DoublePendulum* pendulum)
{
    QObject::connect(pendulum, SIGNAL(updateSignal()), m_drawingArea, SLOT(refresh()));
    QObject::connect(pendulum, SIGNAL(integrationMethodChanged(int)), this, SLOT(resetFPS()));
    QObject::connect(m_dtDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeDt(double)));
    QObject::connect(pendulum, SIGNAL(dtValueChanged(double)), this, SLOT(resetFPS()));
}

void MainWindow::connectCurrentPendulumToWidgets()
{
    DoublePendulum* pendulum = m_pendula[m_currentPendulumIndex];

    QObject::connect(pendulum, SIGNAL(updateSignal()), this, SLOT(updateDoubleSpinBoxes()));

    QObject::connect(pendulum, SIGNAL(restarted()), this, SLOT(clearCurrentPendulum()));

    QObject::connect(m_x1DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosX1(double)));
    QObject::connect(m_y1DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosY1(double)));
    QObject::connect(m_x2DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosX2(double)));
    QObject::connect(m_y2DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosY2(double)));

    QObject::connect(m_angleDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeAngle(double)));
    QObject::connect(m_angleUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setAngleUncertainty(double)));

    QObject::connect(m_angVelDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeAngularVelocity(double)));
    QObject::connect(m_angVelUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setAngularVelocityUncertainty(double)));

    QObject::connect(m_radiusDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeRadius(double)));
    QObject::connect(m_radiusUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setRadiusUncertainty(double)));

    QObject::connect(m_massDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeMass(double)));
    QObject::connect(m_massUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setMassUncertainty(double)));

    QObject::connect(m_gDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeGravity(double)));
    QObject::connect(m_gUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setGravityUncertainty(double)));

    QObject::connect(m_integrationMethodComboBox, SIGNAL(currentIndexChanged(int)), pendulum, SLOT(setIntegrationMethod(int)));

    QObject::connect(pendulum, SIGNAL(dtValueChanged(double)), this, SLOT(adjustOutputsPerSecond(double)));
}

//action slots
void MainWindow::rewindSimulation()
{
        m_playAction->setChecked(false);
        m_pauseAction->setChecked(false);
        m_fastForwardAction->setChecked(false);
        m_rewindAction->setChecked(true);

        if (m_pendula[m_currentPendulumIndex]->dt() > 0)
        {
            setPlaybackSpeed(-1);
        }
        else
        {
            setPlaybackSpeed(m_playbackSpeed-0.3);
        }

        if(!m_isPlaying)
        {
            startDrawing();
        }
}

void MainWindow::playSimulation(bool isChecked)
{
    double precisionFactor(pow(10, m_playbackSpeedDoubleSpinBox->decimals()+1));
    if(isChecked)
    {
        m_rewindAction->setChecked(false);
        m_pauseAction->setChecked(false);
        m_fastForwardAction->setChecked(false);

        if(m_isPlaying)
        {
           setPlaybackSpeed(1);
        }
        else
        {
            startDrawing();
        }
    }
    else
    {
        if(int(round(m_playbackSpeed*precisionFactor)) == int(round(1.0*precisionFactor)))
        {
            m_pauseAction->trigger();
        }
        else
        {
            m_playAction->setChecked(true);
            setPlaybackSpeed(1);
        }
    }
}
void MainWindow::pauseSimulation(bool isChecked)
{
    if(isChecked)
    {
        m_rewindAction->setChecked(false);
        m_playAction->setChecked(false);
        m_fastForwardAction->setChecked(false);

        m_isPlaying=false;
    }
    else
    {
        m_playAction->trigger();
    }
}
void MainWindow::stopSimulation()
{
    m_rewindAction->setChecked(false);
    m_playAction->setChecked(false);
    m_pauseAction->setChecked(true);
    m_fastForwardAction->setChecked(false);

    m_isPlaying=false;
    setPlaybackSpeed(1);

    for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
    {
        m_pendula[i]->restart();
    }
    clearAll();
}
void MainWindow::fastForwardSimulation()
{
    m_rewindAction->setChecked(false);
    m_playAction->setChecked(false);
    m_pauseAction->setChecked(false);
    m_fastForwardAction->setChecked(true);

    if (m_pendula[m_currentPendulumIndex]->dt() < 0)
    {
        m_pendula[m_currentPendulumIndex]->setDt(fabs(m_pendula[m_currentPendulumIndex]->dt()));
        setPlaybackSpeed(1);
    }
    setPlaybackSpeed(m_playbackSpeed+0.3);

    if(!m_isPlaying)
    {
        startDrawing();
    }
}

void MainWindow::setPlaybackSpeed(double playbackSpeed)
{
    double precisionFactor(pow(10, m_playbackSpeedDoubleSpinBox->decimals()+1));

    if(int(round(m_playbackSpeed*precisionFactor)) != int(round(playbackSpeed*precisionFactor)))
    {
        if(int(round(playbackSpeed*precisionFactor)) != 0)
        {
            m_playbackSpeed = playbackSpeed;
        }
        if(round(playbackSpeed*precisionFactor)/precisionFactor > 0)
        {
            for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
            {
                m_pendula[i]->setDt(fabs(m_pendula[i]->dt()));
            }
        }
        else if(round(playbackSpeed*precisionFactor)/precisionFactor < 0)
        {
            for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
            {
                m_pendula[i]->setDt(-fabs(m_pendula[i]->dt()));
            }
        }

        if(round(playbackSpeed*precisionFactor)/precisionFactor <= 1.0 && round(playbackSpeed*precisionFactor)/precisionFactor > 0)
        {
            for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
            {
                m_pendula[i]->setDt(fabs(m_pendula[i]->dt()));
            }
            if(m_isPlaying)
            {
                m_rewindAction->setChecked(false);
                m_pauseAction->setChecked(false);
                m_fastForwardAction->setChecked(false);
                m_playAction->setChecked(true);
            }
        }
        else if(int(round(playbackSpeed*precisionFactor)) == 0)
        {
            if(m_isPlaying)
            {
                m_pauseAction->trigger();
            }
        }
        else if(round(playbackSpeed*precisionFactor)/precisionFactor > 1.0)
        {
            for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
            {
                m_pendula[i]->setDt(fabs(m_pendula[i]->dt()));
            }
            if(m_isPlaying)
            {
                m_rewindAction->setChecked(false);
                m_playAction->setChecked(false);
                m_pauseAction->setChecked(false);
                m_fastForwardAction->setChecked(true);
            }
        }
        else
        {
            for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
            {
                m_pendula[i]->setDt(-fabs(m_pendula[i]->dt()));
            }
            if(m_isPlaying)
            {
                m_fastForwardAction->setChecked(false);
                m_playAction->setChecked(false);
                m_pauseAction->setChecked(false);
                m_rewindAction->setChecked(true);
            }
        }
    }
    if(int(round(m_playbackSpeed*precisionFactor)) != 0)
    {
        m_outputCountSinceLastSpeedChange = 1;
        m_dataBlockCountSinceLastSpeedChange = 1;
        m_frameCountSinceLastSpeedChange = 1;
        m_simulationTimeSinceLastSpeedChange = 0;
    }
    emit playbackSpeedChanged(m_playbackSpeed);
}


void MainWindow::zoomIn()
{
    if(m_tabWidget->currentIndex()==0)
    {
        m_drawingArea->scale(1+5.0/100.0, 1+5.0/100.0);
    }
    else
    {
        m_graphArea->zoom(1+5.0/100.0);
    }
}
void MainWindow::zoomOut()
{
    if(m_tabWidget->currentIndex()==0)
    {
        m_drawingArea->scale(1-5.0/100.0, 1-5.0/100.0);
    }
    else
    {
        m_graphArea->zoom(1-5.0/100.0);
    }
}

void MainWindow::displayColorDialog()
{
    QColorDialog dialog(m_pendula[m_currentPendulumIndex]->color(), this);
    dialog.setOption(QColorDialog::ShowAlphaChannel);
    if(dialog.exec() == QDialog::Accepted)
    {
        m_pendula[m_currentPendulumIndex]->setColor(dialog.selectedColor());
        m_drawingArea->changeColor(m_currentPendulumIndex);
        m_graphArea->changeColor(m_currentPendulumIndex);
        changeColorButtonColor();
    }
}

void MainWindow::changeColorButtonColor()
{
    QColor color(m_pendula[m_currentPendulumIndex]->color());
    m_colorPushButton->setStyleSheet(QString("border: none;"
                                            "background-color: rgba(%1, %2, %3, %4);").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

void MainWindow::changeCurrentPendulum(int pendulumPlace)
{
    if(pendulumPlace == m_currentPendulumIndex)  //not needed, but the function is pointeless in that case, so it returns
    {
        return;
    }
    disconnectCurrentPendulum();
    m_currentPendulumIndex = pendulumPlace;
    switchWidgetsValues();
    connectCurrentPendulumToWidgets();
}

void MainWindow::disconnectCurrentPendulum()
{
    DoublePendulum* pendulum = m_pendula[m_currentPendulumIndex];
    QObject::disconnect(pendulum, SIGNAL(updateSignal()), this, SLOT(updateDoubleSpinBoxes()));

    QObject::disconnect(m_x1DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosX1(double)));
    QObject::disconnect(m_y1DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosY1(double)));
    QObject::disconnect(m_x2DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosX2(double)));
    QObject::disconnect(m_y2DoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setInitPosY2(double)));

    QObject::disconnect(m_angleDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeAngle(double)));
    QObject::disconnect(m_angleUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setAngleUncertainty(double)));

    QObject::disconnect(m_angVelDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeAngularVelocity(double)));
    QObject::disconnect(m_angVelUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setAngularVelocityUncertainty(double)));

    QObject::disconnect(m_radiusDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeRadius(double)));
    QObject::disconnect(m_radiusUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setRadiusUncertainty(double)));

    QObject::disconnect(m_massDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeMass(double)));
    QObject::disconnect(m_massUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setMassUncertainty(double)));

    QObject::disconnect(m_gDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(changeGravity(double)));
    QObject::disconnect(m_gUncertaintyDoubleSpinBox, SIGNAL(valueChanged(double)), pendulum, SLOT(setGravityUncertainty(double)));

    QObject::disconnect(m_integrationMethodComboBox, SIGNAL(currentIndexChanged(int)), pendulum, SLOT(setIntegrationMethod(int)));

    QObject::disconnect(pendulum, SIGNAL(dtValueChanged(double)), this, SLOT(adjustOutputsPerSecond(double)));
}

void MainWindow::createNewPendulum()
{
    unsigned short numberOfPendula(DoublePendulum::numberOfPendula()); //must do that, because otherwhise the next line doesn't work (the number of pendula changes)
    DoublePendulum* newPendulum = new DoublePendulum(*m_pendula[m_currentPendulumIndex], QString(""), Color::pendulumColor[numberOfPendula]);
    m_pendula[numberOfPendula] = newPendulum;
    connectPendulum(m_pendula[DoublePendulum::numberOfPendula()-1]);

    m_pendulaComboBox->setCurrentIndex(m_pendulaComboBox->count()-1);
    int lastPendulumNumber = m_pendulaComboBox->currentText().replace(TranslatedStrings::pendulum + " ", QString::null).toInt();
    QString pendulumName = TranslatedStrings::pendulum + " " + QString::number(lastPendulumNumber+1);
    newPendulum->setName(pendulumName);
    m_drawingArea->addPendulum();
    m_graphArea->addPendulum();
    m_pendulaComboBox->addItem(pendulumName);
    m_pendulaComboBox->setCurrentIndex(m_pendulaComboBox->count()-1);

    if(DoublePendulum::numberOfPendula() >= Limitations::maxNumberOfPendula)
    {
        m_addPendulumPushButton->setEnabled(false);
    }
    m_deleteCurrentPendulumPushButton->setEnabled(true);
}

void MainWindow::deleteCurrentPendulum()
{
    int pendulumPlace(m_currentPendulumIndex);
    m_pendulaComboBox->removeItem(pendulumPlace);
    delete m_pendula[pendulumPlace];
    m_drawingArea->removePendulum(pendulumPlace);
    m_graphArea->removePendulum(pendulumPlace);

    for(int i(pendulumPlace); i<DoublePendulum::numberOfPendula(); i++)
    {
        m_pendula[i] = m_pendula[i+1];
    }
    m_pendula[DoublePendulum::numberOfPendula()] = nullptr;
    switchWidgetsValues();
    connectCurrentPendulumToWidgets();

    m_addPendulumPushButton->setEnabled(true);
}

void MainWindow::switchWidgetsValues()
{
    DoublePendulum* currPendulum(m_pendula[m_currentPendulumIndex]);
    changeColorButtonColor();
    switch(m_currentIndex[Index::angle])
    {
    case 0:
        m_angleDoubleSpinBox->setValue(currPendulum->measuredInitAng1()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
        m_angleUncertaintyDoubleSpinBox->setValue(currPendulum->ang1Uncertainty()*100);
        break;
    case 1:
        m_angleDoubleSpinBox->setValue(currPendulum->measuredInitAng2()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
        m_angleUncertaintyDoubleSpinBox->setValue(currPendulum->ang2Uncertainty()*100);
        break;
    }
    switch(m_currentIndex[Index::angVel])
    {
    case 0:
        m_angVelDoubleSpinBox->setValue(currPendulum->measuredInitAngVel1()/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]]);
        m_angVelUncertaintyDoubleSpinBox->setValue(currPendulum->angVel1Uncertainty()*100);
        break;
    case 1:
        m_angVelDoubleSpinBox->setValue(currPendulum->measuredInitAngVel2()/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]]);
        m_angVelUncertaintyDoubleSpinBox->setValue(currPendulum->angVel2Uncertainty()*100);
        break;
    }
    switch(m_currentIndex[Index::radius])
    {
    case 0:
        m_radiusDoubleSpinBox->setValue(currPendulum->measuredR1()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
        m_radiusUncertaintyDoubleSpinBox->setValue(currPendulum->r1Uncertainty()*100);
        break;
    case 1:
        m_radiusDoubleSpinBox->setValue(currPendulum->measuredR2()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
        m_radiusUncertaintyDoubleSpinBox->setValue(currPendulum->r2Uncertainty()*100);
        break;
    }
    switch(m_currentIndex[Index::mass])
    {
    case 0:
        m_massDoubleSpinBox->setValue(currPendulum->measuredM1()/UnitConversion::toKg[m_currentUnit[UnitOf::mass]]);
        m_massUncertaintyDoubleSpinBox->setValue(currPendulum->m1Uncertainty()*100);
        break;
    case 1:
        m_massDoubleSpinBox->setValue(currPendulum->measuredM2()/UnitConversion::toKg[m_currentUnit[UnitOf::mass]]);
        m_massUncertaintyDoubleSpinBox->setValue(currPendulum->m2Uncertainty()*100);
        break;
    }
    m_gDoubleSpinBox->setValue(currPendulum->measuredG()/UnitConversion::toMps2[m_currentUnit[UnitOf::acc]]);
    m_gUncertaintyDoubleSpinBox->setValue(currPendulum->gUncertainty()*100);
    m_integrationMethodComboBox->setCurrentIndex(currPendulum->integrationMethod());
}

void MainWindow::openDataFile()
{
    try
    {
        m_dataStream.open(m_dataPath.toStdString().c_str(), std::ofstream::out | std::ofstream::trunc);
        if(m_dataStream.is_open())
        {
            m_fileIsOpened = true;
            m_dataStream << "#" << "\"" << tr("Name").toStdString() << "\"" << ",";
            for(int i(0); i < Parameter::namesList.count(); i++)
            {
                m_dataStream << Parameter::namesList.at(i).toStdString();
                if(i < Parameter::namesList.count()-1)
                {
                    m_dataStream << ",";
                }
            }
            m_dataStream << std::endl;
        }
        else
        {
            throw(Error(0, tr("Could not open the selected file.").toStdString(), 1));
        }
    } catch(Error const& e)
    {
        e.display(this);
        m_exportDataCheckBox->toggle();
    }
}

void MainWindow::closeDataFile()
{
    m_dataStream.close();
    m_fileIsOpened = false;
}

void MainWindow::warningNoFileChosen()
{
    QMessageBox messageBox(QMessageBox::Warning, tr("No file chosen"), tr("To export data you first have to chose a file"), QMessageBox::Close, this);
    messageBox.addButton(TranslatedStrings::chooseFile, QMessageBox::AcceptRole);
    QObject::connect(&messageBox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onWarningButtonClicked(QAbstractButton*)));
    messageBox.exec();
    QObject::disconnect(&messageBox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onWarningButtonClicked(QAbstractButton*)));
}

void MainWindow::askToDeleteCurrentPendulum()
{
    if(DoublePendulum::numberOfPendula() > 1)
    {
        if(m_deleteWarning)
        {
            QMessageBox messageBox(QMessageBox::Information, tr("Confirmation"),
                                   tr("Are you sure you want to delete the current Pendulum? \n"
                                      "This operation cannot be undone."),
                                   QMessageBox::Yes | QMessageBox::No, this);
            QCheckBox* dontShowAgainCheckBox = new QCheckBox(tr("Don't show this message again"));
            messageBox.setCheckBox(dontShowAgainCheckBox);

            int ret = messageBox.exec();


            if(dontShowAgainCheckBox->checkState() == Qt::Checked)
            {
                m_deleteWarning = false;
            }

            if(ret == QMessageBox::Yes)
            {
                deleteCurrentPendulum();
            }
        }
        else
        {
            deleteCurrentPendulum();
        }

        if(DoublePendulum::numberOfPendula() == 1)
        {
            m_deleteCurrentPendulumPushButton->setEnabled(false);
        }
    }
}

void MainWindow::onTabChanged(int tabIndex)
{
    switch(tabIndex)
    {
    case 0:
        m_drawPathCheckBox->setEnabled(true);
        m_autoScrollCheckBox->setEnabled(false);
        m_xAxisLabel->setEnabled(false);
        m_xAxisComboBox->setEnabled(false);
        m_yAxisLabel->setEnabled(false);
        m_yAxisComboBox->setEnabled(false);
        m_FPSlCDNumber->setParent(m_drawingArea);
        m_FPSlCDNumber->setVisible(true);
        m_FPSlCDNumber->raise();
        break;
    case 1:
        m_drawPathCheckBox->setEnabled(false);
        m_autoScrollCheckBox->setEnabled(true);
        m_xAxisLabel->setEnabled(true);
        m_xAxisComboBox->setEnabled(true);
        m_yAxisLabel->setEnabled(true);
        m_yAxisComboBox->setEnabled(true);
        m_FPSlCDNumber->setParent(m_graphArea);
        m_FPSlCDNumber->setVisible(true);
        m_FPSlCDNumber->raise();
        break;
    }
}

//widget slots
void MainWindow::switchParameter(QString newParameter)
{
    bool restartIsDisabled(m_pendula[m_currentPendulumIndex]->restartIsDisabled());
    m_pendula[m_currentPendulumIndex]->setDisableRestart(true);

    if(newParameter==QString(SpecChar::theta) + QString(SpecChar::indice1)) //ang1
    {
        m_currentIndex[Index::angle]=0;
        m_angleDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAng1()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
        m_angleUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->ang1Uncertainty()*100.0);
    }
    else if (newParameter==QString(SpecChar::theta) + QString(SpecChar::indice2)) //ang2
    {
        m_currentIndex[Index::angle]=1;
        m_angleDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAng2()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
        m_angleUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->ang2Uncertainty()*100.0);
    }
    else if (newParameter==QString(SpecChar::omega) + QString(SpecChar::indice1))
    {
        m_currentIndex[Index::angVel]=0;
        m_angVelDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAngVel1()/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]]);
        m_angVelUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->angVel1Uncertainty()*100.0);
    }
    else if(newParameter==QString(SpecChar::omega) + QString(SpecChar::indice2))
    {
        m_currentIndex[Index::angVel]=1;
        m_angVelDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAngVel2()/UnitConversion::toRadps[m_currentUnit[UnitOf::angVel]]);
        m_angVelUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->angVel2Uncertainty()*100.0);
    }
    else if (newParameter==QString("r")+SpecChar::indice1)
    {
        m_currentIndex[Index::radius]=0;
        m_radiusDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredR1()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
        m_radiusUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->r1Uncertainty()*100.0);
    }
    else if (newParameter==QString("r")+SpecChar::indice2)
    {
        m_currentIndex[Index::radius]=1;
        m_radiusDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredR2()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
        m_radiusUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->r2Uncertainty()*100.0);
    }
    else if (newParameter==QString("m")+SpecChar::indice1)
    {
        m_currentIndex[Index::mass]=0;
        m_massDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredM1()/UnitConversion::toKg[m_currentUnit[UnitOf::mass]]);
        m_massUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->m1Uncertainty()*100.0);
    }
    else if (newParameter==QString("m")+SpecChar::indice2)
    {
        m_currentIndex[Index::mass]=1;
        m_massDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredM2()/UnitConversion::toKg[m_currentUnit[UnitOf::mass]]);
        m_massUncertaintyDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->m2Uncertainty()*100.0);
    }

    m_pendula[m_currentPendulumIndex]->setDisableRestart(restartIsDisabled);
}

void MainWindow::changeAngUnit(int unit)
{
    if(unit >= 0)
    {
        bool restartIsDisabled(m_pendula[m_currentPendulumIndex]->restartIsDisabled());
        m_pendula[m_currentPendulumIndex]->setDisableRestart(true);

        m_currentUnit[UnitOf::angle]=unit;
        m_angleSlider->changeRange(-DefaultParameterValue::maxAng/UnitConversion::toRad[unit], DefaultParameterValue::maxAng/UnitConversion::toRad[unit]);
        m_angleDoubleSpinBox->setRange(-DefaultParameterValue::maxAng/UnitConversion::toRad[unit], DefaultParameterValue::maxAng/UnitConversion::toRad[unit]);
        m_pendula[m_currentPendulumIndex]->changeAngle(m_angleDoubleSpinBox->value());

        m_pendula[m_currentPendulumIndex]->setDisableRestart(restartIsDisabled);

        m_pendula[m_currentPendulumIndex]->restart();
    }
}
void MainWindow::changeAngVelUnit(int unit)
{
    if(unit >= 0)
    {
        bool restartIsDisabled(m_pendula[m_currentPendulumIndex]->restartIsDisabled());
        m_pendula[m_currentPendulumIndex]->setDisableRestart(true);

        m_currentUnit[UnitOf::angVel]=unit;
        m_angVelSlider->changeRange(-DefaultParameterValue::defaultMax/UnitConversion::toRadps[unit], DefaultParameterValue::defaultMax/UnitConversion::toRadps[unit]);
        m_angVelDoubleSpinBox->setRange(-DefaultParameterValue::defaultMax/UnitConversion::toRadps[unit], DefaultParameterValue::defaultMax/UnitConversion::toRadps[unit]);
        m_pendula[m_currentPendulumIndex]->changeAngularVelocity(m_angVelDoubleSpinBox->value());

        m_pendula[m_currentPendulumIndex]->setDisableRestart(restartIsDisabled);

        m_pendula[m_currentPendulumIndex]->restart();
    }
}
void MainWindow::changeRadiusUnit(int unit)
{
    if(unit >= 0)
    {
        m_currentUnit[UnitOf::length]=unit;
        m_radiusSlider->changeRange(DefaultParameterValue::minRadius/UnitConversion::toM[unit], DefaultParameterValue::maxRadius/UnitConversion::toM[unit]);
        m_radiusDoubleSpinBox->setRange(DefaultParameterValue::minRadius/UnitConversion::toM[unit], DefaultParameterValue::maxRadius/UnitConversion::toM[unit]);
        m_pendula[m_currentPendulumIndex]->changeRadius(m_radiusDoubleSpinBox->value());
        m_drawingArea->refresh();
    }
}
void MainWindow::changeMassUnit(int unit)
{
    if(unit >= 0)
    {
        m_currentUnit[UnitOf::mass]=unit;
        m_massSlider->changeRange(DefaultParameterValue::defaultMin/UnitConversion::toKg[unit], DefaultParameterValue::defaultMax/UnitConversion::toKg[unit]);
        m_massDoubleSpinBox->setRange(DefaultParameterValue::defaultMin/UnitConversion::toKg[unit], DefaultParameterValue::defaultMax/UnitConversion::toKg[unit]);
        m_pendula[m_currentPendulumIndex]->changeMass(m_massDoubleSpinBox->value());
        m_drawingArea->refresh();
    }
}
void MainWindow::changeGUnit(int unit)
{
    if(unit >= 0)
    {
        m_currentUnit[UnitOf::acc]=unit;
        m_gSlider->changeRange(0, DefaultParameterValue::defaultMax/UnitConversion::toMps2[unit]);
        m_gDoubleSpinBox->setRange(0, DefaultParameterValue::defaultMax/UnitConversion::toMps2[unit]);
        m_pendula[m_currentPendulumIndex]->changeGravity(m_gDoubleSpinBox->value());
    }
}
void MainWindow::changeDtUnit(int unit)
{
    if(unit >= 0)
    {
        m_currentUnit[UnitOf::time]=unit;
        m_dtSlider->changeRange(DefaultParameterValue::minDt/UnitConversion::toS[unit], DefaultParameterValue::maxDt/UnitConversion::toS[unit]);
        m_dtDoubleSpinBox->setRange(DefaultParameterValue::minDt/UnitConversion::toS[unit], DefaultParameterValue::maxDt/UnitConversion::toS[unit]);
        m_pendula[m_currentPendulumIndex]->changeDt(m_dtDoubleSpinBox->value());
    }
}

void MainWindow::onExportDataClicked(bool isChecked)
{
    if(isChecked)
    {
        m_writeData = true;
        if(!m_fileIsChosen)
        {
            onConfigureClicked();
        }
    }
    else
    {
        m_writeData = false;
    }
}

void MainWindow::onConfigureClicked()
{
    bool retry;
    do
    {
        retry = false;
        if(!QDir(m_lastDataDirectory).exists())
        {
            m_lastDataDirectory = QStandardPaths::displayName(QStandardPaths::DocumentsLocation);
        }

        QFileDialog fileDialog(this, tr("Choose file location"), m_lastDataDirectory);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setFilter(QDir::Readable | QDir::Writable | QDir::NoDotAndDotDot);
        fileDialog.setNameFilter(tr("Data files") + " (*.txt *.dat)");
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setLabelText(QFileDialog::Accept, tr("Choose"));
        if(fileDialog.exec() == QDialog::Accepted)
        {
            QString oldDataPath(m_dataPath);
            m_dataPath = fileDialog.selectedFiles().at(0);
            if(QFile(m_dataPath).exists())
            {
                int ret = QMessageBox::warning(this, tr("Existing file selected"), tr("All the data from this file will be erased."), QMessageBox::Ok, QMessageBox::Cancel);
                if(ret == QMessageBox::Cancel)
                {
                    m_dataPath = oldDataPath;
                    retry = true;
                    continue;
                }
            }
            m_lastDataDirectory = m_dataPath;
            m_lastDataDirectory.remove("/" + QFileInfo(m_dataPath).fileName());
            m_fileIsChosen = true;
            if(m_fileIsOpened)
            {
                closeDataFile();
            }
        }
        else if(!m_fileIsChosen)
        {
            warningNoFileChosen();
        }
    }while(retry);
}

void MainWindow::onWarningButtonClicked(QAbstractButton* button)
{
    if(button->text() == TranslatedStrings::chooseFile)
    {
        onConfigureClicked();
    }
    else
    {
        m_exportDataCheckBox->toggle();
    }
}

void MainWindow::setOutputsPerSecond(double outputsPerSecond)
{
    double dt(m_pendula[m_currentPendulumIndex]->dt());
    if(dt <= 1.0/outputsPerSecond)
    {
        m_outputsPerSecond = outputsPerSecond;
    }
    else
    {
        QSignalBlocker b(m_outputsPerSecondDoubleSpinBox);
        m_outputsPerSecondDoubleSpinBox->setValue(1.0/dt);
        b.~QSignalBlocker();
        m_outputsPerSecond = m_outputsPerSecondDoubleSpinBox->value();
    }
}

void MainWindow::adjustOutputsPerSecond(double dt)
{
    if(dt > 1.0/m_outputsPerSecond)
    {
        QSignalBlocker b(m_outputsPerSecondDoubleSpinBox);
        m_outputsPerSecondDoubleSpinBox->setValue(1.0/dt);
        b.~QSignalBlocker();
        m_outputsPerSecond = m_outputsPerSecondDoubleSpinBox->value();
    }
}

void MainWindow::setRenderOffscreen(bool isEnabled)
{
    m_renderOffscreen = isEnabled;
}

void MainWindow::clearCurrentTab()
{
    switch(m_tabWidget->currentIndex())
    {
    case 0:
        m_drawingArea->clear();
        break;
    case 1:
        m_graphArea->clear();
        break;
    }
}

void MainWindow::clearAll()
{
    m_drawingArea->clear();
    m_graphArea->clear();
}

void MainWindow::clearAll(int pendulumNumber)
{
    m_drawingArea->clear(pendulumNumber);
    m_graphArea->clear(pendulumNumber);
}

void MainWindow::clearCurrentPendulum()
{
    clearAll(m_currentPendulumIndex);
}

void MainWindow::updateDoubleSpinBoxes()
{
    bool restartIsDisabled(m_pendula[m_currentPendulumIndex]->restartIsDisabled());
    m_pendula[m_currentPendulumIndex]->setDisableRestart(true);

    QSignalBlocker bx1(m_x1DoubleSpinBox); //blocks the signals of the coordinate spin boxes to prevent an infinite loop
    QSignalBlocker by1(m_y1DoubleSpinBox);
    QSignalBlocker bx2(m_x2DoubleSpinBox);
    QSignalBlocker by2(m_y2DoubleSpinBox);

    m_x1DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosX1());
    m_y1DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosY1());
    m_x2DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosX2());
    m_y2DoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->initPosY2());

    m_x1Slider->changeValue(m_x1DoubleSpinBox->value());
    m_y1Slider->changeValue(m_y1DoubleSpinBox->value());
    m_x2Slider->changeValue(m_x2DoubleSpinBox->value());
    m_y2Slider->changeValue(m_y2DoubleSpinBox->value());

    bx1.~QSignalBlocker();
    by1.~QSignalBlocker();
    bx2.~QSignalBlocker();
    by2.~QSignalBlocker();

    if(m_currentIndex[Index::angle]==0)
    {
        m_angleDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAng1()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
    }
    else
    {
        m_angleDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredInitAng2()/UnitConversion::toRad[m_currentUnit[UnitOf::angle]]);
    }
    if(m_currentIndex[Index::radius]==0)
    {
        m_radiusDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredR1()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
    }
    else
    {
        m_radiusDoubleSpinBox->setValue(m_pendula[m_currentPendulumIndex]->measuredR2()/UnitConversion::toM[m_currentUnit[UnitOf::length]]);
    }

    m_pendula[m_currentPendulumIndex]->setDisableRestart(restartIsDisabled);
}

void MainWindow::startDrawing() /*This function is called each time the simulation starts.
                                 *It tells the pendulum to move on step forward in time
                                 *and chooses when to update m_drawingArea and m_graphArea.*/
{
    m_isPlaying = true;

    std::vector<std::string> outputData;
    std::chrono::steady_clock::time_point startTime;
    m_frameCountModSampleSize = 0;
    bool wasJustUpdated(true);
    m_averageFPS = 0;
    m_isFirstCycle = true;

    while(m_isPlaying)
    {
        if(wasJustUpdated)
        {
            startTime = std::chrono::steady_clock::now();
            wasJustUpdated = false;
        }

        for(int i(0); i<DoublePendulum::numberOfPendula(); i++)
        {
            m_pendula[i]->moveOneStep();
        }
        m_simulationTimeSinceLastSpeedChange += fabs(m_pendula[m_currentPendulumIndex]->dt());

        if(m_simulationTimeSinceLastSpeedChange/m_outputCountSinceLastSpeedChange > 1.0/m_outputsPerSecond)
        {
            outputData.push_back("");
            for(int i(0); i < DoublePendulum::numberOfPendula(); i++)
            {
                outputData.back().append("\"" + m_pendula[i]->name().toStdString() + "\"" + ",");
                for(int j(0); j < Parameter::count; j++)
                {
                    outputData.back().append(std::to_string(m_pendula[i]->parameter(j)));
                    if(j < Parameter::count-1)
                    {
                        outputData.back().append(",");
                    }
                }
                outputData.back().append("\n");
            }
            m_outputCountSinceLastSpeedChange++;
        }

        if(m_simulationTimeSinceLastSpeedChange/m_dataBlockCountSinceLastSpeedChange > Prefix::deci*fabs(m_playbackSpeed)) //writes data in big blocks
        {
            //writes data in a file
            if(m_writeData)
            {
                if(!m_fileIsOpened)
                {
                    openDataFile();
                }
                if(m_fileIsOpened) //in case the file couldn't be opened above
                {
                    for(std::vector<std::string>::iterator it = outputData.begin(); it != outputData.end(); it++)
                    {
                        m_dataStream.write(it->data(), it->size());
                    }
                    m_dataBlockCountSinceLastSpeedChange++;
                    outputData.clear();
                    outputData.resize(0);
                    outputData.reserve(int(m_outputsPerSecond*Prefix::deci*fabs(m_playbackSpeed)) + 1.0);
                }
            }
        }

        if(m_simulationTimeSinceLastSpeedChange/m_frameCountSinceLastSpeedChange > 1.0/Screen::refreshRate*fabs(m_playbackSpeed))
        {
            //draws either the pendulum or the graphs
            if(m_renderOffscreen && (m_tabWidget->currentIndex() == 0 || m_drawPathCheckBox->isChecked())) //if draw path is not checked and the current tab is m_graphArea,
            {                                                                                               //there is no point in rendering m_drawingArea
                m_drawingArea->refresh();
                m_graphArea->refresh();
            }
            else
            {
                switch(m_tabWidget->currentIndex())
                {
                case 0:
                    m_drawingArea->refresh();
                    break;
                case 1:
                    m_graphArea->refresh();
                    break;
                }
            }

            std::chrono::duration<double, std::micro> elapsedTimeUsec = std::chrono::steady_clock::now()-startTime;

            QCoreApplication::processEvents();
            do
            {
                elapsedTimeUsec = std::chrono::steady_clock::now()-startTime;
            } while(elapsedTimeUsec.count()*Prefix::micro < 1.0/Screen::refreshRate);

            auto currentFrameCalculationTimeUsec = elapsedTimeUsec.count();
            if(m_averageFPS >= 0.001)
            {
                if(m_isFirstCycle)
                {
                    m_averageFPS = m_frameCountModSampleSize/m_averageFPS; //sum of all the time intervals between each frame except the last one
                }
                else
                {
                    m_averageFPS = m_sampleSize/m_averageFPS;
                }
            }
            if(!m_isFirstCycle)
            {
                m_averageFPS -= m_timePerFrame[m_frameCountModSampleSize]; //removes the time interval of the oldest frame
            }

            m_averageFPS += currentFrameCalculationTimeUsec*Prefix::micro; //adds the time interval of the newest frame
            m_timePerFrame[m_frameCountModSampleSize] = currentFrameCalculationTimeUsec*Prefix::micro; //replaces the oldest frame with the newest one
            m_frameCountModSampleSize++;
            m_frameCountSinceLastSpeedChange++;

            if(m_isFirstCycle)
            {
                m_averageFPS = m_frameCountModSampleSize/m_averageFPS;
            }
            else
            {
                m_averageFPS = m_sampleSize/m_averageFPS;
            }
            double fps = round(m_averageFPS*100)/100.0;
            int a = int(floor(fps));
            m_FPSlCDNumber->display(QString::number(a) + "." + QString("%1").arg(int(floor((fps-a)*10)), 1, 10, QChar('0'))); //workaround to keep trailling zeros

            if(m_frameCountModSampleSize >= m_sampleSize)
            {
                m_frameCountModSampleSize = 0;
                m_isFirstCycle = false;
            }
            wasJustUpdated = true;
        }
    }
    m_FPSlCDNumber->display(QString("0.0")); //if it's not playing, the frame rate is equal to 0.0
}

void MainWindow::resetFPS()
{
    m_isFirstCycle = true;
    m_averageFPS = 0;
    m_frameCountModSampleSize = 0;
}

void MainWindow::closeEvent(QCloseEvent*)
{
    m_isPlaying=false;
    int numberOfPendula(DoublePendulum::numberOfPendula());
    for(int i(0); i<numberOfPendula; i++)
    {
        delete m_pendula[i];
    }
    if(m_fileIsOpened)
    {
        closeDataFile();
    }
}
