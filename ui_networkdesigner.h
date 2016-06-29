/********************************************************************************
** Form generated from reading ui file 'networkdesigner.ui'
**
** Created: Mon Mar 31 20:46:33 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_NETWORKDESIGNER_H
#define UI_NETWORKDESIGNER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>

class Ui_NetworkDesignerClass
{
public:
    QFrame *frmDesign;
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
    QWidget *pNeuronsParam;
    QLabel *lblThreshold;
    QDoubleSpinBox *dsbThreshold;
    QLabel *lblState;
    QRadioButton *rdbActive;
    QRadioButton *rdbInactive;
    QWidget *pSynapsesParam;
    QLabel *lblBaseNeuron;
    QComboBox *cmbBaseNeuron;
    QLabel *label;
    QComboBox *cmbFinalNeuron;
    QDoubleSpinBox *dsbWeight;
    QLabel *lblWeight;
    QWidget *pUpdateSchedule;
    QLabel *lblUpdateBlock;
    QComboBox *cmbUpdateBlock;
    QLabel *lblNbIterations;
    QSpinBox *sbNbIterations;
    QLabel *lblScheduleType;
    QComboBox *cmbScheduleType;

    void setupUi(QWidget *NetworkDesignerClass)
    {
    if (NetworkDesignerClass->objectName().isEmpty())
        NetworkDesignerClass->setObjectName(QString::fromUtf8("NetworkDesignerClass"));
    NetworkDesignerClass->resize(783, 552);
    frmDesign = new QFrame(NetworkDesignerClass);
    frmDesign->setObjectName(QString::fromUtf8("frmDesign"));
    frmDesign->setGeometry(QRect(230, 80, 541, 461));
    frmDesign->setMouseTracking(true);
    frmDesign->setFrameShape(QFrame::StyledPanel);
    frmDesign->setFrameShadow(QFrame::Raised);
    toolBox = new QToolBox(NetworkDesignerClass);
    toolBox->setObjectName(QString::fromUtf8("toolBox"));
    toolBox->setGeometry(QRect(20, 80, 201, 421));
    pSimulationParam = new QWidget();
    pSimulationParam->setObjectName(QString::fromUtf8("pSimulationParam"));
    pSimulationParam->setGeometry(QRect(0, 0, 201, 297));
    lblTemperature = new QLabel(pSimulationParam);
    lblTemperature->setObjectName(QString::fromUtf8("lblTemperature"));
    lblTemperature->setGeometry(QRect(0, 50, 91, 18));
    dsbTemperature = new QDoubleSpinBox(pSimulationParam);
    dsbTemperature->setObjectName(QString::fromUtf8("dsbTemperature"));
    dsbTemperature->setGeometry(QRect(120, 50, 61, 27));
    dsbTemperature->setDecimals(3);
    dsbTemperature->setMinimum(-99.999);
    dsbTemperature->setMaximum(99.999);
    dsbTemperature->setSingleStep(0.001);
    dsbTemperature->setValue(1);
    sbUpdatesNumber = new QSpinBox(pSimulationParam);
    sbUpdatesNumber->setObjectName(QString::fromUtf8("sbUpdatesNumber"));
    sbUpdatesNumber->setGeometry(QRect(120, 90, 61, 27));
    sbUpdatesNumber->setMinimum(-99);
    sbUpdatesNumber->setValue(50);
    lblUpdatesNumber = new QLabel(pSimulationParam);
    lblUpdatesNumber->setObjectName(QString::fromUtf8("lblUpdatesNumber"));
    lblUpdatesNumber->setGeometry(QRect(0, 90, 111, 18));
    dsbSynchronyRate = new QDoubleSpinBox(pSimulationParam);
    dsbSynchronyRate->setObjectName(QString::fromUtf8("dsbSynchronyRate"));
    dsbSynchronyRate->setGeometry(QRect(120, 130, 61, 27));
    dsbSynchronyRate->setMaximum(1);
    dsbSynchronyRate->setSingleStep(0.001);
    dsbSynchronyRate->setValue(1);
    lblSynchronyRate = new QLabel(pSimulationParam);
    lblSynchronyRate->setObjectName(QString::fromUtf8("lblSynchronyRate"));
    lblSynchronyRate->setGeometry(QRect(0, 130, 101, 18));
    cmbSimulationType = new QComboBox(pSimulationParam);
    cmbSimulationType->setObjectName(QString::fromUtf8("cmbSimulationType"));
    cmbSimulationType->setGeometry(QRect(110, 10, 71, 26));
    label_2 = new QLabel(pSimulationParam);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    label_2->setGeometry(QRect(0, 10, 101, 18));
    pbStart = new QPushButton(pSimulationParam);
    pbStart->setObjectName(QString::fromUtf8("pbStart"));
    pbStart->setGeometry(QRect(0, 170, 80, 27));
    pbStop = new QPushButton(pSimulationParam);
    pbStop->setObjectName(QString::fromUtf8("pbStop"));
    pbStop->setGeometry(QRect(100, 170, 80, 27));
    toolBox->addItem(pSimulationParam, QApplication::translate("NetworkDesignerClass", "Simulation", 0, QApplication::UnicodeUTF8));
    pNeuronsParam = new QWidget();
    pNeuronsParam->setObjectName(QString::fromUtf8("pNeuronsParam"));
    pNeuronsParam->setGeometry(QRect(0, 0, 201, 297));
    lblThreshold = new QLabel(pNeuronsParam);
    lblThreshold->setObjectName(QString::fromUtf8("lblThreshold"));
    lblThreshold->setGeometry(QRect(0, 80, 71, 20));
    dsbThreshold = new QDoubleSpinBox(pNeuronsParam);
    dsbThreshold->setObjectName(QString::fromUtf8("dsbThreshold"));
    dsbThreshold->setGeometry(QRect(70, 80, 71, 27));
    dsbThreshold->setDecimals(3);
    dsbThreshold->setMinimum(-99.99);
    dsbThreshold->setSingleStep(0.001);
    lblState = new QLabel(pNeuronsParam);
    lblState->setObjectName(QString::fromUtf8("lblState"));
    lblState->setGeometry(QRect(0, 10, 57, 18));
    rdbActive = new QRadioButton(pNeuronsParam);
    rdbActive->setObjectName(QString::fromUtf8("rdbActive"));
    rdbActive->setGeometry(QRect(70, 10, 99, 23));
    rdbInactive = new QRadioButton(pNeuronsParam);
    rdbInactive->setObjectName(QString::fromUtf8("rdbInactive"));
    rdbInactive->setGeometry(QRect(70, 40, 71, 23));
    rdbInactive->setChecked(true);
    toolBox->addItem(pNeuronsParam, QApplication::translate("NetworkDesignerClass", "Neurons parameters", 0, QApplication::UnicodeUTF8));
    pSynapsesParam = new QWidget();
    pSynapsesParam->setObjectName(QString::fromUtf8("pSynapsesParam"));
    pSynapsesParam->setGeometry(QRect(0, 0, 201, 297));
    lblBaseNeuron = new QLabel(pSynapsesParam);
    lblBaseNeuron->setObjectName(QString::fromUtf8("lblBaseNeuron"));
    lblBaseNeuron->setGeometry(QRect(0, 10, 91, 21));
    cmbBaseNeuron = new QComboBox(pSynapsesParam);
    cmbBaseNeuron->setObjectName(QString::fromUtf8("cmbBaseNeuron"));
    cmbBaseNeuron->setGeometry(QRect(100, 10, 71, 26));
    label = new QLabel(pSynapsesParam);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(0, 60, 91, 21));
    cmbFinalNeuron = new QComboBox(pSynapsesParam);
    cmbFinalNeuron->setObjectName(QString::fromUtf8("cmbFinalNeuron"));
    cmbFinalNeuron->setGeometry(QRect(100, 60, 71, 26));
    dsbWeight = new QDoubleSpinBox(pSynapsesParam);
    dsbWeight->setObjectName(QString::fromUtf8("dsbWeight"));
    dsbWeight->setGeometry(QRect(101, 100, 71, 27));
    dsbWeight->setDecimals(3);
    dsbWeight->setMinimum(-99.99);
    dsbWeight->setSingleStep(0.001);
    dsbWeight->setValue(1);
    lblWeight = new QLabel(pSynapsesParam);
    lblWeight->setObjectName(QString::fromUtf8("lblWeight"));
    lblWeight->setGeometry(QRect(0, 100, 101, 20));
    toolBox->addItem(pSynapsesParam, QApplication::translate("NetworkDesignerClass", "Synapses parameters", 0, QApplication::UnicodeUTF8));
    pUpdateSchedule = new QWidget();
    pUpdateSchedule->setObjectName(QString::fromUtf8("pUpdateSchedule"));
    pUpdateSchedule->setGeometry(QRect(0, 0, 201, 297));
    lblUpdateBlock = new QLabel(pUpdateSchedule);
    lblUpdateBlock->setObjectName(QString::fromUtf8("lblUpdateBlock"));
    lblUpdateBlock->setGeometry(QRect(10, 40, 91, 18));
    cmbUpdateBlock = new QComboBox(pUpdateSchedule);
    cmbUpdateBlock->setObjectName(QString::fromUtf8("cmbUpdateBlock"));
    cmbUpdateBlock->setGeometry(QRect(120, 40, 71, 26));
    lblNbIterations = new QLabel(pUpdateSchedule);
    lblNbIterations->setObjectName(QString::fromUtf8("lblNbIterations"));
    lblNbIterations->setGeometry(QRect(10, 70, 101, 18));
    sbNbIterations = new QSpinBox(pUpdateSchedule);
    sbNbIterations->setObjectName(QString::fromUtf8("sbNbIterations"));
    sbNbIterations->setGeometry(QRect(120, 70, 71, 27));
    sbNbIterations->setMaximum(10000);
    sbNbIterations->setValue(50);
    lblScheduleType = new QLabel(pUpdateSchedule);
    lblScheduleType->setObjectName(QString::fromUtf8("lblScheduleType"));
    lblScheduleType->setGeometry(QRect(10, 10, 91, 18));
    cmbScheduleType = new QComboBox(pUpdateSchedule);
    cmbScheduleType->setObjectName(QString::fromUtf8("cmbScheduleType"));
    cmbScheduleType->setGeometry(QRect(120, 10, 71, 26));
    toolBox->addItem(pUpdateSchedule, QApplication::translate("NetworkDesignerClass", "Update schedule", 0, QApplication::UnicodeUTF8));

    retranslateUi(NetworkDesignerClass);

    toolBox->setCurrentIndex(3);


    QMetaObject::connectSlotsByName(NetworkDesignerClass);
    } // setupUi

    void retranslateUi(QWidget *NetworkDesignerClass)
    {
    NetworkDesignerClass->setWindowTitle(QApplication::translate("NetworkDesignerClass", "NetworkDesigner", 0, QApplication::UnicodeUTF8));
    lblTemperature->setText(QApplication::translate("NetworkDesignerClass", "Temperature:", 0, QApplication::UnicodeUTF8));
    lblUpdatesNumber->setText(QApplication::translate("NetworkDesignerClass", "Updates number:", 0, QApplication::UnicodeUTF8));
    lblSynchronyRate->setText(QApplication::translate("NetworkDesignerClass", "Synchrony rate:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("NetworkDesignerClass", "Simulation type:", 0, QApplication::UnicodeUTF8));
    pbStart->setText(QApplication::translate("NetworkDesignerClass", "&Start", 0, QApplication::UnicodeUTF8));
    pbStop->setText(QApplication::translate("NetworkDesignerClass", "St&op", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(pSimulationParam), QApplication::translate("NetworkDesignerClass", "Simulation", 0, QApplication::UnicodeUTF8));
    lblThreshold->setText(QApplication::translate("NetworkDesignerClass", "Threshold:", 0, QApplication::UnicodeUTF8));
    lblState->setText(QApplication::translate("NetworkDesignerClass", "State:", 0, QApplication::UnicodeUTF8));
    rdbActive->setText(QApplication::translate("NetworkDesignerClass", "Active", 0, QApplication::UnicodeUTF8));
    rdbInactive->setText(QApplication::translate("NetworkDesignerClass", "Inactive", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(pNeuronsParam), QApplication::translate("NetworkDesignerClass", "Neurons parameters", 0, QApplication::UnicodeUTF8));
    lblBaseNeuron->setText(QApplication::translate("NetworkDesignerClass", "Base neuron:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("NetworkDesignerClass", "Final neuron:", 0, QApplication::UnicodeUTF8));
    lblWeight->setText(QApplication::translate("NetworkDesignerClass", "Weight:", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(pSynapsesParam), QApplication::translate("NetworkDesignerClass", "Synapses parameters", 0, QApplication::UnicodeUTF8));
    lblUpdateBlock->setText(QApplication::translate("NetworkDesignerClass", "Update block:", 0, QApplication::UnicodeUTF8));
    lblNbIterations->setText(QApplication::translate("NetworkDesignerClass", "Number of Iterations:", 0, QApplication::UnicodeUTF8));
    lblScheduleType->setText(QApplication::translate("NetworkDesignerClass", "Type of schedule:", 0, QApplication::UnicodeUTF8));
    toolBox->setItemText(toolBox->indexOf(pUpdateSchedule), QApplication::translate("NetworkDesignerClass", "Update schedule", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(NetworkDesignerClass);
    } // retranslateUi

};

namespace Ui {
    class NetworkDesignerClass: public Ui_NetworkDesignerClass {};
} // namespace Ui

#endif // UI_NETWORKDESIGNER_H
