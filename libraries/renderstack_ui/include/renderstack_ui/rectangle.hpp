#ifndef renderstack_ui__rectangle_hpp
#define renderstack_ui__rectangle_hpp

#include "renderstack_toolkit/platform.hpp"
#include <glm/glm.hpp>
#include <limits>
#include <algorithm>

namespace renderstack { namespace ui {

class rectangle
{
public:
   //private:
   glm::vec2   m_min;
   glm::vec2   m_max;

public:
   glm::vec2 const &min() const { return m_min; }
   glm::vec2       &min() { return m_min; }
   void            set_min(glm::vec2 const &value){ m_min = value; }
   glm::vec2 const &max() const { return m_max; }
   glm::vec2       &max(){ return m_max; }
   void            set_max(glm::vec2 const &value){ m_max = value; }
   glm::vec2       size() const
   {
      return glm::vec2(
         m_max.x - m_min.x + 1.0f,
         m_max.y - m_min.y + 1.0f
      );
   }
   void set_size(glm::vec2 const &value)
   {
      m_max.x = m_min.x + value.x - 1.0f;
      m_max.y = m_min.y + value.y + 1.0f;
   }
   inline glm::vec2 half_size() { return size() / 2.0f; }
   inline glm::vec2 center   () { return min() + half_size(); }

   void copy_from(rectangle const &other)
   {
      m_min = other.min();
      m_max = other.max();
   }
   rectangle(float min_x, float min_y, float max_x, float max_y)
   {
      m_min.x = min_x;
      m_max.x = max_x;
      m_min.y = min_y;
      m_max.y = max_y;
   }
   rectangle(float width, float height)
   {
      m_min.x = 0.0f;
      m_min.y = 0.0f;
      m_max.x = width - 1.0f;
      m_max.y = height - 1.0f;
   }
   rectangle()
   {
      reset_for_grow();
   }
   void reset_for_grow()
   {
      m_min.x =  std::numeric_limits<float>::max();
      m_max.x = -std::numeric_limits<float>::max();
      m_min.y =  std::numeric_limits<float>::max();
      m_max.y = -std::numeric_limits<float>::max();
   }

   rectangle(glm::vec2 const &min, glm::vec2 const &max)
   {
      m_min = min;
      m_max = max;
   }
   bool hit(glm::vec2 const &hit_position) const
   {
      if (
         (hit_position.x < m_min.x) || 
         (hit_position.y < m_min.y) || 
         (hit_position.x > m_max.x) ||
         (hit_position.y > m_max.y)
      )
         return false;
      return true;
   }
   bool hit(glm::ivec2 const &hit_position) const
   {
      if (
         (hit_position.x < m_min.x) || 
         (hit_position.y < m_min.y) || 
         (hit_position.x > m_max.x) ||
         (hit_position.y > m_max.y)
      )
         return false;
      return true;
   }
   void extend_by(float x, float y)
   {
      m_min.x = (std::min)(m_min.x, x);
      m_max.x = (std::max)(m_max.x, x);
      m_min.y = (std::min)(m_min.y, y);
      m_max.y = (std::max)(m_max.y, y);
   }
   void clip_to(rectangle const &reference)
   {
      m_min.x = (std::max)(m_min.x, reference.min().x);
      m_max.x = (std::min)(m_max.x, reference.max().x);
      m_min.y = (std::max)(m_min.y, reference.min().y);
      m_max.y = (std::min)(m_max.y, reference.max().y);
   }
   rectangle shrink(glm::vec2 const &padding)
   {
      return rectangle(
         m_min + padding,
         m_max - padding
      );
   }
   void grow_by(float padding_x, float padding_y)
   {
      m_min.x -= padding_x;
      m_min.y -= padding_y;
      m_max.x += padding_x;
      m_max.y += padding_y;
   }
};

} }

#endif
