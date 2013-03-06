#ifndef _IMAGE_H
#define _IMAGE_H

#include <hidef.h>
#include "derivative.h"
#include  "cardef.h"

#define IMAGE_MAX_LINE 60
#define IMAGE_MAX_PIXEL 60

#define BLACK 0x00
#define WHITE 0xff

typedef uchar Image[IMAGE_MAX_LINE*IMAGE_MAX_PIXEL];

extern Image image_image;

/*gather*/
extern char image_gather_is_start;
extern char image_gather_is_finish;
extern int image_gather_line_index;

void image_Gather_VSYN(Timer_Channel channel,int time);
void image_Gather_HREFF(Timer_Channel channel,int time);

/*handle*/
extern int image_roade[IMAGE_MAX_LINE];
extern char image_roade_over_is;
extern char image_roade_bend_is;
extern char image_roade_end_is;

extern Image* image_Get_image();
extern int image_Get_roade();

void image_Image_to_white();
void image_Get_middle_line();
int image_Get_middle_point(int line);
int image_Get_roade_width(int line,int point,int *start,int *end);
void image_Smoothing();
void image_Get_KB(int line,int *k,int *b);
char image_Check_end(int line);

#endif