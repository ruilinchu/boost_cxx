//mpicxx -std=c++11 6_gather_string.cpp -I$HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi

#include <boost/mpi.hpp>
#include <boost/serialization/string.hpp>
#include <vector>
#include <string>
//#include <iterator>
//#include <algorithm>
#include <iostream>
#include <boost/algorithm/string/join.hpp>//join vector of string to a string
int main()
{
  boost::mpi::environment env;
  boost::mpi::communicator world;
  std::string s_local;

  if (world.rank() == 0)
    {
      s_local = "I'm master!";
      std::vector<std::string> s_gather;
      boost::mpi::gather<std::string>(world, s_local, s_gather, 0);
      //std::ostream_iterator<std::string> out{std::cout, "#"};
      //std::copy(s_gather.begin(), s_gather.end(), out);

      std::string joinedString = boost::algorithm::join(s_gather, "-#-");
      std::cout << joinedString << std::endl;
    }
  else if (world.rank() == 1)
    {
      s_local = "Hello, world!";
      boost::mpi::gather(world, s_local, 0);
    }
  else if (world.rank() == 2)
    {
      s_local = "Hello, moon!";
      boost::mpi::gather(world, s_local, 0);
    }

  return 0;
}
