
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
static const char _command_actions[] = {
	0, 1, 1, 1, 2, 1, 3, 1, 
	5, 1, 6, 2, 0, 2, 2, 1, 
	3, 2, 4, 7
};

static const char _command_key_offsets[] = {
	0, 0, 2, 4, 6, 7, 14, 21, 
	24, 28, 29, 31, 33, 35, 37, 39, 
	41, 42, 49, 57, 59
};

static const char _command_trans_keys[] = {
	79, 111, 67, 99, 75, 107, 32, 32, 
	48, 57, 65, 90, 97, 122, 32, 48, 
	57, 65, 90, 97, 122, 32, 48, 57, 
	13, 32, 48, 57, 10, 13, 32, 78, 
	110, 76, 108, 79, 111, 67, 99, 75, 
	107, 32, 32, 48, 57, 65, 90, 97, 
	122, 13, 32, 48, 57, 65, 90, 97, 
	122, 13, 32, 76, 85, 108, 117, 0
};

static const char _command_single_lengths[] = {
	0, 2, 2, 2, 1, 1, 1, 1, 
	2, 1, 2, 2, 2, 2, 2, 2, 
	1, 1, 2, 2, 4
};

static const char _command_range_lengths[] = {
	0, 0, 0, 0, 0, 3, 3, 1, 
	1, 0, 0, 0, 0, 0, 0, 0, 
	0, 3, 3, 0, 0
};

static const char _command_index_offsets[] = {
	0, 0, 3, 6, 9, 11, 16, 21, 
	24, 28, 30, 33, 36, 39, 42, 45, 
	48, 50, 55, 61, 64
};

static const char _command_indicies[] = {
	0, 0, 1, 2, 2, 1, 3, 3, 
	1, 4, 1, 4, 5, 5, 5, 1, 
	6, 5, 5, 5, 1, 6, 7, 1, 
	8, 9, 7, 1, 10, 1, 8, 9, 
	1, 11, 11, 1, 12, 12, 1, 13, 
	13, 1, 14, 14, 1, 15, 15, 1, 
	16, 1, 16, 17, 17, 17, 1, 8, 
	18, 17, 17, 17, 1, 8, 18, 1, 
	19, 20, 19, 20, 1, 0
};

static const char _command_trans_targs[] = {
	2, 0, 3, 4, 5, 6, 7, 8, 
	9, 10, 20, 12, 13, 14, 15, 16, 
	17, 18, 19, 1, 11
};

static const char _command_trans_actions[] = {
	0, 0, 0, 0, 0, 1, 0, 11, 
	0, 3, 17, 0, 0, 0, 0, 0, 
	0, 14, 5, 0, 0
};

static const char _command_to_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 7
};

static const char _command_from_state_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 9
};

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

    
#line 127 "parse.c"
	{
	cs = command_start;
	ts = 0;
	te = 0;
	act = 0;
	}

#line 76 "parse.rl"

    while (1) {
        printf("top!\n");
        int bread = fdread(st->fd, writeptr, canwrite);
        if (bread == 0)
            eof = writeptr;
        if (bread < 0) {
            switch (errno) {
                case EINTR: continue;
                default:
                    perror("dreadlock client read");
                    eof = writeptr;
            }
        }

        p = writeptr;
        pe = writeptr + bread;

        
#line 155 "parse.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_acts = _command_actions + _command_from_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 6:
#line 1 "NONE"
	{ts = p;}
	break;
#line 176 "parse.c"
		}
	}

	_keys = _command_trans_keys + _command_key_offsets[cs];
	_trans = _command_index_offsets[cs];

	_klen = _command_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _command_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _command_indicies[_trans];
	cs = _command_trans_targs[_trans];

	if ( _command_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _command_actions + _command_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 18 "parse.rl"
	{
                if (!timeout_s) 
                    timeout_s = p;
            }
	break;
	case 1:
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
	break;
	case 2:
#line 34 "parse.rl"
	{cmd = CMD_LOCK;}
	break;
	case 3:
#line 37 "parse.rl"
	{cmd = CMD_UNLOCK;}
	break;
	case 4:
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
	break;
	case 7:
#line 53 "parse.rl"
	{te = p+1;}
	break;
#line 288 "parse.c"
		}
	}

_again:
	_acts = _command_actions + _command_to_state_actions[cs];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 ) {
		switch ( *_acts++ ) {
	case 5:
#line 1 "NONE"
	{ts = 0;}
	break;
#line 301 "parse.c"
		}
	}

	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}

#line 95 "parse.rl"

        if (cs == command_error) {
            fprintf(stderr, "syntax error from client!\n");
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
        }
        else {
            writeptr = buf;
            canwrite = PARSE_BUFSIZ;
        }
    }
}
