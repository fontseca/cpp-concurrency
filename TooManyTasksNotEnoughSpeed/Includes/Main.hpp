#ifndef C2601F16_6296_4B3D_978F_A6980DA0EC82
#define C2601F16_6296_4B3D_978F_A6980DA0EC82

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>

#include "EmailQueueManager.hpp"

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

#endif /* C2601F16_6296_4B3D_978F_A6980DA0EC82 */
