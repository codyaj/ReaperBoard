#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void on_actionNetworks_triggered();
    void on_actionBad_AP_Emulator_2_triggered();
    void on_actionChannel_Graph_triggered();
    void on_actionPacket_Sniffer_triggered();
    void on_actionBeacon_Flood_triggered();
    void on_actionKarma_Attack_triggered();
    void on_actionMAC_Address_Spoofing_triggered();
    void on_actionPIN_Lock_triggered();
    void on_actionSelf_Destruct_triggered();
    void on_actionBluetooth_BLE_Scan_triggered();
    void on_actionRF_Jamming_triggered();
    void on_actionSpectrum_Analyzer_triggered();
    void on_actionAbout_Legal_triggered();
    void on_actionRFID_NFC_Settings_triggered();
    void on_actionTheme_UI_triggered();
    void on_actionWi_FI_Settings_triggered();
    void on_actionHome_triggered();
    void on_actionRFID_NFC_triggered();
};


#endif // MAINWINDOW_H
