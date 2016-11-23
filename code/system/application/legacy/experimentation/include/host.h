/**
 * Run a data triggered guest module in a different conference than the host. The host module
 * forwards all containers to the guest module, therefore serving as a redirection between the
 * conferences.
 * 
 * @author Crispin Kirchner
 */

#ifndef OPENDLV_LEGACY_EXPERIMENTATION_HOST_H_
#define OPENDLV_LEGACY_EXPERIMENTATION_HOST_H_

#include <opendavinci/odcore/base/Service.h>
#include <opendavinci/odcore/base/module/DataTriggeredConferenceClientModule.h>

namespace opendlv {
namespace legacy {
namespace experimentation {

using odcore::base::Service;

using odcore::base::module::ClientModule;
using odcore::base::module::DataTriggeredConferenceClientModule;

using odcore::data::Container;
using odcore::data::dmcp::ModuleExitCodeMessage;
using odcore::data::dmcp::ModuleStateMessage;

/**
 * Class encapsulating an argument count and a pointer to the values. Sparing getters as the
 * members are const.
 */
class Arguments {
public:
    Arguments(int argc_, char ** argv_) : argc(argc_), argv(argv_) {}

    const int argc;
    char ** const argv;
};

/**
 * Run a module as a service in a separate thread.
 */
class ParallelModule : public Service {
public:
    ParallelModule(std::shared_ptr<ClientModule> module) : module_(module) { }

    /**
     * Get the exit code of the module. This throws if called before the module terminated.
     */
    ModuleExitCodeMessage::ModuleExitCode getExitCode() {
        if(!exitCode_) {
            throw std::runtime_error("module did not exit yet");
        }

        return *exitCode_;
    }

protected:
    void run() {
        serviceReady();

        exitCode_.reset(new ModuleExitCodeMessage::ModuleExitCode(module_->runModule()));
    }

    void beforeStop() {
        module_->setModuleState(ModuleStateMessage::NOT_RUNNING);
    }

private:
    std::shared_ptr<ClientModule> module_;
    std::unique_ptr<ModuleExitCodeMessage::ModuleExitCode> exitCode_;
};

/**
 * Non-template parts of the host module class.
 */
class HostBase : public DataTriggeredConferenceClientModule {
public:

    /**
     * Used to split the given arguments into host and guest args. Runs through the arguments until
     * the keyword "guest" is found. Everything before is considered host arguments. Everything
     * after (including the keyword) is considered to belong to the guest arguments.
     *
     * @return A pair of argument objects. The first element are the host arguments, the second the
     *         guest arguments.
     */
    static std::pair<Arguments, Arguments> splitArgs(int argc, char ** argv);

protected:
    HostBase(int argc, char ** argv) : DataTriggeredConferenceClientModule(argc, argv, MODULE_NAME) {}

    static const std::string MODULE_NAME;
};

/**
 * Data-triggered module which forwards all containers to its guest. Needs to be a template, as
 * nextContainer() in DataTriggeredModule is protected
 */
template<class G>
class Host : public HostBase {
public:
    Host(int argc, char ** argv, std::shared_ptr<G> guest)
        : HostBase(argc, argv)
        , guest_(guest) {}

    /**
     * Calling the body of the guest via runModule() in a separate thread, then run the host's body.
     */
    ModuleExitCodeMessage::ModuleExitCode runModule() {
        ParallelModule guestService(guest_);
        guestService.start();

        auto exitCode = HostBase::runModule();
        guestService.stop();

        return guestService.getExitCode();
    }

    void setUp() { }

    void nextContainer(Container & container) {
        guest_->nextContainer(container);
    }

    void tearDown() { }

private:
    std::shared_ptr<G> guest_;
};

}
}
}

#endif /* OPENDLV_LEGACY_EXPERIMENTATION_HOST_H_ */
