/**
    Header files
*/

/** 
    Own header
*/
#include "dbusCommon.h"
#include "libServer.h"

/** 
    System header
*/
#include <gio/gio.h>   // GIO for dbus
#include <glib-2.0/glib.h>   // GLIB for main loop
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/**
    Generated headers
*/
#include "Example-generated.h"

/**
    Instance init
*/
static GMainLoop         *pLoop     = NULL;
static CHiworldGdbusExample *pSkeleton = NULL;
static int callNum = 0;

/**
    Normal functions
*/
static gboolean Emit_My_signal1(gconstpointer p)
{
    g_print("Emit_My_Signal1() is called.\n");

    gchar buf[20];
    memset(buf, 0, 20);
    callNum++;
    sprintf(buf, "Server Signal1(%d)", callNum);

    chiworld_gdbus_example_emit_my_signal1(pSkeleton, buf);

    return TRUE;
}

static gboolean Emit_My_Signal2(gconstpointer p)
{
    g_print("Emit_My_Signal2() is called.\n");

    callNum++;
    GVariant *payload = g_variant_new("(is)", callNum, "Server Signal2");

    chiworld_gdbus_example_emit_my_signal2(pSkeleton, payload);

    return TRUE;
}

static gboolean My_Method1(CHiworldGdbusExample     *object,
                               GDBusMethodInvocation *invocation,
                               const gchar           *in_arg,
                               gpointer               user_data)
{
    g_print("Server MyMethod1 call. Parameter: %s.\n", in_arg);

    in_arg = "Server My_Method1 to output.";

    chiworld_gdbus_example_complete_my_method1(object, invocation, in_arg);

    return TRUE;
}

static gboolean My_Method2(CHiworldGdbusExample     *object,
                               GDBusMethodInvocation *invocation,
                               GVariant           *in_arg,
                               gpointer               user_data)
{
  gint in_arg1 = 0;
  gint in_arg2 = 0;
  gint out_arg2 = 0;
  gchar *out_arg1 = "Server MyMethod2 to output.";
  GVariant *out_arg = NULL;

  /** Get the input parameters */
  g_variant_get(in_arg, "(ii)", &in_arg1, &in_arg2);

  g_print("Server MyMethod2 call. Parameter: in_arg1 = %d , in_arg2 = %d.\n", in_arg1, in_arg2);

  /** Generate the output parameters */
  out_arg2 = in_arg1 + in_arg2;
  out_arg = g_variant_new("(si)", out_arg1, out_arg2);

  chiworld_gdbus_example_complete_my_method2(object, invocation, out_arg);

  return TRUE;

}

static void bus_acquired_cb(GDBusConnection *connection,
                                const gchar     *bus_name,
                                gpointer         user_data)
{
    GError *pError = NULL;

    /** Second step: Try to get a connection to the given bus. */
    pSkeleton = chiworld_gdbus_example_skeleton_new();

    /** Third step: Attach to dbus signals. */
    (void) g_signal_connect(pSkeleton, "handle-my-method1", G_CALLBACK(My_Method1), NULL);
    (void) g_signal_connect(pSkeleton, "handle-my-method2", G_CALLBACK(My_Method2), NULL);

    /** Fourth step: Export interface skeleton. */
    (void) g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pSkeleton),
                                              connection,
                                              HIWORLD_GDBUS_EXAMPLE_OBJECT_PATH,
                                              &pError);
    if(pError != NULL){
        g_print("Error: Failed to export object. Reason: %s.\n", pError->message);
        g_error_free(pError);
        g_main_loop_quit(pLoop);
    }
}

static void name_acquired_cb(GDBusConnection *connection,
                                     const gchar     *bus_name,
                                     gpointer         user_data)
{
    g_print("name_acquired_cb call, Acquired bus name: %s.\n", HIWORLD_GDBUS_EXAMPLE_BUS_NAME);
}

static void name_lost_cb(GDBusConnection *connection,
                             const gchar     *bus_name,
                             gpointer         user_data)
{
    if(connection == NULL)
    {
        g_print("name_lost_cb call, Error: Failed to connect to dbus.\n");
    }
    else
    {
        g_print("name_lost_cb call, Error: Failed to obtain bus name: %s.\n", HIWORLD_GDBUS_EXAMPLE_BUS_NAME);
    }

    g_main_loop_quit(pLoop);
}

void* run(void* para)
{
    g_print("run call in the server.\n");
        /** Start the Main Event Loop which manages all available sources of events */
    g_main_loop_run( pLoop );

    return ((void*)0);
}

int thread_create(void)
{
    int err;
    pthread_t thr;

    err = pthread_create(&thr, NULL, run, NULL);

    if (err != 0)
    {
        g_print("Can't create thread: %s\n", strerror(err));
    }
    else
    {
        g_print("New thread created: %s\n", strerror(err));
    }

    return err;
}


bool InitDBusCommunicationServer(void)
{
    bool bRet = TRUE;

    /** init for usage of "g" types */
    g_type_init();

    g_print("InitDBusCommunicationServer: Server started.\n");

    /** create main loop, but do not start it. */
    pLoop = g_main_loop_new(NULL, FALSE);

    /** first step: connect to dbus */
    (void)g_bus_own_name(HIWORLD_GDBUS_EXAMPLE_BUS,
                        HIWORLD_GDBUS_EXAMPLE_BUS_NAME,
                        G_BUS_NAME_OWNER_FLAGS_NONE,
                        &bus_acquired_cb,
                        &name_acquired_cb,
                        &name_lost_cb,
                        NULL,
                        NULL);
    /** emit my signal1 every 1 second to trigger example communication */
    g_timeout_add(1000, (GSourceFunc)Emit_My_signal1, NULL);

    /** emit my signal2 every2 second to trigger example communication */
    g_timeout_add(2000, (GSourceFunc)Emit_My_Signal2, NULL);

    thread_create();

    return bRet;    
}

bool DeinitDBusCommunicationServer(void)
{
    bool bRet = FALSE;
    if(0 != pLoop){
        g_main_loop_quit(pLoop);
        g_main_loop_unref(pLoop);
        bRet = TRUE;
    }

    return bRet;

}
