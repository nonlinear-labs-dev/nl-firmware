#!/usr/bin/python3
import os
import shutil
import subprocess
import sys
from subprocess import Popen, PIPE

def do_shell_action(commands):
  ret = subprocess.Popen(commands, stdout=PIPE)
  listOfBytes = ret.stdout.readlines()
  ret = []
  for line in listOfBytes:
    l = line.decode("utf-8").replace('\n', '')
    ret.append(l)
  return ret

#without this i was not allowed to do git diff --cached??
do_shell_action(["git", "config", "--global", "--add", "safe.directory", "/workdir"])

work_dir=do_shell_action(["git", "rev-parse", "--show-toplevel"])[0]
all_files=do_shell_action(["find", f"{work_dir}", "-iname", "*.h", "-o", "-iname", "*.cpp", "-o", "-iname", "*.c", "-o", "-iname", "*.hpp"])
black_list = ["cmake-build-", "json.hpp"]

for b in black_list:
  all_files = list(filter(lambda k: b not in k, all_files))

print(f"found {len(all_files)} files to be formatted")

def exists_clang_format_in_dir(dir):
  for file in os.listdir(dir):
    if file.endswith(".clang-format"):
      return True
  return False

def find_clang_format_for_file(file):
  file = os.path.join(work_dir, file)
  directory_name = os.path.dirname(file)
  while not exists_clang_format_in_dir(directory_name):
    directory_name = os.path.dirname(directory_name)
  return os.path.join(directory_name, ".clang-format")

for idx, file in enumerate(all_files):
  print(f"formatting file [{idx}/{len(all_files)}] {file}")
  clang_format_file = find_clang_format_for_file(file)
  do_shell_action(["clang-format-14", "-i", f"-style=file:{clang_format_file}", file])