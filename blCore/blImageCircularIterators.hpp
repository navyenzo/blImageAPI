#ifndef BL_IMAGECIRCULARITERATORS_HPP
#define BL_IMAGECIRCULARITERATORS_HPP


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
#include <iterator>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following classes define custom
// circular iterators that can be used
// with our image class so that an stl
// algorithm could run though our image
// in a circular way and stop after so
// many times it has gone around
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// A forward moving circular iterator for our images
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

    bool                                        operator==(const blImageCircularIterator<blDataType>& imageIterator)const;
    bool                                        operator!=(const blImageCircularIterator<blDataType>& imageIterator)const;

    // Arithmetic operators

    blImageIterator<blDataType>&                operator+=(const int& movement);
    blImageIterator<blDataType>&                operator-=(const int& movement);
    blImageIterator<blDataType>&                operator++();
    blImageIterator<blDataType>&                operator--();
    blImageIterator<blDataType>                 operator++(int);
    blImageIterator<blDataType>                 operator--(int);
    blImageIterator<blDataType>                 operator+(const int& movement)const;
    blImageIterator<blDataType>                 operator-(const int& movement)const;

    int                                         operator-(const blImageIterator<blDataType>& imageIterator)const;

    blDataType&                                 operator*();
    const blDataType&                           operator*()const;
    blDataType*                                 operator->();

public: // Public functions

    // Functions used to get/set
    // the iterator components

    blImage2<blDataType>&                       getImage();
    const blImage2<blDataType>&                 getImage()const;
    const int&                                  getDataIndex()const;
    const int&                                  getMaxNumberOfCirculations()const;
    const int&                                  getStartIndex()const;

    // Functions used to set the
    // current data index the
    // iterator is pointing at

    void                                        setDataIndex(const int& dataIndex);
    void                                        setRowAndColIndeces(const int& rowIndex,const int& colIndex);

    // Special function used to
    // calculate how many times
    // the iterator has circled
    // around from where it started
    // (Could be negative when circling
    // in the reverse direction)

    const int&                                  getCurrentNumberOfCirculations()const;

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

    m_maxNumberOfCirculations = maxNumberOfCirculations;
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

    setRowAndColIndeces(rowIndex,colIndex);

    m_maxNumberOfCirculations = maxNumberOfCirculations;
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
inline void blImageCircularIterator<blDataType>::setDataIndex(const int& dataIndex)
{
    // First we just set the
    // current and start data
    // indeces

    m_dataIndex = m_image.getDataIndex_circ_atROI(dataIndex);

    m_startIndex = m_dataIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImageCircularIterator<blDataType>::setRowAndColIndeces(const int& rowIndex,
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
inline const int& blImageCircularIterator<blDataType>::getCurrentNumberOfCirculations()const
{
    // We have to calculate how many times
    // we have gone around and passed the
    // start point over and over again

    // First we calculate how far the current
    // data index is from the start index
    // (The current index could be negative)

    int distanceIteratorHasMoved = m_dataIndex - m_startIndex;

    // Finally we calculate and return how many
    // times the iterator has circled around
    // (Again this number could be negative)

    int numberOfCirculations = distanceIteratorHasMoved % m_image.sizeROI();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator==(const blImageCircularIterator<blDataType>& imageIterator)const
{
    if(this->m_image != imageIterator.getImage)
        return false;
    else if(this->m_startIndex != imageIterator.getStartIndex())
        return false;
    else if(this->m_dataIndex != imageIterator.getDataIndex())
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator!=(const blImageCircularIterator<blDataType>& imageIterator)const
{
    if(this->m_image != imageIterator.getImage)
        return true;
    else if(this->m_startIndex != imageIterator.getStartIndex())
        return true;
    else if(this->m_dataIndex != imageIterator.getDataIndex())
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageCircularIterator<blDataType>::operator+=(const int& movement)
{
    m_dataIndex += movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageCircularIterator<blDataType>::operator-=(const int& movement)
{
    m_dataIndex -= movement;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageCircularIterator<blDataType>::operator++()
{
    ++m_dataIndex;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType>& blImageCircularIterator<blDataType>::operator--()
{
    --m_dataIndex;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageCircularIterator<blDataType>::operator++(int)
{
    auto temp(*this);

    ++m_dataIndex;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageCircularIterator<blDataType>::operator--(int)
{
    auto temp(*this);

    --m_dataIndex;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageCircularIterator<blDataType>::operator+(const int& movement)const
{
    auto temp(*this);

    temp += movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageIterator<blDataType> blImageCircularIterator<blDataType>::operator-(const int& movement)const
{
    auto temp(*this);

    temp -= movement;

    return temp;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImageCircularIterator<blDataType>::operator-(const blImageIterator<blDataType>& imageIterator)const
{
    return ( (m_dataIndex - m_startIndex) - (imageIterator.getDataIndex() - imageIterator.getStartIndex()) );
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

    if(getCurrentNumberOfCirculations() >= m_maxNumberOfCirculations)
    {
        auto ROIRectangle = m_image.getROI();
        return ( &m_image[ROIRectangle.p2.y][ROIRectangle.p2.x] );
    }
    else
    {
        return &m_image.circ_atROI(m_dataIndex);
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGECIRCULARITERATORS_HPP
