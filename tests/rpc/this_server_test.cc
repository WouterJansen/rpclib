#include "gtest/gtest.h"

#include <future>

#include "rpc/client.h"
#include "rpc/server.h"
#include "rpc/this_server.h"

#include "testutils.h"

using namespace rpc::testutils;
using namespace rpc;

#ifdef RPCLIB_USE_LOCAL_SOCKETS
#define ENDPOINT "/tmp/test.socket"
#else
static RPCLIB_CONSTEXPR uint16_t test_port = rpc::constants::DEFAULT_PORT;
#define ENDPOINT "127.0.0.1", test_port
#endif

class this_server_test : public testing::Test {
public:
    this_server_test()
        : s(ENDPOINT),
          c1(ENDPOINT),
          c2(ENDPOINT) {}

protected:
    rpc::server s;
    rpc::client c1;
    rpc::client c2;
};

TEST_F(this_server_test, stop_async) {
    s.bind("stop_server", []() { rpc::this_server().stop(); });
    s.async_run();
    c1.call("stop_server");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(c1.get_connection_state(),
              client::connection_state::disconnected);
    EXPECT_EQ(c2.get_connection_state(),
              client::connection_state::disconnected);
}

TEST_F(this_server_test, stop_sync) {
    s.bind("stop_server", []() { rpc::this_server().stop(); });

    auto handle = std::async(std::launch::async, [this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        c1.call("stop_server");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });

    s.run();
    handle.get();

    EXPECT_EQ(c1.get_connection_state(),
              client::connection_state::disconnected);
    EXPECT_EQ(c2.get_connection_state(),
              client::connection_state::disconnected);
}
