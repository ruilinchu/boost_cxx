#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <ctime>
 
int main()
{ 
  typedef boost::mt19937                     ENG;    // Mersenne Twister
  typedef boost::normal_distribution<double> DIST;   // Normal Distribution
  typedef boost::variate_generator<ENG,DIST> GEN;    // Variate generator

  ENG  eng;
  eng.seed(static_cast<unsigned int>(std::time(0))); 

  DIST dist(0,1);
  GEN  gen(eng,dist);

  for (int i = 0; i < 20; i++)
    std::cout << gen() << std::endl;
 
  return 0;
}
