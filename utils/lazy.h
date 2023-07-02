#ifndef VS_LAZY_H
#define VS_LAZY_H

#include <array>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <new>

namespace utils {

/**
 * @brief This class is used for lazy initialization. This class implement lazy initialization idiom.
 * @details This class contains buffer, enough size, to store object of type T.
 * The size of this class is always sizeof(T) + 1 byte(for binary flag)
 * @warning  This class really call constructor of the object type T then user call Lazy& operator=(T) or Lazy(T).
 * We should not access to internal object of this class before call Lazy& operator=(T) or Lazy(T).
 * Accessing to internal object of this class is Undefined behavior!
 */
template<typename T>
class LazyConstructedObject {
public:
    using ValueType = T;

    explicit LazyConstructedObject();
    explicit LazyConstructedObject(const T& value);
    explicit LazyConstructedObject(T&& value);
    LazyConstructedObject(const LazyConstructedObject&) = default;

    /**
    * @brief Move operation meaningless because this class contains std::array<uint8_t, sizeof(T)>.
    * In c++, we can`t apply move operation on static array!
    **/
    LazyConstructedObject(LazyConstructedObject&&) noexcept = delete;
    LazyConstructedObject& operator=(LazyConstructedObject&&) noexcept = delete;

    LazyConstructedObject& operator=(const T& value);
    LazyConstructedObject& operator=(T&& value);
    LazyConstructedObject& operator=(const LazyConstructedObject&) = default;

    //! These function(methods) use in order to access to internal object of this class
    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

    T* get();
    const T* get() const;

    /**
    * @brief Drop this object
    * @warning Access to internal object of this class after call drop function is undefined behavior!
    */
    void drop();

    operator bool() const;
    bool isEmpty() const;

private:
    std::array<uint8_t, sizeof(T)> m_buffer; //! storage that contains object T
    bool m_isEmpty; //! flag to determine that this class was initialized
};

template<typename T>
LazyConstructedObject<T>::LazyConstructedObject():
m_buffer(),
m_isEmpty(true)
{
    std::memset(m_buffer.data(), 0, m_buffer.size());
}

template<typename T>
LazyConstructedObject<T>::LazyConstructedObject(const T& value) :
m_buffer(),
m_isEmpty(false)
{
    std::memset(m_buffer.data(), 0, m_buffer.size());
    new(m_buffer.data()) T(value);
}

template<typename T>
LazyConstructedObject<T>::LazyConstructedObject(T&& value) :
m_buffer(),
m_isEmpty(false)
{
    std::memset(m_buffer.data(), 0, m_buffer.size());
    new(m_buffer.data()) T(std::move(value));
}

template<typename T>
LazyConstructedObject<T>& LazyConstructedObject<T>::operator=(const T& value)
{
    new(m_buffer.data()) T(value);
    m_isEmpty = false;
    return *this;
}

template<typename T>
LazyConstructedObject<T>& LazyConstructedObject<T>::operator=(T&& value)
{
    new(m_buffer.data()) T(std::move(value));
    m_isEmpty = false;
    return *this;
}

template<typename T>
T& LazyConstructedObject<T>::operator*()
{
    return *get();
}

template<typename T>
const T& LazyConstructedObject<T>::operator*() const
{
    return *get();
}

template<typename T>
T* LazyConstructedObject<T>::operator->()
{
    return get();
}

template<typename T>
const T* LazyConstructedObject<T>::operator->() const
{
    return get();
}

template<typename T>
T* LazyConstructedObject<T>::get()
{
    assert(!isEmpty());
    return reinterpret_cast<T*>(m_buffer.data());
}

template<typename T>
const T* LazyConstructedObject<T>::get() const
{
    assert(!isEmpty());
    //! A lot of developers believe that `const_cast` should only be used when it is necessary and safe to do so.
    //! I think this case is an appropriate and safe for using `const_cast`.
    //! We suppose that the user is sure that it is not empty class.
    //! If this statement is true, it means that m_buffer already contains the constructed object T and
    //! we may interpret this buffer as object of type T.
    //! The user will not be able to modify the object. The object of type T will always be read-only. It is safe!
    return reinterpret_cast<T*>(
        const_cast<uint8_t*>(m_buffer.data())
    );
}

template<typename T>
void LazyConstructedObject<T>::drop()
{
    get()->T();
    m_isEmpty = true;
}

template<typename T>
bool LazyConstructedObject<T>::isEmpty() const
{
    return m_isEmpty;
}

template<typename T>
LazyConstructedObject<T>::operator bool() const
{
    return !isEmpty();
}

} //! namespace utils


#endif //! VS_LAZY_H
