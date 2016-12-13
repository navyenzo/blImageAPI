#ifndef BL_IMAGECIRCULARITERATORS_HPP
#define BL_IMAGECIRCULARITERATORS_HPP


//-------------------------------------------------------------------
// FILE:            blImageCircularIterator.hpp
// CLASS:           blImageCircularIterator
// BASE CLASS:      None
//
// PURPOSE:         - Defines a simple random access circular iterator
//                    to allow the use of blImage variables (used as
//                    circular buffers) in stl algorithms
//                  - It uses a "maxNumberOfCirculations" variables that
//                    allows the circular buffer to be used in stl
//                    algorithms because once the iterator ciculates past
//                    the set limit, it ends
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
class blImageCircularIterator : public std::iterator<std::random_access_iterator_tag,
                                                     blDataType,
                                                     ptrdiff_t,
                                                     blDataType*,
                                                     blDataType&>
{
public:

    // Constructors

    blImageCircularIterator(blImage2<blDataType>& image,
                            const int& dataIndex,
                            const int& maxNumberOfCirculations);

    blImageCircularIterator(blImage2<blDataType>& image,
                            const int& rowIndex,
                            const int& colIndex,
                            const int& maxNumberOfCirculations);

    // Copy constructor

    blImageCircularIterator(const blImageCircularIterator<blDataType>& imageCircularIterator);

    // Destructor

    ~blImageCircularIterator();

public: // Overloaded operators

    // Boolean operator to check validity
    // of iterator

    operator                                    bool()const
    {
        if(m_image.getImageSharedPtr())
            return true;
        else
            return false;
    }

    // Comparator operators

    bool                                        operator==(const blImageCircularIterator<blDataType>& imageCircularIterator)const;
    bool                                        operator!=(const blImageCircularIterator<blDataType>& imageCircularIterator)const;

    // Arithmetic operators

    blImageCircularIterator<blDataType>&        operator+=(const int& movement);
    blImageCircularIterator<blDataType>&        operator-=(const int& movement);
    blImageCircularIterator<blDataType>&        operator++();
    blImageCircularIterator<blDataType>&        operator--();
    blImageCircularIterator<blDataType>         operator++(int);
    blImageCircularIterator<blDataType>         operator--(int);
    blImageCircularIterator<blDataType>         operator+(const int& movement)const;
    blImageCircularIterator<blDataType>         operator-(const int& movement)const;

    int                                         operator-(const blImageCircularIterator<blDataType>& imageCircularIterator)const;

    // Dereferencing operators

    blDataType&                                 operator*();
    const blDataType&                           operator*()const;

    blDataType*                                 operator->();
    const blDataType*                           operator->()const;

public: // Public functions

    // Functions used to get/set
    // the iterator components

    blImage2<blDataType>&                       getImage();
    const blImage2<blDataType>&                 getImage()const;
    const int&                                  getDataIndex()const;
    const int&                                  getStartIndex()const;
    int                                         getCurrentNumberOfCirculations()const;
    const int&                                  getMaxNumberOfCirculations()const;

    // Functions used to get the
    // raw pointer (used in the
    // equality operator to check
    // if two circular iterators
    // are pointing to the same
    // data point

    blDataType*                                 getRawPointer();
    const blDataType*                           getRawPointer()const;

    // Functions used to set the
    // current data index the
    // iterator is pointing at

    void                                        setDataIndex(const int& dataIndex);
    void                                        setDataIndex(const int& rowIndex,const int& colIndex);

private: // Private variables

    // The image that we're
    // iterating over

    blImage2<blDataType>                        m_image;

    // Indeces indicating current
    // place where it's pointing
    // in the image

    int                                         m_dataIndex;

    // Index where the iterator started
    // iterating from

    int                                         m_startIndex;

    // Current number of circulations

    int                                         m_currentNumberOfCirculations;

    // Max number of allowed times
    // the iterator can circulate
    // around the image before reaching
    // the "end" of the image

    int                                         m_maxNumberOfCirculations;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::blImageCircularIterator(blImage2<blDataType>& image,
                                                                    const int& dataIndex,
                                                                    const int& maxNumberOfCirculations)
{
    m_image = image;

    setDataIndex(dataIndex);

    m_maxNumberOfCirculations = std::abs(maxNumberOfCirculations);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::blImageCircularIterator(blImage2<blDataType>& image,
                                                                    const int& rowIndex,
                                                                    const int& colIndex,
                                                                    const int& maxNumberOfCirculations)
{
    m_image = image;

    setDataIndex(rowIndex,colIndex);

    m_maxNumberOfCirculations = std::abs(maxNumberOfCirculations);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::blImageCircularIterator(const blImageCircularIterator<blDataType>& imageCircularIterator)
{
    m_image = imageCircularIterator.getImage();

    m_dataIndex = imageCircularIterator.getDataIndex();

    m_startIndex = imageCircularIterator.getStartIndex();


    m_maxNumberOfCirculations = imageCircularIterator.getMaxNumberOfCirculations();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::~blImageCircularIterator()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImageCircularIterator<blDataType>::setDataIndex(const int& dataIndex)
{
    m_dataIndex = m_image.getDataIndex_circ_atROI(dataIndex);

    m_startIndex = m_dataIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImageCircularIterator<blDataType>::setDataIndex(const int& rowIndex,
                                                              const int& colIndex)
{
    setDataIndex(m_image.getDataIndex_circ_atROI(rowIndex,colIndex));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage2<blDataType>& blImageCircularIterator<blDataType>::getImage()
{
    return m_image;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blImage2<blDataType>& blImageCircularIterator<blDataType>::getImage()const
{
    return m_image;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getDataIndex()const
{
    return m_dataIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getMaxNumberOfCirculations()const
{
    return m_maxNumberOfCirculations;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getStartIndex()const
{
    return m_startIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImageCircularIterator<blDataType>::getCurrentNumberOfCirculations()const
{
    return ( (m_dataIndex - m_startIndex) / m_image.sizeROI() );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator==(const blImageCircularIterator<blDataType>& imageCircularIterator)const
{
    if(this->getRawPointer() == imageCircularIterator.getRawPointer())
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator!=(const blImageCircularIterator<blDataType>& imageCircularIterator)const
{
    if(this->getRawPointer() != imageCircularIterator.getRawPointer())
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>& blImageCircularIterator<blDataType>::operator+=(const int& movement)
{
    m_dataIndex += movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>& blImageCircularIterator<blDataType>::operator-=(const int& movement)
{
    m_dataIndex -= movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>& blImageCircularIterator<blDataType>::operator++()
{
    ++m_dataIndex;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>& blImageCircularIterator<blDataType>::operator--()
{
    --m_dataIndex;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType> blImageCircularIterator<blDataType>::operator++(int)
{
    auto temp(*this);

    ++m_dataIndex;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType> blImageCircularIterator<blDataType>::operator--(int)
{
    auto temp(*this);

    --m_dataIndex;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType> blImageCircularIterator<blDataType>::operator+(const int& movement)const
{
    auto temp(*this);

    temp += movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType> blImageCircularIterator<blDataType>::operator-(const int& movement)const
{
    auto temp(*this);

    temp -= movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImageCircularIterator<blDataType>::operator-(const blImageCircularIterator<blDataType>& imageCircularIterator)const
{
    return ( (m_dataIndex - m_startIndex) - (imageCircularIterator.getDataIndex() - imageCircularIterator.getStartIndex()) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImageCircularIterator<blDataType>::operator*()
{
    return m_image.circ_atROI(m_dataIndex);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImageCircularIterator<blDataType>::operator*()const
{
    return m_image.circ_atROI(m_dataIndex);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImageCircularIterator<blDataType>::operator->()
{
    // If the max number of circulations
    // has been passed, then we return the
    // "end" iterator, otherwise we return
    // the correct iterator

    if(std::abs(getCurrentNumberOfCirculations()) >= m_maxNumberOfCirculations)
    {
        return &m_image.atROI(m_image.sizeROI());
    }
    else
    {
        return &m_image.circ_atROI(m_dataIndex);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType* blImageCircularIterator<blDataType>::operator->()const
{
    // If the max number of circulations
    // has been passed, then we return the
    // "end" iterator, otherwise we return
    // the correct iterator

    if(std::abs(getCurrentNumberOfCirculations()) >= m_maxNumberOfCirculations)
    {
        return &m_image.atROI(m_image.sizeROI());
    }
    else
    {
        return &m_image.circ_atROI(m_dataIndex);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImageCircularIterator<blDataType>::getRawPointer()
{
    // If the max number of circulations
    // has been passed, then we return the
    // "end" iterator, otherwise we return
    // the correct iterator

    if(std::abs(getCurrentNumberOfCirculations()) >= m_maxNumberOfCirculations)
    {
        return &m_image.atROI(m_image.sizeROI());
    }
    else
    {
        return &m_image.circ_atROI(m_dataIndex);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType* blImageCircularIterator<blDataType>::getRawPointer()const
{
    // If the max number of circulations
    // has been passed, then we return the
    // "end" iterator, otherwise we return
    // the correct iterator

    if(std::abs(getCurrentNumberOfCirculations()) >= m_maxNumberOfCirculations)
    {
        return &m_image.atROI(m_image.sizeROI());
    }
    else
    {
        return &m_image.circ_atROI(m_dataIndex);
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGECIRCULARITERATORS_HPP
