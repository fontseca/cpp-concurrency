//
//  Created by Jeremy Fonseca on September, 2022
//

#ifndef CE2BCA6A_2911_4473_82EC_3DB5815F76A8
#define CE2BCA6A_2911_4473_82EC_3DB5815F76A8

#include <random>

#include "Common.hpp"

struct Email
{

  Email()
  {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<std::mt19937::result_type> priority(1, 3);
    std::uniform_int_distribution<std::mt19937::result_type> id_generator(1, 9999999999);
    this->Priority = priority(gen);
    this->Id = std::string(std::to_string(id_generator(gen)) + "-" + std::to_string(id_generator(gen)) + "-" + std::to_string(id_generator(gen)));
    this->Status = EmailStatus::Pending;
  }

  std::string Id;
  uint32_t Priority;
  uint32_t Attempts = 0;
  EmailStatus Status;
};

#endif /* CE2BCA6A_2911_4473_82EC_3DB5815F76A8 */
