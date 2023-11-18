# cmake module do download only required subdirs from repository
#   GIT_REPO        - url to git repository
#   GIT_SUB_DIRS    - relative path of subdirs to download
#   GIT_TAG         - SHA or branch

# https://stackoverflow.com/questions/600079/how-do-i-clone-a-subdirectory-only-of-a-git-repository/52269934#52269934

message("girish subdirs ${GIT_SUB_DIRS} ${GIT_REPO}")

if(NOT DEFINED GIT_REPO OR NOT DEFINED GIT_SUB_DIRS OR NOT DEFINED GIT_TAG)
    message(FATAL_ERROR "Unspecified git repo")
else()
    execute_process(COMMAND git -c init.defaultBranch=master init)
    execute_process(COMMAND git remote add origin ${GIT_REPO})
    execute_process(COMMAND git sparse-checkout init)
    execute_process(COMMAND git sparse-checkout set ${GIT_SUB_DIRS})
    execute_process(COMMAND git pull --ff-only --depth 1 origin ${GIT_TAG})
    execute_process(COMMAND git submodule init lib/printf)
    execute_process(COMMAND git submodule update --depth 1)
    execute_process(COMMAND git pull --ff-only --depth 1 origin ${GIT_TAG})
endif()
