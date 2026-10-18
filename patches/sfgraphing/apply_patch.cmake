# Apply the SFML 3 port to SFGraphing, unless it is already applied.
#
# Usage: cmake -DPATCH_FILE=<patch> -P apply_patch.cmake
# (run in the SFGraphing source directory, e.g. as a FetchContent PATCH_COMMAND)
execute_process(
  COMMAND git apply --reverse --check "${PATCH_FILE}"
  RESULT_VARIABLE is_not_applied
  OUTPUT_QUIET ERROR_QUIET
)
if(is_not_applied)
  execute_process(
    COMMAND git apply "${PATCH_FILE}"
    RESULT_VARIABLE apply_result
  )
  if(apply_result)
    message(FATAL_ERROR "Cannot apply ${PATCH_FILE}")
  endif()
  message(STATUS "Applied ${PATCH_FILE}")
else()
  message(STATUS "Already applied: ${PATCH_FILE}")
endif()
