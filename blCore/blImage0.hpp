#ifndef BL_IMAGE0_HPP
#define BL_IMAGE0_HPP


//-------------------------------------------------------------------
// FILE:            blImage0.hpp
// CLASS:           blImage0
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's image
//                  structure IplImage with a smart pointer
//                  and provide a fast, generic and easy way
//                  to access image pixels, while remaining
//                  interchangable with IplImage in Opencv's
//                  algorithms
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - IplImage and all its dependencies
//
//                  - std::shared_ptr or boost::shared_ptr
//                    defined by a typedef
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
// Enums needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
class blImage0
{
public: // Public typedefs

    typedef typename std::shared_ptr<IplImage>              blImagePtr;

public: // Constructors and destructors

    // Default constructor
    blImage0();

    // Copy constructor
    blImage0(const blImage0<blDataType>& Image0);

    // Destructor
    ~blImage0()
    {
    }

public: // Overloaded operators

    // Assignment operator

    blImage0<blDataType>&                   operator=(const blImage0<blDataType>& Image0);

    // Overloaded operators for
    // easy access of image pixels

    blDataType*                             operator[](const int& rowIndex);
    const blDataType*                       operator[](const int& rowIndex)const;

    blDataType&                             operator()(const int& rowIndex,const int& colIndex);
    const blDataType&                       operator()(const int& rowIndex,const int& colIndex)const;

    blDataType&                             operator()(const int& imageElementIndex);
    const blDataType&                       operator()(const int& imageElementIndex)const;

    // Functions used to
    // circularly access
    // image pixels

    blDataType&                             circ_at(const int& rowIndex,const int& colIndex);
    const blDataType&                       circ_at(const int& rowIndex,const int& colIndex)const;

    blDataType&                             circ_at(const int& imageElementIndex);
    const blDataType&                       circ_at(const int& imageElementIndex)const;

    // Functions used to
    // get the data index
    // based on circular
    // lookup

    int                                     getDataIndex_circ_at(const int& imageElementIndex)const;
    int                                     getDataIndex_circ_at(const int& rowIndex,const int& colIndex)const;

    // Operators used to
    // convert this image
    // to an IplImage pointer

    operator IplImage*()
    {
        return m_imageSharedPtr.get();
    }

    operator const IplImage*()const
    {
        return m_imageSharedPtr.get();
    }

public: // Public functions

    // Function used to ask
    // whether the data type
    // is natively supported
    // by opencv

    bool                                    isDataTypeNativelySupported()const;

    // Function used to get the
    // std::shared_ptr that points to
    // to the IplImage
    // This function can be used
    // to link another blImage0 to
    // the same image as this one

    const blImagePtr&                       getImageSharedPtr()const;

    // Function used to get
    // the depth of the image

    int                                     getDepth()const;

    // Function used to get the
    // number of channels of the
    // image

    int                                     getNumOfChannels()const;

    // Functions used to get
    // raw image data char*
    // pointer

    char*                                   getImageData()const;
    unsigned char*                          getImageUnsignedData()const;

    // Function used to get
    // the raw data pointer
    // cast to the intended
    // data type

    blDataType*                             getImageDataCastToDataType()const;

    // Function used to get
    // the raw IplImage pointer

    IplImage*                               getImagePtr()const;

    // Function used to
    // dump the image data
    // and resize the image
    // to zero

    void                                    clear();

    // Functions used to get
    // the size of the image
    // (size1 = height = rows)
    // (size2 = width = cols)
    // (size = total number of
    // pixels/elements in the image)

    int                                     size1()const;
    int                                     size2()const;

    int                                     size()const;
    int                                     length()const;
    int                                     max_size()const;
    bool                                    empty()const;

    // Function used to get
    // whether an index points
    // to an image pixel.
    // For example:  if this
    // is a 10x10 image, the
    // index (11,2) is NOT
    // within the image.

    bool                                    doesIndexPointToPixelInImage(const int& RowIndex,
                                                                         const int& ColIndex)const;

    // Function used to get the
    // size of the image in bytes

    int                                     getSizeInBytes()const;

