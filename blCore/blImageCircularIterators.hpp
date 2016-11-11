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

    // Constructor

    blImageCircularIterator<blDataType>(blImage2<blDataType>& image,
                                        const int& rowIndex,
                                        const int& colIndex,
                                        const int& maxNumberOfCircularIterations);

    // Copy constructor

    blImageCircularIterator<blDataType>(const blImageCircularIterator<blDataType>& imageCircularIterator);

    // Destructor

    ~blImageCircularIterator<blDataType>();

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
    bool                                        operator!=(const blImageCircularIterator<blDataType>& imageIterator)cons;

    // Arithmetic operators

    blImageIterator<blDataType>&                operator+=(const ptrdiff_t& movement){this->m_index += movement;return (*this);}
    blImageIterator<blDataType>&                operator-=(const ptrdiff_t& movement){this->m_index -= movement;return (*this);}
    blImageIterator<blDataType>&                operator++(){++this->m_index;return (*this);}
    blImageIterator<blDataType>&                operator--(){--this->m_index;return (*this);}
    blImageIterator<blDataType>                 operator++(int){auto temp(*this);++this->index;return temp;}
    blImageIterator<blDataType>                 operator--(int){auto temp(*this);--this->m_index;return temp;}
    blImageIterator<blDataType>                 operator+(const ptrdiff_t& movement){auto oldIndex = this->m_index;this->m_index+=movement;auto temp(*this);this->m_index = oldIndex;return temp;}
    blImageIterator<blDataType>                 operator-(const ptrdiff_t& movement){auto oldIndex = this->m_index;this->m_index-=movement;auto temp(*this);this->m_index = oldIndex;return temp;}

    ptrdiff_t                                   operator-(const blImageIterator<blDataType>& imageIterator){return std::distance(imageIterator.getIndex(),this->getIndex());}

    blDataType&                                 operator*(){return this->m_image.circ_at(this->m_index);}
    const blDataType&                           operator*()const{return this->m_image.circ_at(this->m_index);}
    blDataType*                                 operator->(){return &this->m_image->circ_at(this->m_index);}

    ptrdiff_t                                   getIndex()const{return this->m_index;}

public: // Public functions

    // Functions used to get the
    // iterator components

    blImage2<blDataType>&                       getImage();
    const blImage2<blDataType>&                 getImage()const;

    const int&                                  getRowIndex()const;
    const int&                                  getColIndex()const;

    const int&                                  getMaxNumberOfCircularIterations()const;
    const int&                                  getCurrentNumberOfIterations()const;

    const int&                                  getStartIndex()const;

    // Functions used to set the
    // current row and column that
    // the iterator is pointing at

    void                                        setRowAndColIndeces(const int& rowIndex,const int& colIndex);
    void                                        setDataIndex(const int& dataIndex);

    // Function used to advance the
    // iterator by a specified amount

    void                                        advance(const int& distance);

private: // Private functions

    // Function used to place the
    // start index (where the iterator
    // started iterating from)

    void                                        setStartIndex();

protected: // Protected variables

    // The image that we're
    // iterating over

    blImage2<blDataType>                        m_image;

    // Indeces indicating current
    // place where it's pointing
    // in the image

    int                                         m_rowIndex;
    int                                         m_colIndex;

    // Index where the iterator started
    // iterating from

    int                                         m_startIndex;

    // Max number of allowed times
    // the iterator can circulate
    // around the image before reaching
    // the "end" of the image

    int                                         m_maxNumberOfCircularIterations;

private: // Private variables

    // Current number of times the iterator
    // has gone around the image

    int                                         m_currentNumberOfIterations;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::blImageCircularIterator(blImage2<blDataType>& image,
                                                                    const int& rowIndex,
                                                                    const int& colIndex,
                                                                    const int& maxNumberOfCircularIterations)
{
    m_image = image;

    setRowAndColIndeces(rowIndex,colIndex);

    m_maxNumberOfCircularIterations = maxNumberOfCircularIterations;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImageCircularIterator<blDataType>::blImageCircularIterator(const blImageCircularIterator<blDataType>& imageCircularIterator)
{
    m_image = imageCircularIterator.getImage();

    m_rowIndex = imageCircularIterator.getRowIndex();
    m_colIndex = imageCircularIterator.getColIndex();

    m_startIndex = imageCircularIterator.getStartIndex();


    m_maxNumberOfCircularIterations = imageCircularIterator.getMaxNumberOfCircularIterations();
    m_currentNumberOfIterations = imageCircularIterator.getCurrentNumberOfIterations();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage2<blDataType>& blImageCircularIterator<blDataType>::setRowAndColIndeces(const int& rowIndex,
                                                                                      const int& colIndex)
{
    // First we just set the
    // row and column indeces

    m_rowIndex = rowIndex;
    m_colIndex = colIndex;

    // Then we zero the number of times
    // we have gone around

    m_currentNumberOfIterations = 0;

    // Then we calculate the starting
    // point of the iteration process

    m_startIndex = m_image.getDataIndex_circ_atROI(m_rowIndex,m_colIndex);
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
inline const int& blImageCircularIterator<blDataType>::getRowIndex()const
{
    return m_rowIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getColIndex()const
{
    return m_colIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getMaxNumberOfCircularIterations()const
{
    return m_maxNumberOfCircularIterations;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const int& blImageCircularIterator<blDataType>::getCurrentNumberOfIterations()const
{
    return m_currentNumberOfIterations;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImageCircularIterator<blDataType>::advance(const int& distance)
{
    // First we compare the distance
    // we've been asked to move to the
    // size of the image

    if(distance > m_image.sizeROI())
    {
        // In this case we've been asked to
        // move more than the image size and
        // so we have to circle around, so
        // we count how many times we have to
        // circle around

        m_currentNumberOfIterations += static_cast<int>(distance / m_image.sizeROI());
    }

    // First we get the data index
    // where the iterator is currently
    // pointint at

    dataIndex = m_image.getDataIndex_circ_atROI(m_rowIndex,m_colIndex);

    // Either the data index is ahead of where
    // we started iterating from or behind

    if(dataIndex > m_startIndex)
    {
        // In this case the iterator is
        // currently ahead of where it
        // started from


    }

    if(distance > 0)
    {
        // First we compare the distance
        // that we've been asked to move
        // to the size of the image

        if(distance < m_image.sizeROI())
        {
            // In this case we're moving without
            // fully circulating around where we
            // started from
        }


}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator==(const blImageCircularIterator<blDataType>& imageIterator)const
{
    return (this->m_image == imageIterator.getImage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator!=(const blImageCircularIterator<blDataType>& imageIterator)const
{
    return (this->m_image != imageIterator.getImage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImageCircularIterator<blDataType>::operator+=(const int& movement)
{
    // First we get the current data index

    int currentDataIndex = m_image.getDataIndex_circ_at(m_rowIndex,m_colIndex);

    // We then





    int newColIndex = m_rowIndex + movement;

    this->m_index += movement;

    return (*this);
}
//-------------------------------------------------------------------


#endif // BL_IMAGECIRCULARITERATORS_HPP
