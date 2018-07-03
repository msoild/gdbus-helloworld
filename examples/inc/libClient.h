#ifndef __LIB_CLIENT__
#define __LIB_CLIENT__

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <glib-2.0/glib.h>

#include "Example-generated.h"

#define RET_SIGNAL_HANDLE_SIZE 3

typedef void (*SIGNAL1CALLBACK)(const gchar*);
typedef void (*SIGNAL2CALLBACK)(const gint, const gchar*);
typedef struct signalcallback
{
    SIGNAL1CALLBACK callback1;  
    SIGNAL2CALLBACK callback2;
}S_SIGNAL_CALLBACK;

typedef void (*METHOD1CALLBACK)(const gchar*, const GError*);
typedef void (*METHOD2CALLBACK)(const gchar*, const gint, GError*);
typedef struct methodcallback
{
    METHOD1CALLBACK callback1;  
    METHOD2CALLBACK callback2;
}S_METHOD_CALLBACK;



static gboolean My_Signal_1_Handler(CHiworldGdbusExample *object, const gchar *arg, gpointer userdata);
static gboolean My_Signal_2_Handler(CHiworldGdbusExample *object,  GVariant *arg, gpointer userdata);
static void cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata);
bool registerClientSignalHandler(CHiworldGdbusExample *pProxy);
void registerSignalCallback(S_SIGNAL_CALLBACK *callback);
void registerMethodCallback(S_METHOD_CALLBACK *callback);
static void MethodCallback_1(GObject* source_object, GAsyncResult* res, gpointer userdata);
void myMethodAsync_1(const char* in_arg);
void myMethod_1(const gchar *in_arg, gchar** out_arg, GError** pError);
static void MethodCallback_2(GObject* source_object, GAsyncResult* res, gpointer user_data);
void myMethodAsync_2(const gint in_arg1, const gint in_arg2);
void myMethod_2(const gint in_arg1, const gint in_arg2, gchar **out_arg1, gint *out_arg2, GError** pError);
void *run(void* arg);
int thread_create(void);
bool ExampleDBusServiceIsReady(void);
bool InitDBusCommunication(void);
bool DeInitDBusCommunication(void);
#endif