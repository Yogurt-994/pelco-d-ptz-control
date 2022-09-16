#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QToolButton>
#include "protocol.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

//    int wide = width();//获取界面的宽度

//    //构建最小化、最大化、关闭按钮
//    QToolButton *minButton = new QToolButton(this);
//    QToolButton *closeButton= new QToolButton(this);
//    QToolButton *maxButton= new QToolButton(this);

//    //获取最小化、关闭按钮图标
//    QPixmap minPix  = style()->standardPixmap(QStyle::SP_TitleBarMinButton);
//    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
//    QPixmap maxPix = style()->standardPixmap(QStyle::SP_TitleBarMaxButton);

//    //设置最小化、关闭按钮图标
//    minButton->setIcon(minPix);
//    closeButton->setIcon(closePix);
//    maxButton->setIcon(maxPix);

//    //设置最小化、关闭按钮在界面的位置
//    minButton->setGeometry(wide-65,5,20,20);
//    closeButton->setGeometry(wide-25,5,20,20);
//    maxButton->setGeometry(wide-45,5,20,20);

//    //设置鼠标移至按钮上的提示信息
//    minButton->setToolTip(tr("最小化"));
//    closeButton->setToolTip(tr("关闭"));
//    maxButton->setToolTip(tr("最大化"));

//    //设置最小化、关闭按钮的样式
//    minButton->setStyleSheet("background-color:transparent;");
//    closeButton->setStyleSheet("background-color:transparent;");
//    maxButton->setStyleSheet("background-color:transparent;");

//    connect(closeButton, SIGNAL(clicked()), this, SLOT(windowclosed()) );
//    connect(minButton, SIGNAL(clicked()), this, SLOT(windowmin()));
//    connect(maxButton, SIGNAL(clicked()), this, SLOT(winmax()));

    //to do work
    mIsSerialOpen = false;

    //识别当前系统的串口号
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
    int count = serialPortInfo.count();
    for(int i = 0;i < count;i++){
        ui->cBoxUart->addItem(serialPortInfo.at(i).portName());       //添加串口号信息至界面
    }

    preset_num = ui->cBoxPresetBit->currentText();                    //预置点

    ui->pushBtnStartUp->setEnabled(false);

    connect(&mSerialPort,SIGNAL(readyRead()),this,SLOT(onSerialPortReadyRead()));

}

Dialog::~Dialog()
{
    delete ui;
}

//void Dialog::windowclosed()
//{
//    //QApplication::exit();
//    this->close();
//}
//void Dialog::windowmin()
//{
//    this->showMinimized();
//}
//void Dialog::winmax()
//{
//    this->showMaximized();
//}

QByteArray Dialog::charToHex(QByteArray chr)
{
    QByteArray tempHex = chr.toHex().toUpper();
    QByteArray resultHex;

    for (int i = 0; i < tempHex.length() / 2; i++) {
        resultHex += tempHex.mid(i * 2, 2) + " ";
    }

    return resultHex;
}

void Dialog::send_cmd(void *dat, unsigned int len)
{
    unsigned char *data;

    data = (unsigned char *)dat;

    if(mIsSerialOpen){
       mSerialPort.write((const char *)data,(qint64)len);
    }

    free(dat);
}

