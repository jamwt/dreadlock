
#line 1 "parse.rl"
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


#line 55 "parse.rl"


#line 22 "parse.c"
static const int command_start = 20;
static const int command_first_final = 20;
static const int command_error = 0;

static const int command_en_main = 20;


#line 57 "parse.rl"

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

    
#line 50 "parse.c"
	{
	cs = command_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 76 "parse.rl"

    while (1) {
        int bread = fdread(st->fd, writeptr, canwrite);
        if (bread == 0)
            eof = writeptr;
        else if (bread < 0) {
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

        
#line 78 "parse.c"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr10:
#line 41 "parse.rl"
	{ 
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
            }
#line 53 "parse.rl"
	{te = p+1;}
	goto st20;
st20:
#line 1 "NONE"
	{ts = 0;}
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1 "NONE"
	{ts = p;}
#line 110 "parse.c"
	switch( (*p) ) {
		case 76: goto st1;
		case 85: goto st11;
		case 108: goto st1;
		case 117: goto st11;
	}
	goto st0;
st0:
cs = 0;
	goto _out;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 79: goto st2;
		case 111: goto st2;
	}
	goto st0;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 67: goto st3;
		case 99: goto st3;
	}
	goto st0;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 75: goto st4;
		case 107: goto st4;
	}
	goto st0;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	if ( (*p) == 32 )
		goto st5;
	goto st0;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( (*p) == 32 )
		goto st5;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr5;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
tr5:
#line 23 "parse.rl"
	{
                if (!key) {
                    key = p;
                    keylen = 1;
                }
                else {
                    keylen++;
                }
            }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 186 "parse.c"
	if ( (*p) == 32 )
		goto st7;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr5;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr5;
	} else
		goto tr5;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( (*p) == 32 )
		goto st7;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
tr7:
#line 18 "parse.rl"
	{
                if (!timeout_s) 
                    timeout_s = p;
            }
#line 34 "parse.rl"
	{cmd = CMD_LOCK;}
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 220 "parse.c"
	switch( (*p) ) {
		case 13: goto st9;
		case 32: goto tr9;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto tr7;
	goto st0;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( (*p) == 10 )
		goto tr10;
	goto st0;
tr9:
#line 34 "parse.rl"
	{cmd = CMD_LOCK;}
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 243 "parse.c"
	switch( (*p) ) {
		case 13: goto st9;
		case 32: goto tr9;
	}
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 78: goto st12;
		case 110: goto st12;
	}
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 76: goto st13;
		case 108: goto st13;
	}
	goto st0;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 79: goto st14;
		case 111: goto st14;
	}
	goto st0;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 67: goto st15;
		case 99: goto st15;
	}
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 75: goto st16;
		case 107: goto st16;
	}
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( (*p) == 32 )
		goto st17;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 32 )
		goto st17;
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr17;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr17;
	} else
		goto tr17;
	goto st0;
tr17:
#line 23 "parse.rl"
	{
                if (!key) {
                    key = p;
                    keylen = 1;
                }
                else {
                    keylen++;
                }
            }
#line 37 "parse.rl"
	{cmd = CMD_UNLOCK;}
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 334 "parse.c"
	switch( (*p) ) {
		case 13: goto st9;
		case 32: goto tr18;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr17;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr17;
	} else
		goto tr17;
	goto st0;
tr18:
#line 37 "parse.rl"
	{cmd = CMD_UNLOCK;}
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 356 "parse.c"
	switch( (*p) ) {
		case 13: goto st9;
		case 32: goto tr18;
	}
	goto st0;
	}
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 

	_test_eof: {}
	_out: {}
	}

#line 95 "parse.rl"

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
