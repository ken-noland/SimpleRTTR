#GoogleTest
set(SIMPLERTTR_TEST_SRC		testallocator.cpp
							testmeta.cpp
							testmethod.cpp
							testsimplerttr.cpp
							testtypehelper.cpp
							testtypes.cpp)

set(SIMPLERTTR_TEST_HEADERS testallocator.h
							testallocator.inl)

add_executable(SimpleRTTRTest ${SIMPLERTTR_TEST_SRC} ${SIMPLERTTR_TEST_HEADERS})
target_link_libraries(SimpleRTTRTest
  SimpleRTTR
  GTest::gtest_main
)

add_test(SimpleRTTRGTest SimpleRTTRTest)
