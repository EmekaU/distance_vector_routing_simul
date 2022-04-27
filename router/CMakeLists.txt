#Include Directories to not have to specify dirs when including project headers
include_directories(${dvRouter_SOURCE_DIR}/kafkaService/include)
include_directories(${dvRouter_SOURCE_DIR}/router/include)

add_executable(${PROJECT_NAME} router.c helper.c)
target_link_directories(${PROJECT_NAME} PRIVATE kafkaService) # specify library location for linking process
target_link_libraries(${PROJECT_NAME} kafkaService)