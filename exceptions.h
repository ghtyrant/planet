#include <exception>

class PropertyNotFoundException : public std::exception
{
public:
  PropertyNotFoundException(const std::string &name) : name_(name) {}
  virtual const char *what() const throw() {return name_.c_str();}
protected:
  std::string name_;
};
