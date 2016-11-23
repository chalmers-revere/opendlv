#include <iostream>

#include <opendavinci/generated/odcore/data/LogMessage.h>

#include "host.h"

using odcore::base::module::DataTriggeredConferenceClientModule;
using odcore::data::Container;
using odcore::data::LogMessage;
using odcore::data::TimeStamp;

/**
 * Dummy data-triggered module, sends out a log message whenever it receives a time stamp
 */
class DemoModule : public DataTriggeredConferenceClientModule {
public:
    DemoModule(int argc, char ** argv) : DataTriggeredConferenceClientModule(argc, argv, MODULE_NAME) {}

    void setUp() {
        std::cout << MODULE_NAME << "::setUp()" << std::endl;
    }

    void nextContainer(Container & container) {
        if(TimeStamp::ID() != container.getDataType()) {
            return;
        }

        std::cout << MODULE_NAME << "::nextContainer()" << std::endl;

        LogMessage message;

        Container c(message);
        getConference().send(c);
    }

    void tearDown() {
        std::cout << MODULE_NAME << "::tearDown()" << std::endl;
    }

private:
    static const std::string MODULE_NAME;
};
const std::string DemoModule::MODULE_NAME("module");

/**
 * Main routine.
 */
int main(int argc, char ** argv) {
    using namespace opendlv::legacy::experimentation;

    auto args = HostBase::splitArgs(argc, argv);

    Arguments & hostArgs = args.first,
              & guestArgs = args.second;

    auto guest = std::make_shared<DemoModule>(guestArgs.argc, guestArgs.argv);
    Host<DemoModule> host(hostArgs.argc, hostArgs.argv, guest);

    host.runModule();
}
