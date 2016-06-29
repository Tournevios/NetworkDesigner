/********************************************************************************
** Form generated from reading ui file 'mainWindow.ui'
**
** Created: Mon Apr 12 15:58:18 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>
#include "designplan.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *actionO_pen;
    QAction *action_Save;
    QAction *actionSave_As;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionDelete;
    QAction *actionSelect_All;
    QAction *actionSelect_borders;
    QAction *actionSelect_centers;
    QAction *actionKernel;
    QAction *actionExport_To_GNBox_Premodel;
    QAction *action_Quit;
    QWidget *centralwidget;
    QToolBox *toolBox;
    QWidget *pSimulationParam;
    QLabel *lblTemperature;
    QDoubleSpinBox *dsbTemperature;
    QSpinBox *sbUpdatesNumber;
    QLabel *lblUpdatesNumber;
    QDoubleSpinBox *dsbSynchronyRate;
    QLabel *lblSynchronyRate;
    QComboBox *cmbSimulationType;
    QLabel *label_2;
    QPushButton *pbStart;
    QPushButton *pbStop;
    QCheckBox *chkUniformalTemperature;
    QLabel *lblSpeedPercent;
    QSpinBox *sbSpeedPercent;
    QProgressBar *progressBar;
    QWidget *pNeuronsParam;
    QLabel *lblThreshold;
    QDoubleSpinBox *dsbThreshold;
    QLabel *lblState;
    QLabel *lblNeuronTemperature;
    QDoubleSpinBox *dsbNeuronTemperature;
    QSpinBox *sbNbStates;
    QLabel *lblStateNumber;
    QComboBox *cmbState;
    QLineEdit *txtNodeID;
    QLabel *lblLabel;
    QLabel *lblCurrent;
    QLineEdit *txtIndex;
    QWidget *pSynapsesParam;
    QDoubleSpinBox *dsbWeight;
    QLabel *lblWeight;
    QSpinBox *sbDelay;
    QLabel *lblDelay;
    QWidget *pUpdateSchedule;
    QLabel *lblUpdateBlock;
    QLabel *lblUpdateMethods;
    QLabel *lblScheduleType;
    QComboBox *cmbUpdateBlock;
    QComboBox *cmbScheduleType;
    QComboBox *cmbUpdateMethods;
    DesignPlan *frmDesign;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(782, 569);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        actionO_pen = new QAction(MainWindow);
        actionO_pen->setObjectName(QString::fromUtf8("actionO_pen"));
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionDelete = new QAction(MainWindow);
        actionDelete->setObjectName(QString::fromUtf8("actionDelete"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionSelect_borders = new QAction(MainWindow);
        actionSelect_borders->setObjectName(QString::fromUtf8("actionSelect_borders"));
        actionSelect_centers = new QAction(MainWindow);
        actionSelect_centers->setObjectName(QString::fromUtf8("actionSelect_centers"));
        actionKernel = new QAction(MainWindow);
        actionKernel->setObjectName(QString::fromUtf8("actionKernel"));
        actionExport_To_GNBox_Premodel = new QAction(MainWindow);
        actionExport_To_GNBox_Premodel->setObjectName(QString::fromUtf8("actionExport_To_GNBox_Premodel"));
        action_Quit = new QAction(MainWindow);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        toolBox = new QToolBox(centralwidget);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        toolBox->setGeometry(QRect(10, 50, 201, 421));
        pSimulationParam = new QWidget();
        pSimulationParam->setObjectName(QString::fromUtf8("pSimulationParam"));
        pSimulationParam->setGeometry(QRect(0, 0, 201, 293));
        lblTemperature = new QLabel(pSimulationParam);
        lblTemperature->setObjectName(QString::fromUtf8("lblTemperature"));
        lblTemperature->setGeometry(QRect(0, 50, 91, 18));
        dsbTemperature = new QDoubleSpinBox(pSimulationParam);
        dsbTemperature->setObjectName(QString::fromUtf8("dsbTemperature"));
        dsbTemperature->setGeometry(QRect(100, 50, 71, 27));
        dsbTemperature->setAccelerated(true);
        dsbTemperature->setDecimals(10);
        dsbTemperature->setMinimum(0);
        dsbTemperature->setMaximum(1e+10);
        dsbTemperature->setSingleStep(0.001);
        dsbTemperature->setValue(0);
        sbUpdatesNumber = new QSpinBox(pSimulationParam);
        sbUpdatesNumber->setObjectName(QString::fromUtf8("sbUpdatesNumber"));
        sbUpdatesNumber->setGeometry(QRect(100, 110, 71, 27));
        sbUpdatesNumber->setAccelerated(true);
        sbUpdatesNumber->setMinimum(1);
        sbUpdatesNumber->setMaximum(100000);
        sbUpdatesNumber->setValue(100);
        lblUpdatesNumber = new QLabel(pSimulationParam);
        lblUpdatesNumber->setObjectName(QString::fromUtf8("lblUpdatesNumber"));
        lblUpdatesNumber->setGeometry(QRect(0, 110, 111, 18));
        dsbSynchronyRate = new QDoubleSpinBox(pSimulationParam);
        dsbSynchronyRate->setObjectName(QString::fromUtf8("dsbSynchronyRate"));
        dsbSynchronyRate->setGeometry(QRect(100, 150, 71, 27));
        dsbSynchronyRate->setAccelerated(true);
        dsbSynchronyRate->setDecimals(3);
        dsbSynchronyRate->setMaximum(1);
        dsbSynchronyRate->setSingleStep(0.001);
        dsbSynchronyRate->setValue(1);
        lblSynchronyRate = new QLabel(pSimulationParam);
        lblSynchronyRate->setObjectName(QString::fromUtf8("lblSynchronyRate"));
        lblSynchronyRate->setGeometry(QRect(0, 150, 101, 18));
        cmbSimulationType = new QComboBox(pSimulationParam);
        cmbSimulationType->setObjectName(QString::fromUtf8("cmbSimulationType"));
        cmbSimulationType->setGeometry(QRect(100, 10, 71, 26));
        label_2 = new QLabel(pSimulationParam);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(0, 10, 101, 18));
        pbStart = new QPushButton(pSimulationParam);
        pbStart->setObjectName(QString::fromUtf8("pbStart"));
        pbStart->setGeometry(QRect(0, 230, 80, 27));
        pbStop = new QPushButton(pSimulationParam);
        pbStop->setObjectName(QString::fromUtf8("pbStop"));
        pbStop->setGeometry(QRect(100, 230, 80, 27));
        chkUniformalTemperature = new QCheckBox(pSimulationParam);
        chkUniformalTemperature->setObjectName(QString::fromUtf8("chkUniformalTemperature"));
        chkUniformalTemperature->setGeometry(QRect(-10, 80, 141, 23));
        chkUniformalTemperature->setLayoutDirection(Qt::RightToLeft);
        chkUniformalTemperature->setChecked(true);
        lblSpeedPercent = new QLabel(pSimulationParam);
        lblSpeedPercent->setObjectName(QString::fromUtf8("lblSpeedPercent"));
        lblSpeedPercent->setGeometry(QRect(0, 190, 71, 18));
        sbSpeedPercent = new QSpinBox(pSimulationParam);
        sbSpeedPercent->setObjectName(QString::fromUtf8("sbSpeedPercent"));
        sbSpeedPercent->setGeometry(QRect(100, 190, 71, 27));
        sbSpeedPercent->setMaximum(100);
        sbSpeedPercent->setValue(100);
        progressBar = new QProgressBar(pSimulationParam);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(0, 270, 201, 23));
        progressBar->setValue(0);
        toolBox->addItem(pSimulationParam, QString::fromUtf8("Simulation"));
        pNeuronsParam = new QWidget();
        pNeuronsParam->setObjectName(QString::fromUtf8("pNeuronsParam"));
        pNeuronsParam->setGeometry(QRect(0, 0, 201, 293));
        lblThreshold = new QLabel(pNeuronsParam);
        lblThreshold->setObjectName(QString::fromUtf8("lblThreshold"));
        lblThreshold->setGeometry(QRect(10, 180, 71, 20));
        dsbThreshold = new QDoubleSpinBox(pNeuronsParam);
        dsbThreshold->setObjectName(QString::fromUtf8("dsbThreshold"));
        dsbThreshold->setGeometry(QRect(80, 180, 101, 27));
        dsbThreshold->setAccelerated(true);
        dsbThreshold->setDecimals(10);
        dsbThreshold->setMinimum(-99.99);
        dsbThreshold->setSingleStep(0.001);
        dsbThreshold->setValue(0.001);
        lblState = new QLabel(pNeuronsParam);
        lblState->setObjectName(QString::fromUtf8("lblState"));
        lblState->setGeometry(QRect(10, 150, 57, 18));
        lblNeuronTemperature = new QLabel(pNeuronsParam);
        lblNeuronTemperature->setObjectName(QString::fromUtf8("lblNeuronTemperature"));
        lblNeuronTemperature->setGeometry(QRect(10, 70, 71, 20));
        dsbNeuronTemperature = new QDoubleSpinBox(pNeuronsParam);
        dsbNeuronTemperature->setObjectName(QString::fromUtf8("dsbNeuronTemperature"));
        dsbNeuronTemperature->setGeometry(QRect(100, 70, 81, 27));
        dsbNeuronTemperature->setDecimals(10);
        dsbNeuronTemperature->setMaximum(1e+10);
        dsbNeuronTemperature->setSingleStep(0.001);
        dsbNeuronTemperature->setValue(0);
        sbNbStates = new QSpinBox(pNeuronsParam);
        sbNbStates->setObjectName(QString::fromUtf8("sbNbStates"));
        sbNbStates->setGeometry(QRect(120, 110, 61, 27));
        sbNbStates->setMinimum(2);
        sbNbStates->setValue(2);
        lblStateNumber = new QLabel(pNeuronsParam);
        lblStateNumber->setObjectName(QString::fromUtf8("lblStateNumber"));
        lblStateNumber->setGeometry(QRect(10, 110, 91, 18));
        cmbState = new QComboBox(pNeuronsParam);
        cmbState->setObjectName(QString::fromUtf8("cmbState"));
        cmbState->setGeometry(QRect(90, 140, 91, 26));
        txtNodeID = new QLineEdit(pNeuronsParam);
        txtNodeID->setObjectName(QString::fromUtf8("txtNodeID"));
        txtNodeID->setGeometry(QRect(70, 40, 111, 27));
        lblLabel = new QLabel(pNeuronsParam);
        lblLabel->setObjectName(QString::fromUtf8("lblLabel"));
        lblLabel->setGeometry(QRect(10, 40, 61, 18));
        lblCurrent = new QLabel(pNeuronsParam);
        lblCurrent->setObjectName(QString::fromUtf8("lblCurrent"));
        lblCurrent->setGeometry(QRect(10, 13, 51, 20));
        txtIndex = new QLineEdit(pNeuronsParam);
        txtIndex->setObjectName(QString::fromUtf8("txtIndex"));
        txtIndex->setGeometry(QRect(70, 10, 113, 27));
        toolBox->addItem(pNeuronsParam, QString::fromUtf8("Neurons parameters"));
        pSynapsesParam = new QWidget();
        pSynapsesParam->setObjectName(QString::fromUtf8("pSynapsesParam"));
        pSynapsesParam->setGeometry(QRect(0, 0, 201, 293));
        dsbWeight = new QDoubleSpinBox(pSynapsesParam);
        dsbWeight->setObjectName(QString::fromUtf8("dsbWeight"));
        dsbWeight->setGeometry(QRect(111, 10, 71, 27));
        dsbWeight->setAccelerated(true);
        dsbWeight->setDecimals(3);
        dsbWeight->setMinimum(-99.99);
        dsbWeight->setSingleStep(0.001);
        dsbWeight->setValue(1);
        lblWeight = new QLabel(pSynapsesParam);
        lblWeight->setObjectName(QString::fromUtf8("lblWeight"));
        lblWeight->setGeometry(QRect(10, 10, 101, 20));
        sbDelay = new QSpinBox(pSynapsesParam);
        sbDelay->setObjectName(QString::fromUtf8("sbDelay"));
        sbDelay->setGeometry(QRect(110, 50, 71, 27));
        sbDelay->setMaximum(10000);
        lblDelay = new QLabel(pSynapsesParam);
        lblDelay->setObjectName(QString::fromUtf8("lblDelay"));
        lblDelay->setGeometry(QRect(10, 50, 48, 18));
        toolBox->addItem(pSynapsesParam, QString::fromUtf8("Synapses parameters"));
        pUpdateSchedule = new QWidget();
        pUpdateSchedule->setObjectName(QString::fromUtf8("pUpdateSchedule"));
        pUpdateSchedule->setGeometry(QRect(0, 0, 201, 293));
        lblUpdateBlock = new QLabel(pUpdateSchedule);
        lblUpdateBlock->setObjectName(QString::fromUtf8("lblUpdateBlock"));
        lblUpdateBlock->setGeometry(QRect(10, 40, 91, 18));
        lblUpdateMethods = new QLabel(pUpdateSchedule);
        lblUpdateMethods->setObjectName(QString::fromUtf8("lblUpdateMethods"));
        lblUpdateMethods->setGeometry(QRect(10, 70, 101, 18));
        lblScheduleType = new QLabel(pUpdateSchedule);
        lblScheduleType->setObjectName(QString::fromUtf8("lblScheduleType"));
        lblScheduleType->setGeometry(QRect(10, 10, 91, 18));
        cmbUpdateBlock = new QComboBox(pUpdateSchedule);
        cmbUpdateBlock->setObjectName(QString::fromUtf8("cmbUpdateBlock"));
        cmbUpdateBlock->setGeometry(QRect(100, 30, 91, 26));
        cmbScheduleType = new QComboBox(pUpdateSchedule);
        cmbScheduleType->setObjectName(QString::fromUtf8("cmbScheduleType"));
        cmbScheduleType->setGeometry(QRect(100, 0, 91, 26));
        cmbUpdateMethods = new QComboBox(pUpdateSchedule);
        cmbUpdateMethods->setObjectName(QString::fromUtf8("cmbUpdateMethods"));
        cmbUpdateMethods->setGeometry(QRect(120, 60, 71, 26));
        toolBox->addItem(pUpdateSchedule, QString::fromUtf8("Update schedule"));
        frmDesign = new DesignPlan(centralwidget);
        frmDesign->setObjectName(QString::fromUtf8("frmDesign"));
        frmDesign->setGeometry(QRect(220, 50, 551, 461));
        frmDesign->setMouseTracking(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 782, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menuFile->addAction(action_New);
        menuFile->addAction(actionO_pen);
        menuFile->addAction(action_Save);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExport_To_GNBox_Premodel);
        menuFile->addSeparator();
        menuFile->addAction(action_Quit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionDelete);
        menuEdit->addAction(actionSelect_All);

        retranslateUi(MainWindow);

        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        action_New->setText(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
        actionO_pen->setText(QApplication::translate("MainWindow", "O&pen", 0, QApplication::UnicodeUTF8));
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        actionSave_As->setText(QApplication::translate("MainWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
        actionCut->setText(QApplication::translate("MainWindow", "Cut", 0, QApplication::UnicodeUTF8));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", 0, QApplication::UnicodeUTF8));
        actionPaste->setText(QApplication::translate("MainWindow", "Paste", 0, QApplication::UnicodeUTF8));
        actionDelete->setText(QApplication::translate("MainWindow", "Delete", 0, QApplication::UnicodeUTF8));
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select all", 0, QApplication::UnicodeUTF8));
        actionSelect_borders->setText(QApplication::translate("MainWindow", "Select borders", 0, QApplication::UnicodeUTF8));
        actionSelect_centers->setText(QApplication::translate("MainWindow", "Select centers", 0, QApplication::UnicodeUTF8));
        actionKernel->setText(QApplication::translate("MainWindow", "Kernel", 0, QApplication::UnicodeUTF8));
        actionExport_To_GNBox_Premodel->setText(QApplication::translate("MainWindow", "Export To &GNBox Premodel...", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("MainWindow", "&Quit", 0, QApplication::UnicodeUTF8));
        lblTemperature->setText(QApplication::translate("MainWindow", "Temperature:", 0, QApplication::UnicodeUTF8));
        dsbTemperature->setPrefix(QString());
        lblUpdatesNumber->setText(QApplication::translate("MainWindow", "Updates number:", 0, QApplication::UnicodeUTF8));
        lblSynchronyRate->setText(QApplication::translate("MainWindow", "Synchrony rate:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Simulation type:", 0, QApplication::UnicodeUTF8));
        pbStart->setText(QApplication::translate("MainWindow", "&Start", 0, QApplication::UnicodeUTF8));
        pbStop->setText(QApplication::translate("MainWindow", "St&op", 0, QApplication::UnicodeUTF8));
        chkUniformalTemperature->setText(QApplication::translate("MainWindow", "Uniformal Temperature", 0, QApplication::UnicodeUTF8));
        lblSpeedPercent->setText(QApplication::translate("MainWindow", "SpeedPercent", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pSimulationParam), QApplication::translate("MainWindow", "Simulation", 0, QApplication::UnicodeUTF8));
        lblThreshold->setText(QApplication::translate("MainWindow", "Threshold:", 0, QApplication::UnicodeUTF8));
        lblState->setText(QApplication::translate("MainWindow", "State:", 0, QApplication::UnicodeUTF8));
        lblNeuronTemperature->setText(QApplication::translate("MainWindow", "Temperature:", 0, QApplication::UnicodeUTF8));
        lblStateNumber->setText(QApplication::translate("MainWindow", "Number of states:", 0, QApplication::UnicodeUTF8));
        lblLabel->setText(QApplication::translate("MainWindow", "Node Id:", 0, QApplication::UnicodeUTF8));
        lblCurrent->setText(QApplication::translate("MainWindow", "Index:", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pNeuronsParam), QApplication::translate("MainWindow", "Neurons parameters", 0, QApplication::UnicodeUTF8));
        lblWeight->setText(QApplication::translate("MainWindow", "Weight:", 0, QApplication::UnicodeUTF8));
        lblDelay->setText(QApplication::translate("MainWindow", "Delay:", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pSynapsesParam), QApplication::translate("MainWindow", "Synapses parameters", 0, QApplication::UnicodeUTF8));
        lblUpdateBlock->setText(QApplication::translate("MainWindow", "Update block:", 0, QApplication::UnicodeUTF8));
        lblUpdateMethods->setText(QApplication::translate("MainWindow", "Update Methods:", 0, QApplication::UnicodeUTF8));
        lblScheduleType->setText(QApplication::translate("MainWindow", "Type of schedule:", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(pUpdateSchedule), QApplication::translate("MainWindow", "Update schedule", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
