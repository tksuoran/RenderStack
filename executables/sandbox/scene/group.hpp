#error old
#ifndef group_hpp_scene
#    define group_hpp_scene

#    include "renderstack_toolkit/platform.hpp"
#    include <memory>
#    include <vector>

class Model;

template <typename T>
class Group
{
public:
    void clear()
    {
        m_models.clear();
    }

    void add(std::shared_ptr<T> model);

    std::vector<std::shared_ptr<T>> &models()
    {
        return m_models;
    }

    std::vector<std::shared_ptr<T>> const &models() const
    {
        return m_models;
    }

    bool visible() const
    {
        return m_visible;
    }

    void set_visible(bool value)
    {
        m_visible = value;
    }

private:
    std::vector<std::shared_ptr<Model>> m_models;
    bool                                m_visible;
};

#endif
