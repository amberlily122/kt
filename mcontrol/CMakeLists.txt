cmake_minimum_required(VERSION 3.31)

project(kt-master-control)

add_library(kt-nameserver
  mc-nameserver.cpp
)
add_executable(ktmc
  ktmc.cpp
)
