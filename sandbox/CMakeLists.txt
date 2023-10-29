cmake_minimum_required (VERSION 3.8)
project(Sandbox VERSION 1.0.0)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DLOGGING")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DLOGGING")

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE "source/*.*")
file(GLOB_RECURSE HEADERS "include/*.*")
foreach(SRC_FILE ${SOURCE} ${HEADERS})
  file(RELATIVE_PATH RELATIVE ${CMAKE_SOURCE_DIR} ${SRC_FILE})
  get_filename_component(SRC_FILE_DIR ${RELATIVE} DIRECTORY)
  source_group(${SRC_FILE_DIR} FILES ${SRC_FILE})
endforeach()

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(SDL2 REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../library_binaries)
find_package(SDL2_IMAGE REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../library_binaries)

set(LIB_FILES 
    "${SDL2_LIBRARIES}"
    "${PROJECT_SOURCE_DIR}/../library_binaries/lib/SDL2_image.lib"
    "${PROJECT_SOURCE_DIR}/../library_binaries/lib/AIL.lib"
) 

add_custom_command(TARGET Sandbox
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../library_binaries/bin/SDL2.dll ${Sandbox_BINARY_DIR}/.
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../library_binaries/bin/SDL2_image.dll ${Sandbox_BINARY_DIR}/.
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../library_binaries/bin/AIL.dll ${Sandbox_BINARY_DIR}/.
)

include_directories(${PROJECT_SOURCE_DIR}/../library_binaries/include)
target_link_libraries(${PROJECT_NAME} ${LIB_FILES})