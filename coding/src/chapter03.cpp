#include <iostream>

namespace math{
    int max(int a, int b){
        return (a > b) ? a : b;
    }
}
int main(){
    
    // 3.1
    std::cout << math::max(10,20) << std::endl;

    return 0;
}