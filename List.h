#include <list>
#include <iostream>


int main() {
    std::list<int> a(23);
    std::allocator<int> b = a.get_allocator();
    std::cout << 1;
}
