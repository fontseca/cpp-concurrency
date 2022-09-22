#ifndef C2601F16_6296_4B3D_978F_A6980DA0EC82
#define C2601F16_6296_4B3D_978F_A6980DA0EC82

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <csignal>
#include <map>
#include <fstream>

#include "EmailQueueManager.hpp"
#include "gnuplot-iostream.h"

const auto START_TIME = std::chrono::high_resolution_clock::now();

// For each thread keep metadata about the emails in a sub-dictionary.
std::map<uint32_t, std::map<std::string, uint32_t>> threads_map{{}};

void handle_process_finalization(int signum)
{
  if (signum == SIGINT)
  {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - START_TIME);
    uint32_t added_emails = 0;
    uint32_t executed_emails = 0;
    uint32_t failed_emails = 0;
    Gnuplot gp;

    for (auto &current_thread : threads_map)
      for (auto &th_metadata : current_thread.second)
      {
        if (th_metadata.first == "added")
          added_emails += th_metadata.second;
        if (th_metadata.first == "executed")
          executed_emails += th_metadata.second;
        if (th_metadata.first == "failed")
          failed_emails += th_metadata.second;
      }

    std::fstream file_data;
    file_data.open("data.dat", std::ios::out);

    file_data << "0 Added\t" << added_emails << std::endl;
    file_data << "1 Executed " << executed_emails << std::endl;
    file_data << "2 Failed\t" << failed_emails << std::endl;
    file_data.close();

    gp << "set terminal jpeg\n";
    gp << "set output 'out.jpeg'\n";
    gp << "plot 'data.dat' using 1:3:xtic(2) with boxes";

    gp.close();
    exit(signum);
  }
};

#endif /* C2601F16_6296_4B3D_978F_A6980DA0EC82 */
