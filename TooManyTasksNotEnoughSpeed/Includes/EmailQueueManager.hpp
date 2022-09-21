//
//  Created by Jeremy Fonseca on September, 2022
//

#ifndef A5DD61EB_42AF_4395_A305_647BCB92BF3C
#define A5DD61EB_42AF_4395_A305_647BCB92BF3C

#include <iostream>
#include <queue>
#include <random>
#include <fstream>

#include "Common.hpp"
#include "Email.hpp"
#include "Response.hpp"
#include "EmailGenerator.hpp"
#include "EmailPriorityComparator.hpp"
#include "SMTPClient.h"

struct EmailQueueManager
{
  EmailQueueManager()
  {
    this->OutFile.open("SentEmails.log.txt", std::ios::out);
  }

  ~EmailQueueManager()
  {
    this->OutFile.close();
  }

  std::priority_queue<Email, std::vector<Email>, EmailPriorityComparator> EmailsQueue;

  void QueueProcessor(const std::string_view recipient_name, const std::string_view recipient_email)
  {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 8);

    while (true)
    {
      // Generate [1, 10] random emails.
      this->EmailsQueue = this->GenerateEmails(dist(gen));
      while (!this->EmailsQueue.empty())
      {
        Email email = this->EmailsQueue.top();
        this->EmailsQueue.pop();

        if (email.Status == EmailStatus::Sent)
          this->OutFile << email.Id << ", " << email.Attempts << ", " << email.Priority << std::endl;

        if (email.Status == EmailStatus::Pending)
        {
          Response sending_response = this->SendEmail(email, recipient_name, recipient_email); // (Should be done in a thread.)
          this->EmailsQueue.push(sending_response.ResponseEmail);
        }
      }
      std::cout << "pending emails to send: " << this->EmailsQueue.size() << std::endl;
    }
  }

private:
  std::fstream OutFile;

  std::priority_queue<Email, std::vector<Email>, EmailPriorityComparator> GenerateEmails(uint32_t quantity)
  {
    return EmailGenerator::GenerateMany(quantity);
  }

  Response SendEmail(Email &email, const std::string_view recipient_name, const std::string_view recipient_email)
  {
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
    uint32_t error = dist(gen);

    if (!error)
    {
      auto logger = [](const std::string &str)
      {
        std::cout << str;
        return;
      };

      CSMTPClient SMTPClient(logger);
      const std::string email_provider{"smtps://smtp.gmail.com:465"};
      std::string sender_email = "xxxx";
      std::string sender_name = "xxxx";
      std::string sender_password = "xxxx";
      std::string sender_subject = "Report " + email.Id;
      std::stringstream ss;

      const bool session_started = SMTPClient.InitSession(email_provider, sender_email, sender_password, CMailClient::SettingsFlag::ALL_FLAGS, CMailClient::SslTlsFlag::ENABLE_SSL);

      if (session_started)
      {
        ss << "From: " << sender_name << " <" << sender_email << ">" << std::endl
           << "To: " << recipient_name << " <" << recipient_email << ">" << std::endl
           << "Subject: " << sender_subject << std::endl
           << std::endl
           << "Sending report: " + email.Id << std::endl
           << "Priority: " << email.Priority << std::endl;

        const bool send_status = SMTPClient.SendString(sender_email, recipient_email.data(), "", ss.str());
        SMTPClient.CleanupSession();
      }
      else
        error = 1;
    }

    if (error)
      email.Attempts += 1;

    Response res{};

    res.Status = error ? ResponseStatus::Error : ResponseStatus::Sent;
    email.Status = error ? EmailStatus::Pending : EmailStatus::Sent;
    res.ResponseEmail = std::move(email);

    std::cout << "[status:";

    if (res.ResponseEmail.Status == EmailStatus::Pending)
      std::cout << "pending]\t";

    if (res.ResponseEmail.Status == EmailStatus::Sent)
      std::cout << "sent]\t\t";

    std::cout << " email: " << res.ResponseEmail.Id;
    std::cout << ", attempts: " << res.ResponseEmail.Attempts;
    std::cout << ", recipient: " << recipient_email << std::endl;

    return res;
  }
};

#endif /* A5DD61EB_42AF_4395_A305_647BCB92BF3C */
