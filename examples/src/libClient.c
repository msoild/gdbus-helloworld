/**
    Header files
*/

/**
    Own header
*/
#include "dbusCommon.h"
#include "libClient.h"

/**
    Generated headers
*/
#include "Example-generated.h"

/**
    System header
*/
#include <gio/gio.h>    /** GIO for dbus*/
#include <glib-2.0/glib.h>  /** GLIB for main loop*/  
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
    instance init
*/
static GMainLoop *pLoop = NULL;
static GDBusConnection *pConnection = NULL;
static CHiworldGdbusExample *pProxy = NULL;
static gulong g_RetSignalHandler[RET_SIGNAL_HANDLE_SIZE];
static S_SIGNAL_CALLBACK g_signal_callback;
static S_METHOD_CALLBACK g_method_callback;

static gboolean My_Signal_1_Handler(CHiworldGdbusExample *object, const gchar *arg, gpointer userdata)
{
    if (NULL != g_signal_callback.callback1){
        g_signal_callback.callback1(arg);
    }

    return TRUE;
}

static gboolean My_Signal_2_Handler(CHiworldGdbusExample *object,  GVariant *arg, gpointer userdata)
{
    /** get the value from GVariant */
    gint int_arg = 0;
    gchar *char_arg = NULL;

    g_variant_get(arg, "(is)", &int_arg, &char_arg);

    if (NULL != g_signal_callback.callback2){
        g_signal_callback.callback2(int_arg, char_arg);
    }
    g_free(char_arg);

    return TRUE;
}

static void cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata)
{
    gchar *pname_owner = NULL;
    pname_owner = g_dbus_proxy_get_name_owner((GDBusProxy*)object);

    if (NULL != pname_owner){
        g_print("DBus service is ready!\n");
        g_free(pname_owner);
    }
    else{
        g_print("DBus service is NOT ready!\n");
        g_free(pname_owner);
    }
}
bool registerClientSignalHandler(CHiworldGdbusExample *pProxy)
{
    g_print("registerClientSignalHandler call.\n");
    g_RetSignalHandler[0] = g_signal_connect(pProxy, "notify::g-name-owner", G_CALLBACK(cb_OwnerNameChangedNotify), NULL);
    g_RetSignalHandler[1] = g_signal_connect(pProxy, "my-signal1", G_CALLBACK(My_Signal_1_Handler), NULL);
    g_RetSignalHandler[2] = g_signal_connect(pProxy, "my-signal2", G_CALLBACK(My_Signal_2_Handler), NULL);
    return TRUE;
}

void registerSignalCallback(S_SIGNAL_CALLBACK *callback)
{
    if (NULL != callback){
        memcpy(&g_signal_callback, callback, sizeof(S_SIGNAL_CALLBACK));
    }
    else{
            g_print("registerSignalCallback: parameter point is NULL");
    }
}

void registerMethodCallback(S_METHOD_CALLBACK *callback)
{
    if (NULL != callback){
        memcpy(&g_method_callback, callback, sizeof(S_SIGNAL_CALLBACK));
    }
    else{
            g_print("registerMethodCallback: parameter point is NULL");
    }
}

static void MethodCallback_1(GObject* source_object, GAsyncResult* res, gpointer userdata)
{
    g_print("client MethodCallback_1 call.\n");

    gchar* out_arg = NULL;
    GError* pError = NULL;

    chiworld_gdbus_example_call_my_method1_finish(pProxy, &out_arg, res, &pError);

    if(NULL == pError){
        if(NULL != g_method_callback.callback1){
            g_method_callback.callback1(out_arg, pError);
            g_free(out_arg);
        }
    }
    else{
        g_error_free(pError);
    }
}
void myMethodAsync_1(const char* in_arg)
{
    g_print("client myMethodAsync_1 call, in_arg = %s.\n", in_arg);

    chiworld_gdbus_example_call_my_method1(pProxy, in_arg, NULL, MethodCallback_1, NULL);
    return;
}

void myMethod_1(const gchar *in_arg, gchar** out_arg, GError** pError)
{
    chiworld_gdbus_example_call_my_method1_sync(pProxy, in_arg, out_arg, NULL, pError);

    if (*pError == NULL){
        g_print("client myMethod_1 call, in_arg = %s out_arg = %s.\n", in_arg, *out_arg);
            g_free(*out_arg);
    }
    else{
            g_print("client myMethod_1: Failed to call method. Reason: %s.\n", (*pError)->message);
            g_error_free(*pError);
    }

    return; 
}

