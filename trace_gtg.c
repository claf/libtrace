#include <stdio.h>
#include <sys/time.h>

#include "trace_gtg.h"

#define MAX_FRAME 3050

/* TODO : create this function to mask use of clock_gettime or GET_TICK
 * void mjpeg_elapsed_ns ();
 * */

static uint64_t epoc = 0;

void gtg_init ()
{
  static int is_init = 0;
  struct timeval time;

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

    /* EPOC init : */
    gettimeofday (&time, NULL);
    epoc = time.tv_sec * 1000000 + time.tv_usec;
  }
}

void gtg_finalize ()
{
  endTrace ();
}

void gtg_threadTrace_init ()
{
  /* Add a container for Render thread because C2X doesn't create one for
   * pthread created threads : */
  addContainer (0, "R", "T", "P", "Render", "0");

  addEntityValue ("Re", "ST", "Read", GTG_RED);
  addEntityValue ("Wa", "ST", "Wait", GTG_BLACK);
  addEntityValue ("Fo", "ST", "Fork", GTG_BLUE);
  addEntityValue ("De", "ST", "Decode", GTG_BLUE);
  addEntityValue ("Rs", "ST", "Resize", GTG_GREEN);

  addEventType ("D", "T", "Drop");
  addEventType ("P", "T", "Print");

  addLinkType ("F", "Fetch", "T", "T", "T");
}

void gtg_frameTrace_init ()
{
  char buf[10];

  for (int i=0; i < MAX_FRAME; i++)
  {
    sprintf (buf, "F%d", i);
    addContainer (0, buf, "F", "0", buf, "0");
  }

  addEntityValue ("F", "SF", "Fetch", GTG_BLUE);
  addEntityValue ("Rs", "SF", "Resize", GTG_GREEN);
  addEntityValue ("De", "SF", "Decode", GTG_BLUE);
  addEventType ("N", "F", "Next");
  addEventType ("P", "F", "Print");
  addEventType ("D", "F", "Drop");
}

void doFrameEvent(char* op, int value)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  sprintf(buffer,"F%d", value);

  addEvent (ts, op, buffer, buffer);
}

void doEvent(char* op, int value)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  sprintf(buffer,"%d", value);

  addEvent (ts, op, "R", buffer);
}

void doVar(int value)
{
  struct timeval time;
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  setVar (ts, "W", "1", value);
}

void doState(char* state, int tid)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  //sprintf(buffer,"%d", tid + 1);
  sprintf(buffer,"%d", tid);

  setState (ts, "ST", buffer, state);
}

void pushFrameState (char* state, int fid)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts; 

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  //sprintf(buffer,"%d", tid + 1);
  sprintf(buffer,"F%d", fid);

  pushState (ts, "SF", buffer, state);
}

void popFrameState (char* state, int fid)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  //sprintf(buffer,"%d", tid + 1);
  sprintf(buffer,"F%d", fid);

  popState (ts, "SF", buffer);
}

void linkStart(char* thr, int frame_id)
{
  struct timeval time;
  char buffer[10];
  uint64_t ts;

  gettimeofday (&time, NULL);
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

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
  ts = (time.tv_sec * 1000000 + time.tv_usec) - epoc;

  sprintf(buffer,"%d",frame_id);  /* Now buffer has "20" */
  //sprintf(buffer2,"%d", tid + 1);
  sprintf(buffer2,"%d", tid);

  endLink (ts, "F", "P", "0", buffer2, buffer, buffer);

}

