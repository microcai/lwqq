/**
 * @file   async.h
 * @author xiehuc<xiehuc@gmail.com>
 * @date   Sun May 20 22:24:30 2012
 *
 * @brief  Linux WebQQ Async API
 *
 *
 */
#ifndef LWQQ_ASYNC_H
#define LWQQ_ASYNC_H
#include "type.h"

#include <event2/event.h>
typedef struct timeval  LwqqAsyncTimer;
typedef int   LwqqAsyncIo;
typedef void* LwqqAsyncTimerHandle;
typedef void* LwqqAsyncIoHandle;
#define LWQQ_ASYNC_READ EV_READ
#define LWQQ_ASYNC_WRITE EV_WRITE

/** 
 * call this function when you quit your program.
 * NOTE!! you must call lwqq_http_global_free first !!
 * */
void lwqq_async_global_quit();
/**===================EVSET API==========================================**/
/** design prospective ::
 * since most method return a LwqqAsyncEvent* object.
 * do not try to create by your self.
 *
 * you can use handle to add a listener use lwqq_async_add_event_listener.
 * when http request finished .it would raise a callback.
 * since all LwqqAsyncEvent object automatic freed. you do not need care about it.
 *
 * you can get a errno by use lwqq_async_event_get_result in a callback.
 * sometimes it is usefull
 *
 * if you need care about multi http request at same time.
 * consider about use LwqqAsyncEvset object use lwqq_async_evset_new();
 * note evset is means sets of event.
 * then you can add some event to a evset use lwqq_async_evset_add_event;
 * well finally you always want use lwqq_async_add_evset_listener.
 * it likes event listener. but it raised only all event finished in your evset.
 * note each event can still raise a callback if you add event listener.
 *
 * if a evset successfully raise a callback.then it would be freed.
 * if not it would raise a memleak.to avoid this. free it by s_free by your self.
 */
/** this api is better than old async api.
 * it is more powerful and easier to use
 */
typedef void (*EVENT_CALLBACK)(LwqqAsyncEvent* event,void* data);
typedef void (*EVSET_CALLBACK)(LwqqAsyncEvset* evset,void* data);
/** return a new evset. a evset can link multi of event.
 * you can wait a evset. means it would block ultil all event is finished
 */
LwqqAsyncEvset* lwqq_async_evset_new();
/** return a new event. 
 * you can wait a event by use evset or LWQQ_SYNC macro simply.
 * you can also add a event listener
 */
LwqqAsyncEvent* lwqq_async_event_new(void* req);
/** this would remove a event.
 * and call a event listener if is set.
 * and try to wake up a evset if all events of evset are finished
 */
void lwqq_async_event_finish(LwqqAsyncEvent* event);
/** this is same as lwqq_async_event_finish */
#define lwqq_async_event_emit(event) lwqq_async_event_finish(event)
/** this would add a event to a evset.
 * note one event can only link to one evset.
 * one evset can link to multi event.
 * do not repeatly add one event twice
 */
void lwqq_async_evset_add_event(LwqqAsyncEvset* host,LwqqAsyncEvent *handle);
/** this add a event listener to a event.
 * it is better than lwqq_async_add_listener.
 * because you can set different data to different event which may the same function.
 * async listener can only set one data for one ListenerType.
 */
void lwqq_async_add_event_listener(LwqqAsyncEvent* event,EVENT_CALLBACK callback,void* data);
void lwqq_async_add_evset_listener(LwqqAsyncEvset* evset,EVSET_CALLBACK callback,void* data);
/** !!not finished yet */
void lwqq_async_event_set_progress(LwqqAsyncEvent* event,LWQQ_PROGRESS callback,void* data);
/** this set the errno for a event.
 * it is a hack code.
 * ensure LwqqAsyncEvent first member is a int.
 */
#define lwqq_async_event_set_result(ev,res)\
do{\
    *((int*)ev) = res;\
}while(0)
/** this return a errno of a event. */
#define lwqq_async_event_get_result(ev) (ev?*((int*)ev):-1)
/** this return one of errno of event in set ,so do not use it*/
#define lwqq_async_evset_get_result(ev) (ev?*((int*)ev):-1)

extern int LWQQ_ASYNC_GLOBAL_SYNC_ENABLED;
#define LWQQ_SYNC_BEGIN() { LWQQ_ASYNC_GLOBAL_SYNC_ENABLED = 1;
#define LWQQ_SYNC_END() LWQQ_ASYNC_GLOBAL_SYNC_ENABLED = 0;}
#define LWQQ_SYNC_ENABLED() (LWQQ_ASYNC_GLOBAL_SYNC_ENABLED == 1)
//============================ASYNC DISPATCH API===========================///
/**
 * this part include a async dispatch merchanism. that is use a small timeout
 * event to let function called by main event loop thread.
 * it is useful when some case that X11 function call require called in only main
 * gtk event loop .
 * 
 * it include lc->dispatch function pointer and lwqq async option struct.
 * 
 * lc->dispatch is a low level dispatch function. by default make a 50ms timeout 
 * using default AsyncTimer object.
 *
 * override this to provide customized timer loop.
 * 
 * then provide lwqq async option struct .these are some inner callback of lwqq.
 * like this:
 * lc->async_opt = & your_static_async_opt;
 *
 */
struct _LwqqAsyncOption {
    DISPATCH_FUNC poll_msg;
    DISPATCH_FUNC poll_lost;

};

//initialize lwqq client with default dispatch function
void lwqq_async_init(LwqqClient* lc);


//=========================LWQQ ASYNC LOW LEVEL EVENT LOOP API====================//
//=======================PLEASE MAKE SURE DONT USED IN YOUR CODE==================//
/** the call back of io watch 
 * @param data user defined data
 * @param fd the socket
 * @param action read/write enum value
 */
typedef void (*LwqqAsyncIoCallback)(void* data,int fd,int action);
/** the call back of timer watch
 * @param data user defined data
 * return 1 to continue timer 0 to stop timer.
 */
typedef int (*LwqqAsyncTimerCallback)(void* data);
/** watch an io socket for special event
 * implement by libev or libpurple
 * @param io this is pointer to a LwqqAsyncIo struct
 * @param fd socket
 * @param action combination of LWQQ_ASYNC_READ and LWQQ_ASYNC_WRITE
 */
void lwqq_async_io_watch(LwqqAsyncIoHandle io,int fd,int action,LwqqAsyncIoCallback func,void* data);
/** stop a io watcher */
void lwqq_async_io_stop(LwqqAsyncIoHandle io);
/** start a timer count down
 * @param timer the pointer to a LwqqAsyncTimer struct
 * @param ms microsecond time
 */
void lwqq_async_timer_watch(LwqqAsyncTimerHandle timer,unsigned int ms,LwqqAsyncTimerCallback func,void* data);
/** stop a timer */
void lwqq_async_timer_stop(LwqqAsyncTimerHandle timer);
//when caller finished . it would raise called finish yet.
//so it is calld event chain.
void lwqq_async_add_event_chain(LwqqAsyncEvent* caller,LwqqAsyncEvent* called);
//=========================LWQQ ASYNC LOW LEVEL EVENT LOOP API====================//

#endif
