#ifndef selection_manager_hpp_logic
#define selection_manager_hpp_logic

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/service.hpp"

class selection_manager : public renderstack::toolkit::service
{
public:
   selection_manager();
   /*virtual*/ ~selection_manager();

   void connect();

   /*virtual*/ void initialize_service();

private:
};



#endif

