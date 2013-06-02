#error
#ifndef renderstack_graphics__render_state_hpp
#define renderstack_graphics__render_state_hpp

namespace renderstack { namespace graphics {

class render_state
{
public:
    virtual ~render_state(){}
    
    virtual void reset() = 0;
    virtual void execute() const = 0;

    virtual bool expand()
    {
        return false;
    }
};

} }

#endif
