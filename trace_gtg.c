#include <stdio.h>
#include <sys/time.h>

#include "trace_gtg.h"

/* TODO : create this function to mask use of clock_gettime or GET_TICK
 * void mjpeg_elapsed_ns ();
 * */

void gtg_init ()
{
  static int is_init = 0;

  if (is_init == 0)
  {
    setTraceType (PAJE);
    initTrace ("trace", 0, GTG_FLAG_NONE);

    /* For Gantt trace : */
    addContType ("T", "0", "Thread");
    addStateType ("ST", "T", "StateThread");

    /* For Frame trace : */
    addContType ("F", "0", "Frame");
    addStateType ("SF", "F", "StateFrame");
  }
}

void mjpeg_gtg_init ()
{
  /* Add a container for Render thread because C2X doesn't create one for
   * pthread created threads : */
  addContainer (0, "R", "T", "P", "Render", "0");

  addEntityValue ("Re", "S", "Read", GTG_RED);
  addEntityValue ("Wa", "S", "Wait", GTG_BLACK);
  addEntityValue ("Fo", "S", "Fork", GTG_BLUE);
  addEntityValue ("De", "S", "Decode", GTG_BLUE);
  addEntityValue ("Rs", "S", "Resize", GTG_GREEN);

  addEventType ("D", "T", "Drop");
  addEventType ("P", "T", "Print");

  addLinkType ("F", "Fetch", "T", "T", "T");
}

void doEvent(char* op, int value)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) ;//- epoc;

  sprintf(buffer,"%d", value);

  addEvent (ts, op, "R", buffer);
}

void doVar(int value)
{
  struct timeval time;
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) ;//- epoc;

  setVar (ts, "W", "1", value);
}

void doState(char* state, int tid)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) ;//- epoc;

  //sprintf(buffer,"%d", tid + 1);
  sprintf(buffer,"%d", tid);

  setState (ts, "S", buffer, state);
}

void linkStart(char* thr, int frame_id)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) ;//- epoc;

  sprintf(buffer,"%d",frame_id);  /* Now buffer has "20" */

  startLink (ts, "F", "P", thr, "0", buffer, buffer);
}

void linkEnd(int frame_id, int tid)
{
  struct timeval time;
  char buffer[10];
  char buffer2[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) ;//- epoc;

  sprintf(buffer,"%d",frame_id);  /* Now buffer has "20" */
  //sprintf(buffer2,"%d", tid + 1);
  sprintf(buffer2,"%d", tid);

  endLink (ts, "F", "P", "0", buffer2, buffer, buffer);

}

