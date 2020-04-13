cmake_minimum_required(VERSION 3.4)

project(BigIntegerSample VERSION 1.0.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cc
    BigInteger.cc
)