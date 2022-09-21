#ifndef A0EC2683_9902_4AB4_B44D_4DEA28FCB448
#define A0EC2683_9902_4AB4_B44D_4DEA28FCB448

#include "Email.hpp"

struct EmailPriorityComparator
{
  bool operator()(const Email &email1, const Email &email2)
  {
    return email1.Priority > email2.Priority;
  }
};

#endif /* A0EC2683_9902_4AB4_B44D_4DEA28FCB448 */
