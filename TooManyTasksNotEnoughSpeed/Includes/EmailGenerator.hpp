//
//  Created by Jeremy Fonseca on September, 2022
//

#ifndef DBA35DB9_15E1_43C7_80DB_F863F0964A60
#define DBA35DB9_15E1_43C7_80DB_F863F0964A60

#include <queue>

#include "Email.hpp"
#include "EmailPriorityComparator.hpp"

struct EmailGenerator final
{
  static auto GenerateOne()
  {
    return Email();
  }

  static std::priority_queue<Email, std::vector<Email>, EmailPriorityComparator> GenerateMany(uint32_t quantity)
  {
    std::priority_queue<Email, std::vector<Email>, EmailPriorityComparator> mails{};
    for (uint32_t i = 0; i < quantity; ++i)
    {
      Email tmp;
      mails.push(tmp);
    }
    return mails;
  }
};

#endif /* DBA35DB9_15E1_43C7_80DB_F863F0964A60 */
