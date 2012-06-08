#ifndef DREADLOCK_H
#define DREADLOCK_H

#include "utlist.h"
#include "uthash.h"


typedef struct dreadlock_claimant {
    unsigned int id;
    int won;

    Rendez wait;

    struct dreadlock_claimant *prev;
    struct dreadlock_claimant *next;
} dreadlock_claimant;

typedef struct dreadlock_lock {
    char *key;

    dreadlock_claimant *waits;

    UT_hash_handle hh;
} dreadlock_lock;

typedef struct dreadlock_user_lock {
    dreadlock_claimant *wait;
    dreadlock_lock *lock;

    UT_hash_handle hh;
} dreadlock_user_lock;

typedef struct dreadlock_client_state {
    char id[500];
    int fd;

    dreadlock_user_lock *locks;
} dreadlock_client_state;

void do_lock(dreadlock_client_state *st, char *key, int len, int timeout_ms);
void do_unlock(dreadlock_client_state *st, char *key, int len);
void ragel_parse(dreadlock_client_state *st);

#endif /* DREADLOCK_H */
