/* testlog.c -- Test syslog()
*
* Compile with:
*   gcc -Wall -fPIC -o testlog testlog.c
*
* Works only with the syslog() call (This won't work with other ways like
* using /dev/log socket).
*
*/
#include <unistd.h>
#include <sys/types.h>
#include <syslog.h>

int
main()
{
	setlogmask (LOG_UPTO (LOG_NOTICE));

	openlog ("exampleprog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);

	syslog (LOG_NOTICE, "Program started by User %d", getuid ());
	syslog (LOG_INFO, "A tree falls in a forest");

	closelog ();
}
