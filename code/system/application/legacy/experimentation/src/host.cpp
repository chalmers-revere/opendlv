/**
 * @author Crispin Kirchner
 */

#include "host.h"

namespace opendlv {
namespace legacy {
namespace experimentation {

/**
 * Builder class for arguments, allowing delayed setting of the members and conversion to the final
 * data structure.
 */
class ArgumentsBuilder {
public:
    void setArgc(int argc) {
        argc_ = argc;
    }

    void setArgv(char ** argv) {
        argv_ = argv;
    }

    Arguments build() const {
        return Arguments(argc_, argv_);
    }

private:
    int argc_;
    char ** argv_;
};

std::pair<Arguments, Arguments> HostBase::splitArgs(int argc, char ** argv) {
    const std::string guestKeyword("guest");

    ArgumentsBuilder hostArgsBuilder,
                     guestArgsBuilder;

    hostArgsBuilder.setArgv(argv);

    for(int i = 0; i < argc; ++i) {
        if(0 == guestKeyword.compare(argv[i])) {
            hostArgsBuilder.setArgc(i);

            guestArgsBuilder.setArgv(&argv[i]);
            guestArgsBuilder.setArgc(argc - i);

            break;
        }
    }

    return std::make_pair(hostArgsBuilder.build(), guestArgsBuilder.build());
}

const std::string HostBase::MODULE_NAME("host");

}
}
}

