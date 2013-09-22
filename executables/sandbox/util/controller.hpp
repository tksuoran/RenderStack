#ifndef controller_hpp_util
#define controller_hpp_util

class controller
{
private:
   bool  m_more;
   bool  m_less;
   bool  m_stop;
   bool  m_active;
   bool  m_inhibit;
   bool  m_dampen_linear;
   bool  m_dampen_multiply;
   float m_damp;
   float m_max_delta;
   float m_max_value;
   float m_min_value;
   float m_current_delta;
   float m_current_value;

public:
   float damp() const;
   void  set_damp(float value);
   float max_value() const;
   void  set_max_value(float value);
   float max_delta() const;
   void  set_max_delta(float value);
   void  update();
   void  adjust(float delta);
   void  adjust(double delta) { adjust(static_cast<float>(delta)); }

private:
   void dampen();

public:
   void set_inhibit(bool value);
   bool more() const;
   void set_more(bool value);
   bool less() const;
   void set_less(bool value);
   bool stop() const;
   void set_stop(bool value);

   controller();
   controller(bool linear, bool multiply);
   void clear();
   float current_value() const;
   void set_damp_mode(bool linear, bool multiply);
   void set_damp_and_max_delta(float damp, float max_delta);
};

#endif
