# Fakelog - Fake logging fix for Citrix ctxcwalogd

Since my latest update of Citrix Workspace, ctxcwalogd has been spamming the
console with emergency syslog messages every once in a while.

My solution to this is an LD_PRELOAD library to send the logs to a logfile.

If the logfile cannot be written to (which should be the default) it will not log
at all.

## Build

Just run:

```sh
make
```

For a debug build:

```sh
make debug
```

A `teslog` program is provided for testing.

## Install

You cna set `PREFIX` to the installation directory. Defaults to `.`.

This would install straight up in /:

```sh
make PREFIX= install
```

Else just run `make install` and manually copy the files in `etc/` anbd `opt/`
to their respective location in `/`.

Finally, you need to create the lofgile, reload systemd and restart
`ctxcwalogd`. As root:

```sh
touch /var/log/ctxcwalogd_logfix.log
chown citrixlog:citrixlog /var/log/ctxcwalogd_logfix.log
systemctl daemon-reload
systemctl restart ctxcwalogd.service
```

Ensure the service runs and has logged to tthe file:

```sh
systemctl status ctxcwalogd.service
cat /var/log/ctxcwalogd_logfix.log
```

## Maintenance

You're left on your own to rotate or cleanup the logfile. Since the file is
opened each time you can just truncate it, no reload is even required:

```sh
truncate -s0 /var/log/ctxcwalogd_logfix.log
```

As of writing this I have no idea how much will end up in this file and whenever
I'll have to even rotate it before Citrix fixes their sh*t.

## Notes

I initially planned to log to `STDERR`, and had some issues which I mistakenly
thought were caused by the daemon closing the stream (TBH I didn't even check
that, even yet). Turned out I was reading obsolete docs and `__syslog_chk()`
didn't follow `syslog()`'s prototype. At this point syslog() was merely an alias
and it was easier to just copy the function all over than figure out how to pass
down a `va_list`.

This was developped and tested on Debian Bullseye (11.6). On my platform the
prototype for `__syslog_chk` is defined in
`/usr/include/x86_64-linux-gnu/bits/syslog.h`.

## Bugs

There are other ways to send syslog messages, this library only handles
`syslog()`.

The `openlog()`and `closelog()` calls are not handled, which is not really a
problem but should be skipped for cleanliness.
