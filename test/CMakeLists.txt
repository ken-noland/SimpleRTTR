#GoogleTest
set(SIMPLERTTR_TEST_SRC		
							testallocator.cpp
							testconstructor.cpp
							testenum.cpp
							testmeta.cpp
							testmethod.cpp
							testfactory.cpp
							testhash.cpp
							testiterator.cpp
							testpreprocessor.cpp
							testproperties.cpp
							testsimplerttr.cpp
							testtypehelper.cpp
							testtypeinstance.cpp
							testtypes.cpp
							testvariant.cpp)

set(SIMPLERTTR_TEST_HEADERS testallocator.h
							testallocator.inl)

add_executable(SimpleRTTRTest ${SIMPLERTTR_TEST_SRC} ${SIMPLERTTR_TEST_HEADERS})
target_link_libraries(SimpleRTTRTest
  SimpleRTTR
  GTest::gtest_main
)

target_include_directories(SimpleRTTRTest PRIVATE ../include/simplerttr)

#target_link_options(SimpleRTTRTest PUBLIC /PROFILE)
add_test(SimpleRTTRGTest SimpleRTTRTest)
