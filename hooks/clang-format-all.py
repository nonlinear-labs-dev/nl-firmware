#!/usr/bin/python3
import os
import reformat

#without this i was not allowed to do git diff --cached??
reformat.do_shell_action(["git", "config", "--global", "--add", "safe.directory", "/workdir"])

work_dir=reformat.do_shell_action(["git", "rev-parse", "--show-toplevel"])[0]
all_files=reformat.do_shell_action(["find", f"{work_dir}", "-iname", "*.h", "-o", "-iname", "*.cpp", "-o", "-iname", "*.c", "-o", "-iname", "*.hpp"])
black_list = ["cmake-build-", "json.hpp"]

for b in black_list:
  all_files = list(filter(lambda k: b not in k, all_files))

print(f"found {len(all_files)} files to be formatted")

for idx, file in enumerate(all_files):
  print(f"formatting file [{idx}/{len(all_files)}] {file}")
  file = os.path.join(work_dir, file)
  clang_format_file = reformat.find_clang_format_for_file(file)
  reformat.do_shell_action(["clang-format-14", "-i", f"-style=file:{clang_format_file}", file])