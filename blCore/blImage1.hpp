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

    typedef blImageCircularIterator<const blDataType>                    circ_iterator;
    typedef blImageCircularIterator<const blDataType>                    const_circ_iterator;


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

    circ_iterator                           circ_begin();
    circ_iterator                           circ_end();
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
    if(this->size() > 0)
        return ( &(*this)(0) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::iterator blImage1<blDataType>::end()
{
    if(this->size() > 0)
        return ( &(*this)(this->size()) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_iterator blImage1<blDataType>::cbegin()const
{
    if(this->size() > 0)
        return ( &(*this)(0) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_iterator blImage1<blDataType>::cend()const
{
    if(this->size() > 0)
        return ( &(*this)(this->size()) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::reverse_iterator blImage1<blDataType>::rbegin()
{
    if(this->size() > 0)
        return ( &(*this)(this->size() - 1) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::reverse_iterator blImage1<blDataType>::rend()
{
    if(this->size() > 0)
        return ( &(*this)(0) - 1);
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_reverse_iterator blImage1<blDataType>::crbegin()const
{
    if(this->size() > 0)
        return ( &(*this)(this->size() - 1) );
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage1<blDataType>::const_reverse_iterator blImage1<blDataType>::crend()const
{
    if(this->size() > 0)
        return ( &(*this)(0) - 1);
    else
        return NULL;
}
//-------------------------------------------------------------------


#endif // BL_IMAGE1_HPP
