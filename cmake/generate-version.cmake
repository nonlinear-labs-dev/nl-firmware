execute_process(
  COMMAND git log -n 1 "--pretty=format:%cd" "--date=format:%Y-%m-%d %H:%M:%S"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE LAST_COMMIT_STAMP
)

configure_file(${INPUT_FILE} ${OUTPUT_FILE} @ONLY)
message("Got Git Version ${LAST_COMMIT_STAMP}") 