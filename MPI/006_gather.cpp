
#include <boost/mpi.hpp>
#include <iostream>
#include <vector> //not needed actually
#include <cstdlib>
namespace mpi = boost::mpi;

int main()
{
  mpi::environment env;
  mpi::communicator world;

  //std::srand(time(0) + world.rank());
  int my_number = world.rank(); //std::rand();

  /*
  if (world.rank() == 0) {
    std::vector<int> all_numbers;
    gather(world, my_number, all_numbers, 0);
    for (int proc = 0; proc < world.size(); ++proc)
      std::cout << "Process #" << proc << " thought of " << all_numbers[proc] << std::endl;
  }else {
    gather(world, my_number, 0);
  }
  */

  //if (world.rank() == 0) 
  std::vector<int> all_numbers;

  gather(world, my_number, all_numbers, 0);

  if (world.rank() == 0) 
    for (int proc = 0; proc < world.size(); ++proc)
      std::cout << "Process #" << proc << " thought of " << all_numbers[proc] << std::endl;

  std::cout << "on proc " << world.rank() << " all_numbers size is " << all_numbers.size() << std::endl;
    

  return 0;
}
