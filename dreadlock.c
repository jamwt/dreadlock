#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <task.h>
#include <assert.h>
#include <errno.h>
#include <sys/time.h>
#include "utlist.h"
#include "uthash.h"

#include "dreadlock.h"

static dreadlock_lock *locks;

static dreadlock_timer *timers;

void send_string_response(dreadlock_client_state *st, char *msg) {
    char *ptr = msg;
    int left = strlen(msg);
    while (left) {
        int bwrite = fdwrite(st->fd, ptr, left);
        if (bwrite < 0) {
            perror("client write:");
            return;
        }
        assert(bwrite != 0);
        left -= bwrite;
        ptr += bwrite;
    }
}

double now_in_double() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    double now = (double)tv.tv_sec;
    now += tv.tv_usec * 0.000001;
    return now;
}

double now_plus_ms(int ms) {
    double msdelt = ms * 0.001;
    double now = now_in_double();
    return now + msdelt;
}

int timer_compare(void *p1, void *p2) {
    dreadlock_timer *t1 = (dreadlock_timer *)p1;
    dreadlock_timer *t2 = (dreadlock_timer *)p2;

    if (t1->wake_at < t2->wake_at)
        return -1;
    if (t1->wake_at == t2->wake_at)
        return 0;
    return 1;
}

void do_lock(dreadlock_client_state *st, char *key, int len, int timeout_ms) {
    dreadlock_lock *l;

    HASH_FIND(hh, locks, key, len, l);

    /* Someone already owns */
    if (l) {
        dreadlock_claimant *c = l->waits->prev;
        if (c->id == taskid()) {
            send_string_response(st, "e already locked\r\n");
            return;
        }
        c = malloc(sizeof(dreadlock_claimant));
        bzero(&c->wait, sizeof(Rendez));
        c->id = taskid();
        c->won = 0;
        c->next = NULL;
        c->prev = NULL;
        DL_PREPEND(l->waits, c);

        dreadlock_timer timer = {now_plus_ms(timeout_ms), &c->wait, NULL, NULL};
        DL_APPEND(timers, &timer);
        DL_SORT(timers, timer_compare);

        tasksleep(&c->wait);

        if (c->won)  {
            DL_DELETE(timers, &timer);
            dreadlock_user_lock *ul = malloc(sizeof(dreadlock_user_lock));
            ul->lock = l;
            ul->wait = c;
            HASH_ADD_KEYPTR(hh, st->locks, l->key, len, ul);

            send_string_response(st, "l locked\r\n");
        }
        else {
            send_string_response(st, "t timeout\r\n");
            DL_DELETE(l->waits, c);
            if (!l->waits) {
                HASH_DELETE(hh, locks, l);
                free(l->key);
                free(l);
            }
            free(c);
        }
    }
    else {
        l = malloc(sizeof(dreadlock_lock));
        l->waits = NULL;
        l->key = malloc(len + 1);
        memcpy(l->key, key, len);
        l->key[len] = 0;

        dreadlock_claimant *c = malloc(sizeof(dreadlock_claimant));
        bzero(&c->wait, sizeof(Rendez));
        c->id = taskid();
        c->won = 1;
        c->next = NULL;
        c->prev = NULL;
        DL_PREPEND(l->waits, c);
        HASH_ADD_KEYPTR(hh, locks, l->key, len, l);

        dreadlock_user_lock *ul = malloc(sizeof(dreadlock_user_lock));
        ul->lock = l;
        ul->wait = c;
        HASH_ADD_KEYPTR(hh, st->locks, l->key, len, ul);

        send_string_response(st, "l locked\r\n");
    }
}

void do_unlock(dreadlock_client_state *st, char *key, int len) {
    dreadlock_lock *l;

    HASH_FIND(hh, locks, key, len, l);

    if (!l) {
        if (st)
            send_string_response(st, "e not locked\r\n");
    }
    else {
        dreadlock_claimant *c = l->waits->prev; // tail
        if (c->id != taskid()) {
            if (st)
                send_string_response(st, "e not owner\r\n");
        }
        else {
            DL_DELETE(l->waits, c);
            if (st) {
                dreadlock_user_lock *ul;
                HASH_FIND(hh, st->locks, key, len, ul);
                assert(ul);
                HASH_DELETE(hh, st->locks, ul);
                free(ul);
            }
            free(c);
            if (l->waits) {
                c = l->waits->prev;
                c->won = 1;
                taskwakeup(&c->wait);
            }
            else {
                HASH_DELETE(hh, locks, l);
                free(l->key);
                free(l);
            }
            if (st) {
                send_string_response(st, "u unlocked\r\n");
            }
        }
    }
}

void dreadlock_client(void *arg) {
    dreadlock_client_state *st = (dreadlock_client_state *)arg;
    taskname(st->id);

    ragel_parse(st);

    close(st->fd);

    /* cleanup held locks */
    dreadlock_user_lock *ul, *tmp;
    HASH_ITER(hh, st->locks, ul, tmp) {
        assert(ul->wait->won); /* if this was the waiting lock,
                                  we couldn't have done the 0-byte
                                  read() */
        do_unlock(NULL, ul->lock->key, strlen(ul->lock->key));
        HASH_DELETE(hh, st->locks, ul);
        free(ul);
    }

    free(st);

    taskexit(0);
}

void waker(void *p) {
    while (1) {
        double now = now_in_double();
        while (timers && timers->wake_at < now) {
            taskwakeup(timers->wake);
            DL_DELETE(timers, timers);
        }
        taskdelay(100);
    }
}

void taskmain(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 6001;
    int boundfd = netannounce(TCP, NULL, port);
    if (boundfd < 0) {
        perror("dreadlock bind error");
        taskexitall(1);
    }

    taskcreate(waker, NULL, 32768);

    while (1) {
        char server[16];
        int port;
        int clientfd = netaccept(boundfd, server, &port);
        if (clientfd >= 0) {
            /* handle client */
            dreadlock_client_state *st = malloc(sizeof(dreadlock_client_state));
            snprintf(st->id, 500, "%s:%d", server, port);
            st->fd = clientfd;
            st->locks = NULL;
            fdnoblock(clientfd);
            taskcreate(dreadlock_client, (void *)st, 32768);
        }
        else {
            perror("dreadlock accept error");
        }
    }
    taskexitall(0);
}
