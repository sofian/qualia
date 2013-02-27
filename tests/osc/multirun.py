#!/usr/bin/python
from subprocess import Popen
import sys
import signal
import time

procs = []

if len(sys.argv) < 4:
  print "Usage: " + sys.argv[0] + " [-r] <start_id> <n_processes>  <exec> <exec_args ...>"
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

arg = 1
if (sys.argv[arg] == "-r"):
  remoteAgent = True
  arg += 1
else:
  remoteAgent = False

startId = int(sys.argv[arg])
arg+=1
nProcs = int(sys.argv[arg])
arg+=1
procPath = sys.argv[arg]
arg+=1
procArgs = sys.argv[arg:]

for i in range(nProcs):
  id = startId + i
  procCompleteArgs = [ procPath, str(id) ] + procArgs
  if (remoteAgent and id==0):
    procCompleteArgs += [ '-remote-agent' ]
  print procCompleteArgs
  p = Popen( procCompleteArgs )
  procs += [ p ]

while (True):
  time.sleep(1)


