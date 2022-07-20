cmake_minimum_required(VERSION 3.20)
project(sshLooter_Local C)

set(CMAKE_C_STANDARD 11)

add_executable(sshLooter_Local pam_sshaudit.c)
