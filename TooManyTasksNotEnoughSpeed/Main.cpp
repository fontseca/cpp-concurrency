//
//  Created by Jeremy Fonseca on September, 2022
//

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <csignal>

#include "SMTPClient.h"

auto start_time = std::chrono::high_resolution_clock::now();

enum class EmailStatus
{
  Pending,
  Sent,
};

enum class ResponseStatus
{
  Sent,
  Error,
};

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

struct Response
{
  ResponseStatus Status;
  Email ResponseEmail;
};

struct EmailGenerator final
{
  static auto GenerateOne()
  {
    return Email();
  }

  static auto GenerateMany(uint32_t quantity)
  {
    std::queue<Email> mails{};
    for (uint32_t i = 0; i < quantity; ++i)
    {
      Email tmp;
      mails.push(tmp);
    }
    return mails;
  }
};

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

  std::queue<Email> EmailsQueue{};

  void QueueProcessor(const std::string_view recipient_name, const std::string_view recipient_email)
  {
    using namespace std::chrono_literals;
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1, 8);

    while (true)
    {
      // Generate [1, 10] random emails.
      this->EmailsQueue = this->GenerateEmails(dist(gen));
      while (!this->EmailsQueue.empty())
      {
        Email email = this->EmailsQueue.front();
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

  std::queue<Email> GenerateEmails(uint32_t quantity)
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

void handle_end(int signum)
{
  if (signum == SIGINT)
  {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
    std::cout << "\ntotal time to process emails: " << elapsed_time.count() << "s" << std::endl;
    exit(signum);
  }
};

void process_queue(const std::string_view recipient_name, const std::string_view recipient_email)
{
  EmailQueueManager email_que_managuer{};
  email_que_managuer.QueueProcessor(recipient_name, recipient_email);
}

auto main() -> int32_t
{
  std::signal(SIGINT, handle_end);
  std::vector<std::thread> threads_to_process_emails{};

  threads_to_process_emails.push_back(std::thread(process_queue, "1", "iso45823@xcoxc.com"));
  // threads_to_process_emails.push_back(std::thread(process_queue, "2", "fhm70240@xcoxc.com"));

  if (threads_to_process_emails[0].joinable())
    threads_to_process_emails[0].join();

  // if (threads_to_process_emails[1].joinable())
  //   threads_to_process_emails[1].join();

  return EXIT_SUCCESS;
}
