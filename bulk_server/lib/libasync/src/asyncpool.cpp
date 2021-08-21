#include "asyncpool.h"

AsyncPool* AsyncPool::m_instance;
std::once_flag AsyncPool::m_instance_once;
