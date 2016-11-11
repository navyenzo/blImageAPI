#ifndef BL_IMAGE1_HPP
#define BL_IMAGE1_HPP


//-------------------------------------------------------------------
// FILE:            blImage1.hpp
// CLASS:           blImage1
// BASE CLASS:      None
//
// PURPOSE:         Based on blImage0, this class adds some
//                  iterators to allow the image to be used
//                  as a standard vector
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - blImage0 and all its dependencies
//                  - blIteratorAPI and all its dependencies
//
// NOTES:
//
// DATE CREATED:    Jan/18/2012
//
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
class blImage1 : public blImage0<blDataType>
{
public: // Public typedefs

    typedef blImageIterator<blDataType>                                 iterator;
    typedef blImageIterator<const blDataType>                           const_iterator;
    typedef blImageReverseIterator<blDataType>                          reverse_iterator;
    typedef blImageReverseIterator<const blDataType>                    const_reverse_iterator;


public: // Constructors and destructors

    // Default constructor

    blImage1();

    // Copy constructor

    blImage1(const blImage1<blDataType>& Image1);

    // Destructor

    ~blImage1()
    {
    }

public: // Iterator functions

    // Useful iterators
    // pointing at the
    // "first" element
    // of the array and
    // its "end"

    iterator                                begin();
    iterator                                end();

    const_iterator                          cbegin()const;
    const_iterator                          cend()const;

    reverse_iterator                        rbegin();
    reverse_iterator                        rend();

    const_reverse_iterator                  crbegin()const;
    const_reverse_iterator                  crend()const;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage1<blDataType>::blImage1() : blImage0<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage1<blDataType>::blImage1(const blImage1<blDataType>& Image1)
                                      : blImage0<blDataType>(Image1)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::iterator blImage1<blDataType>::begin()
{
    return ( &(*this)(0) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::iterator blImage1<blDataType>::end()
{
    return ( &(*this)(this->size()) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_iterator blImage1<blDataType>::cbegin()const
{
    return ( &(*this)(0) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_iterator blImage1<blDataType>::cend()const
{
    return ( &(*this)(this->size()) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::reverse_iterator blImage1<blDataType>::rbegin()
{
    return ( &(*this)(this->size() - 1) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::reverse_iterator blImage1<blDataType>::rend()
{
    return ( &(*this)(0) - 1);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_reverse_iterator blImage1<blDataType>::crbegin()const
{
    return ( &(*this)(this->size() - 1) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_reverse_iterator blImage1<blDataType>::crend()const
{
    return ( &(*this)(0) - 1);
}
//-------------------------------------------------------------------


#endif // BL_IMAGE1_HPP
