//mpicxx 4_user_data_serialization.cpp -I $HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi

#include <boost/mpi.hpp>
#include <boost/mpi/communicator.hpp> //using mpi::any_tag to match any tag in recv
#include <iostream>
//#include <string>
#include <boost/serialization/base_object.hpp>
namespace mpi = boost::mpi;

struct ABC
{
  double cur_stock_price;
  double strike_price;
  double risk_free_rate;
  double option_price;
  std::vector <char> data;
};

namespace boost
{
 namespace serialization
 {
   template<class Archive>
   void serialize (Archive &ar, struct ABC &abc, unsigned int version)
   {
     ar & abc.cur_stock_price;
     ar & abc.strike_price;
     ar & abc.risk_free_rate;
     ar & abc.option_price;
     ar & abc.data;
   }
 }
}

//BOOST_IS_MPI_DATATYPE (ABC); //BUG, only small data with no pointers can use this

int main()
{
  mpi::environment env;
  mpi::communicator world;
  //int ANY_TAG = 0;
  if (world.rank () == 0)
    {
      ABC abc;
      abc.cur_stock_price = 1.0;
      abc.strike_price = 5.0;
      abc.risk_free_rate = 2.5;
      abc.option_price = 3.0;
      abc.data.push_back ('a');
      abc.data.push_back ('b');
      abc.data.push_back ('c');

      world.send ( 1, 199, abc );
      std::cout << "Rank 0 OK!" << std::endl;
    }

  else if (world.rank () == 1)
    {
      ABC abc;

      world.recv (0, mpi::any_tag, abc );
      std::cout << "Rank 1 OK!" << std::endl;

      for (int i = 0; i < abc.data.size(); i++)
	std::cout << i << "=" << abc.data[i] << std::endl;
    }

  return 0;
}
