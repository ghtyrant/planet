#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>
#include <memory>

#include "property.h"
#include "exceptions.h"

class GameObject
{
  public:
    GameObject(const std::string &name) : name_(name) {}
    virtual ~GameObject() {};

    const std::string &getName() { return name_; }

    template< typename T >
    T getProperty(const std::string &name) const throw(PropertyNotFoundException)
    {
      auto it = properties_.find(name);
      if (it == properties_.end())
        throw PropertyNotFoundException(name);

      return std::move(std::static_pointer_cast<TypedProperty<T>>(it->second)->get());
    }

    template< typename T >
    void setProperty(const std::string &name, const T& value)
    {
      if (properties_.find(name) == properties_.end())
        properties_[name] = std::shared_ptr<Property>(new TypedProperty<T>(value));
      else
      {
        std::shared_ptr<Property> tmp = properties_[name];
        std::shared_ptr<TypedProperty<T>> tmp_t = std::static_pointer_cast<TypedProperty<T>>(tmp);
        tmp_t->set(value);
      }
    }
  protected:
    std::string name_;
    std::map<std::string, std::shared_ptr<Property>> properties_;
  private:
};

#endif // GAMEOBJECT_H
