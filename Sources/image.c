#include "base.h"
#include "core.h"
#include "image.h"

Image image_image;

/*gather*/
char image_gather_is_start=0;
char image_gather_is_finish=1;
int image_gather_line_index=0;

//场中断
void image_Gather_VSYN(Timer_Channel channel,int time){
    int i=0;
    if(image_gather_is_start && image_gather_is_finish){
        image_gather_line_index=0;
        base_Timer_set(TIMER_CHANNEL4,TIMER_OPEN,0,image_Gather_HREFF);
        image_gather_is_finish=0;  
    }else{
        base_Timer_set(TIMER_CHANNEL4,TIMER_CLOSE,0,0);
        base_Timer_set(TIMER_CHANNEL3,TIMER_CLOSE,0,0);
        image_gather_is_start=0;
        image_gather_is_finish=1;
    }
}

//行中断
void image_Gather_HREFF(Timer_Channel channel,int time){
    int line=0,pixel=0;
    if(image_gather_line_index%2==0 && image_gather_line_index<IMAGE_MAX_LINE*2){
        line=image_gather_line_index/2;
        for(pixel=0;pixel<40;pixel++){
            asm nop;
        }
        for(pixel=0;pixel<IMAGE_MAX_PIXEL;pixel++){
            image_image[line*IMAGE_MAX_PIXEL+pixel]=base_Port_read(A);
            asm nop;
            asm nop;
            asm nop;
        }
    }
    image_gather_line_index++;
}

/*handle*/
int image_roade_start_middle_point=29;
int image_roade_left[IMAGE_MAX_LINE];
int image_roade_right[IMAGE_MAX_LINE]; 
int image_roade[IMAGE_MAX_LINE];
char image_roade_over_is=0;
char image_roade_bend_is=0;
char image_roade_end_is=0;

//从摄像头读取图像数据
Image* image_Get_image(){
    image_gather_is_start=1;
    base_Port_set(A,0x00);
    base_Timer_set(TIMER_CHANNEL3,TIMER_OPEN,0,image_Gather_VSYN);
    while(image_gather_is_start || !image_gather_is_finish);
    return &image_image;
}

//获取跑道
int image_Get_roade(){
    image_Get_image();
    memset(image_roade,0,IMAGE_MAX_LINE*sizeof(int));
    memset(image_roade_left,0,IMAGE_MAX_LINE*sizeof(int));
    memset(image_roade_right,60,IMAGE_MAX_LINE*sizeof(int));
    image_Image_to_white();
    //image_Filtering();
    image_Get_middle_line();
    return Error(ERROR_SUCCED);
}

//二值化
void image_Image_to_white(){
    int i=0;
    for(i=0;i<3600;i++){
        image_image[i]=image_image[i]<0x57 ? BLACK : WHITE;
    }
}

//获取中心线
void image_Get_middle_line(){
    int i=0,mp=0,start=0,end=0;
    image_roade_bend_is=1;
    image_roade_end_is=0;
    if((mp=image_Get_middle_point(IMAGE_MAX_LINE-1))>=0){
        image_roade_bend_is=0;
        for(i=IMAGE_MAX_LINE-1;i>=0;i--){
           mp=image_Get_roade_width(i,mp,&start,&end);
           image_roade_left[IMAGE_MAX_LINE-i-1]=start;
           image_roade_right[IMAGE_MAX_LINE-i-1]=end;
           if(mp==0){
              if(!image_Check_end(i)){
                  image_roade_bend_is=1;
                  break;
              }
           }
           if(i<IMAGE_MAX_LINE-1 && i>IMAGE_MAX_LINE-20){
              if(start-image_roade_left[IMAGE_MAX_LINE-i-2]>8 || image_roade_right[IMAGE_MAX_LINE-i-2]-end>8) image_Check_end(i); 
           }
           mp=start+mp/2;
           image_roade[IMAGE_MAX_LINE-i-1]=mp;
        }
        image_roade_over_is=0; 
        image_roade_start_middle_point=image_roade[0];
        
        image_Smoothing();
        return;
    }
    image_roade_over_is=1;
    image_roade_start_middle_point=29;
}

