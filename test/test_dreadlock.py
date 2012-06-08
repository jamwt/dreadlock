from diesel import quickstart, sleep, fork
from diesel.util.event import Countdown
from diesel.protocols.dreadlock import DreadlockService
import time


v = 0

def nolocks(cd):
    global v
    for x in xrange(500):
        s = v
        sleep()
        v = s + 1
    cd.tick()

def withlocks(locker, cd):
    global v
    for x in xrange(50):
        with locker.hold('v'):
            s = v
            sleep()
            v = s + 1
        if x % 10 == 0:
            print x
    cd.tick()

def runner():
    global v
    v = 0
    print "First, without dreadlock"
    t = time.time()
    cd = Countdown(500)
    for x in xrange(500):
        fork(nolocks, cd)

    cd.wait()
    print "Test took %.3f" % (time.time() - t)
    print "%s < %s?" % (v, 50 * 500)
    assert (v < 50 * 500)
    print '-' * 72

    v = 0

    locker = DreadlockService('localhost', 6001, 100)

    t = time.time()
    cd = Countdown(500)
    for x in xrange(500):
        fork(withlocks, locker, cd)

    cd.wait()
    print "Test took %.3f" % (time.time() - t)
    print "%s == %s?" % (v, 50 * 500)
    assert (v == 50 * 500)
    print '-' * 72

    print "okay."

quickstart(runner)
