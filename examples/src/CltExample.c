/**
    Header files
*/

#include "libClient.h"

/**
    Callback function
*/

void SignalCallback_1(const gchar* arg)
{
    g_print("Signal_1_Callback: Paramater value: %s.\n", arg);

    return;
}

void SignalCallback_2(const gint int_arg, const gchar* str_arg)
{
    g_print("Signal_2_Callback: Int value: %d, String value: %s\n", int_arg, str_arg);

    return;
}

void MethodCallback1(const gchar* arg, const GError* error)
{
    if (error == NULL)
    {
            g_print("Method1Callback: return value: %s.\n", arg);
    }
    else
    {
            g_print("Method1Callback: error message: %s.\n", error->message);
    }

    return;
}

void MethodCallback2(const gchar* out_arg1, const gint out_arg2, GError* error)
{
    if (error == NULL)
    {
            g_print("Method2Callback: return value: (%s, %d).\n", out_arg1, out_arg2);
    }
    else
    {
            g_print("Method2Callback: error message: %s.\n", error->message);
    }

    return;
}


int main(void)
{
    S_SIGNAL_CALLBACK signal_callback;
    S_METHOD_CALLBACK method_callback;

    signal_callback.callback1 = SignalCallback_1;
    signal_callback.callback2 = SignalCallback_2;

    registerSignalCallback(&signal_callback);

    method_callback.callback1 = MethodCallback1;
    method_callback.callback2 = MethodCallback2;

    registerMethodCallback(&method_callback);

    InitDBusCommunication();

    while(!ExampleDBusServiceIsReady());
#if 1
    const gchar *syncMehtod1Arg = "method1 synchronize call.";
    gchar *syncMethod1Out = NULL;
    GError *syncMethodError = NULL;

    myMethod_1(syncMehtod1Arg, &syncMethod1Out, &syncMethodError);
#endif
    gchar *asyncMethod1arg = "method1 asynchronize call.";
#if 1
    myMethodAsync_1(asyncMethod1arg);

    GError *syncMethod2Error = NULL;
    gint syncMethod2Arg1 = 12;
    gint syncMethod2Arg2 = 34;
    gchar *syncMethod2Out1 = NULL;
    gint syncMethod2Out2 = 0;

    myMethod_2(syncMethod2Arg1, syncMethod2Arg2, &syncMethod2Out1, &syncMethod2Out2, &syncMethod2Error);


    gint asyncMethod2Arg1 = 10;
    gint asyncMethod2Arg2 = 21;
    myMethodAsync_2(asyncMethod2Arg1, asyncMethod2Arg2);
#endif
    sleep(100);

    return 0;
}