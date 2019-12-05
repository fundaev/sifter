# Sifter
Sifter is lightweight C++ headers-only library, providing container for filter.

# Motivation
Let's imagine there is some repository-object. It allows to select the list of entities in according to some filter - set of conditions, joined by AND or OR operators. Number of conditions in this filter may vary: filter may be empty, may contain a single condition or several ones.

There is also something, which uses this repository. For example, it may be some REST API handler. It should transform requested URL into filter for repository to send in response the list of entities, satisfying this filter.

Sifter provides a possible solution for this task.
