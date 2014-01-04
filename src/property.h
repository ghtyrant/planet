class Property
{
public:
  Property() {};
  virtual ~Property() {};
};

template< typename T >
class TypedProperty : public Property
{
public:
  TypedProperty(const T& data) : Property(), data_(data) {}
  virtual const T& get() { return data_; }
  virtual void set(const T& data) { data_ = data; }
private:
  T data_;
};
