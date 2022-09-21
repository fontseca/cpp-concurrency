cmake_minimum_required(VERSION 3.24)

project(
  SimpleGmailClient
  VERSION 1.0.0
  LANGUAGES C CXX
  HOMEPAGE_URL "https://github.com/fontseca/super-simple-gmail-client"
  DESCRIPTION "Super-Simple C++ Gmail client."
)

add_executable(
  ${PROJECT_NAME}
    Main.cpp
)

target_include_directories(
  ${PROJECT_NAME}
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/Libs/MailClient/Includes
)

target_link_libraries(
  ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/Libs/MailClient/Binaries/libmailclient.a
    curl
)