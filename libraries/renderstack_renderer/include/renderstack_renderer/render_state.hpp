#ifndef renderstack_renderer__render_state_hpp
#define renderstack_renderer__render_state_hpp

namespace renderstack { namespace renderer {

class render_state
{
public:
    virtual ~render_state(){}
    
    virtual void reset() = 0;
    virtual void execute() = 0;

    virtual bool expand()
    {
        return false;
    }
};

} }

#endif
