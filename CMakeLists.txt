# Copyright (c) 2021 Sergei Fundaev
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.9)
project(sifter)

include(CTest)

set(SIFTER_USE_BOOST_VARIANT OFF CACHE BOOL "Use boost::variant instead of std::variant")

if (${SIFTER_USE_BOOST_VARIANT})
    set(CMAKE_CXX_STANDARD 11)
    add_definitions(-DSIFTER_USE_BOOST_VARIANT)
    message("boost::variant2 is used instead of std::variant")
else()
    set(CMAKE_CXX_STANDARD 17)
endif()

include_directories(include)
add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(test)
endif()

set(BUILD_EXAMPLES ON CACHE BOOL BUILD_EXAMPLES)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()