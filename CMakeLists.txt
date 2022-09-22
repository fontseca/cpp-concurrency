cmake_minimum_required(VERSION 3.24)

project(
  TooManyTasksNotEnoughSpeed
  VERSION 1.0.0
  LANGUAGES C CXX
  HOMEPAGE_URL "https://github.com/fontseca/super-simple-gmail-client"
  DESCRIPTION "Super-Simple C++ Gmail client."
)

set(Boost_NO_WARN_NEW_VERSIONS ON)
find_package(Boost 1.80.0 REQUIRED COMPONENTS iostreams)


add_subdirectory(${PROJECT_NAME})