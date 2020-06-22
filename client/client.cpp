#include <client.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

namespace andeme {

Client::Client(const std::string &address)
    : sender_(grpc::CreateChannel(address, grpc::InsecureChannelCredentials())),
      receiver_(
          grpc::CreateChannel(address, grpc::InsecureChannelCredentials())) {
  worker_ = std::thread([this]() {
    while (true) this->threadBody();
  });
}

bool Client::send(const andeme::schema::Message &msg) {
  grpc::ClientContext context;
  google::protobuf::Empty empty;
  std::lock_guard<std::mutex> lock(sender_mutex_);
  grpc::Status status = sender_.Send(&context, msg, &empty);
  return status.ok();
}

void Client::threadBody() {
  grpc::ClientContext context;
  ::google::protobuf::Empty empty;
  andeme::schema::Message msg;
  std::unique_ptr<grpc::ClientReader<andeme::schema::Message>> reader(
      receiver_.Subscribe(&context, empty));
  while (reader->Read(&msg)) {
    std::lock_guard lock(mutex_);
    for (auto callback : callbacks_) {
      callback(msg);
    }
  }
}

void Client::subscribe(const Callback &callback) {
  std::lock_guard lock(mutex_);
  callbacks_.push_back(callback);
}
}  // namespace andeme
