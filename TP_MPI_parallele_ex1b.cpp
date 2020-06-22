#include <iostream>

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

int main(int argc, char ** argv)
{
    mpi::environment env(argc, argv);
    mpi::communicator world;
    mpi::timer timer;

    if (world.rank() == 0)  // master process
    {
        int nbRecv = 0;
        while (nbRecv < world.size() - 1)
        {
            std::string inMsg;
            mpi::status status = world.recv(mpi::any_source, 0, inMsg);
            std::cout << "Process #" << status.source() << " sent: " << inMsg << std::endl;
            nbRecv++;
        }
        std::cout << "Done in " << timer.elapsed() << " s" << std::endl;
    }
    else  // other process
    {
        sleep(10);
        std::string outMsg("hello");
        world.send(0, 0, outMsg);
    }

    return 0;
}

