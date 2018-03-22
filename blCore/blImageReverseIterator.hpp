#ifndef BL_IMAGEREVERSEITERATOR_HPP
#define BL_IMAGEREVERSEITERATOR_HPP


//-------------------------------------------------------------------
// FILE:            blImageReverseIterator.hpp
// CLASS:           blImageReverseIterator
// BASE CLASS:      None
//
// PURPOSE:         Based on blImageIterator, defines a simple random
//                  access "reverse" iterator to allow the use of blImage
//                  variables in stl algorithms
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
template<typename blDataType>
class blImageReverseIterator : public blImageIterator<blDataType>
{
public:

    // Default constructor

    blImageReverseIterator(blDataType* ptr = nullptr);

    // Constructor from a forward iterator

    blImageReverseIterator(const blImageIterator<blDataType>& imageIterator);

    // Copy constructor

    blImageReverseIterator(const blImageReverseIterator<blDataType>& imageReverseIterator) = default;

    // Destructor

    ~blImageReverseIterator(){}

public: // Overloaded assignment operators

    blImageReverseIterator<blDataType>&         operator=(const blImageReverseIterator<blDataType>& imageReverseIterator) = default;
    blImageReverseIterator<blDataType>&         operator=(const blImageIterator<blDataType>& imageIterator);
    blImageReverseIterator<blDataType>&         operator=(blDataType* ptr);

    // Overloaded assignment/arithmetic operators

    blImageReverseIterator<blDataType>&         operator+=(const ptrdiff_t& movement);
    blImageReverseIterator<blDataType>&         operator-=(const ptrdiff_t& movement);
    blImageReverseIterator<blDataType>&         operator++();
    blImageReverseIterator<blDataType>&         operator--();
    blImageReverseIterator<blDataType>          operator++(int);
    blImageReverseIterator<blDataType>          operator--(int);
    blImageReverseIterator<blDataType>          operator+(const int& movement)const;
    blImageReverseIterator<blDataType>          operator-(const int& movement)const;

    // Operator used to "subtract"
    // two pointers (it gives the
    // distance)

    ptrdiff_t                                   operator-(const blImageReverseIterator<blDataType>& imageReverseIterator)const;

    // base operator

    blImageReverseIterator<blDataType>          base();
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>::blImageReverseIterator(blDataType* ptr):blImageReverseIterator<blDataType>(ptr)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>::blImageReverseIterator(const blImageIterator<blDataType>& imageIterator)
{
    this->m_ptr = imageIterator.getPtr();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator=(const blImageIterator<blDataType>& imageIterator)
{
    this->m_ptr = imageIterator.getPtr();

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator=(blDataType* ptr)
{
    this->setPtr(ptr);

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator+=(const ptrdiff_t& movement)
{
    this->m_ptr -= movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator-=(const ptrdiff_t& movement)
{
    this->m_ptr += movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator++()
{
    --this->m_ptr;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType>& blImageReverseIterator<blDataType>::operator--()
{
    ++this->m_ptr;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType> blImageReverseIterator<blDataType>::operator++(int)
{
    auto temp(*this);

    --this->m_ptr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType> blImageReverseIterator<blDataType>::operator--(int)
{
    auto temp(*this);

    ++this->m_ptr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType> blImageReverseIterator<blDataType>::operator+(const int& movement)const
{
    auto oldPtr = this->m_ptr;

    this->m_ptr-=movement;

    auto temp(*this);

    this->m_ptr = oldPtr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType> blImageReverseIterator<blDataType>::operator-(const int& movement)const
{
    auto oldPtr = this->m_ptr;

    this->m_ptr+=movement;

    auto temp(*this);

    this->m_ptr = oldPtr;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline ptrdiff_t blImageReverseIterator<blDataType>::operator-(const blImageReverseIterator<blDataType>& imageReverseIterator)const
{
    return std::distance(this->getPtr(),imageReverseIterator.getPtr());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageReverseIterator<blDataType> blImageReverseIterator<blDataType>::base()
{
    blImageIterator<blDataType> forwardIterator(this->m_ptr);

    ++forwardIterator;

    return forwardIterator;
}
//-------------------------------------------------------------------


#endif // BL_IMAGEREVERSEITERATOR_HPP
