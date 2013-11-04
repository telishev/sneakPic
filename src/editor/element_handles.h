#ifndef ELEMENT_HANDLES_H
#define ELEMENT_HANDLES_H

#include <vector>

class abstract_handle;

class element_handles
{
public:
  virtual ~element_handles () {}

  virtual std::vector<abstract_handle *> handles () = 0;
};

#endif // ELEMENT_HANDLES_H
