/*
 * Original code from :
 * https://leonardoce.wordpress.com/2015/04/01/dbus-tutorial-a-simple-server/
 *
 * Modified according to the need.
 */


#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "bkd.h"

#define DBUS_SERVICE_NAME	"org.geekpur.vishal.BKD"
#define BKD_DBUS_INTERFACE_NAME	"org.geekpur.vishal.BKD"
#define DBUS_OBJECT_PATH	"/org/geekpur/vishal/BKD"

static void check_and_abort(DBusError *error);
static DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data);
static void respond_to_introspect(DBusConnection *connection, DBusMessage *request);
static void return_backspace_count(DBusConnection *connection, DBusMessage *request);

static void check_and_abort(DBusError *error)
{
	if (dbus_error_is_set(error)) {
		puts(error->message);
		abort();
	}
}

static DBusHandlerResult tutorial_messages(DBusConnection *connection, DBusMessage *message, void *user_data)
{
	const char *interface_name = dbus_message_get_interface(message);
	const char *member_name = dbus_message_get_member(message);

	if (0==strcmp("org.freedesktop.DBus.Introspectable", interface_name) &&
			0==strcmp("Introspect", member_name)) {
		respond_to_introspect(connection, message);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else if (0==strcmp(BKD_DBUS_INTERFACE_NAME, interface_name) &&
			0==strcmp("GetBackspaceCount", member_name)) {
		return_backspace_count(connection, message);
		return DBUS_HANDLER_RESULT_HANDLED;
	} else {
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}
}

static void respond_to_introspect(DBusConnection *connection, DBusMessage *request)
{
	DBusMessage *reply;

	const char *introspection_data =
		" <!DOCTYPE node PUBLIC \"-//freedesktop//DTD D-BUS Object Introspection 1.0//EN\" "
		"\"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd\">"
		" <!-- dbus-sharp 0.8.1 -->"
		" <node>"
		"   <interface name=\"org.freedesktop.DBus.Introspectable\">"
		"     <method name=\"Introspect\">"
		"       <arg name=\"data\" direction=\"out\" type=\"s\" />"
		"     </method>"
		"   </interface>"
		"   <interface name=\"org.geekpur.vishal.BKD\">"
		"     <method name=\"GetBackspaceCount\">"
		"       <arg name=\"ret\" direction=\"out\" type=\"i\" />"
		"     </method>"
		"   </interface>"
		" </node>";

	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
			DBUS_TYPE_STRING, &introspection_data,
			DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}

static void return_backspace_count(DBusConnection *connection, DBusMessage *request)
{
	DBusMessage *reply;
	DBusError error;
	int bkd_backspace_count = bkd_get_current_count();

	dbus_error_init(&error);

	reply = dbus_message_new_method_return(request);
	dbus_message_append_args(reply,
			DBUS_TYPE_INT32, &bkd_backspace_count,
			DBUS_TYPE_INVALID);
	dbus_connection_send(connection, reply, NULL);
	dbus_message_unref(reply);
}

int main()
{
	DBusConnection *connection;
	DBusError error;
	DBusObjectPathVTable vtable;

	uid_t cur = getuid();

	if (setuid(0)) {
		fprintf(stderr, "[-] Cannot setup environment\n");
		return EXIT_FAILURE;
	}

	if(bkd_start_daemon("/dev/input/event4"))
		return EXIT_FAILURE;

	if (setuid(cur)) {
		fprintf(stderr, "[-] Cannot restore environment\n");
		return EXIT_FAILURE;
	}

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
	check_and_abort(&error);

	dbus_bus_request_name(connection, DBUS_SERVICE_NAME, 0, &error);
	check_and_abort(&error);

	vtable.message_function = tutorial_messages;
	vtable.unregister_function = NULL;

	dbus_connection_try_register_object_path(connection,
			DBUS_OBJECT_PATH,
			&vtable,
			NULL,
			&error);
	check_and_abort(&error);

	while(1) {
		dbus_connection_read_write_dispatch(connection, 1000);
	}

	bkd_stop_daemon();

	return 0;
}


