#include <iostream>
#include "docker.hpp"

int main(int argc,char *argv[]) {
    std::cout << "...start container" << std::endl;
    docker::container_config config;

    docker::container container(config);
    container.start();
    std::cout<<"stop container..."<<std::endl;

    return 0;
}
