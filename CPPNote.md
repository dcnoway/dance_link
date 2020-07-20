#smart_pointer
1. Do NOT constract more than one smart_pointer on one obj
2. std::enable_shared_from_this is a utility class helper to safely get a smart pointer like make_shared(this)
    2.1 class foo : public std::enable_shared_from_this<foo>
    {
        using std::enable_shared_from_this::shared_from_this();
    }
    2.2 IMPORTANT: every enabled_shared_from_this derived class, MUST call the shared_from_this() before its was hold by a shared_ptr
3. Unit testing framework
    Google test
    Catch2  https://github.com/catchorg/Catch2
4. Stab framework

5. Cpp package management tool
    microsoft/vspkg   https://github.com/microsoft/vcpkg/