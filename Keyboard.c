#include "fsm.h"

typedef struct Main Main;
struct Main
{
   Fsm super_; /* extend the Fsm class */
   /* ... other attributes of Main */
};

void MainCtor(Main *me);
void Main_initial(Main *me, Event const *e);
void Main_default(Main *me, Event const *e);
void Door_opened(Main *me, Event const *e);
void Door_closed(Main *me, Event const *e);

typedef struct MainEvt MainEvt;
struct MainEvt
{
   Event super_; /* extend the Event class */
   char code;
};

/* signals used by the Main FSM */
enum
{
   DOOR_OPEN_SIG,
   DOOR_CLOSED_SIG,
   LIGHT_ON_SIG,
};


void MainCtor(Main *me)
{
   // Main constructor creates an FSM for Main with initial
   // state set to Main_initial
   FsmCtor_(&me->super_, &Main_initial);
}
 
void Main_initial(Main *me, Event const *e)
{
   // Do stuff here to init the Main app
   FsmTran_((Fsm *)me, &Main_default);
}

void Main_default(Main *me, Event const *e)
{
   switch (e->sig)
   {
      case DOOR_OPEN_SIG:
         FsmTran_((Fsm *)me, &Door_opened);
         break;
      case DOOR_CLOSED_SIG:
         FsmTran_((Fsm *)me, &Door_closed);
         break;
      default:
         break;
   }
}

void Door_opened(Main *me, Event const *e)
{
   // Turn on light
   //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 4);
   FsmTran_((Fsm *)me, &Main_default);
}

void Door_closed(Main *me, Event const *e)
{
   // Turn off light
   //GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
   FsmTran_((Fsm *)me, &Main_default);
}
