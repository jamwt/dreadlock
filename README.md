Dreadlock
=========

Dreadlock is a simple + fast network-locking daemon.  A client
can connect to dreadlock over TCP and ensure exclusive access
to a particular resource.

*Note: due to network partitions and things of that nature,
this is only a SOFT guarantee of exclusivity; don't use this
program for banking.  It is optimized for speedy locking,
not safe locking.  Something like zookeeper is better
for safe locking.*


Protocol
--------

It talks over TCP and supports two commands.  First, `lock`:

    lock <key> <timeout>\r\n

Issuing `lock` will block the calling client for `timeout`
milliseconds until either the key is held exclusively by
this client or the timeout elapses.  Keys must contain only
ascii letters and digits.

The response will be either:

    l locked\r\n

or

    t timeout\r\n

Your client should raise an exception on `t timeout` (or
return an error code, depending on language).

Clients will be given the lock as it is released in
first-come-first-serve order.  If a client's TCP connection
to dreadlock drops, it is considered dead, and all locks
it holds are immediately released.

To explicitly release a lock when a client is done, use:

    unlock <key>\r\n

The response will be:

    u unlocked\r\n

Errors
------

A few errors are possible, like unlocking a lock you do
not hold, re-locking a lock you already hold, etc.  Errors
are always of the format:

    e error message\r\n

Your client should raise an exception and provide the `error message`
as a descriptive string for the error.

A protocol syntax error will cause the connection to be rudely
dropped on your client (and dreadlock will print a warning
to stderr).

Building
========

To build `dreadlock`, you need Russ Cox's `libtask` library
installed.  For running high concurrency `dreadlock` servers
on linux, it's recommended to use 
[this fork](https://github.com/jamwt/libtask) which supports
`epoll()` on linux.  The standard libtask has a fixed limit
of 1024 sockets; the `epoll()`-enabled fork has no arbitrary
limits (other than `ulimit`).


`make` and `make install` will do it; `make all` will attempt
to use `ragel` to rebuild the parsing state machine C file,
but you must have a recent version of `ragel` installed on
your system to do so.

Running
=======

Just run `dreadlock` and it will listen on port 6001.  Optionally,
if you specify a command line argument, it will listen on that port
instead:

    dreadlock

or

    dreadlock 11066

Status
======

Consider `dreadlock` beta software for now.

Author
======

Jamie Turner <jamie@bu.mp>
