// sending ublas matrix using serialization
// ublas have serializatioin built-in

//non-blocking MPI_Isend and MPI_Irecv, must call wait or test to finish the communication
#include <boost/mpi.hpp>
#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/serialization/serialization.hpp>

namespace mpi = boost::mpi;
namespace ublas = boost::numeric::ublas;

typedef ublas::matrix<int, ublas::column_major> m_t;
//typedef ublas::matrix<int> m_t;  // ublas default is row_major !!

using std::cout;
using std::endl;

BOOST_IS_BITWISE_SERIALIZABLE(m_t);//Homogeneous machines

int main()
{
  mpi::environment env;
  mpi::communicator world;
  
  std::size_t M = 5;
  std::size_t N = 2;
  m_t a (M, N);
  int tag = 100;
  if( world.rank() == 0 ){
    cout << " rows: " << a.size1() << endl;
    cout << " cols: " << a.size2() << endl;
    
    for(std::size_t i =0; i < a.size1(); ++i)
      for( std::size_t j =0; j < a.size2(); ++j)
	a( i, j) = i+j+1;
    
    cout << "a on proc " << world.rank() << endl << a << endl;      
    
    mpi::request reqs;
    reqs = world.isend(1, tag, a);
    //do something else here
    reqs.wait();
  }  else {
    mpi::request reqs;
    reqs = world.irecv(0, tag, a);
    //do something else here
    reqs.wait();
    cout << "a on proc " << world.rank() << endl << a << endl;      
  }
  return 0;
}
