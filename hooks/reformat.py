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

def exists_clang_format_in_dir(dir):
  for file in os.listdir(dir):
    if file.endswith(".clang-format"):
      return True
  return False

def find_clang_format_for_file(file):
  directory_name = os.path.dirname(file)
  while not exists_clang_format_in_dir(directory_name):
    directory_name = os.path.dirname(directory_name)
  return os.path.join(directory_name, ".clang-format")
