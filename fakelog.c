
/* fakelog.c -- Fake syslog() to log to STDERR
*
* Compile with:
*   gcc -Wall -fPIC -shared -o fakelog.so fakelog.c
*
* Works only with the syslog() call (This won't work with other ways like
* using /dev/log socket).
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define DEFAULT_LOGFILE "/var/log/ctxcwalogd_logfix.log"

void
syslog(int priority, const char *format, ...)
{
	int fmtlen;
	char *newfmt;
	char *logfile;
	FILE *outf;
	va_list argptr;

	fmtlen = strlen(format);
	newfmt = malloc(fmtlen+2);

	if (!newfmt)
		return;

	/* TODO: add priority */
	memcpy(newfmt, format, fmtlen);
	strcpy(newfmt+fmtlen, "\n");

	logfile = getenv("FAKELOG_FILE");
	if (!logfile) {
		logfile = DEFAULT_LOGFILE;
	}
	outf = fopen(logfile, "a+");
	if (!outf)
		goto CLEANUP_FREE;

	if (!format) {
		fprintf(outf, "syslog(%i): missing format", priority);
		return;
	}

	va_start(argptr, format);
	vfprintf(outf, newfmt, argptr);
	va_end(argptr);

	fclose(outf);
	CLEANUP_FREE:
	free(newfmt);
}

/* There got to be a better way but I have no time to check so this is
 * just a copy-paste of syslog()
 */
void
__syslog_chk(int priority, void *flags, const char *format, ...)
{
	int fmtlen;
	char *newfmt;
	char *logfile;
	FILE *outf;
	va_list argptr;

	fmtlen = strlen(format);
	newfmt = malloc(fmtlen+2);

	if (!newfmt)
		return;

	/* TODO: add priority */
	memcpy(newfmt, format, fmtlen);
	strcpy(newfmt+fmtlen, "\n");

	logfile = getenv("FAKELOG_FILE");
	if (!logfile) {
		logfile = DEFAULT_LOGFILE;
	}
	outf = fopen(logfile, "a+");
	if (!outf)
		goto CLEANUP_FREE;

	if (!format) {
		fprintf(outf, "syslog(%i): missing format", priority);
		return;
	}

	va_start(argptr, format);
	vfprintf(outf, newfmt, argptr);
	va_end(argptr);

	fclose(outf);
	CLEANUP_FREE:
	free(newfmt);
}
