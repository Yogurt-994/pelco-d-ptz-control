/*
 * @date :2022.9.7
 * @author :Yogurt
 */
#include <stdlib.h>
#include "protocol.h"

//校验码
static unsigned char check_sum(unsigned char* pak){
    int i;
    unsigned char sum = 0;

    for(i = 0;i < 5;i++){
        sum += pak[i];
    }
    pak[i] = sum;

    return sum;
}


//指令集
static struct pelco_d *pack_cmd(unsigned char cmd1,unsigned char cmd2,unsigned char dat1,unsigned char dat2){
    struct pelco_d *package;

    package = (struct pelco_d *)malloc(sizeof(struct pelco_d));
    if(package == NULL)
        return NULL;

    package->sync = 0xff;
    package->cam_addr = 0x01;
    package->cmd1 = cmd1;
    package->cmd2 = cmd2;
    package->dat1 = dat1;
    package->dat2 = dat2;
    check_sum((unsigned char *)package + 1);    //校验码检测

    return package;
}

void free_pack(struct pelco_d *pack){
    if(pack != NULL){
        free(pack);
        pack = NULL;
    }
}

//云台控制：停止
struct pelco_d *protocol_stop(){
    return pack_cmd(0,0,0,0);
}

//云台控制：上
struct pelco_d *protocol_up(){
    return pack_cmd(0,C2UP,0,D2UP_DOWN);
}

//云台控制：下
struct pelco_d *protocol_down(){
    return pack_cmd(0,C2DOWN,0,D2UP_DOWN);
}

//云台控制：左
struct pelco_d *protocol_left(){
    return pack_cmd(0,C2LEFT,D1LE_RIG,0);
}

//云台控制：右
struct pelco_d *protocol_right(){
    return pack_cmd(0,C2RIGHT,D1LE_RIG,0);
}

//云台控制：左上
struct pelco_d *protocol_upleft(){
    return pack_cmd(0,C2UPLEFT,0x0f,0x0f);
}

//云台控制：右上
struct pelco_d *protocol_upright(){
    return pack_cmd(0,C2UPRIGHT,0x0f,0x0f);
}

//云台控制：左下
struct pelco_d *protocol_downleft(){
    return pack_cmd(0,C2DOWMLEFT,0x0f,0x0f);
}

//云台控制：右下
struct pelco_d *protocol_downright(){
    return pack_cmd(0,C2DOWNRIGHT,0x0f,0x0f);
}

//云台控制：变倍+
struct pelco_d *protocol_zoom_up(){
    return  pack_cmd(0,C2ZOOMUP,0,0);
}

//云台控制：变倍-
struct pelco_d *protocol_zoom_down(){
    return pack_cmd(0,C2ZOOMDOWN,0,0);
}

//云台控制：变焦+
struct pelco_d *protocol_focal_up(){
    return pack_cmd(C1FOCALUP,0,0,0);
}

//云台控制：变焦-
struct pelco_d *protocol_focal_down(){
    return pack_cmd(0,C2FOCALDOWN,0,0);
}

//云台控制：光圈+
struct pelco_d *protocol_halo_up(){
    return pack_cmd(C1HAOLUP,0,0,0);
}

//云台控制：光圈-
struct pelco_d *protocol_halo_down(){
    return pack_cmd(C1HALODOWN,0,0,0);
}

//辅助开关：灯光开
struct pelco_d *protocol_light_on(){
    return pack_cmd(0,C2LIGHT_WIPERON,0,D2LIGHT);
}

//辅助开关：灯光关
struct pelco_d *protocol_ligtt_off(){
    return pack_cmd(0,C2LIGHT_WIPEROFF,0,D2LIGHT);
}

//辅助开关：雨刷开
struct pelco_d *protocol_wiper_on(){
    return pack_cmd(0,C2LIGHT_WIPERON,0,D2WIPER);
}

//辅助开关：雨刷关
struct pelco_d *protocol_wiper_off(){
    return pack_cmd(0,C2LIGHT_WIPEROFF,0,D2WIPER);
}

//预置点：转置预置点
struct pelco_d *protocol_goto_preset(unsigned char num){
    return pack_cmd(0,C2GOTOPRESET,0,num);
}

//预置点：设置预置点
struct pelco_d *protocol_set_preset(unsigned char num){
    return pack_cmd(0,C2SETPRESET,0,num);
}

//预置点：删除预置点
struct pelco_d *protocol_delete_preset(unsigned char num){
    return pack_cmd(0,C2DELPRESET,0,num);
}

//自动扫描：启动
struct pelco_d *protocol_auto_scan_run(){
    return pack_cmd(0,C2AUTOSCANRUN,0,D2AUTOSCANRUN);
}

//线性扫描：设置左边界
struct pelco_d *protocol_set_left_border(unsigned char id){
    return pack_cmd(0x20,C2SETLEFTBORDER,0,id);
}

//线性扫描：设置右边界
struct pelco_d *protocol_set_right_border(unsigned char id){
    return pack_cmd(0x20,C2SETRIGHTBORDER,0,id);
}

//线性扫描：线性扫描启动
struct pelco_d *protocol_set_scan_start(unsigned char id){
    return pack_cmd(0x20,C2LINESCANRUN,0,id);
}

//线性扫描：停止
struct pelco_d *protocol_stop_line_scan(unsigned char id){
    return pack_cmd(0x20,C2LINESCANSTOP,0,id);
}

//线性扫描：速度设置
struct pelco_d *protocol_set_line_speed(unsigned char speed,unsigned char id){
    return pack_cmd(0x20,C2LINESPEEDSET,speed,id);
}

