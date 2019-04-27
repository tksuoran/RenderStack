#include "scene/group.hpp"

void Group::add(std::shared_ptr<Model> model)
{
    m_models.push_back(model);
}
