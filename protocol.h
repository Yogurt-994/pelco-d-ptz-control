/*
 * @ date :2022.9.7
 * @ author :Yogurt
*/

/*
PELCO_D 协议
https://www.pelco.com/
https://www.commfront.com/pages/pelco-d-protocol-tutorial
PELCO_P 协议
https://www.commfront.com/pages/pelco-p-protocol-tutorial
VISCA 协议
https://www.epiphan.com/userguides/LUMiO12x/Content/UserGuides/PTZ/3-operation/rs232.htm

消息格式：
+-----------+---------+-----------+-----------+--------+--------+----------+
|  Byte 1   | Byte 2  |  Byte 3   |  Byte 4   | Byte 5 | Byte 6 |  Byte 7  |
+-----------+---------+-----------+-----------+--------+--------+----------+
| Sync Byte | Address | Command 1 | Command 2 | Data 1 | Data 2 | Checksum |
+-----------+---------+-----------+-----------+--------+--------+----------+

标准命令集：
.-----------.-----------.-----------.-----------.------------------.---------------.------------.-----------.------------.
|           |   Bit 7   |   Bit 6   |   Bit 5   |      Bit 4       |     Bit 3     |   Bit 2    |   Bit 1   |   Bit 0    |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 1 | Sense     | Reserved  | Reserved  | Auto/Manual Scan | Camera On/Off | Iris Close | Iris Open | Focus Near |
:-----------+-----------+-----------+-----------+------------------+---------------+------------+-----------+------------:
| Command 2 | Focus Far | Zoom Wide | Zoom Tele | Down             | Up            | Left       | Right     | Always 0   |
'-----------'-----------'-----------'-----------'------------------'---------------'------------'-----------'------------'
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* start cmd1 */
#define C1FOCALUP    0x01
#define C1HAOLUP     0x02
#define C1HALODOWN   0x04

/* end cmd1 */

/* start cmd2 */
#define C2UP         0x08
#define C2DOWN       0x10
#define C2LEFT       0x04
#define C2RIGHT      0x02
#define C2UPLEFT     0x0c
#define C2UPRIGHT    0x0a
#define C2DOWMLEFT   0x14
#define C2DOWNRIGHT  0x12
#define C2ZOOMUP     0x20
#define C2ZOOMDOWN   0x40
#define C2FOCALDOWN  0x80

#define C2LIGHT_WIPERON    0x09
#define C2LIGHT_WIPEROFF   0x0b

#define C2GOTOPRESET       0x07
#define C2SETPRESET        0x03
#define C2DELPRESET        0x05

#define C2AUTOSCANRUN      0x07

#define C2SETLEFTBORDER    0x09
#define C2SETRIGHTBORDER   0x0b
#define C2LINESCANRUN      0x0d
#define C2LINESCANSTOP     0x0f
#define C2LINESPEEDSET     0x07
/* end cmd2 */

/* start dat1 */
#define D1LE_RIG     0xff
/* end dat1 */

/* start dat2 */
#define D2UP_DOWN    0xff
#define D2LIGHT      0x02
#define D2WIPER      0x01

#define D2AUTOSCANRUN   0x63
/* end dat2 */

struct __attribute__((packed)) pelco_d{
    unsigned char sync;     //同步字节
    unsigned char cam_addr; //地址码
    unsigned char cmd1;
    unsigned char cmd2;
    unsigned char dat1;
    unsigned char dat2;
    unsigned char check_sum;//校验码
};

#define PELCO_D_LEN sizeof(struct pelco_d)

extern void free_pack(struct pelco_d *pack);
extern struct pelco_d *protocol_stop();
extern struct pelco_d *protocol_up();
extern struct pelco_d *protocol_down();
extern struct pelco_d *protocol_left();
extern struct pelco_d *protocol_right();
extern struct pelco_d *protocol_upleft();
extern struct pelco_d *protocol_upright();
extern struct pelco_d *protocol_downleft();
extern struct pelco_d *protocol_downright();

extern struct pelco_d *protocol_zoom_up();      //变倍+
extern struct pelco_d *protocol_zoom_down();    //变倍-
extern struct pelco_d *protocol_focal_up();     //变焦+
extern struct pelco_d *protocol_focal_down();   //变焦-
extern struct pelco_d *protocol_halo_up();
extern struct pelco_d *protocol_halo_down();

extern struct pelco_d *protocol_light_on();     //辅助开关操作
extern struct pelco_d *protocol_ligtt_off();
extern struct pelco_d *protocol_wiper_on();
extern struct pelco_d *protocol_wiper_off();

extern struct pelco_d *protocol_goto_preset(unsigned char num);
extern struct pelco_d *protocol_set_preset(unsigned char num);
extern struct pelco_d *protocol_delete_preset(unsigned char num);

extern struct pelco_d *protocol_auto_scan_run();                        //自动扫描开启

extern struct pelco_d *protocol_set_left_border(unsigned char id);      //线性扫描操作
extern struct pelco_d *protocol_set_right_border(unsigned char id);
extern struct pelco_d *protocol_set_scan_start(unsigned char id);
extern struct pelco_d *protocol_stop_line_scan(unsigned char id);
extern struct pelco_d *protocol_set_line_speed(unsigned char speed,unsigned char id);


#endif // PROTOCOL_H
