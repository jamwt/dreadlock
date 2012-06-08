#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <task.h>

#include "dreadlock.h"

enum CMDLIST {
    CMD_LOCK,
    CMD_UNLOCK
};

%%{
        machine command;
        timeout = (digit+)
            @{
                if (!timeout_s) 
                    timeout_s = p;
            };
        key = (alnum+)
            @{
                if (!key) {
                    key = p;
                    keylen = 1;
                }
                else {
                    keylen++;
                }
            };
        lock =
            ( [lL][oO][cC][kK] ' '+ key ' '+ timeout ' '*)
            @{cmd = CMD_LOCK;};
        unlock =
            ( [uU][nN][lL][oO][cC][kK] ' '+ key ' '*)
            @{cmd = CMD_UNLOCK;};
        crlf = '\r\n';
        main := |*
            ( lock | unlock ) crlf
            @{ 
                int timeout_ms = -1;
                if (timeout_s) {
                    timeout_ms = atoi(timeout_s);
                    timeout_s = NULL;
                }
                switch(cmd) {
                    case CMD_LOCK: do_lock(st, key, keylen, timeout_ms); break;
                    case CMD_UNLOCK: do_unlock(st, key, keylen); break;
                    default: assert(0); break;
                }
                key = NULL;
            };
        *|;
}%%
%% write data;

#define PARSE_BUFSIZ 10000

void ragel_parse(dreadlock_client_state *st) {
    int cs;
    char *key = NULL;
    int keylen = -1;
    int cmd = -1;
    char *timeout_s = NULL;

    char buf[PARSE_BUFSIZ];
    char *writeptr = buf;
    char *eof = NULL;
    char *ts = NULL, *te = NULL, *p = NULL, *pe = NULL;
    int act;
    (void)act;
    int canwrite = PARSE_BUFSIZ;

    %% write init;

    while (1) {
        int bread = fdread(st->fd, writeptr, canwrite);
        if (bread == 0)
            eof = writeptr;
        if (bread < 0) {
            switch (errno) {
                case EINTR: continue;
                default:
                    perror("dreadlock client read");
                    eof = writeptr;
                    bread = 0;
            }
        }

        p = writeptr;
        pe = writeptr + bread;

        %% write exec;

        if (cs == command_error) {
            fprintf(stderr, "syntax error from client\n");
            break;
        }

        if (eof)
            break;

        if (ts) {
            int have = te - ts;
            memmove(buf, ts, have);
            ts = buf;
            te = buf + have;
            canwrite = PARSE_BUFSIZ - have;
            writeptr = buf + have;
        }
        else {
            writeptr = buf;
            canwrite = PARSE_BUFSIZ;
        }
    }
}
