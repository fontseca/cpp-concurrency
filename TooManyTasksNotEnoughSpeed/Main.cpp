//
//  Created by Jeremy Fonseca on September, 2022
//

#include "Includes/Main.hpp"

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
