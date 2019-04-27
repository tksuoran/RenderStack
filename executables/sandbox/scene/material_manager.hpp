#ifndef material_manager_hpp
#define material_manager_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"

class Material_manager : public renderstack::toolkit::service
{
public:
    Material_manager();

    virtual ~Material_manager() = default;

    void initialize_service() override;
};

#endif
