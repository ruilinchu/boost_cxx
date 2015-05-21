//mpicxx -std=c++0x 11_scatter_vector_string.cpp -I$HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi
#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <vector>
#include <string>
#include <iostream>

int main()
{
  boost::mpi::environment env;
  boost::mpi::communicator world;
  std::vector<std::string> v{"Hello, world!", "Hello, moon!",
      "Hello, sun!"};
  std::string s;
  boost::mpi::scatter(world, v, s, 0);
  std::cout << "on proc: " << world.rank() << ": " << s << '\n';

  return 0;
}
