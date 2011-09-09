/*---------------------------------
* FILE Name: videoitem.h
*
* Description: like its name, Hisoft ListBox
*
* COPYRIGHT
*
* Created on: 2011.9.7 Author: zhang.shenhua
*
* Modified by zsh on 2011.9.7
*---------------------------------------------------------------------*/
#ifndef _VIDEOITEM_H_
#define _VIDEOITEM_H_

#include "defines.h"
#include "hwidget.h"
#include "hcontainer.h"

typedef char VID[20];				/* video id*/
typedef HWidget		Item;
typedef VideoItem_  VideoItem;
typedef VideoItem   VideoItem1;
typedef VideoItem   VideoItem2;

enum VideoType {NONE_TYPE,COPYRIGHT,NO_COPYRIGHT};

struct VideoItem_ {
	HContainer*	 p_base_container;
	VID			 vid;
};

extern VideoItem* VideoItem_new	( VID vid, HImage* p_himg_video, char* cstr_title, float f_starlevel); 
extern VideoItem* VideoItem1_new( VideoItem* p_vitem, char* csrt_playedtimes); // make vitem of copyright
extern VideoItem* VideoItem2_new( VideoItem* p_vitem, char* cstr_timelong); // make vitem of none copyright

extern VideoItem_delete( VideoItem* p_it );


#endif