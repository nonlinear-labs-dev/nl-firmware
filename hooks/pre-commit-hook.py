#!/usr/bin/python3
import os
import reformat

#without this i was not allowed to do git diff --cached??
reformat.do_shell_action(["git", "config", "--global", "--add", "safe.directory", "/workdir"])

changed_files=reformat.do_shell_action(["git", "diff", "--name-only", "--cached"])
work_dir=reformat.do_shell_action(["git", "rev-parse", "--show-toplevel"])[0]

for file in changed_files:
  if file.endswith(".cpp") or file.endswith(".c") or file.endswith(".h") or file.endswith(".hpp"):
    file = os.path.join(work_dir, file)
    clang_format_file = reformat.find_clang_format_for_file(file)
    reformat.do_shell_action(["clang-format-14", "-i", f"-style=file:{clang_format_file}", file])
    reformat.do_shell_action(["git", "add", file])