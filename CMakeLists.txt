cmake_minimum_required(VERSION 3.20)
project(dvRouter)
add_subdirectory(kafkaService)

#Download json-c submodule
find_package(Git QUIET)
if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    #UPDATE SUBMODULE AS NEEDED
    option(GIT_SUBMODULE "Check json-c submodule during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout")
        endif ()
    endif()
endif ()

#Check all submodules
if (NOT EXISTS "${PROJECT_SOURCE_DIR}/external/json_lib/CMakeLists.txt")
    message(FATAL_ERROR "The json-c submodule was not downloaded! GIT_SUBMODULE was turned off or failed. Please check")
endif ()

add_subdirectory(external/json_lib)
add_subdirectory(router)