#pragma once
#include <common/threadsafequeue.h>
#include <schema/message.pb.h>
#include <schema/services/messagesvc.grpc.pb.h>

#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace andeme {

class Client {
 public:
  using Callback = std::function<void(andeme::schema::Message &)>;

  Client(const std::string &address);

  bool send(const andeme::schema::Message &msg);
  void subscribe(const Callback &callback);

 private:
  void threadBody();

  std::vector<Callback> callbacks_;
  andeme::schema::services::MessageSvc::Stub sender_;
  andeme::schema::services::MessageSvc::Stub receiver_;
  std::thread worker_;
  std::mutex sender_mutex_;
  std::mutex mutex_;
};
}  // namespace andeme
