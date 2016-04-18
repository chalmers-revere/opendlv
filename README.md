# OpenDLV


OpenDLV is an open software environment intended to run in autonomous self-driving vehicles. It runs on the vehicle itself and should handle hardware communication, safety and override functions, sensor fusion, decision making, and visualisation. It is written entirely in standard C++, and is formally tested (unit tests and code coverage). Its focus is on code clarity, portability, and performance.

The software is based on the compact middle-ware OpenDaVINCI (http://www.opendavinci.org).

## Getting started

OpenDLV is designed to work with standard C++ (C++11 or newer) and to depend on few dependencies.

### Clone and build source repository

Clone this repository:

    $ git clone https://github.com/chalmers-revere/opendlv.git

You can build the entire source tree:

    $ cd opendlv && mkdir build && cd build
    $ cmake ..
    $ make
 
Alternatively you can use locally compiled version of OpenDaVINCI by instead running:

    $ cmake -DOPENDAVINCI_DIR=/path/to/opendavinci ..

### Run the software

Next, you can test the software by executing different parts of the software stack.

Terminal 1 (odsupercomponent will load and provide the configuration data and realize the other components' life-cycle management; cf. http://docs.opendavinci.org):

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/odsupercomponent --cid=111 --verbose=1

Terminal 2:

    $ cd opendlv/build/out
    $ LD_LIBRARY_PATH=lib bin/opendlv-system-core-diagnostics --cid=111

## Development

The development of OpenDLV shall follow some principles:
* The code is intended to run on modern Linux systems, but we should avoid linking libraries which are not cross-platform.
* Always use the highest compiler warning level that is offered; let the compiler treat warnings as errors.
* Avoid using int/long; instead, use types with explicit size information like uint32_t for a 4 byte unsigned integer to improve portability.
* Obey the DRY principle: Don't repeat yourself (for comments, code-cloning, etc.).
* Write ISO-compliant code and avoid specific compiler-hacks; enable strict adherence to standards in the compiler.
* _NEVER_ use macros as they violate type safety.
* If you have to deal with critical resources, _always_ use scoped locks (cf. http://opendavinci.readthedocs.org/en/latest/tutorials.html#dining-philosophers).
* We shall minimize the number of dependencies in order to keep control of code quality, clarity, and portability. Currently the following external libraries are allowed:
  - OpenDaVINCI
  - Eigen
  - OpenCV
  - Qt (only for the visualization layer)
* The code shall be well tested. Consider that code is written once but read often. To avoid unintended side-effects make sure to provide proper test code (there are many CxxTest unit test cases available that will be run automatically during the build).
* (Can we test branch code coverage?)

### Function testing

TODO: Christian please add some guidelines and perhaps an example.

### Code coverage

TODO: Is this something we should test? In that case, how?

### Coding standard

This section describes the coding standard used within OpenDLV. Make sure to
keep the code readable, especially when it comes to variable, class, and method
names. Only add meaningful comments, the code itself should be
self-explanatory.

Further information about coding guidelines are found here:
* https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md
* J. Lakos: Large-Scale C++ Software Design: http://www.amazon.de/Large-Scale-Software-Addison-Wesley-Professional-Computing/dp/0201633620
* A. Hunt and D. Thomas: The Pragmatic Programmer. http://www.amazon.de/Pragmatic-Programmer-From-Journeyman-Master/dp/020161622X

General layout rules:
* Use double space indentation, and four space indentation when breaking a line.
* When declaring a constructor, always break the line at any class inheritance (after ':') or initialization list, then indent four spaces (see below).
* Namespaces do not indent.
* Generally break lines that are longer than 80 characters.
* When listing variables and methods, generally list them alphabetically.
* Use Doxygen-style documentation.

Naming rules:
* Files should generally be named in lowercase only.
* C++ files should end with 'cpp' for source files, and 'hpp' for header files.
* Classes and methods should be in camelcase and start in _uppercase_.
* Variables should be in camelcase and start in _lowercase_. 
* Abbreviations should be treated as a single word, staring in uppercase followed by lowercase letters.
* Always use explanatory names, there is _no need_ for short names.
* Use the Hungarian notations 'm_' and 'a_' for members and arguments.

Specific coding rules:
* Use the C++11 standard (we might soon change to C++14).
* Do _not_ use 'using namespace', the full path should be written for readability.
* _Never_ use raw pointers, instead use smart pointers to delegate resource management:
  - std::shared_ptr<TYPE> whenever an object needs to be shared.
  - std::unique_ptr<TYPE> whenever an object is _not_ shared.
  - std::weak_ptr<TYPE> when an object is optional or when breaking dependencies.

Here follows a code example that shows many of the aspects of the coding
standard. Please also browse the code for more examples.

```C++
#ifndef EXAMPLECLASSB_HPP_
#define EXAMPLECLASSB_HPP_

// Always include standard libs first, then external libs, then our own headers.
#include <memory>
#include <string>

#include "exampleclassa.hpp"

namespace opendlv {
namespace system {

// Use forward declaration whenever possible to speed up the compilation of a translation unit (cf. J. Lakos).
class ExampleClassC;

/**
 * Class documentation goes here.
 */
class ExampleClassB : public ExampleClassA {
  public:
    ExampleClassB(std::string const &); // Argument names are omitted.
    // C++11 style method deletion. These two methods should almost always be 
    // deleted to let the compiler warn us of unwanted object copying or assignment.
    ExampleClassB(ExampleClassB const &) = delete;
    ExampleClassB &operator=(ExampleClassB const &) = delete;
    virtual ~ExampleClassB();

    std::shared_ptr<ExampleClassC> GetClassMemberPointer() const;
    bool IsValid() const;

  private:
    // Generally list methods alphabetically.
    void FancyMethod();

    // In general, start listing objects before primitives, group on type, then 
    // go alphabetically.
    std::shared_ptr<ExampleClassC> m_classMemberPointer;
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
// All member MUST be declared in the initialization list and the constructor.
ExampleClassB::ExampleClassB(std::string const &a_arg) :
    ExampleClassA(),
    m_classMember(42),
    m_classMemberPointer(new ExampleClassC)
{
  std::cout << "This is a very long text thats prints the argument " << a_arg
      << " and breaks the line at column 80 with a double indent." 
      << std::endl; 
}

// Method brackets begins on new lines.
ExampleClassB::~ExampleClassB() 
{
}

std::shared_ptr<ExampleClassC> ExampleClassB::GetClassMemberPointer() const
{
  return m_classMemberPointer;
}

/**
 * Method documentation goes here.
 *
 */
void ExampleClassB::FancyMethod() const 
{
  uint32_t const numberOfRuns = 10;

  for (uint32_t i = 0; i < numberOfRuns; i++) {
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
