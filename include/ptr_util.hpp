/*
Utilities wirten and collected by willswu@outlook.com
*/
#include<memory>

//Check if a weak_ptr is initialized or NOT (which means a weak_ptr constracted by a default constructor and never been assigned with a shared_ptr)
template <typename T>
bool is_uninitialized(std::weak_ptr<T> const& weak) {
    using wt = std::weak_ptr<T>;
    return !weak.owner_before(wt{}) && !wt{}.owner_before(weak);
}