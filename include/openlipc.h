/**
 * @file openlipc.h
 * @brief Open-source LIPC header file.
 *
 * This file is a part of [open]lipc.
 *
 * @copyright
 * This project is licensed under the terms of the MIT license.
 *
 * @note
 * This header file is based on the reverse-engineered proprietary library
 * lipc, which is a part of the Kindle firmware. The lipc library itself is
 * copyrighted under the terms of the Amazon Technologies, Inc.
 *
 */

#ifndef OPENLIPC_H
#define OPENLIPC_H

#include <stdarg.h>

/**
 * @defgroup lipc-init Initialization
 * @{ */

/** LIPC library handler. */
typedef void LIPC;

/**
 * Status codes returned by all sorts of lipc library functions.
 *
 * @warning
 * This list was obtained from the LipcGetErrorString() function and may
 * be not complete - be prepared for other values as well. */
typedef enum {
	LIPC_OK = 0,
	LIPC_ERROR_UNKNOWN,                 /* 1 */
	LIPC_ERROR_INTERNAL,                /* 2 */
	LIPC_ERROR_NO_SUCH_SOURCE,          /* 3 */
	LIPC_ERROR_OPERATION_NOT_SUPPORTED, /* 4 */
	LIPC_ERROR_OUT_OF_MEMORY,           /* 5 */
	LIPC_ERROR_SUBSCRIPTION_FAILED,     /* 6 */
	LIPC_ERROR_NO_SUCH_PARAM,           /* 7 */
	LIPC_ERROR_NO_SUCH_PROPERTY,        /* 8 */
	LIPC_ERROR_ACCESS_NOT_ALLOWED,      /* 9 */
	LIPC_ERROR_BUFFER_TOO_SMALL,        /* 10 */
	LIPC_ERROR_INVALID_HANDLE,          /* 11 */
	LIPC_ERROR_INVALID_ARG,             /* 12 */
	LIPC_ERROR_OPERATION_NOT_ALLOWED,   /* 13 */
	LIPC_ERROR_PARAMS_SIZE_EXCEEDED,    /* 14 */
	LIPC_ERROR_TIMED_OUT,               /* 15 */
	LIPC_ERROR_SERVICE_NAME_TOO_LONG,   /* 16 */
	LIPC_ERROR_DUPLICATE_SERVICE_NAME,  /* 17 */
	LIPC_ERROR_INIT_DBUS,               /* 18 */
	LIPC_PROP_ERROR_INVALID_STATE   = 0x100,
	LIPC_PROP_ERROR_NOT_INITIALIZED = 0x101,
	LIPC_PROP_ERROR_INTERNAL        = 0x102,
} LIPCcode;

/**
 * Initialize LIPC library without registering a new service.
 *
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpenNoName(void);

/**
 * Initialize LIPC library and register a new service.
 *
 * @param service The service name which should be registered. The name has
 *   to be a fully qualified dot-separated identifier, e.g. "org.MyService".
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpen(const char *service);

/**
 * Initialize LIPC library and register a new service.
 *
 * This function is an extended version of the LipcOpen().
 *
 * @param service The service name which should be registered. The name has
 *   to be a fully qualified dot-separated identifier, e.g. "org.MyService".
 * @param code If not NULL, the status code will be stored in this argument.
 * @return On success the LIPC library handler is returned, which should be
 *   closed with the LipcClose(). Upon error this function returns NULL. */
LIPC *LipcOpenEx(const char *service, LIPCcode *code);

/**
 * Close the LIPC handler and release all associated resources.
 *
 * @param lipc LIPC library handler. */
void LipcClose(LIPC *lipc);

/**
 * Get the service name associated with the LIPC handler.
 *
 * @param lipc LIPC library handler.
 * @return The service name which was registered during LIPC opening or NULL
 *   if the handler was obtained by the call to the LipcOpenNoName(). */
const char *LipcGetServiceName(LIPC *lipc);

/**
 * Get status code in the string format.
 *
 * @param code The status code.
 * @return String with the human-readable status. */
const char *LipcGetErrorString(LIPCcode code);

/** @}
 ***/

/**
 * @defgroup lipc-props Properties
 * @{ */

/**
 * Get property access timeout.
 *
 * @note
 * The timeout value can be set via the file "/var/local/system/lipctimeout",
 * otherwise it is set to the default 10000 ms.
 *
 * @param lipc LIPC library handler.
 * @return The timeout value in milliseconds. */
