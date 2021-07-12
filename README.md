[![Build Status](https://travis-ci.com/fundaev/sifter.svg?token=nnpi8UFeGgGq9v43yxVB&branch=master)](https://travis-ci.com/fundaev/sifter)

# Sifter
Sifter is the lightweight C++ library, providing container for a filter.

# Motivation
Let's imagine there is some repository-object. It allows to select the list of entities in according to some filter - set of conditions, joined by AND or OR operators. Number of conditions in this filter may vary: filter may be empty, may contain a single condition or several ones.

There is also something, which uses this repository. For example, it may be some REST API handler. It should transform requested URL into filter for repository to send in response the list of entities, satisfying this filter.

Sifter provides a possible solution for this task.

# Requirements
* C++17 or C++11 plus Boost v1.71 or later

# Terminology
Sifter provides two kinds of objects: condition and filter.

## Condition
Condition contains three fields:
* left value;
* right value;
* comparison operator.

In sifter this object is represented by `sifter::basic_condition` variadic template. The first template parameter - is comparison operator. It is assumed that it is some enumiration. The second template parameter - is default value of comparison, used in constructor. All other parameters are used in `std::variant` for left and right values.

Library provides also `sifter::comparison` comparison type and `sifter::condition` template - `sifter::basic_condition` template, specialized by `sifter::comparison` type.

## Filter
Filter is tree. It's left and right nodes are conditions or other filters. Filter contains also logic operator (AND or OR) of `sifter::operation` type.

Filter is represented by `sifter::basic_filter` variadic template. It has the same parameters as `sifter::basic_condition` template. Filter, specialized by `sifter::comparison` type, is represented by `sifter::filter` type.

# Example
```C++
#include <sifter/filter.hpp>
...

enum field
{
    id,
    name,
    age
};

using condition = sifter::condition<field, int, std::string>;
using filter = sifter::filter<field, int, std::string>;

...

filter f = condition(id) < 10 && (condition(name) % "John%" || condition(age) == 20);
std::vector<entity> entities = repository.get(f);
```
# Installation
```bash
mkdir build
cd build
cmake ..
make
make test
sudo make install
```

If you would like to avoid testing and building of examples you can run cmake with these arguments:
```
...
cmake -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF ..
...
```
