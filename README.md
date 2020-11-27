## `member_count`
---
A `constexpr` utility to determine the number of member fields in an aggregate `struct`.

### Example usage
---
```cpp
// Struct with three member variables
struct S
{
    char a, b, c;
};

// Check that S has three members at compile time
static_assert(stdex::member_count<S> == 3, "S does not have 3 members.");
```

### Requirements
---
This lib requires GCC version 9 or above due to a bug fix provided for uniform aggregate initialisation of `unions`.
I haven't traced back the minimum MSVC version yet, but it does work with v19.14 and above.
* GCC > 9.0.0
* Clang > 3.5.0
* MSVC > 19.14 (probably lower?)
* C++14

### Build
---
The utility is header only, simply include `"member_count.hpp"` to use (or `"member_count_cpp20.hpp"` to use concepts).

### Tests
---
The unit tests have been added as `static_asserts` (because why not?) and are included at the end of the main header so they are always checked.
A dummy translation unit has been provided for easy of checking the compilation:

```
$(CXX) test/test.cpp
```

### TODO
---
- [x] Support for anonymous `unions` and unnamed `structs`.
- [x] Portable across GCC and Clang.
- [x] Basic unit tests.
- [ ] Trace minimum MSVC version.
- [ ] More unit tests.
- [ ] Configurable `namespace`.
- [ ] Continuous integration.
