#include "element_handles.h"
#include "abstract_handle.h"



element_handles_vector::element_handles_vector (const std::vector<abstract_handle *> &handles) : m_handles (handles)
{

}

element_handles_vector::~element_handles_vector ()
{
  for (auto handle : m_handles)
    delete (handle);
}
