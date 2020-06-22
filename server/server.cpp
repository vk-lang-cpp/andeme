#include <server.h>

namespace andeme {

::grpc::Status Server::Send(::grpc::ServerContext *context,
                            const ::andeme::schema::Message *request,
                            ::google::protobuf::Empty *response) {
  std::unique_lock lock(mutex_);

  for (auto q : queues_) {
    auto queue = q.lock();
    if (queue) {
      queue->push(*request);
    }
  }

  return grpc::Status::OK;
}

::grpc::Status Server::Subscribe(
    ::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
    ::grpc::ServerWriter<::andeme::schema::Message> *writer) {
  auto queue = std::make_shared<ThreadSafeQueue<andeme::schema::Message>>();

  {
    std::unique_lock lock(mutex_);
    queues_.push_back(queue);
  }

  while (true) {
    writer->Write(queue->pop());
  }
  return grpc::Status::OK;
}
}  // namespace andeme
