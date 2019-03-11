#!/usr/bin/env python3

import subprocess
import sys
import getopt

# Driver program for getting performance info from sorting programs
class Tracer:
    
    programDirectory = "./examples/"
    qtest = ["merge-sort", "quick-sort", "insert-sort"]

    def runTrace(self, prog, size):
        clist = [prog, str(size)]
        try:
            retcode = subprocess.call(clist)
        except Exception as e:
            print("Call of '%s' failed: %s" % (" ".join(clist), e))
            return False
        return retcode == 0

    def run(self, size = 50000):
        if size > 65535:
            print("ERROR: Exceed the size limit 65535")
        offset = 10 ** (len(str(size)) - 1)
        sizeList = [i for i in range(size, 0, -1 * offset)]
        sizeList.reverse()

        print("Size\t{}\t{}\t{}".format(*(self.qtest)))

        for i in sizeList:
            print(i, end='\t', flush=True)
            for prog in self.qtest:
                if self.runTrace(self.programDirectory + prog, i) == False:
                    return
            print('')

def usage(name):
    print("Usage: %s [-h] [-s SIZE]" % name)
    print("  -h        Print this message")
    print("  -s SIZE   Size to test")
    sys.exit(0)

def run(name, args):
    size = 50000

    optlist, args = getopt.getopt(args, 'hp:s:')
    for (opt, val) in optlist:
        if opt == '-h':
            usage(name)
        elif opt == '-s':
            size = int(val)
        else:
            print("Unrecognized option '%s'" % opt)
            usage(name)
    t = Tracer()
    t.run(size)

if __name__ == "__main__":
    run(sys.argv[0], sys.argv[1:])
