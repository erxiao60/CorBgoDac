#include <boost/python.hpp>
#include "DmpAlgBgoDacor.h"

BOOST_PYTHON_MODULE(libDmpAlgBgoDacor){
  using namespace boost::python;

  class_<DmpAlgBgoDacor,boost::noncopyable,bases<DmpVAlg> >("DmpAlgBgoDacor",init<>());
}
