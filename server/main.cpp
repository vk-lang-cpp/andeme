#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <server.h>

#include <string>

int main() {
  std::string address = "0.0.0.0:12345";

  andeme::Server service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

  std::cout << "Server listening on " << address << std::endl;
  server->Wait();

  return 0;
}
