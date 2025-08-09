#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNetworks_triggered() {
    qDebug() << "Networks clicked";
    QWidget* pageToShow = ui->Networks;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionBad_AP_Emulator_2_triggered() {
    qDebug() << "Bad AP clicked";
    QWidget* pageToShow = ui->Bad_AP_Emulator_2;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionChannel_Graph_triggered() {
    qDebug() << "on_actionChannel_Graph_triggered clicked";
    QWidget* pageToShow = ui->Channel_Graph;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionPacket_Sniffer_triggered() {
    qDebug() << "on_actionPacket_Sniffer_triggered clicked";
    QWidget* pageToShow = ui->Packet_Sniffer;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionBeacon_Flood_triggered() {
    qDebug() << "on_actionBeacon_Flood_triggered clicked";
    QWidget* pageToShow = ui->Beacon_Flood;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionKarma_Attack_triggered() {
    qDebug() << "on_actionKarma_Attack_triggered clicked";
    QWidget* pageToShow = ui->Karma_Attack;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionMAC_Address_Spoofing_triggered() {
    qDebug() << "on_actionMAC_Address_Spoofing_triggered clicked";
    QWidget* pageToShow = ui->MAC_Address_Spoofing;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionPIN_Lock_triggered() {
    qDebug() << "on_actionPIN_Lock_triggered clicked";
    QWidget* pageToShow = ui->PIN_Lock;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionSelf_Destruct_triggered() {
    qDebug() << "on_actionSelf_Destruct_triggered clicked";
    QWidget* pageToShow = ui->Self_Destruct;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionBluetooth_BLE_Scan_triggered() {
    qDebug() << "on_actionBluetooth_BLE_Scan_triggered clicked";
    QWidget* pageToShow = ui->Bluetooth_BLE_Scan;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionRF_Jamming_triggered() {
    qDebug() << "on_actionRF_Jamming_triggered clicked";
    QWidget* pageToShow = ui->RF_Jamming;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionSpectrum_Analyzer_triggered() {
    qDebug() << "on_actionSpectrum_Analyzer_triggered clicked";
    QWidget* pageToShow = ui->Spectrum_Analyzer;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionAbout_Legal_triggered() {
    qDebug() << "on_actionAbout_Legal_triggered clicked";
    QWidget* pageToShow = ui->About_Legal;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionRFID_NFC_Settings_triggered() {
    qDebug() << "on_actionRFID_NFC_Settings_triggered clicked";
    QWidget* pageToShow = ui->RFID_NFC_Settings;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionTheme_UI_triggered() {
    qDebug() << "on_actionTheme_UI_triggered clicked";
    QWidget* pageToShow = ui->Theme_UI;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionWi_FI_Settings_triggered() {
    qDebug() << "on_actionWi_FI_Settings_triggered clicked";
    QWidget* pageToShow = ui->Wi_FI_Settings;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionHome_triggered() {
    qDebug() << "on_actionHome_triggered clicked";
    QWidget* pageToShow = ui->Home;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}

void MainWindow::on_actionRFID_NFC_triggered() {
    qDebug() << "on_actionRFID_NFC_triggered clicked";
    QWidget* pageToShow = ui->RFID_NFC;
    ui->stackedWidget->setCurrentWidget(pageToShow);
}
