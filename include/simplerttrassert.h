#pragma once

#if !defined(SIMPLERTTR_ASSERT)

#if defined( SIMPLERTTR_ERROR_TOSTDOUT )
#define SIMPLERTTR_ERROR(str) printf(str); throw std::runtime_error(str);
#else
#define SIMPLERTTR_ERROR(str) throw std::runtime_error(str);
#endif

#define SIMPLERTTR_STRINGIFY(value) #value
#define SIMPLERTTR_TOSTR(value) SIMPLERTTR_STRINGIFY(value)
#define SIMPLERTTR_ASSERT(exp)                                      \
            if(!(exp))                                              \
            {                                                       \
                std::string message =                               \
                    "Assertion Failed \n"                           \
                    "    File: " __FILE__ "\n"                      \
                    "    Line: " SIMPLERTTR_TOSTR(__LINE__) "\n"    \
                    "    Statement: " #exp "\n";                    \
                SIMPLERTTR_ERROR(message.c_str());                  \
            }
#define SIMPLERTTR_ASSERT_MSG(exp, msg)                             \
            if(!(exp))                                              \
            {                                                       \
                std::string message =                               \
                    "Assertion Failed \n"                           \
                    "    File: " __FILE__ "\n"                      \
                    "    Line: " SIMPLERTTR_TOSTR(__LINE__) "\n"    \
                    "    Statement: " #exp "\n";                    \
                    "    Message: " msg "\n";                       \
                SIMPLERTTR_ERROR(message.c_str());                  \
            }
#endif