static void MethodCallback_2(GObject* source_object, GAsyncResult* res, gpointer user_data)
{
    g_print("client MethodCallback_2 call.\n");

    GVariant *out_arg = NULL;
    GError *pError = NULL;
    gchar *out_arg1 = NULL;
    gint out_arg2 = 0;

    chiworld_gdbus_example_call_my_method2_finish(pProxy, &out_arg, res, &pError);
    if (NULL == pError){
        /** get the result from the output paramter */
        g_variant_get(out_arg, "(si)", &out_arg1, &out_arg2);
        if(NULL != g_method_callback.callback2){
            g_method_callback.callback2(out_arg1, out_arg2, pError);
            g_free(out_arg1);
        }
    }
    else{
        g_error_free(pError);
    }

}
void myMethodAsync_2(const gint in_arg1, const gint in_arg2)
{
    g_print("client myMethodAsync_2 call, in_arg1 = %d, in_arg2 = %d.\n", in_arg1, in_arg2);

    GVariant *in_arg = g_variant_new("(ii)", in_arg1, in_arg2);
    chiworld_gdbus_example_call_my_method2(pProxy, in_arg, NULL, MethodCallback_2, NULL);

    return;
}

void myMethod_2(const gint in_arg1, const gint in_arg2, gchar **out_arg1, gint *out_arg2, GError** pError)
{
    GVariant *in_arg = NULL;
    GVariant *out_arg = NULL;
    GError *Error = NULL;

    /** generate the input parameters */
    in_arg = g_variant_new("(ii)", in_arg1, in_arg2);

    /** call the server's dbus methods */
    chiworld_gdbus_example_call_my_method2_sync(pProxy, in_arg, &out_arg, NULL, pError);
    if(NULL == *pError){
        /** get the result from the output parameter */
        g_variant_get(out_arg, "(si)", out_arg1, out_arg2);
        g_print("client myMethod_2 call, in_arg1 = %d, in_arg2 = %d, out_arg1 = %s, out_arg2 = %d.\n", in_arg1, in_arg2, *out_arg1, *out_arg2);
            g_free(*pError);
    }
    else{
            g_print("client myMethod_2: Failed to call method. Reason: %s.\n", (*pError)->message);
            g_error_free(*pError);
    }

    return; 
}

void *run(void* arg)
{
    g_print("run call in the client.\n");
    /** start the main event loop which manages all available sources of events */
    g_main_loop_run(pLoop);

    return ((void*)0);
}

int thread_create(void)
{
    int err;
    pthread_t thr;

    err = pthread_create(&thr, NULL, run, NULL);

    if(0 != err){
            g_print("Can't create thread: %s\n", strerror(err));
    }
    else{       
            g_print("New thread created: %s\n", strerror(err));
    }

    return err;
}

bool ExampleDBusServiceIsReady(void)
{
    gchar *owner_name = NULL;
    owner_name = g_dbus_proxy_get_name_owner((GDBusProxy*)pProxy);
    if(NULL != owner_name)
    {
        g_print("Owner Name: %s\n", owner_name);        
        g_free(owner_name);
        return true;
    }
    else
    {   
        g_print("Owner Name is NULL.");     
        return false;
    }
}

bool InitDBusCommunication(void)
{
    bool bRet = TRUE;
    GError *pConnError = NULL;
    GError *pProxyError = NULL;

    g_type_init();  /** initial for usage of "g" types */

    g_print("InitDBusCommunication: Client started.\n");

    do{
        bRet = TRUE;
        pLoop = g_main_loop_new(NULL, FALSE);   /** create main loop, but do not start it.*/

        /** First step: get a connection */
        pConnection = g_bus_get_sync(HIWORLD_GDBUS_EXAMPLE_BUS, NULL, &pConnError);

        if (NULL == pConnError){
            /** Second step: try to get a connection to the given bus.*/
            pProxy = chiworld_gdbus_example_proxy_new_sync(pConnection,
                                        G_DBUS_PROXY_FLAGS_NONE,
                                        HIWORLD_GDBUS_EXAMPLE_BUS_NAME,
                                        HIWORLD_GDBUS_EXAMPLE_OBJECT_PATH,
                                        NULL,
                                        &pProxyError);
            if (0 == pProxy){
                g_print("InitDBusCommunication: Failed to create proxy. Reason: %s.\n", pProxyError->message);
                g_error_free(pProxyError);
                bRet = FALSE;
            }


        }
        else{
            g_print("InitDBusCommunication: Failed to connect to dbus. Reason: %s.\n", pConnError->message);
            g_error_free(pConnError);
            bRet = FALSE;
        }
    }while(FALSE == bRet);

    if (TRUE == bRet){
        /** Third step: Attach to dbus signals */
        registerClientSignalHandler(pProxy);
    }

    thread_create();
}

bool DeInitDBusCommunication(void)
{
    bool bRet = FALSE;

    if(0 != pLoop){
        g_main_loop_quit(pLoop);
        g_main_loop_unref(pLoop);
        bRet = TRUE;
    }

    return bRet;
}