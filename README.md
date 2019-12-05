# Sifter
Sifter is lightweight C++ headers-only library, providing container for filter.

# Motivation
Let's imagine there is some repository-object. It allows to select the list of entities in according to some filter - set of conditions, joined by AND or OR operators. Number of conditions in this filter may vary: filter may be empty, may contain a single condition or several ones.

There is also something, which uses this repository. For example, it may be some REST API handler. It should transform requested URL into filter for repository to send in response the list of entities, satisfying this filter.

Sifter provides a possible solution for this task.

# Requirements
* C++17

# Terminology
Sifter provides two kinds of objects: condition and filter.

## Conditions
Condition contains three fields:
* left value;
* right value;
* comparison operator.

In sifter this object is represented by `sifter::basic_condition` variadic template. The first template parameter - is comparison operator. It is assumed that it is some enumiration. The second template parameter - is default value of comparison, used in constructor. All other parameters are used in `std::variant` for left and right values.
