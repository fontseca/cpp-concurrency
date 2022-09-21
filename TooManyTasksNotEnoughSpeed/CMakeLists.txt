cmake_minimum_required(VERSION 3.24)

project(
  SimpleGmailClient
  VERSION 1.0.0
  LANGUAGES C CXX
)

add_executable(
  ${PROJECT_NAME}
    Includes/Common.hpp
    Includes/Email.hpp
    Includes/EmailQueueManager.hpp
    Includes/EmailGenerator.hpp
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