#include "ns3/test.h"
#include "ns3/log.h"
#include <quiche.h> // Includes Cloudflare's quiche FFI header

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Ns3QuicheTest");

// Simple test case to verify quiche library integration
class Ns3QuicheTestCase : public TestCase
{
public:
  Ns3QuicheTestCase ();
  virtual ~Ns3QuicheTestCase ();

private:
  virtual void DoRun (void);
};

Ns3QuicheTestCase::Ns3QuicheTestCase ()
  : TestCase ("Ns3Quiche basic integration test")
{
}

Ns3QuicheTestCase::~Ns3QuicheTestCase ()
{
}

void
Ns3QuicheTestCase::DoRun (void)
{
  // 1. Test calling a basic function from quiche.h
  // quiche_version() returns the version string of the underlying rust quiche library
  const char* version = quiche_version();
  
  NS_LOG_INFO ("==========================================");
  NS_LOG_INFO ("Successfully linked Cloudflare Quiche!");
  NS_LOG_INFO ("Quiche Version: " << (version ? version : "Unknown"));
  NS_LOG_INFO ("==========================================");

  // Ensure that the version string is successfully fetched
  NS_TEST_ASSERT_MSG_NOT_NULL (version, "Failed to retrieve Quiche version via FFI");

  // 2. Test initializing a quiche configuration object
  // QUICHE_PROTOCOL_VERSION corresponds to the supported IETF QUIC version
  quiche_config *config = quiche_config_new (QUICHE_PROTOCOL_VERSION);
  NS_TEST_ASSERT_MSG_NOT_NULL (config, "Failed to create quiche_config");

  // Clean up config object
  quiche_config_free (config);
}

// The Test Suite that registers the test case
class Ns3QuicheTestSuite : public TestSuite
{
public:
  Ns3QuicheTestSuite ();
};

Ns3QuicheTestSuite::Ns3QuicheTestSuite ()
  : TestSuite ("ns3-quiche", UNIT)
{
  AddTestCase (new Ns3QuicheTestCase, TestCase::QUICK);
}

// Static instance to register the test suite
static Ns3QuicheTestSuite s_ns3QuicheTestSuite;