    // Function used to get the
    // size of the depth in bytes

    int                                     getSizeOfDepthInBytes()const;

    // Function used to get
    // the width step

    int                                     getWidthStep()const;

    // Funtion used to get
    // whether the image origin
    // is top-left (0) or
    // bottom-left (1)

    int                                     getOrigin()const;

    // Function used to get
    // the data order, whether
    // the channels are interleaved (0)
    // or separate (1)

    int                                     getDataOrder()const;

    // Function used to get
    // the alignment of the
    // image (4 or 8 byte)

    int                                     getAlign()const;

    // Functions used to set/get
    // the COI of the image

    int                                     getCOI()const;
    void                                    setCOI(const int& COI);

protected: // Protected variables

    // Image variable

    blImagePtr                              m_imageSharedPtr;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage0<blDataType>::blImage0()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage0<blDataType>::blImage0(const blImage0<blDataType>& Image0)
{
    m_imageSharedPtr = Image0.getImageSharedPtr();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage0<blDataType>& blImage0<blDataType>::operator=(const blImage0<blDataType>& Image0)
{
    m_imageSharedPtr = Image0.getImageSharedPtr();
    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImage0<blDataType>::operator[](const int& rowIndex)
{
    char* value = (m_imageSharedPtr->imageData + m_imageSharedPtr->widthStep * rowIndex);

    return reinterpret_cast<blDataType*>(value);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType* blImage0<blDataType>::operator[](const int& rowIndex)const
{
    char* value = (m_imageSharedPtr->imageData + m_imageSharedPtr->widthStep * rowIndex);

    return reinterpret_cast<blDataType*>(value);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage0<blDataType>::operator()(const int& rowIndex,const int& colIndex)
{
    return ( (*this)[rowIndex][colIndex] );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage0<blDataType>::operator()(const int& rowIndex,const int& colIndex)const
{
    return ( (*this)[rowIndex][colIndex] );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage0<blDataType>::operator()(const int& ImageElementIndex)
{
    return (*this)[0][ImageElementIndex];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage0<blDataType>::operator()(const int& ImageElementIndex)const
{
    return (*this)[0][ImageElementIndex];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage0<blDataType>::circ_at(const int& rowIndex,
                                                 const int& colIndex)
{
    auto rows = this->size1();
    auto cols = this->size2();

    if(rowIndex < 0)
    {
        if(colIndex < 0)
            return ( (*this)(rows + rowIndex % rows,
                             cols + colIndex % cols) );
        else
            return ( (*this)(rows + rowIndex % rows,
                             colIndex % cols) );
    }
    else
    {
        if(colIndex < 0)
            return ( (*this)(rowIndex % rows,
                             cols + colIndex % cols) );
        else
            return ( (*this)(rowIndex % rows,
                             colIndex % cols) );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage0<blDataType>::circ_at(const int& rowIndex,
                                                       const int& colIndex)const
{
    auto rows = this->size1();
    auto cols = this->size2();

    if(rowIndex < 0)
    {
        if(colIndex < 0)
            return ( (*this)(rows + rowIndex % rows,
                             cols + colIndex % cols) );
        else
            return ( (*this)(rows + rowIndex % rows,
                             colIndex % cols) );
    }
    else
    {
        if(colIndex < 0)
            return ( (*this)(rowIndex % rows,
                             cols + colIndex % cols) );
        else
            return ( (*this)(rowIndex % rows,
                             colIndex % cols) );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage0<blDataType>::circ_at(const int& imageElementIndex)
{
    if(imageElementIndex < 0)
    {
        return (*this)(this->size() + imageElementIndex % this->size());
    }
    else
        return (*this)(imageElementIndex % this->size());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage0<blDataType>::circ_at(const int& imageElementIndex)const
{
    if(imageElementIndex < 0)
    {
        return (*this)(this->size() + imageElementIndex % this->size());
    }
    else
        return (*this)(imageElementIndex % this->size());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getDataIndex_circ_at(const int& rowIndex,const int& colIndex)const
{
    auto rows = this->size1();
    auto cols = this->size2();

    if(rowIndex < 0)
    {
        if(colIndex < 0)
            return ( (rows + rowIndex % rows) * cols + (cols + colIndex % cols) );
        else
            return ( (rows + rowIndex % rows) * cols + (colIndex % cols) );
    }
    else
    {
        if(colIndex < 0)
            return ( (rowIndex % rows) * cols + (cols + colIndex % cols) );
        else
            return ( (rowIndex % rows) * cols + (colIndex % cols) );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getDataIndex_circ_at(const int& imageElementIndex)const
{
    if(imageElementIndex < 0)
    {
        return (this->size() + imageElementIndex % this->size());
    }
    else
        return (imageElementIndex % this->size());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage0<blDataType>::clear()
{
    // We dump the
    // image by replacing
    // the shared pointer

    blImagePtr NoImage;
    m_imageSharedPtr = NoImage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const typename blImage0<blDataType>::blImagePtr& blImage0<blDataType>::getImageSharedPtr()const
{
    return m_imageSharedPtr;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getSizeOfDepthInBytes()const
{
    // This function returns
    // the size in bytes needed
    // to represent the specified
    // depth of the image

    switch(getDepth())
    {
    case IPL_DEPTH_1U:
        return sizeof(bool);
    case IPL_DEPTH_16U:
    case IPL_DEPTH_16S:
        return sizeof(short);

    case IPL_DEPTH_32S:
        return sizeof(int);

    case IPL_DEPTH_32F:
        return sizeof(float);

    case IPL_DEPTH_64F:
        return sizeof(double);

    default:
    case IPL_DEPTH_8U:
    case IPL_DEPTH_8S:
        return sizeof(char);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::size1()const
{
    return m_imageSharedPtr->height;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::size2()const
{
    if(isDataTypeNativelySupported())
    {
        return m_imageSharedPtr->width;
    }
    else
    {
        return ( this->getWidthStep() / sizeof(blDataType) );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::size()const
{
    return size1() * size2();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage0<blDataType>::doesIndexPointToPixelInImage(const int& RowIndex,
                                                               const int& ColIndex)const
{
    if(RowIndex < 0 || RowIndex >= this->size1())
        return false;

    if(ColIndex < 0 || ColIndex >= this->size2())
        return false;

    // At this point we know the
    // index is valid

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getSizeInBytes()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->imageSize;
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getWidthStep()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->widthStep;
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getOrigin()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->origin;
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getDataOrder()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->dataOrder;
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getAlign()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->align;
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getCOI()const
{
    if(m_imageSharedPtr)
        return cvGetImageCOI(*this);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage0<blDataType>::setCOI(const int& COI)
{
    if(m_imageSharedPtr)
        cvSetImageCOI(*this,COI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline char* blImage0<blDataType>::getImageData()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr->imageData;
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline unsigned char* blImage0<blDataType>::getImageUnsignedData()const
{
    if(m_imageSharedPtr)
        return reinterpret_cast<unsigned char*>(m_imageSharedPtr->imageData);
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType* blImage0<blDataType>::getImageDataCastToDataType()const
{
    if(m_imageSharedPtr)
        return reinterpret_cast<blDataType*>(m_imageSharedPtr->imageData);
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline IplImage* blImage0<blDataType>::getImagePtr()const
{
    if(m_imageSharedPtr)
        return m_imageSharedPtr.get();
    else
        return NULL;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specializations of the function
// isDataTypeNativelySupported used to know
// whether blDataType is natively supported
// by IplImage
//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage0<blDataType>::isDataTypeNativelySupported()const
{
    if(this->getSizeOfDepthInBytes()*this->getNumOfChannels() == sizeof(blDataType))
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specializations of the function
// getDepth used to get the OpenCV depth value
// for this image
//
// NOTE:  For data types not natively supported by opencv, we
//        choose IPL_DEPTH_8U as the depth
//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0<bool>::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0<unsigned char>::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0<char>::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0<unsigned short>::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0<short>::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0<int>::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0<float>::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0<double>::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< std::complex<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< std::complex<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< std::complex<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< std::complex<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< std::complex<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< std::complex<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< std::complex<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< std::complex<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< blMathAPI::blPoint2d<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< blMathAPI::blPoint2d<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< blMathAPI::blPoint2d<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< blMathAPI::blPoint2d<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< blMathAPI::blPoint2d<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< blMathAPI::blPoint2d<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< blMathAPI::blPoint2d<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< blMathAPI::blPoint2d<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0<CvPoint>::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0<CvPoint2D32f>::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0<CvPoint2D64f>::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0<CvPoint3D32f>::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0<CvPoint3D64f>::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< blColor3<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< blColor3<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< blColor3<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< blColor3<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< blColor3<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< blColor3<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< blColor3<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< blColor3<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< blMathAPI::blVector3d<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< blMathAPI::blVector3d<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< blMathAPI::blVector3d<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< blMathAPI::blVector3d<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< blMathAPI::blVector3d<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< blMathAPI::blVector3d<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< blMathAPI::blVector3d<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< blMathAPI::blVector3d<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< blColor4<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< blColor4<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< blColor4<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< blColor4<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< blColor4<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< blColor4<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< blColor4<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< blColor4<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0< blMathAPI::blRectangle<bool> >::getDepth()const{return IPL_DEPTH_1U;}

template<>
inline int blImage0< blMathAPI::blRectangle<unsigned char> >::getDepth()const{return IPL_DEPTH_8U;}

template<>
inline int blImage0< blMathAPI::blRectangle<char> >::getDepth()const{return IPL_DEPTH_8S;}

template<>
inline int blImage0< blMathAPI::blRectangle<unsigned short> >::getDepth()const{return IPL_DEPTH_16U;}

template<>
inline int blImage0< blMathAPI::blRectangle<short> >::getDepth()const{return IPL_DEPTH_16S;}

template<>
inline int blImage0< blMathAPI::blRectangle<int> >::getDepth()const{return IPL_DEPTH_32S;}

template<>
inline int blImage0< blMathAPI::blRectangle<float> >::getDepth()const{return IPL_DEPTH_32F;}

template<>
inline int blImage0< blMathAPI::blRectangle<double> >::getDepth()const{return IPL_DEPTH_64F;}

template<>
inline int blImage0<CvRect>::getDepth()const{return IPL_DEPTH_8U;}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specializations of the function
// getNumOfChannels used to get the number
// of channels of an IplImage
//
// NOTE:  For data types not natively supported by opencv, we
//        choose 1 as the number of channels
//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage0<blDataType>::getNumOfChannels()const{return 1;}

template<>
inline int blImage0< std::complex<bool> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<unsigned char> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<char> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<unsigned short> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<short> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<int> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<float> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< std::complex<double> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<bool> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<unsigned char> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<char> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<unsigned short> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<short> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<int> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<float> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0< blMathAPI::blPoint2d<double> >::getNumOfChannels()const{return 2;}

template<>
inline int blImage0<CvPoint>::getNumOfChannels()const{return 2;}

template<>
inline int blImage0<CvPoint2D32f>::getNumOfChannels()const{return 2;}

template<>
inline int blImage0<CvPoint2D64f>::getNumOfChannels()const{return 2;}

template<>
inline int blImage0<CvPoint3D32f>::getNumOfChannels()const{return 3;}

template<>
inline int blImage0<CvPoint3D64f>::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<bool> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<unsigned char> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<char> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<unsigned short> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<short> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<int> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<float> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor3<double> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<bool> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<unsigned char> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<char> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<unsigned short> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<short> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<int> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<float> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blMathAPI::blVector3d<double> >::getNumOfChannels()const{return 3;}

template<>
inline int blImage0< blColor4<bool> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<unsigned char> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<char> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<unsigned short> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<short> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<int> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<float> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blColor4<double> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<bool> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<unsigned char> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<char> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<unsigned short> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<short> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<int> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<float> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0< blMathAPI::blRectangle<double> >::getNumOfChannels()const{return 4;}

template<>
inline int blImage0<CvRect>::getNumOfChannels()const{return 4;}
//-------------------------------------------------------------------


#endif // BL_IMAGE0_HPP
