# SimpleRTTR
Run Time Type Reflection for C++x17

SimpleRTTR is a simple to use Run Time Type Reflection system built using C++17 and tested on multiple compilers and hardware.

There were 3 main requirements for SimpleRTTR.

1) It must be easy to use.
2) Minimal use of preprocessor declarations.
3) Must include as much information as possible to the user in a cross platform and predictable way.

Being easy to use is probably the biggest strength. To get started, all the need to do is include one file, then start registering your classes and types. There are no libraries to include, no compiler flags you need to maintain. It works with exceptions on, and it works with exceptions disabled. It works with namespaces, templates, classes, and structs. It is unit tested(although, I have to admit, I don't think it has %100 unit test coverage) and I do check for compiler and platform differences. Basically, what I'm trying to say here is that it should just work, everywhere, anywhere, all the time. Guaranteed or your money back!

Some use-cases for SimpleRTTR include

1) Automatic serialization of data(loading and unloading to JSON, BSON, etc.)
2) Automatic script binding, both through the use of code generation and dynamically at run-time
3) Anytime you need to know specifics about a class or struct at run time.

