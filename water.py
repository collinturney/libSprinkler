#!/usr/bin/env python

import sys
import time
import signal
from datetime import datetime
from sprinkler import Sprinkler, SprinklerError

WATER_TIMER=60 * 2  # water time in seconds

def main(argv):
    print('Started @ ' + str(datetime.now()))

    sprinkler = Sprinkler()            

    def sig_handler(signum, frame):
        print('Caught signal %d, exiting' % signum)
        sprinkler.all_off()
        sys.exit(1)

    signal.signal(signal.SIGINT, sig_handler)

    sprinkler.on()
    time.sleep(WATER_TIMER)
    sprinkler.off()

    print('Finished @ ' + str(datetime.now()))

    return 0

if __name__ == "__main__":
    rc = main(sys.argv)
    sys.exit(rc)
