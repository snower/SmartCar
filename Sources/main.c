#include <hidef.h>      /* common defines and macros */
#include "derivative.h"  /* derivative-specific definitions */
#include "base.h"
#include "core.h"
#include "image.h"
#include "car.h"
#include "cmd.h"  

void main(void) {
  /* put your own code here */
  car_Init();
  
  car_start=1;
  car_Start();

  for(;;) {
    //_FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}