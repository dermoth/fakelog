CFLAGS=-Wall -fPIC
PREFIX=.

all: CFLAGS+= -Werror -O2
all: fakelog.so testlog

debug: CFLAGS+= -g
debug: all

fakelog.so: fakelog.c
	gcc ${CFLAGS} -shared -o fakelog.so fakelog.c

testlog: testlog.c
	gcc ${CFLAGS} -o testlog testlog.c

install:
	install -d ${PREFIX}/etc/systemd/system/ctxcwalogd.service.d
	install -d ${PREFIX}/opt
	[ etc/systemd/system/ctxcwalogd.service.d/logfix.conf -ef ${PREFIX}/etc/systemd/system/ctxcwalogd.service.d/logfix.conf ] || \
		install -m 644 etc/systemd/system/ctxcwalogd.service.d/logfix.conf ${PREFIX}/etc/systemd/system/ctxcwalogd.service.d/logfix.conf
	install -s fakelog.so ${PREFIX}/opt/fakelog.so

clean:
	rm -f fakelog.so testlog

.PHONY: all debug clean
