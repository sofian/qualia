#!/usr/bin/python
from subprocess import Popen
import sys
import signal
import time

procs = []

if len(sys.argv) < 4:
  print "Usage: " + sys.argv[0] + " <start_id> <n_processes>  <exec> <exec_args ...>"
  exit(0)

def stop(sig, frame):
#  NO NEED TO DO THIS (it seems the CTRL-C is just sent automatically to the forked processes)
#  for i in range(nProcs):
#    procs[i].terminate()
#    procs[i].send_signal(signal.SIGTERM)
#    procs[i].send_signal(signal.CTRL_C_EVENT)
  time.sleep(3)
  exit(0)

signal.signal(signal.SIGINT, stop)

startId = int(sys.argv[1])
nProcs = int(sys.argv[2])
procPath = sys.argv[3]
procArgs = sys.argv[4:]

for i in range(nProcs):
  procCompleteArgs = [ procPath, str(i) ] + procArgs
  print procCompleteArgs
  p = Popen( procCompleteArgs )
  procs += [ p ]

while (True):
  time.sleep(1)


