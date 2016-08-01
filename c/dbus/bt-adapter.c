#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>

static void indent (int depth)
{
	while (depth-- > 0)
		printf ("   ");
}


static void print_iter (DBusMessageIter *iter, dbus_bool_t literal, int depth)
{
	do
	{
		int type = dbus_message_iter_get_arg_type (iter);

		if (type == DBUS_TYPE_INVALID)
			break;

		indent(depth);

		switch (type)
		{
			case DBUS_TYPE_STRING:
				{
					char *val;
					dbus_message_iter_get_basic (iter, &val);
					if (!literal)
						printf ("string \"");
					printf ("%s", val);
					if (!literal)
						printf ("\"\n");
					break;
				}

			case DBUS_TYPE_SIGNATURE:
				{
					char *val;
					dbus_message_iter_get_basic (iter, &val);
					if (!literal)
						printf ("signature \"");
					printf ("%s", val);
					if (!literal)
						printf ("\"\n");
					break;
				}

			case DBUS_TYPE_OBJECT_PATH:
				{
					char *val;
					dbus_message_iter_get_basic (iter, &val);
					if (!literal)
						printf ("object path \"");
					printf ("%s", val);
					if (!literal)
						printf ("\"\n");
					break;
				}

			case DBUS_TYPE_INT16:
				{
					dbus_int16_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("int16 %d\n", val);
					break;
				}

			case DBUS_TYPE_UINT16:
				{
					dbus_uint16_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("uint16 %u\n", val);
					break;
				}

			case DBUS_TYPE_INT32:
				{
					dbus_int32_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("int32 %d\n", val);
					break;
				}

			case DBUS_TYPE_UINT32:
				{
					dbus_uint32_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("uint32 %u\n", val);
					break;
				}

			case DBUS_TYPE_INT64:
				{
					dbus_int64_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("int64 %lld\n", val);
					break;
				}

			case DBUS_TYPE_UINT64:
				{
					dbus_uint64_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("uint64 %llu\n", val);
					break;
				}

			case DBUS_TYPE_DOUBLE:
				{
					double val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("double %g\n", val);
					break;
				}

			case DBUS_TYPE_BYTE:
				{
					unsigned char val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("byte %d\n", val);
					break;
				}

			case DBUS_TYPE_BOOLEAN:
				{
					dbus_bool_t val;
					dbus_message_iter_get_basic (iter, &val);
					printf ("boolean %s\n", val ? "true" : "false");
					break;
				}

			case DBUS_TYPE_VARIANT:
				{
					DBusMessageIter subiter;

					dbus_message_iter_recurse (iter, &subiter);

					printf ("variant ");
					print_iter (&subiter, literal, depth+1);
					break;
				}
			case DBUS_TYPE_ARRAY:
				{
					int current_type;
					DBusMessageIter subiter;

					dbus_message_iter_recurse (iter, &subiter);

					printf("array [\n");
					while ((current_type = dbus_message_iter_get_arg_type (&subiter)) != DBUS_TYPE_INVALID)
					{
						print_iter (&subiter, literal, depth+1);
						dbus_message_iter_next (&subiter);
						if (dbus_message_iter_get_arg_type (&subiter) != DBUS_TYPE_INVALID)
							printf (",");
					}
					indent(depth);
					printf("]\n");
					break;
				}
			case DBUS_TYPE_DICT_ENTRY:
				{
					DBusMessageIter subiter;

					dbus_message_iter_recurse (iter, &subiter);

					printf("dict entry(\n");
					print_iter (&subiter, literal, depth+1);
					dbus_message_iter_next (&subiter);
					print_iter (&subiter, literal, depth+1);
					indent(depth);
					printf(")\n");
					break;
				}

			case DBUS_TYPE_STRUCT:
				{
					int current_type;
					DBusMessageIter subiter;

					dbus_message_iter_recurse (iter, &subiter);

					printf("struct {\n");
					while ((current_type = dbus_message_iter_get_arg_type (&subiter)) != DBUS_TYPE_INVALID)
					{
						print_iter (&subiter, literal, depth+1);
						dbus_message_iter_next (&subiter);
						if (dbus_message_iter_get_arg_type (&subiter) != DBUS_TYPE_INVALID)
							printf (",");
					}
					indent(depth);
					printf("}\n");
					break;
				}

			default:
				printf (" (dbus-monitor too dumb to decipher arg type '%c')\n", type);
				break;
		}
	} while (dbus_message_iter_next (iter));
}

static void abort_on_error(DBusError *error)
{
	if (dbus_error_is_set(error)) {
		fprintf(stderr, "%s", error->message);
		abort();
	}
}

static void get_managed_objs(DBusConnection *connection,
		const char *bus_name, const char *path, const char *iface,
		const char *method, DBusError *error)
{
	DBusMessage *msgQuery = NULL;
	DBusMessage *msgReply = NULL;
	DBusMessageIter iter;

	msgQuery = dbus_message_new_method_call(
			"org.bluez", path, iface, method);

	msgReply = dbus_connection_send_with_reply_and_block(connection, msgQuery, -1, error);
	abort_on_error(error);

	/* Read the objects in the form of {o{s{s,v}}} */
	dbus_message_iter_init (msgReply, &iter);
	print_iter(&iter, FALSE, 1);

	dbus_message_unref(msgQuery);
}

int main()
{
	DBusConnection *connection = NULL;
	DBusError error;

	dbus_error_init(&error);
	connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
	abort_on_error(&error);

	get_managed_objs(connection, "org.bluez",
			"/",
			"org.freedesktop.DBus.ObjectManager",
			"GetManagedObjects",
			&error);
	abort_on_error(&error);

	return 0;
}

