from subprocess import Popen, PIPE
import os

def do_shell_action(commands):
  ret = Popen(commands, stdout=PIPE)
  listOfBytes = ret.stdout.readlines()
  ret = []
  for line in listOfBytes:
    l = line.decode("utf-8").replace('\n', '')
    ret.append(l)
  return ret