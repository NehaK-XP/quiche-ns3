/*
 * Smoke test: verifies quiche is found, linked and callable from ns-3.
 * Place at: src/ns3-quiche/test/ns3-quiche-test-suite.cc
 */
#include "ns3/test.h"

#include <quiche.h>

#include <cstring>
#include <iostream>

using namespace ns3;

class QuicheLinkTestCase : public TestCase
{
  public:
    QuicheLinkTestCase()
        : TestCase("quiche FFI links and basic calls work")
    {
    }

  private:
    void DoRun() override
    {
        // 1. Simplest possible call: version string
        const char* version = quiche_version();
        NS_TEST_ASSERT_MSG_NE(version, nullptr, "quiche_version() returned null");
        NS_TEST_ASSERT_MSG_GT(std::strlen(version), 0u, "quiche_version() returned empty string");
        std::cout << "[quiche-ns3] quiche version: " << version << std::endl;

        // 2. Create and configure a quiche_config, then free it
        quiche_config* config = quiche_config_new(QUICHE_PROTOCOL_VERSION);
        NS_TEST_ASSERT_MSG_NE(config, nullptr, "quiche_config_new() failed");

        // ALPN: length-prefixed "h3"
        static const uint8_t alpn[] = {0x02, 'h', '3'};
        int rc = quiche_config_set_application_protos(config, alpn, sizeof(alpn));
        NS_TEST_ASSERT_MSG_EQ(rc, 0, "set_application_protos failed");

        quiche_config_set_max_idle_timeout(config, 5000);
        quiche_config_set_initial_max_data(config, 10000000);
        quiche_config_set_initial_max_stream_data_bidi_local(config, 1000000);
        quiche_config_set_initial_max_streams_bidi(config, 100);

        quiche_config_free(config);
    }
};

class QuicheTestSuite : public TestSuite
{
  public:
    QuicheTestSuite()
        : TestSuite("ns3-quiche", Type::UNIT)
    {
        AddTestCase(new QuicheLinkTestCase, Duration::QUICK);
    }
};

static QuicheTestSuite g_quicheTestSuite;