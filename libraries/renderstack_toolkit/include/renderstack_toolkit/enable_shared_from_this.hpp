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

//  http://www.codeproject.com/Articles/286304/Solution-for-multiple-enable_shared_from_this-in-i

#ifndef ENABLESHAREDFROMTHIS_H_
#    define ENABLESHAREDFROMTHIS_H_

#    include <memory>

namespace renderstack
{
namespace toolkit // Generic
{

template <typename T>
class enable_shared_from_this
{
public:
    std::shared_ptr<T>       shared_from_this();
    std::shared_ptr<T const> shared_from_this() const;

    virtual ~enable_shared_from_this();

protected:
    enable_shared_from_this();
    enable_shared_from_this(enable_shared_from_this const &);
    enable_shared_from_this &operator=(enable_shared_from_this const &);

private:
    friend class smart_ptr_builder;

    template <typename X, typename Y>
    void set_weak_ptr(std::shared_ptr<X> const &iSharedThis, Y *iThis) const;

    mutable std::weak_ptr<T> mWeakThis;
};

template <typename T>
enable_shared_from_this<T>::enable_shared_from_this()
{
}

template <typename T>
enable_shared_from_this<T>::enable_shared_from_this(enable_shared_from_this const &)
{
}

template <typename T>
enable_shared_from_this<T>::~enable_shared_from_this()
{
}

template <typename T>
enable_shared_from_this<T> &enable_shared_from_this<T>::operator=(enable_shared_from_this<T> const &)
{
    return *this;
}

template <typename T>
std::shared_ptr<T> enable_shared_from_this<T>::shared_from_this()
{
    std::shared_ptr<T> wSharedThis(mWeakThis);
    return wSharedThis;
}

template <typename T>
std::shared_ptr<T const> enable_shared_from_this<T>::shared_from_this() const
{
    std::shared_ptr<T const> wSharedThis(mWeakThis);
    return wSharedThis;
}

template <typename T>
template <typename X, typename Y>
void enable_shared_from_this<T>::set_weak_ptr(std::shared_ptr<X> const &iSharedThis, Y *iThis) const
{
    if (mWeakThis.expired())
    {
        mWeakThis = std::shared_ptr<T>(iSharedThis, iThis);
    }
}

class smart_ptr_builder
{
public:
    template <typename T0, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T>
    static std::shared_ptr<T> create_shared_ptr(T *iPtr);

    template <typename T0, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

    template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T>
    static void assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr);

private:
    smart_ptr_builder();
};

template <typename T0, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4, T5>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4, T5, T6>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4, T5, T6, T7>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4, T5, T6, T7, T8>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T>
inline std::shared_ptr<T> smart_ptr_builder::create_shared_ptr(T *iPtr)
{
    std::shared_ptr<T> wSharedPtr(iPtr);
    assign_weak_ptr<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>(wSharedPtr);
    return wSharedPtr;
}

template <typename T0, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T5>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T5>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T6>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T5>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T6>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T7>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T5>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T6>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T7>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T8>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T>
inline void smart_ptr_builder::assign_weak_ptr(std::shared_ptr<T> &io_shared_ptr)
{
    io_shared_ptr->enable_shared_from_this<T0>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T1>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T2>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T3>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T4>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T5>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T6>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T7>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
    io_shared_ptr->enable_shared_from_this<T9>::set_weak_ptr(io_shared_ptr, io_shared_ptr.get());
}

} // namespace toolkit
} // namespace renderstack

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
