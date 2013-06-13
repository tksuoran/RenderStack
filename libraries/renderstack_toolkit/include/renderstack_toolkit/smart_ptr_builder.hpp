// http://www.codeproject.com/Articles/286304/Solution-for-multiple-enable_shared_from_this-in-i
#ifndef smart_ptr_builder_hpp_renderstack_toolkit
#define smart_ptr_builder_hpp_renderstack_toolkit

#include "enable_shared_from_this.hpp"
#include <memory>

namespace renderstack { namespace generic { //Generic
{

class smart_ptr_builder
{
public:
  template< typename T0, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T >
  static std::shared_ptr< T > make_shared(T *iPtr);

  template< typename T0, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

  template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T >
  static void assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr);

private:
  smart_ptr_builder();
};

template< typename T0, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4, T5 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4, T5, T6 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4, T5, T6, T7 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4, T5, T6, T7, T8 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T >
inline std::shared_ptr< T > smart_ptr_builder::make_shared(T *iPtr)
{
  std::shared_ptr< T > wSharedPtr(iPtr);
  assign_weak_ptr< T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 >(wSharedPtr);
  return wSharedPtr;
}

template< typename T0, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T5 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T5 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T6 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T5 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T6 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T7 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T5 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T6 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T7 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T8 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

template< typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T >
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr< T > &ioSharedPtr)
{
  ioSharedPtr->enable_shared_from_this< T0 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T1 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T2 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T3 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T4 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T5 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T6 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T7 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
  ioSharedPtr->enable_shared_from_this< T9 >::set_weak_ptr(ioSharedPtr, ioSharedPtr.get());
}

} }

#endif /* SMARTPTRBUILDER_H_ */

// Copyright (C) 2011 by Philippe Cayouette
     
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
     
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
     
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
