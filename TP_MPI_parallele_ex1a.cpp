#include <iostream>

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

int main(int argc, char ** argv)
{
	mpi::environment env(argc, argv);    
    mpi::communicator world;
    std::cout << "Hello, I am process #" << world.rank() << std::endl; 

    return 0;
}

