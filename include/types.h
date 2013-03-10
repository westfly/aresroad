#ifndef WESTFLY_TYPES_H_
#define WESTFLY_TYPES_H_
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  TypeName& operator=(const TypeName&)
typedef unsigned int uint32_t;
class NonCopyable {
 private:
   NonCopyable();
   NonCopyable & operator=(const NonCopyable& that);
};
#endif
