#include "scene/group.hpp"

using namespace std;

void group::add(shared_ptr<model> model)
{
    m_models.push_back(model);
}
