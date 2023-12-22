# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

# cmake module to download only required subdirs from repository
#   GIT_REPO        - url to git repository
#   GIT_SUB_DIRS    - relative path of subdirs to download
#   GIT_TAG         - SHA or branch

# https://stackoverflow.com/questions/600079/how-do-i-clone-a-subdirectory-only-of-a-git-repository/52269934#52269934

if(NOT DEFINED GIT_REPO OR NOT DEFINED GIT_TAG)
    message(FATAL_ERROR "Unspecified git repo")
endif()
execute_process(COMMAND git -c init.defaultBranch=master init)
execute_process(COMMAND git remote add origin ${GIT_REPO})
if(DEFINED GIT_SUB_DIRS)
    execute_process(COMMAND git sparse-checkout init)
    execute_process(COMMAND git sparse-checkout set --no-cone ${GIT_SUB_DIRS})
endif()
execute_process(COMMAND git pull --ff-only --depth 1 origin ${GIT_TAG})
if(DEFINED GIT_SUB_MODULES)
    execute_process(COMMAND git submodule init ${GIT_SUB_MODULES})
    execute_process(COMMAND git submodule update --depth 1)
    execute_process(COMMAND git pull --ff-only --depth 1 origin ${GIT_TAG})
endif()
