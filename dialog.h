#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    bool getSerialPortConfig(void);
private slots:
//    void windowclosed();

//    void windowmin();

//    void winmax();

    void onSerialPortReadyRead();

    void on_pushBtnSerialOpen_clicked();

    void on_cBoxPresetBit_currentTextChanged(const QString &arg1);

    void on_pushBtnTranspost_clicked();

    void on_pushBtnSet_clicked();

    void on_pushBtnDelete_clicked();

    void on_pushBtnUpLeft_clicked();

    void on_pushBtnUp_clicked();

    void on_pushBtnUpRight_clicked();

    void on_pushBtnLeft_clicked();

    void on_pushBtnStop_clicked();

    void on_pushBtnRight_clicked();

    void on_pushBtnDownLeft_clicked();

    void on_pushBtnDown_clicked();

    void on_pushBtnDownRight_clicked();

    void on_pushBtnMultAdd_clicked();

    void on_pushBtnMultSub_clicked();

    void on_pushBtnZoomAdd_clicked();

    void on_pushBtnZoomSub_clicked();

    void on_pushBtnHaloAdd_clicked();

    void on_pushBtnHaloSub_clicked();

    void on_pushBtnLight_clicked();

    void on_pushBtnWiper_clicked();

    void on_pushBtnFreeAir_clicked();

    void on_pushBtnStartUp_clicked();

    void on_lnEditScanSpeed_textEdited(const QString &arg1);

    void on_cBoxLeftMargin_activated(const QString &arg1);

    void on_cBoxRightMargin_2_activated(const QString &arg1);

private:
    Ui::Dialog *ui;

    QSerialPort mSerialPort;       //串口
    bool mIsSerialOpen;
    QString mPortName;
    QString mBaudRate;
    QString mParity;
    QString mDataBits;
    QString mStopBits;
    QByteArray charToHex(QByteArray chr);
    void send_cmd(void *dat, unsigned int len);

    QString preset_num;        //预置位
    QString scan_speed;        //线性扫描速度

};
#endif // DIALOG_H