int LipcGetPropAccessTimeout(LIPC *lipc);

/**
 * Get the value of the integer property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The address where the integer value will be stored.
 * @return The status code. */
LIPCcode LipcGetIntProperty(LIPC *lipc, const char *service,
                            const char *property, int *value);

/**
 * Set the value of the integer property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The new value to set.
 * @return The status code. */
LIPCcode LipcSetIntProperty(LIPC *lipc, const char *service,
                            const char *property, int value);

/**
 * Get the value of the string property.
 *
 * The memory for the returned string is allocated internally by the library
 * and should be freed with the LipcFreeString().
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The address where the pointer to the string will be stored.
 * @return The status code. */
LIPCcode LipcGetStringProperty(LIPC *lipc, const char *service,
                               const char *property, char **value);

/**
 * Set the value of the string property.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param property The property name.
 * @param value The new value to set.
 * @return The status code. */
LIPCcode LipcSetStringProperty(LIPC *lipc, const char *service,
                               const char *property, const char *value);

/**
 * Convenience macro for getting all properties of given service.
 *
 * The list of properties is formated as a space-delimited string, where every
 * element is in the format as follows: "<property> <type> <access> ". Note,
 * that at the end of the string, there is always a hanging space character.
 *
 * The access mode of the property can be either read-only: r, write-only: w,
 * or both: rw.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param value The address where the pointer to the string will stored.
 * @return The status code. */
#define LipcGetProperties(lipc, service, value) \
	LipcGetStringProperty(lipc, service, "_properties", value)

/**
 * Free memory allocated by the LIPC.
 *
 * @param string The string pointer. */
void LipcFreeString(char *string);

/**
 * Property getter/setter callback function.
 *
 * This callback function can be used either for getting properties or for
 * setting ones. However, there is a slight difference in both actions. Also,
 * there is a difference when the callback function is called for getting a
 * string, and when it is called for getting an integer.
 *
 * When the getter is called for the integer property, one has to set the
 * address pointed by the *value to the desired integer value.
 *
 * When the getter is called for the string property, the buffer pointed by
 * the *value is preallocated to the initial size. This size is available in
 * the *data parameter. If the buffer is too small for storing desired string
 * value, one should return the LIPC_ERROR_BUFFER_TOO_SMALL code and pass the
 * required buffer size in the *data argument. In such a case, the callback
 * function will be called again with the requested buffer size.
 *
 * When the setter is called, the value parameter contains the integer value
 * itself or it points to the memory buffer, respectively for the integer
 * property or for the string property.
 *
 * For convenience, one can use one of the helper macros (LIPC_GETTER_VTOI(),
 * LIPC_SETTER_VTOI(), LIPC_GETTER_VTOS() or LIPC_SETTER_VTOS()) for casting
 * the value parameter into the proper type based on the callback type.
 *
 * In all cases, with the exception of the string property getter, the data
 * parameter will contain the value passed during the property registration.
 *
 * The return value of the callback function will be used as a return value
 * for the caller, e.g. LipcGetIntProperty(). One exception from this rule is
 * a getter for a string property, where the LIPC_ERROR_BUFFER_TOO_SMALL code
 * is used internally by the LIPC library.
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param value Pointer to the memory area for a value storage.
 * @param data Data passed during property registration.
 * @return The status code. */
typedef LIPCcode (*LipcPropCallback)(LIPC *lipc, const char *property,
                                     void *value, void *data);

/** Cast the value parameter of the getter callback into the integer type. */
#define LIPC_GETTER_VTOI(value) (*(int *)(value))
/** Cast the value parameter of the setter callback into the integer type. */
#define LIPC_SETTER_VTOI(value) ((long int)(value))
/** Cast the value parameter of the getter callback into the char * type. */
#define LIPC_GETTER_VTOS(value) ((char *)(value))
/** Cast the value parameter of the setter callback into the char * type. */
#define LIPC_SETTER_VTOS(value) ((char *)(value))

/**
 * Register new integer property.
 *
 * The access mode of the property is determined by the presence of the getter
 * and/or the setter callback function. Passing NULL value in the getter and
 * the setter parameters will make the property read-only, however it will not
 * be possible to retrieve any value, so it is pointless to do so.
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param getter Getter callback if property is readable.
 * @param setter Setter callback if property is writable.
 * @param data Data pointer passed to the callback function.
 * @return The status code. */
