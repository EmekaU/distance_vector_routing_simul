cmake_minimum_required(VERSION 3.20)
project(distance_vector_routing_simul C)

set(CMAKE_C_STANDARD 99)

add_executable(distance_vector_routing_simul router.c router.h helper.c helper.h kafkaService/kafkaProp.h kafkaService/producer.c)