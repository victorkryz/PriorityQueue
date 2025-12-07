#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Server/Server.h"
#include "Utils/apptm.h"

using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::_;
using ::testing::DoAll;
using ::testing::IgnoreResult;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::Expectation;
using ::testing::internal::GetTypeName;

class MockServer : public Server
{
  protected:       
   ~MockServer() override = default;  

  public:   
    MOCK_METHOD(void, onIdle, ());
    MOCK_METHOD(void, onMsg, (const Msg& msg), (override));

    void onMsgOrigin(const Msg& msg) {
        Server::onMsg(msg);
    };
};

class ServerTS :  public ::testing::Test
{
    void SetUp() override 
    {
        server_ = new NiceMock<MockServer>();
        ASSERT_NE(server_, nullptr);

        spChannel_.reset(ChannelFactory::getPriorityQueueInstance());
        ASSERT_TRUE(spChannel_);
    };

    void TearDown() override {
        delete server_;
        server_ = nullptr;
    };

  protected:  
    NiceMock<MockServer>* server_ = nullptr;
    std::unique_ptr<Channel> spChannel_;
};


MATCHER_P(CheckMsg, ref_msg, "") {
    const auto& param_msg = std::get<0>(arg);
    return (param_msg.getContent().dwClientId == ref_msg.getContent().dwClientId &&
            param_msg.getContent().cPriority == ref_msg.getContent().cPriority &&
            param_msg.getContent().dwTicks == ref_msg.getContent().dwTicks);
};

TEST_F(ServerTS, MsgFlowTest)
{
    Msg msg(TDATA{5, getTicksSinceAppStart(), 2277});

    ON_CALL(*server_, onMsg)
            .WillByDefault([this](const Msg& msg) {
              server_->onMsgOrigin(msg);
            });

    Expectation onMsgExp = 
    EXPECT_CALL(*server_, onMsg(_))
                .With(CheckMsg(msg));

    EXPECT_CALL(*server_, onIdle())
                .After(onMsgExp)
                .WillOnce([this]() {
                    server_->scheduleShutDown();
                });
                
    spChannel_->pushMsg(msg);

    Server::run(server_, spChannel_);

    EXPECT_EQ(server_->getReceivedMsgCount(), 1);
}