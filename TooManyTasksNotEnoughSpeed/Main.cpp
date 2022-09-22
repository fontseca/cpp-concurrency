//
//  Created by Jeremy Fonseca on September, 2022
//

#include "Includes/Main.hpp"

auto main() -> int32_t
{
  std::cout << "Type ^C to kill main thread." << std::endl;
  std::cout << "Created by Jeremy Fonseca [https://github.com/fontseca]\n\n";

  EmailQueueManager email_manager{};
  std::signal(SIGINT, handle_process_finalization);
  std::vector<std::thread> threads_to_process_emails{};

  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 1, &threads_map, "Recipient 1", "iso45823@xcoxc.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 2, &threads_map, "Recipient 2", "hgs96707@cdfaq.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 3, &threads_map, "Recipient 3", "uut27591@nezid.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 4, &threads_map, "Recipient 4", "rhz98563@cdfaq.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 5, &threads_map, "Recipient 5", "rjb15996@xcoxc.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 6, &threads_map, "Recipient 6", "fhm70240@xcoxc.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 7, &threads_map, "Recipient 7", "ofa75154@nezid.com"));
  threads_to_process_emails.push_back(std::thread(&EmailQueueManager::QueueProcessor, &email_manager, 8, &threads_map, "Recipient 8", "vmo26821@nezid.com"));

  for (uint32_t i = 0; i < threads_to_process_emails.size(); ++i)
    if (threads_to_process_emails[i].joinable())
      threads_to_process_emails[i].join();

  return EXIT_SUCCESS;
}