//第一行中心线推算
int image_Get_middle_point(int line){
    int i=image_roade_start_middle_point,mp=image_roade_start_middle_point;
    if(image_image[line*IMAGE_MAX_PIXEL+i]==BLACK){
        mp=-1;
        while(i>=0 || (2*image_roade_start_middle_point-i)<IMAGE_MAX_PIXEL){
            if(i>=0){
                if(image_image[line*IMAGE_MAX_PIXEL+i]==WHITE){
                    mp=i;
                    break;
                }
            }
            if((2*image_roade_start_middle_point-i)<IMAGE_MAX_PIXEL){
                if(image_image[line*IMAGE_MAX_PIXEL+2*image_roade_start_middle_point-i]==WHITE){
                    mp=2*image_roade_start_middle_point-i;
                    break;
                }
            }
            i--;
        }
    }
    return mp;
}

//某行中心线推算
int image_Get_roade_width(int line,int point,int *start,int *end){
    int i=0,len=0;
    i=point;
    while(i>=0){
       if(image_image[line*IMAGE_MAX_PIXEL+i]!=WHITE) break;
       len++;
       i--; 
    }
    *start=i;
    i=point;
    while(i<IMAGE_MAX_PIXEL){
       if(image_image[line*IMAGE_MAX_PIXEL+i]!=WHITE) break;
       len++;
       i++;
    }
    *end=i;
    return len;
}
                                                    
//中心线平滑
void image_Smoothing(){
    int i=0,k=0,b=0; 
    if(image_roade_right[0]-image_roade_left[0]<55){
        if(image_roade_left[0]<1){
            image_roade[0]-=(55-image_roade_right[0]+image_roade_left[0])/2;
        } else if(image_roade_right[0]>(IMAGE_MAX_LINE-1)){
            image_roade[0]+=(55-image_roade_right[0]+image_roade_left[0])/2;
        }
    }
    
    for(i=1;i<IMAGE_MAX_LINE;i++){
        if(image_roade_left[i]<=0 && image_roade_right[i]<(IMAGE_MAX_PIXEL-1)){
            image_roade[i]=image_roade[i-1]+image_roade_right[i]-image_roade_right[i-1];
        }else if(image_roade_left[i]>1 && image_roade_right[i]>=(IMAGE_MAX_PIXEL-1)){
            image_roade[i]=image_roade[i-1]+image_roade_left[i]-image_roade_left[i-1];
        }
        
        if(image_roade[i]==0){
            image_Get_KB(i,&k,&b);
            image_roade[i]=image_roade[i-1]+b;
        }
    }
}

void image_Get_KB(int line,int *k,int *b){
    if(line==1){
        *k=0;
        *b=image_roade[0];
    } else if(line==2){
        *k=0;
        *b=image_roade[1]-image_roade[0];
    }else if(line==3){
        *k=0;
        *b=((image_roade[1]-image_roade[0])+(image_roade[2]-image_roade[1]))/2;
    }else{
        *k=0;
        *b=((image_roade[line-3]-image_roade[line-4])+(image_roade[line-2]-image_roade[line-3])+(image_roade[line-1]-image_roade[line-2]))/3;
    }
}

char image_Check_end(int line){
    int i=0,ws1=0,bs1=0,ws2=0,bs2=0,ws3=0,bs3=0;
    char f=0;
    int rl=IMAGE_MAX_LINE-line-1;
    if(line>=IMAGE_MAX_PIXEL-1) return 0;
    for(i=image_roade_left[rl-1];i<image_roade_right[rl-1];i++){
        if(image_image[line*IMAGE_MAX_PIXEL+i]==WHITE && f==1){
            if(ws1==0) ws1=i;
            else if(ws2==0) ws2=i;
            else if(ws3==0) ws3=i;
            f=0;
        }
        if(image_image[line*IMAGE_MAX_PIXEL+i]==BLACK && f==0){
            if(bs1==0) bs1=i;
            else if(bs2==0) bs2=i;
            else if(bs3==0) bs3=i;
            f=1;
        }
    }
    if(ws1-image_roade_left[rl-1]>3 || image_roade_left[rl-1]-bs3>3) return 0;
    if(ws2-bs1>5 && ws3-bs2>5){
        image_roade_end_is=1;
        return 1;
    }
    return 0; 
}