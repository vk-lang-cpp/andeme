#include <iostream>
#include <QObject>
#include <functional>
#include <string>
#include <client/client.h>
#include <schema/message.pb.h>

class MessageManager : public QObject {
   Q_OBJECT
    
  public:
    using InputCallback = std::function<std::string()>;
    using OutputCallback = std::function<void(const std::string&)>;

    MessageManager(
        const std::string& address,
        const InputCallback&  input,
        const OutputCallback& output) :
        client_(address),
        input_(input),
        output_(output) {
        client_.subscribe([this](andeme::schema::Message& msg) {
            output_(msg.mutable_msg()->text());
        });
   }

    public slots:
    void send() {
        std::string text = input_();
        if (!text.empty()) {
            andeme::schema::Message msg;
            msg.mutable_msg()->set_text(text);
            client_.send(msg);
        }
    }

    private:
    andeme::Client client_;
    const InputCallback input_;
    const OutputCallback output_;

};
