#ifndef _USE_TRACE_GTG_H
#define _USE_TRACE_GTG_H

#include <GTG.h>

void gtg_init (char* filename);
void gtg_finalize ();
void gtg_threadTrace_init ();
void gtg_frameTrace_init ();
void doEvent(char* op, int value);
void doFrameEvent(char* op, int value);
void doVar(int value);
void doState(char* state, int tid);
void doFrameState(char* state, int tid);
void linkStart(char* thr, int frame_id);
void linkEnd(int frame, int tid);
void pushFrameState (char* state, int fid);
void popFrameState (char* state, int fid);

#endif
