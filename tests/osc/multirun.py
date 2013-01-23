#!/usr/bin/python
from subprocess import Popen
import sys
import signal
import time

procs = []

if len(sys.argv) < 3:
  print "Usage: " + sys.argv[0] + " <n_processes> " + " <exec> <exec_args ...>"
  exit(0)

def stop(signal, frame):
  for i in range(nProcs):
    procs[i].send_signal(CTRL_C_EVENT)
  time.sleep(1)
  exit(0)

signal.signal(signal.SIGINT, stop)

nProcs = int(sys.argv[1])
procPath = sys.argv[2]
procArgs = sys.argv[3:]

for i in range(nProcs):
  procCompleteArgs = [ procPath, str(i) ] + procArgs
  print procCompleteArgs
  p = Popen( procCompleteArgs )
  procs += [ p ]

while (True):
  time.sleep(1)


