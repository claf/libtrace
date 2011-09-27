#ifndef _USE_TRACE_GTG_H
#define _USE_TRACE_GTG_H

#include <GTG.h>

void gtg_init ();
void mjpeg_gtg_init ();
void doEvent(char* op, int value);
void doVar(int value);
void doState(char* state, int tid);
void linkStart(char* thr, int frame_id);
void linkEnd(int frame, int tid);

#endif
