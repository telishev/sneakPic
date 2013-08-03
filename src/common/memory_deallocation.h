#ifndef MEMORY_DEALLOCATION_H
#define MEMORY_DEALLOCATION_H

#define FREE(DATA) \
do                 \
{                  \
  delete DATA;     \
  DATA = nullptr;  \
} while (0)        \

#define FREE_ARRAY(DATA) \
do                       \
{                        \
  delete[] DATA;         \
  DATA = nullptr;        \
} while (0)              \


#endif // MEMORY_DEALLOCATION_H
