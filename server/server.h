#pragma once

#include <common/threadsafequeue.h>
#include <schema/services/messagesvc.grpc.pb.h>

#include <list>
#include <mutex>

namespace andeme {

class Server : public andeme::schema::services::MessageSvc::Service {
 public:
  ::grpc::Status Send(::grpc::ServerContext *context,
                      const ::andeme::schema::Message *request,
                      ::google::protobuf::Empty *response) override;

  ::grpc::Status Subscribe(
      ::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
      ::grpc::ServerWriter<::andeme::schema::Message> *writer) override;

 private:
  std::vector<std::weak_ptr<ThreadSafeQueue<andeme::schema::Message>>> queues_;
  std::mutex mutex_;
};

}  // namespace andeme
