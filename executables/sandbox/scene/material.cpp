#include "scene/material.hpp"

using namespace std;

material::material(
   string const &name,
   shared_ptr<renderstack::graphics::program> program//,
   //shared_ptr<renderstack::graphics::uniform_block> uniform_block
)
:  m_name(name)
,  m_program(program)
{
   //m_uniform_block = make_shared<renderstack::graphics::uniform_buffer
}
