#include <iostream>
#include <vector>

int main(int arc, char *argv[]){

    std::vector<int> vec = {1,2,3};

    for (auto i : vec){
        std::cout << i << std::endl;
    }
    return EXIT_SUCCESS;
}