LIPCcode LipcRegisterIntProperty(LIPC *lipc, const char *property,
                                 LipcPropCallback getter,
                                 LipcPropCallback setter,
                                 void *data);

/**
 * Register new string property.
 *
 * For the information about the property access mode, see the documentation
 * of the LipcRegisterIntProperty().
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param getter Getter callback if property is readable.
 * @param setter Setter callback if property is writable.
 * @param data Data pointer passed to the callback function.
 * @return The status code. */
LIPCcode LipcRegisterStringProperty(LIPC *lipc, const char *property,
                                    LipcPropCallback getter,
                                    LipcPropCallback setter,
                                    void *data);

/**
 * Unregister property.
 *
 * If the data parameter is not NULL, the address of the data pointer passed
 * during the property registration will be stored in the address pointed by
 * the *data.
 *
 * @param lipc LIPC library handler.
 * @param property The property name.
 * @param data Address where the data pointer is returned.
 * @return The status code. */
LIPCcode LipcUnregisterProperty(LIPC *lipc, const char *property, void **data);

/** @}
 ***/

/**
 * @defgroup lipc-events Events
 * @{ */

/** LIPC event handler. */
typedef void LIPCevent;

/**
 * Create a new event object.
 *
 * In order to use this function the LIPC handler has to be opened with the
 * service name given using LipcOpen() or LipcOpenEx().
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @return On success the LIPCevent handler is returned, which should be freed
 *   with the LipcEventFree(). Upon error this function returns NULL. */
LIPCevent *LipcNewEvent(LIPC *lipc, const char *name);

/**
 * Free memory allocated by the LIPC.
 *
 * @param event LIPC event handler. */
void LipcEventFree(LIPCevent *event);

/**
 * Send event object.
 *
 * @param lipc LIPC library handler.
 * @param event LIPC event handler.
 * @return The status code. */
LIPCcode LipcSendEvent(LIPC *lipc, LIPCevent *event);

/**
 * Create and send event.
 *
 * This function is an equivalent of using LipcNewEvent(), LipcSendEvent() and
 * LipcEventFree() all together. If you want to send a simple event object -
 * one without any parameters - use this function.
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @return The status code. */
LIPCcode LipcCreateAndSendEvent(LIPC *lipc, const char *name);

/**
 * Create and send event.
 *
 * This function creates an event object and sets its parameters according to
 * the format string. This function takes variable number of arguments.
 *
 * LIPC supports two kinds of event parameters: integers and strings. It is
 * possible to specify both of these types via the format string in the same
 * way the printf() function family does. The format string can contain any
 * number of "%d" or/and "%s" conversion specifiers. It is also possible to
 * pass an empty string in the format argument, which will be an equivalent
 * of calling the LipcCreateAndSendEvent().
 *
 * The format string also supports the ".0" precision modifier. However, its
 * purpose is yet to be discovered.
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @param format The format string.
 * @param ... Variable list of parameters.
 * @return The status code. */
LIPCcode LipcCreateAndSendEventWithParameters(LIPC *lipc, const char *name,
                                              const char *format, ...);

/**
 * Create and send event.
 *
 * This function creates an event object and sets its parameters according to
 * the format string. Parameter values should be passed as a variable argument
 * list. For the list of all supported formats, see the documentation of the
 * LipcCreateAndSendEventWithParameters() function.
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @param format The format string.
 * @param ap Variable argument list.
 * @return The status code. */
LIPCcode LipcCreateAndSendEventWithVAListParameters(LIPC *lipc,
                                                    const char *name,
                                                    const char *format,
                                                    va_list ap);

/**
 * Get the source name associated with the event.
 *
 * @param event LIPC event handler.
 * @return The source name from where the event originates. */
const char *LipcGetEventSource(LIPCevent *event);

/**
 * Get the name of the event.
 *
 * @param event LIPC event handler.
 * @return The event name. */
const char *LipcGetEventName(LIPCevent *event);

/**
 * Get the integer parameter from the event.
 *
 * @param event LIPC event handler.
 * @param value The address where the integer value will be stored.
 * @return The status code. */
LIPCcode LipcGetIntParam(LIPCevent *event, int *value);

