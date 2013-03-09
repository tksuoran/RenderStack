#include "renderstack_toolkit/platform.hpp"
#include "util/controller.hpp"

float controller::damp() const
{
   return m_damp;
}
void controller::set_damp(float value)
{
   m_damp = value;
}

float controller::max_value() const
{
   return m_max_value;
}
void controller::set_max_value(float value)
{
   m_max_value = value;
}
float controller::max_delta() const
{
   return m_max_delta;
}
void controller::set_max_delta(float value)
{
   m_max_delta = value;
}

void controller::update()
{
   if (
      (m_active  == true) &&
      (m_inhibit == false)
   )
      adjust(m_current_delta);

   dampen();
}
void controller::adjust(float delta)
{
   m_current_value += delta;
   if (m_current_value > m_max_value)
      m_current_value = m_max_value;
   else if (m_current_value < m_min_value)
      m_current_value = m_min_value;
}

void controller::dampen()
{
   /*  Dampening by multiplying by a constant  */ 
   if (m_dampen_multiply)
   {
      float old_value = m_current_value;
      m_current_value = m_current_value * m_damp;

      if (m_current_value == old_value)
         m_current_value = 0.0f; 

   }
   else if (m_dampen_linear && !m_active) /*  Constant velocity dampening  */ 
   { 
      if (m_current_value > m_max_delta)
      {
         m_current_value -= m_max_delta;
         if (m_current_value < m_max_delta)
            m_current_value = 0.0f;
      }
      else if (m_current_value < -m_max_delta)
      {
         m_current_value += m_max_delta;
         if (m_current_value > -m_max_delta)
            m_current_value = 0.0f;

      }
      else /*  Close to 0.0  */ 
      {

         float old_value = m_current_value;
         m_current_value *= m_damp;
         if (m_current_value == old_value)
            m_current_value = 0.0f;
      }
   }
}

void controller::set_inhibit(bool value)
{ 
   m_inhibit = value; 
}
bool controller::more() const
{
   return m_more;
}
void controller::set_more(bool value)
{
   m_more = value;
   if (m_more)
   {
      m_active = true;
      m_current_delta = m_max_delta;
   }
   else
   {
      if (m_less)
         m_current_delta = -m_max_delta;
      else
      {
         m_active = false;
         m_current_delta = 0.0f;
      }
   }
}
bool controller::less() const
{
   return m_less;
}
void controller::set_less(bool value)
{
   m_less = value;
   if (m_less)
   {
      m_active = true;
      m_current_delta = -m_max_delta;
   }
   else
   {
      if (m_more)
         m_current_delta = m_max_delta;
      else
      {
         m_active = false;
         m_current_delta = 0.0f;
      }
   }
}
bool controller::stop() const
{
   return m_stop;
}
void controller::set_stop(bool value)
{
   m_stop = value;
   if (m_stop)
   {
      if (m_current_value > 0.0f)
         m_current_delta = -m_max_delta;
      else if (m_current_value < 0.0f)
         m_current_delta = m_max_delta;
   }
   else
   {
      if (m_less && !m_more)
         m_current_delta = -m_max_delta;
      else if (!m_less && m_more)
         m_current_delta = m_max_delta;
   }
}

controller::controller()
:  m_inhibit(false)
{
   clear();
}
controller::controller(bool linear, bool multiply)
:  m_inhibit(false)
{
   clear();
   set_damp_mode(linear, multiply);
}
void controller::clear()
{
   m_damp            =  0.950f;
   m_max_delta       =  0.004f;
   m_max_value       =  1.000f;
   m_min_value       = -1.000f;
   m_current_value   =  0.0f;
   m_current_delta   =  0.0f;
   m_more            = false;
   m_less            = false;
   m_stop            = false;
   m_active          = false;
   m_dampen_linear   = false;
   m_dampen_multiply = true;
}
float controller::current_value() const 
{
   return m_current_value;
}
void controller::set_damp_mode(bool linear, bool multiply)
{
   m_dampen_linear = linear;
   m_dampen_multiply = multiply;
}
void controller::set_damp_and_max_delta(float damp, float max_delta)
{
   m_damp      = damp;
   m_max_delta = max_delta;
}
