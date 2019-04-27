#ifndef material_manager_hpp
#define material_manager_hpp

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"

class material_manager : public renderstack::toolkit::service
{
public:
    material_manager();
    /*virtual*/ ~material_manager();

    /*virtual*/ void initialize_service();
};

#endif
