#ifndef BL_IMAGEITERATORS_HPP
#define BL_IMAGEITERATORS_HPP


//-------------------------------------------------------------------
// FILE:            blImageIterator.hpp
// CLASS:           blImageIterator
// BASE CLASS:      None
//
// PURPOSE:         Defines a simple random access iterator to allow
//                  the use of blImage variables in stl algorithms
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    <iterator>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
#include <iterator>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
class blImageIterator : public std::iterator<std::random_access_iterator_tag,
                                             blDataType,
                                             ptrdiff_t,
                                             blDataType*,
                                             blDataType&>
{
public:

    // Default constructor

    blImageIterator(blDataType* ptr = nullptr);

    // Copy constructor

    blImageIterator(const blImageIterator<blDataType>& imageIterator) = default;

    // Destructor

    ~blImageIterator(){}

public: // Assignment operators

        blImageIterator<blDataType>&                operator=(const blImageIterator<blDataType>& imageIterator) = default;
        blImageIterator<blDataType>&                operator=(blDataType* ptr);

public: // Public functions

    // Overloaded boolean operator to
    // act like a normal pointer

    operator                                    bool()const
    {
        if(m_ptr)
            return true;
        else
            return false;
    }

    // Overloaded equality/inequality operators

    bool                                        operator==(const blImageIterator<blDataType>& imageIterator)const;
    bool                                        operator!=(const blImageIterator<blDataType>& imageIterator)const;

    // Overloaded assignment/arithmetic operators

    blImageIterator<blDataType>&                operator+=(const ptrdiff_t& movement);
    blImageIterator<blDataType>&                operator-=(const ptrdiff_t& movement);
    blImageIterator<blDataType>&                operator++();
    blImageIterator<blDataType>&                operator--();
    blImageIterator<blDataType>                 operator++(int);
    blImageIterator<blDataType>                 operator--(int);
    blImageIterator<blDataType>                 operator+(const ptrdiff_t& movement)const;
    blImageIterator<blDataType>                 operator-(const ptrdiff_t& movement)const;

    // Operator used to "subtract"
    // two pointers (it gives the
    // distance)

    ptrdiff_t                                   operator-(const blImageIterator<blDataType>& imageIterator)const;

    // Overloaded referencing/dereferencing operators

    blDataType&                                 operator*();
    const blDataType&                           operator*()const;
    blDataType*                                 operator->();

    // Get functions

    blDataType*                                 getPtr()const;
    const blDataType*                           getConstPtr()const;

protected:

    // The actual raw pointer

    blDataType*                                 m_ptr;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>::blImageIterator(blDataType* ptr)
{
    m_ptr = ptr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageIterator<blDataType>::operator=(blDataType* ptr)
{
    m_ptr = ptr;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageIterator<blDataType>::operator==(const blImageIterator<blDataType>& imageIterator)const
{
    return (m_ptr == imageIterator.getConstPtr());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageIterator<blDataType>::operator!=(const blImageIterator<blDataType>& imageIterator)const
{
    return (m_ptr != imageIterator.getConstPtr());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageIterator<blDataType>::operator+=(const ptrdiff_t& movement)
{
    m_ptr += movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageIterator<blDataType>::operator-=(const ptrdiff_t& movement)
{
    m_ptr -= movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageIterator<blDataType>::operator++()
{
    ++m_ptr;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageIterator<blDataType>::operator--()
{
    --m_ptr;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageIterator<blDataType>::operator++(int)
{
    auto temp(*this);

    ++m_ptr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageIterator<blDataType>::operator--(int)
{
    auto temp(*this);

    --m_ptr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageIterator<blDataType>::operator+(const ptrdiff_t& movement)const
{
    auto temp(*this);

    temp += movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageIterator<blDataType>::operator-(const ptrdiff_t& movement)const
{
    auto temp(*this);

    temp -= movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline ptrdiff_t blImageIterator<blDataType>::operator-(const blImageIterator<blDataType>& imageIterator)const
{
    return std::distance(this->getPtr(),imageIterator.getPtr());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImageIterator<blDataType>::operator*()
{
    return *m_ptr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImageIterator<blDataType>::operator*()const
{
    return *m_ptr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImageIterator<blDataType>::operator->()
{
    return m_ptr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImageIterator<blDataType>::getPtr()const
{
    return m_ptr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType* blImageIterator<blDataType>::getConstPtr()const
{
    return m_ptr;
}
//-------------------------------------------------------------------


#endif // BL_IMAGEITERATORS_HPP
