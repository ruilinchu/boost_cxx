#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace boost;
using namespace boost::numeric::ublas;
using namespace boost::archive;

#include <iostream>
#include <fstream>
using namespace std;

int main( void)
{
  {
    int N =5;
    int M =2;
    matrix<int> mat_write( N, M);
    for( size_t i =0; i <mat_write.size1(); ++i){
      for( size_t j =0; j <mat_write.size2(); ++j){
	mat_write( i, j) =i+(j+1);
      }
    }
    
    cout <<mat_write <<endl;

    ofstream file( "save.dat");
    text_oarchive oa( file);
    //oa <<(const matrix<int>)mat_write;
    oa << mat_write;
    file.close();
  }

  {
    matrix<int> mat_read;
    ifstream file( "save.dat");
    text_iarchive ia( file);
    ia >> mat_read;
    file.close();
    
    cout << mat_read <<endl;
  }

  return 0;
}
