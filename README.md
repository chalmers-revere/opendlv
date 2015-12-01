# OpenDLV

OpenDLV is an open software environment intended to run in autonomous self-driving vehicles. It runs on the vehicle itself and should handle hardware communication, safety and override functions, sensor fusion, decision making, and visualisation. It is written entirely in standard C++, and is formally tested (unit tests and code coverage).

The software is based on the compact middle-ware OpenDaVINCI (http://www.opendavinci.org).

## Clone and build source repository

Clone this repository:

    $ git clone https://github.com/chalmers-revere/opendlv.git

You can build the entire source tree:

    $ cd opendlv && mkdir build && cd build
    $ cmake ..
    $ make

Alternatively you can use locally compiled version of OpenDaVINCI by instead running:

    $ cmake -DOPENDAVINCI_DIR=/path/to/opendavinci ..

## Test the software

Next, you can test the software by executing different parts of the software stack.

Terminal 1 (odsupercomponent will load and provide the configuration data):

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/odsupercomponent --cid=111 --verbose=1

Terminal 2:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/opendlv-system-core-diagnostics --cid=111

## Coding standard

This section describes the coding standard used within OpenDLV. Make sure to
keep the code readable, especially when it comes to variable, class, and method
names. Only add meaningful comments, the code itself should be
self-explanatory. 

Some general things:
* Use double space indentation, and four space indentation when breaking a line.
* Namespaces does not indent.
* Do _not_ use 'using namespace'.
* Use the Hungarian notations 'm_' and 'a_' for members and arguments.
* Use the C++14 standard.
* Use smart pointers, _avoid_ using raw pointers
  - shared_ptr<TYPE> whenever an object needs to be shared
  - unique_ptr<TYPE> whenever an object is _not_ shared
  - weak_ptr<TYPE> when an object is optional or when breaking dependencies 

Here follows a code example that shows many of the aspects of the coding
standard. Please also browse the code for more examples.

```C++
#ifndef EXAMPLECLASSB_HPP_
#define EXAMPLECLASSB_HPP_

#include "exampleclassa.hpp"

namespace opendlv {
namespace system {

// Use forward declaration whenever possible.
class ExampleClassC;

/**
 * Class documentation goes here.
 */
class ExampleClassB : public ExampleClassA {
  public:
    ExampleClassB(std::string const &); // Argument names are omitted.
    // C++11 style method deletion. These two methods should almost always be 
    // deleted.
    ExampleClassB(ExampleClassB const &) = delete;
    ExampleClassB &operator=(ExampleClassB const &) = delete;
    virtual ~ExampleClassB();
    bool IsValid() const;

  private:
    // Generally list methods alphabetically.
    void FancyMethod();

    // In general, start listing objects before primitives, then go 
    // alphabetically.
    std::unique_ptr<ExampleClassC> m_classMemberPointer;
    uint32_t m_classMember;
};

} // system
} // opendlv

#endif
```

```C++
#include <iostream>

#include "exampleclassb.hpp"
#include "exampleclassc.hpp"

// The opendlv namespace should always be in the top.
// Otherwise the structure should be relatively flat.
namespace opendlv {
namespace system {

// Note that we pass references and that const goes after type.
// All member must be declared in the initialization list and the constructor.
ExampleClassB::ExampleClassB(std::string const &a_arg2) :
    ExampleClassA(),
    m_classMember(42),
    m_classMemberPointer(new ExampleClassC)
{
  std::cout << "This is a very long text thats prints the argument " << a_arg2
      << " and breaks the line at column 80 with a double indent." 
      << std::endl; 
}

// Method brackets begins on new lines.
ExampleClassB::~ExampleClassB() 
{
}

/**
 * Method documentation goes here.
 *
 */
void ExampleClassB::FancyMethod() const 
{
  int const numberOfRuns = 10;

  for (int i = 0; i < numberOfRuns; i++) {
    // Even short statements should have brackets.
    if (i > 5) {
      m_classMember += i;
    } else {
      m_classMember -= i;
    }
  }
}

// Methods should be defined const whenever they can.
bool ExampleClassB::IsValid() const
{
  return true;
}

} // system
} // opendlv
```
