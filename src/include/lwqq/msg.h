/**
 * @file   msg.h
 * @author mathslinux <riegamaths@gmail.com>
 * @date   Thu Jun 14 14:42:08 2012
 * 
 * @brief  
 * 
 * 
 */

#ifndef LWQQ_MSG_H
#define LWQQ_MSG_H

#include <pthread.h>
#include "queue.h"

typedef enum LwqqMsgType {
    LWQQ_MT_BUDDY_MSG = 0,
    LWQQ_MT_GROUP_MSG,
    LWQQ_MT_STATUS_CHANGE,
    LWQQ_MT_KICK_MESSAGE,
    LWQQ_MT_UNKNOWN,
} LwqqMsgType;

typedef struct LwqqMsgContent {
    enum {
        LWQQ_CONTENT_STRING,
        LWQQ_CONTENT_FACE,
        LWQQ_CONTENT_OFFPIC,
//custom face :this can send/recv picture
        LWQQ_CONTENT_CFACE
    }type;
    union {
        int face;
        char *str;
        //webqq protocol
        //this used in offpic format which may replaced by cface (custom face)
        struct {
            char* name;
            char* data;
            size_t size;
            int success;
            char* file_path;
        }img;
        struct {
            char* name;
            char* data;
            size_t size;
            char* file_id;
            char* key;
            char serv_ip[24];
            char serv_port[8];
        }cface;
    } data;
    TAILQ_ENTRY(LwqqMsgContent) entries;
} LwqqMsgContent ;

typedef struct LwqqMsgMessage {
    LwqqMsgType type;
    char *from;
    char *to;
    char *msg_id;
    int msg_id2;
    time_t time;

    union{
        struct {
            char *send; /* only group use it to identify who send the group message */
            char *group_code; /* only avaliable in group message */
        }group;
    };
    /* For font  */
    char *f_name;
    int f_size;
    struct {
        int a, b, c; /* bold , italic , underline */
    } f_style;
    char *f_color;

    TAILQ_HEAD(, LwqqMsgContent) content;
} LwqqMsgMessage;

typedef struct LwqqMsgStatusChange {
    char *who;
    char *status;
    int client_type;
} LwqqMsgStatusChange;

typedef struct LwqqMsgKickMessage {
    int show_reason;
    char *reason;
    char *way;
} LwqqMsgKickMessage;


typedef struct LwqqMsg {
    /* Message type. e.g. buddy message or group message */
    LwqqMsgType type;
    void *opaque;               /**< Message details */
} LwqqMsg;

/** 
 * Create a new LwqqMsg object
 * 
 * @param msg_type 
 * 
 * @return NULL on failure
 */
LwqqMsg *lwqq_msg_new(LwqqMsgType type);

/** 
 * Free a LwqqMsg object
 * 
 * @param msg 
 */
void lwqq_msg_free(LwqqMsg *msg);

/************************************************************************/
/* LwqqRecvMsg API */
/**
 * Lwqq Receive Message object, used by receiving message
 * 
 */
typedef struct LwqqRecvMsg {
    LwqqMsg *msg;
    SIMPLEQ_ENTRY(LwqqRecvMsg) entries;
} LwqqRecvMsg;

typedef struct LwqqRecvMsgList {
    int count;                  /**< Number of message  */
    pthread_mutex_t mutex;
    SIMPLEQ_HEAD(, LwqqRecvMsg) head;
    void *lc;                   /**< Lwqq Client reference */
    void (*poll_msg)(struct LwqqRecvMsgList *list); /**< Poll to fetch msg */
} LwqqRecvMsgList;

/** 
 * Create a new LwqqRecvMsgList object
 * 
 * @param client Lwqq Client reference
 * 
 * @return NULL on failure
 */
LwqqRecvMsgList *lwqq_recvmsg_new(void *client);

/** 
 * Free a LwqqRecvMsgList object
 * 
 * @param list 
 */
void lwqq_recvmsg_free(LwqqRecvMsgList *list);

/* LwqqRecvMsg API end */

/************************************************************************/
int lwqq_msg_send(void *client, LwqqMsg *msg);
int lwqq_msg_send2(void *client, const char *to, const char *content);
/*  LwqqSendMsg API */

#endif  /* LWQQ_MSG_H */
