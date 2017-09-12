/**
 * Copyright (C) 2016 Chalmers Revere
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include <Wt/WApplication>

#include <opendavinci/odcore/data/Container.h>
#include <opendavinci/odcore/data/TimeStamp.h>

#include "livefeed.hpp"
#include "livefeedapp.hpp"

namespace opendlv {
namespace gui {

std::vector<LiveFeedApp *> LiveFeed::g_applications;

LiveFeed::LiveFeed(int32_t const &a_argc, char **a_argv)
    : DataTriggeredConferenceClientModule(a_argc, a_argv, "gui-livefeed"),
    m_server(new Wt::WServer("gui-livefeed"))
{
  char wtArg0[] = "gui-livefeed";
  char wtArg1[] = "--docroot";
  char wtArg2[] = ".";
  char wtArg3[] = "--http-address";
  char wtArg4[] = "0.0.0.0";
  char wtArg5[] = "--http-port";
  char wtArg6[] = "80";
  char *wtArgv[] = {&wtArg0[0], &wtArg1[0], &wtArg2[0], &wtArg3[0], &wtArg4[0], 
    &wtArg5[0], &wtArg6[0], NULL};
  int32_t wtArgc   = (int)(sizeof(wtArgv) / sizeof(wtArgv[0])) - 1;

  for (int32_t i = 0; i < wtArgc; i++) {
    std::cout << "Wt argument: " << wtArgv[i] << std::endl;
  }

  m_server->setServerConfiguration(wtArgc, wtArgv, "/etc/wt/wthttpd");
  m_server->addEntryPoint(Wt::Application, LiveFeed::CreateApplication);
}

LiveFeed::~LiveFeed() 
{
}

Wt::WApplication *LiveFeed::CreateApplication(Wt::WEnvironment const &a_environment)
{
  LiveFeedApp *app = new LiveFeedApp(a_environment);
  g_applications.push_back(app);
  return app;
}

void LiveFeed::setUp() 
{
//  std::string setting = getKeyValueConfiguration().getValue<std::string>(
//      "gui-livefeed.setting");
}

void LiveFeed::tearDown()
{
}

void LiveFeed::nextContainer(odcore::data::Container &a_container) 
{
  for (auto liveFeedApp : g_applications) {
    liveFeedApp->AddContainer(a_container);
  }
}

odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode LiveFeed::body() 
{

  puts("Body start");

//  while (getModuleStateAndWaitForRemainingTimeInTimeslice() 
//      == odcore::data::dmcp::ModuleStateMessage::RUNNING) {

  if (m_server->start()) {
    int sig = Wt::WServer::waitForShutdown("gui-livefeed");

    std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
    m_server->stop();

  //    if (sig == SIGHUP)
	//      Wt::WServer::restart(argc, argv, environ);
  //    }

  }
  
  puts("Body end");
 
// }

  return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
}

}
}