//获取串口配置
bool Dialog::getSerialPortConfig()
{
    //获取串口信息配置
    mPortName = ui->cBoxUart->currentText();
    mBaudRate = ui->cBoxBaudRate->currentText();
    mParity = ui->cBoxParity->currentText();
    mDataBits = ui->cBoxDataBit->currentText();
    mStopBits = ui->cBoxStopBit->currentText();

    //设置串口配置
    mSerialPort.setPortName(mPortName);     //串口名

    if("9600" == mBaudRate){                //串口波特率
        mSerialPort.setBaudRate(QSerialPort::Baud9600);
    }
    else if("19200" == mBaudRate){
        mSerialPort.setBaudRate(QSerialPort::Baud19200);
    }
    else if("115200" == mBaudRate){
        mSerialPort.setBaudRate(QSerialPort::Baud115200);
    }

    if("NONE" == mParity){                  //串口校验位
        mSerialPort.setParity(QSerialPort::NoParity);
    }
    else if("ODD" == mParity){
        mSerialPort.setParity(QSerialPort::OddParity);
    }
    else if("EVEN" == mParity){
        mSerialPort.setParity(QSerialPort::EvenParity);
    }
    else if("MARK" == mParity){
        mSerialPort.setParity(QSerialPort::MarkParity);
    }
    else if("SPACE" == mParity){
        mSerialPort.setParity(QSerialPort::SpaceParity);
    }

    if("5" == mDataBits){                   //串口数据位
        mSerialPort.setDataBits(QSerialPort::Data5);
    }
    else if("6" == mDataBits){
        mSerialPort.setDataBits(QSerialPort::Data6);
    }
    else if("7" == mDataBits){
        mSerialPort.setDataBits(QSerialPort::Data7);
    }
    else if("8" == mDataBits){
        mSerialPort.setDataBits(QSerialPort::Data8);
    }

    if("1" == mStopBits){                   //串口停止位
        mSerialPort.setStopBits(QSerialPort::OneStop);
    }
    else if("1.5" == mStopBits){
        mSerialPort.setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if("2" == mStopBits){
        mSerialPort.setStopBits(QSerialPort::TwoStop);
    }

    return mSerialPort.open(QSerialPort::ReadWrite);        //返回
}

void Dialog::onSerialPortReadyRead()
{
    if(mIsSerialOpen){
        QByteArray recvData = mSerialPort.readAll();
        if(!recvData.isEmpty()){
            qDebug()<<"Data recv:"<<recvData;
        }
    }
}

void Dialog::on_pushBtnSerialOpen_clicked()
{
    if(true == mIsSerialOpen){
        mSerialPort.close();       //当前已经打开一个串口，这是执行关闭的操作
        ui->pushBtnSerialOpen->setText("打开");
        mIsSerialOpen = false;
        qDebug()<<"已经打开串口";
    }
    else{
        //当前没有打开串口，执行打开串口的工作
        if(true == getSerialPortConfig()){
            mIsSerialOpen = true;
            ui->pushBtnSerialOpen->setText("关闭");
            qDebug()<<"成功打开串口"<<mPortName;
        }
        else{
            ui->pushBtnSerialOpen->setText("打开");
            mIsSerialOpen = false;
        }
    }
}

void Dialog::on_cBoxPresetBit_currentTextChanged(const QString &arg1)
{
    preset_num = arg1;
//    qDebug()<< preset_num;
}

void Dialog::on_pushBtnTranspost_clicked()
{
    unsigned char presetNum;
    bool ok;
    presetNum = preset_num.mid(0,2).toInt(&ok,16);
//    qDebug()<< preset_num;
    send_cmd((void *)protocol_goto_preset(presetNum),PELCO_D_LEN);
}

void Dialog::on_pushBtnSet_clicked()
{
    unsigned char presetNum;
    bool ok;
    presetNum = preset_num.mid(0,2).toInt(&ok,16);
//    qDebug()<< preset_num;
    send_cmd((void *)protocol_set_preset(presetNum),PELCO_D_LEN);
}

void Dialog::on_pushBtnDelete_clicked()
{
    unsigned char presetNum;
    bool ok;
    presetNum = preset_num.mid(0,2).toInt(&ok,16);
//    qDebug()<< preset_num;
    send_cmd((void *)protocol_delete_preset(presetNum),PELCO_D_LEN);
}

void Dialog::on_pushBtnUpLeft_clicked()
{
    send_cmd((void *)protocol_upleft(),PELCO_D_LEN);
}

void Dialog::on_pushBtnUp_clicked()
{
    send_cmd((void *)protocol_up(),PELCO_D_LEN);
}

void Dialog::on_pushBtnUpRight_clicked()
{
    send_cmd((void *)protocol_upright(),PELCO_D_LEN);
}

void Dialog::on_pushBtnLeft_clicked()
{
    send_cmd((void *)protocol_left(),PELCO_D_LEN);
}

void Dialog::on_pushBtnStop_clicked()
{
    send_cmd((void *)protocol_stop(),PELCO_D_LEN);
}

void Dialog::on_pushBtnRight_clicked()
{
    send_cmd((void *)protocol_right(),PELCO_D_LEN);
}

void Dialog::on_pushBtnDownLeft_clicked()
{
    send_cmd((void *)protocol_downleft(),PELCO_D_LEN);
}

void Dialog::on_pushBtnDown_clicked()
{
    send_cmd((void *)protocol_down(),PELCO_D_LEN);
}

void Dialog::on_pushBtnDownRight_clicked()
{
    send_cmd((void *)protocol_downright(),PELCO_D_LEN);
}


//变倍+
void Dialog::on_pushBtnMultAdd_clicked()
{
    send_cmd((void *)protocol_zoom_up(),PELCO_D_LEN);
}

//变倍-
void Dialog::on_pushBtnMultSub_clicked()
{
    send_cmd((void *)protocol_zoom_down(),PELCO_D_LEN);
}

//变焦+
void Dialog::on_pushBtnZoomAdd_clicked()
{
    send_cmd((void *)protocol_focal_up(),PELCO_D_LEN);
}

//变焦-
void Dialog::on_pushBtnZoomSub_clicked()
{
    send_cmd((void *)protocol_focal_down(),PELCO_D_LEN);
}

void Dialog::on_pushBtnHaloAdd_clicked()
{
    send_cmd((void *)protocol_halo_up(),PELCO_D_LEN);
}

void Dialog::on_pushBtnHaloSub_clicked()
{
    send_cmd((void *)protocol_halo_down(),PELCO_D_LEN);
}

void Dialog::on_pushBtnLight_clicked()
{
    if(ui->pushBtnLight->text() == "打开"){
        send_cmd((void *)protocol_light_on(),PELCO_D_LEN);
//        qDebug()<<"灯光打开";
        ui->pushBtnLight->setText("关闭");
    }
    else if(ui->pushBtnLight->text() == "关闭"){
        send_cmd((void *)protocol_ligtt_off(),PELCO_D_LEN);
//        qDebug()<<"灯光关闭";
        ui->pushBtnLight->setText("打开");
    }
}

void Dialog::on_pushBtnWiper_clicked()
{
    if(ui->pushBtnWiper->text() == "打开"){
        send_cmd((void *)protocol_wiper_on(),PELCO_D_LEN);
//        qDebug()<<"雨刷打开";
        ui->pushBtnWiper->setText("关闭");
    }
    else if(ui->pushBtnWiper->text() == "关闭"){
        send_cmd((void *)protocol_wiper_off(),PELCO_D_LEN);
//        qDebug()<<"雨刷关闭";
        ui->pushBtnWiper->setText("打开");
    }
}

void Dialog::on_pushBtnFreeAir_clicked()
{
    if(ui->pushBtnFreeAir->text() == "打开"){
        send_cmd((void *)protocol_auto_scan_run(),PELCO_D_LEN);
//        qDebug()<<"自动扫描启动";
        ui->pushBtnFreeAir->setText("关闭");
    }
    else if(ui->pushBtnFreeAir->text() == "关闭"){
        send_cmd((void *)protocol_stop(),PELCO_D_LEN);
//        qDebug()<<"自动扫描关闭";
        ui->pushBtnFreeAir->setText("打开");
    }
}

void Dialog::on_pushBtnStartUp_clicked()
{
    unsigned char scanID;
    bool ok;
    scanID = ui->cBoxID->currentText().mid(0,2).toInt(&ok,16);

    if(ui->pushBtnStartUp->text() == "打开"){
        send_cmd((void *)protocol_set_scan_start(scanID),PELCO_D_LEN);
        qDebug()<<"scan start "<<scanID;
        ui->pushBtnStartUp->setText("关闭");
    }
    else if(ui->pushBtnStartUp->text() == "关闭"){
        send_cmd((void *)protocol_stop_line_scan(scanID),PELCO_D_LEN);
        ui->pushBtnStartUp->setText("打开");
    }
}

void Dialog::on_lnEditScanSpeed_textEdited(const QString &arg1)
{
    if(!ui->lnEditScanSpeed->text().isEmpty()){
        ui->pushBtnStartUp->setEnabled(true);
        scan_speed = arg1;

        unsigned char scanSpeed,scanID;
        bool ok;
        scanSpeed = scan_speed.mid(0,2).toInt(&ok,16);
        scanID = ui->cBoxID->currentText().mid(0,2).toInt(&ok,16);
        send_cmd((void *)protocol_set_line_speed(scanSpeed,scanID),PELCO_D_LEN);
        qDebug()<<"textEdit"<<scanSpeed;
    }
    else{
        ui->pushBtnStartUp->setEnabled(false);
    }
}

void Dialog::on_cBoxLeftMargin_activated(const QString &arg1)
{
    unsigned char leftID;
    bool ok;
    leftID = arg1.mid(0,2).toInt(&ok,16);
    qDebug()<<leftID;
    send_cmd((void *)protocol_set_left_border(leftID),PELCO_D_LEN);
}

void Dialog::on_cBoxRightMargin_2_activated(const QString &arg1)
{
    unsigned char rightID;
    bool ok;
    rightID = arg1.mid(0,2).toInt(&ok,16);
    qDebug()<<rightID;
    send_cmd((void *)protocol_set_right_border(rightID),PELCO_D_LEN);
}
