#ifndef BL_IMAGEITERATORS_HPP
#define BL_IMAGEITERATORS_HPP


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
#include <iterator>
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following classes define basic forward
// and reverse ransom access iterators to
// allow the use of blImage class in stl
// algorithms
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// A forward moving random iterator
//-------------------------------------------------------------------
template<typename blDataType>
class blImageIterator : public std::iterator<std::random_access_iterator_tag,
                                             blDataType,
                                             ptrdiff_t,
                                             blDataType*,
                                             blDataType&>
{
public:

    blImageIterator(blDataType* ptr = nullptr){m_ptr = ptr;}
    blImageIterator(const blImageIterator<blDataType>& imageIterator) = default;
    ~blImageIterator(){}

    blImageIterator<blDataType>&                operator=(const blImageIterator<blDataType>& imageIterator) = default;
    blImageIterator<blDataType>&                operator=(blDataType* ptr){m_ptr = ptr;return (*this);}

    operator                                    bool()const
    {
        if(m_ptr)
            return true;
        else
            return false;
    }

    bool                                        operator==(const blImageIterator<blDataType>& imageIterator)const{return (m_ptr == imageIterator.getConstPtr());}
    bool                                        operator!=(const blImageIterator<blDataType>& imageIterator)const{return (m_ptr != imageIterator.getConstPtr());}

    blImageIterator<blDataType>&                operator+=(const ptrdiff_t& movement){m_ptr += movement;return (*this);}
    blImageIterator<blDataType>&                operator-=(const ptrdiff_t& movement){m_ptr -= movement;return (*this);}
    blImageIterator<blDataType>&                operator++(){++m_ptr;return (*this);}
    blImageIterator<blDataType>&                operator--(){--m_ptr;return (*this);}
    blImageIterator<blDataType>                 operator++(int){auto temp(*this);++m_ptr;return temp;}
    blImageIterator<blDataType>                 operator--(int){auto temp(*this);--m_ptr;return temp;}
    blImageIterator<blDataType>                 operator+(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr+=movement;auto temp(*this);m_ptr = oldPtr;return temp;}
    blImageIterator<blDataType>                 operator-(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr-=movement;auto temp(*this);m_ptr = oldPtr;return temp;}

    ptrdiff_t                                   operator-(const blImageIterator<blDataType>& imageIterator){return std::distance(imageIterator.getPtr(),this->getPtr());}

    blDataType&                                 operator*(){return *m_ptr;}
    const blDataType&                           operator*()const{return *m_ptr;}
    blDataType*                                 operator->(){return m_ptr;}

    blDataType*                                 getPtr()const{return m_ptr;}
    const blDataType*                           getConstPtr()const{return m_ptr;}

protected:

    blDataType*                                 m_ptr;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// A reverse moving random iterator
//-------------------------------------------------------------------
template<typename blDataType>
class blImageReverseIterator : public blImageIterator<blDataType>
{
public:

    blImageReverseIterator(blDataType* ptr = nullptr):blImageReverseIterator<blDataType>(ptr){}
    blImageReverseIterator(const blImageIterator<blDataType>& imageIterator){this->m_ptr = imageIterator.getPtr();}
    blImageReverseIterator(const blImageReverseIterator<blDataType>& imageReverseIterator) = default;
    ~blImageReverseIterator(){}

    blImageReverseIterator<blDataType>&         operator=(const blImageReverseIterator<blDataType>& imageReverseIterator) = default;
    blImageReverseIterator<blDataType>&         operator=(const blImageIterator<blDataType>& imageIterator){this->m_ptr = imageIterator.getPtr();return (*this);}
    blImageReverseIterator<blDataType>&         operator=(blDataType* ptr){this->setPtr(ptr);return (*this);}

    blImageReverseIterator<blDataType>&         operator+=(const ptrdiff_t& movement){this->m_ptr -= movement;return (*this);}
    blImageReverseIterator<blDataType>&         operator-=(const ptrdiff_t& movement){this->m_ptr += movement;return (*this);}
    blImageReverseIterator<blDataType>&         operator++(){--this->m_ptr;return (*this);}
    blImageReverseIterator<blDataType>&         operator--(){++this->m_ptr;return (*this);}
    blImageReverseIterator<blDataType>          operator++(int){auto temp(*this);--this->m_ptr;return temp;}
    blImageReverseIterator<blDataType>          operator--(int){auto temp(*this);++this->m_ptr;return temp;}
    blImageReverseIterator<blDataType>          operator+(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr-=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}
    blImageReverseIterator<blDataType>          operator-(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr+=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}

    ptrdiff_t                                   operator-(const blImageReverseIterator<blDataType>& imageReverseIterator){return std::distance(this->getPtr(),imageReverseIterator.getPtr());}

    blImageReverseIterator<blDataType>          base(){blImageIterator<blDataType> forwardIterator(this->m_ptr); ++forwardIterator; return forwardIterator;}
};
//-------------------------------------------------------------------


#endif // BL_IMAGEITERATORS_HPP
