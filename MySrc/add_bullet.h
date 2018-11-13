#ifndef __ADD_BULLET_H
#define __ADD_BULLET_H
#ifdef __cplusplus
#include "bsp.h"
extern u8 timemark,bumark,bu_mark,vmark,redmark,lmark,rmark,cir_mark;
extern u16 rx,rrx,ry,rry,rth,rrth;
void add_bullet(CanRxMsg *msg_rece);
void send_cmd();
void rece_usart(u8*);
#endif
#endif
