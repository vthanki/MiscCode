#include <glib.h>
#include <stdio.h>

int main()
{
	gchar base[64] = {0};
	gchar* silent_update = NULL;
	gchar* ret;
	ret = g_strdup_printf("%s - %s", "hello", silent_update);
	printf("%s\n", ret);

	return 0;
}