/**
 * Add the integer parameter to the event.
 *
 * @param event LIPC event handler.
 * @param value The new value to add.
 * @return The status code. */
LIPCcode LipcAddIntParam(LIPCevent *event, int value);

/**
 * Get the string parameter from the event.
 *
 * @note
 * The obtained memory pointer should not be passed to the LipcFreeString()
 * function. It seems, that the memory is managed internally by the library.
 *
 * @param event LIPC event handler.
 * @param value The address where the pointer to the string will be stored.
 * @return The status code. */
LIPCcode LipcGetStringParam(LIPCevent *event, char **value);

/**
 * Add the string parameter to the event.
 *
 * @param event LIPC event handler.
 * @param value The new value to add.
 * @return The status code. */
LIPCcode LipcAddStringParam(LIPCevent *event, const char *value);

/**
 * Event dispatching callback function.
 *
 * @param lipc LIPC library handler.
 * @param name The event name.
 * @param event The LIPC event handler.
 * @param data Data pointer passed during subscription.
 * @return The status code. */
typedef LIPCcode (*LipcEventCallback)(LIPC *lipc, const char *name,
                                      LIPCevent *event, void *data);

/**
 * Set default event callback function.
 *
 * The callback function will be called when one uses the LipcSubscribe()
 * subscription mechanism. Note, that it is not possible to pass user-defined
 * data via this approach - if one wants to pass extra data to the callback
 * function, one should use the LipcSubscribeExt() instead.
 *
 * @param lipc LIPC library handler.
 * @param callback Event dispatching callback function.
 * @return The status code. */
LIPCcode LipcSetEventCallback(LIPC *lipc, LipcEventCallback callback);

/**
 * Subscribe for all events emitted by the service.
 *
 * In order to receive event notifications, one has to set default callback
 * function using the LipcSetEventCallback() function. Using them both, is an
 * equivalent of using the extended version with parameters set as follows:
 * LipcSubscribeExt(lipc, service, NULL, callback, NULL)
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @return The status code. */
LIPCcode LipcSubscribe(LIPC *lipc, const char *service);

/**
 * Subscribe for events emitted by the service.
 *
 * This function is an extended version of the LipcSubscribe(), which allows
 * to set the event name for which one wants to subscribe and the user-defined
 * data passed to the callback function. Passing NULL instead of the event
 * name will subscribe for all events.
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param name The event name.
 * @param callback Event dispatching callback function.
 * @param data Data pointer passed to the callback function.
 * @return The status code. */
LIPCcode LipcSubscribeExt(LIPC *lipc, const char *service, const char *name,
                          LipcEventCallback callback, void *data);

/**
 * Unsubscribe from event or events.
 *
 * If the event name is NULL, this function will unsubscribe from events
 * delivered to the callback set with the LipcSetEventCallback().
 *
 * @param lipc LIPC library handler.
 * @param service The service name.
 * @param name The event name or NULL.
 * @param data Address where the data pointer is returned or NULL.
 * @return The status code. */
LIPCcode LipcUnsubscribeExt(LIPC *lipc, const char *service,
                            const char *name, void **data);

/** @}
 ***/

/**
 * @defgroup lipc-logs Logging
 * @{ */

/**
 * Log debug messages.
 *
 * LIPC defines 8 levels of debug messages. Via this macro one can select the
 * level which should be logged. Valid values are from 1 to 8. */
#define LAB126_LOG_DEBUG (n) ((1 << ((n) - 1)) << 8)
#define LAB126_LOG_INFO      (0x0080 << 16)
#define LAB126_LOG_WARNING   (0x0100 << 16)
#define LAB126_LOG_ERROR     (0x0200 << 16)
#define LAB126_LOG_CRITICAL  (0x0400 << 16)
#define LAB126_LOG_DEBUG_ALL  0x0000FF00
#define LAB126_LOG_ALL        0xFFFFFF00

/** Global logging mask. */
extern int g_lab126_log_mask;

/** Set the LIPC internal logging mask.
 *
 * The logging mask parameter should be the logical OR of the logging flags
 * (LAB126_LOG_* definitions). The current state of the logging mask can be
 * accessed via the g_lab126_log_mask global variable.
 *
 * @param mask The logging mask. */
void LipcSetLlog(int mask);

/** @}
 ***/

#endif /* OPENLIPC_H */
