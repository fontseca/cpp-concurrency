//
//  Created by Jeremy Fonseca on September, 2022
//

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

#include "Includes/EmailQueueManager.hpp"

const auto START_TIME = std::chrono::high_resolution_clock::now();

void handle_process_finalization(int signum)
{
  if (signum == SIGINT)
  {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - START_TIME);
    std::cout << "\ntotal time to process emails: " << elapsed_time.count() << "s" << std::endl;
    exit(signum);
  }
};

void process_single_queue(const std::string_view recipient_name, const std::string_view recipient_email)
{
  EmailQueueManager email_que_managuer{};
  email_que_managuer.QueueProcessor(recipient_name, recipient_email);
}

auto main() -> int32_t
{
  std::signal(SIGINT, handle_process_finalization);
  std::vector<std::thread> threads_to_process_emails{};

  threads_to_process_emails.push_back(std::thread(process_single_queue, "R2", "iso45823@xcoxc.com"));
  // threads_to_process_emails.push_back(std::thread(process_single_queue, "R1", "fhm70240@xcoxc.com"));

  if (threads_to_process_emails[0].joinable())
    threads_to_process_emails[0].join();

  // if (threads_to_process_emails[1].joinable())
  //   threads_to_process_emails[1].join();

  return EXIT_SUCCESS;
}
