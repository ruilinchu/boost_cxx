// use two procs
// blocking, default MPI_Send, not good choice of send, use MPI_Ssend and MPI_Isend instead
#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;

int main()
{
  mpi::environment env;
  mpi::communicator world;

  if (world.rank() == 0) {
    world.send(1, 0, std::string("Hello"));
    std::string msg;
    world.recv(1, 1, msg);
    std::cout << msg << " #I'm " << world.rank() << std::endl;
  } else {
    std::string msg;
    world.recv(0, 0, msg);
    std::cout << msg << " #I'm " << world.rank() << std::endl;
    //std::cout.flush();
    world.send(0, 1, std::string("world"));
  }

  return 0;
}
