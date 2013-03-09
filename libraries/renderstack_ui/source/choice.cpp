#include "renderstack_ui/choice.hpp"

namespace renderstack { namespace ui {

using namespace std;
using namespace renderstack::toolkit;

choice_item::choice_item(string const &label, shared_ptr<class style> style)
:  push_button(label, style)
{
   set_name("choice item: " + label);
}

choice_item::~choice_item()
{
}

void choice_item::action(weak_ptr<action_source> source)
{
   (void)source; // TODO
   auto choice = m_choice.lock();
   if (choice)
   {
      weak_ptr<choice_item> weak_this = enable_shared_from_this<choice_item>::shared_from_this();
      auto selected = choice->selected().lock();
         if (selected != weak_this.lock())
            choice->set_selected(weak_this);
   }
   else
   {
      throw std::runtime_error("choice item is not connected");
   }
}

void choice_item::connect(weak_ptr<class choice> choice)
{
   m_choice = choice;
}


choice::choice(shared_ptr<class style> style, shared_ptr<class style> choice_item_style, orientation::value orientation_in)
:  dock                 (style, orientation_in)
,  m_choice_item_style  (choice_item_style)
{
   set_name("choice");
}

choice::~choice()
{
}

shared_ptr<choice_item> choice::add(shared_ptr<choice_item> item)
{
   item->connect(enable_shared_from_this<choice>::shared_from_this());

   switch (orientation())
   {
   case orientation::horizontal:   item->set_layout_style(area_layout_style::extend_vertical); break;
   case orientation::vertical:     item->set_layout_style(area_layout_style::extend_horizontal); break;
   }

   m_items.push_back(item);
   dock::add(item);
   if (m_selected.lock() != item)
      item->set_pressed(false);

   return item;
}

void choice::set_selected(weak_ptr<choice_item> value)
{
   auto old_selected = m_selected.lock();
   auto new_selected = value.lock();

   if (value.lock() != old_selected)
   {
      if (old_selected)
         old_selected->set_pressed(false);

      m_selected = value;
      if (new_selected)
         new_selected->set_pressed(true);

      auto s = sink().lock();
      if (s)
         s->action(selected());
   }
}

shared_ptr<choice_item> choice::add_choice_item(string const &label, bool select)
{
   std::shared_ptr<choice_item> item = smart_ptr_builder::create_shared_ptr<action_source, choice_item, area>(
      new choice_item(label, m_choice_item_style)
   );
   // choice_item handles actions itself
   item->set_sink(item); // this can not be done in constructor as shared_from_this won't work
   add(item);
   if (select)
      set_selected(item);

   return item;
}

} }
