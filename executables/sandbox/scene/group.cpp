#include "scene/group.hpp"

void group::add(std::shared_ptr<model> model)
{
   m_models.push_back(model);
}
