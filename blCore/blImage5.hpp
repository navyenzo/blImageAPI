#ifndef BL_IMAGE5_HPP
#define BL_IMAGE5_HPP


//-------------------------------------------------------------------
// FILE:            blImage5.hpp
// CLASS:           blImage5
// BASE CLASS:      blImage4
//
// PURPOSE:         Based on blImage4, this class adds functions
//                  used to wrap or clone static 1d and 2d arrays
//                  as well as "vectors" such as std::vector with
//                  a blImage
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage4 and all its dependencies
//
// NOTES:
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
class blImage5 : public blImage4<blDataType>
{
public: // Constructors and destructors

    // Default constructor
    blImage5();

    // Copy constructor
    blImage5(const blImage5<blDataType>& Image5);

    // Destructor
    ~blImage5()
    {
    }

public: // Public functions

    // Overload the wrap and
    // clone functions from
    // parent classes so that
    // they're not hidden

    using blImage4<blDataType>::wrap;
    using blImage4<blDataType>::clone;

    // Functions used to
    // wrap or copy static 1d or
    // 2d arrays or "vectors"
    // such as std::vector with
    // this blImage structure

    template<int numOfDataPoints>
    bool                                    wrap1DArray(blDataType (&staticArray)[numOfDataPoints],
                                                        int optionalNumOfRows = 1,
                                                        int optionalNumOfCols = numOfDataPoints);

    template<int numOfDataPoints>
    bool                                    clone1DArray(const blDataType (&staticArray)[numOfDataPoints],
                                                         int optionalNumOfRows = 1,
                                                         int optionalNumOfCols = numOfDataPoints);

    template<int numOfRows,int numOfCols>
    bool                                    wrap2DArray(blDataType (&staticArray)[numOfRows][numOfCols]);

    template<typename blDataType2,int numOfRows,int numOfCols>
    bool                                    clone2DArray(const blDataType2 (&staticArray)[numOfRows][numOfCols]);

    // NOTE:  These functions assume
    // that the passed vector defines
    // the following member functions:
    // - size()
    // - operator[]()

    template<typename vectorType>
    bool                                    wrapVector(vectorType& vectorToWrap);

    template<typename vectorType>
    bool                                    cloneVector(const vectorType& vectorToClone);
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage5<blDataType>::blImage5() : blImage4<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage5<blDataType>::blImage5(const blImage5<blDataType>& Image5) : blImage4<blDataType>(Image5)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfDataPoints>
inline bool blImage5<blDataType>::wrap1DArray(blDataType (&staticArray)[numOfDataPoints],
                                              int optionalNumOfRows,
                                              int optionalNumOfCols)
{
    // First we check the
    // size of the passed
    // array

    if(numOfDataPoints <= 0)
    {
        // Error -- Tried to wrap a
        //          static array of
        //          size zero

        return false;
    }

    if(optionalNumOfRows * optionalNumOfCols != numOfDataPoints)
    {
        optionalNumOfRows = 1;
        optionalNumOfCols = numOfDataPoints;
    }

    // We need to create
    // an IplImage header
    // and have its data
    // pointer point to
    // the passed static
    // array

    IplImage* NewImageHeader = NULL;
    NewImageHeader = cvCreateImageHeader(cvSize(optionalNumOfCols,optionalNumOfRows),
                                         this->getDepth(),
                                         this->getNumOfChannels());

    // Let's check if
    // the image header
    // was successfully
    // created

    if(!NewImageHeader)
    {
        // Error -- The new image header
        //          was not successfully
        //          created

        return false;
    }

    // Now that we have
    // the image header,
    // we make its data
    // pointer point to
    // the passed static
    // array

    NewImageHeader->imageData = reinterpret_cast<char*>(&staticArray[0]);

    // Finally we create the
    // shared pointer so that
    // it points to the image
    // header but does not
    // release it once it runs
    // out of scope

    this->m_imageSharedPtr = get_shared_ptr(NewImageHeader);

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // We're done

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfDataPoints>
inline bool blImage5<blDataType>::clone1DArray(const blDataType (&staticArray)[numOfDataPoints],
                                               int optionalNumOfRows,
                                               int optionalNumOfCols)
{
    // First we check the
    // size of the passed
    // array

    if(numOfDataPoints <= 0)
    {
        // Error -- Tried to wrap a
        //          static array of
        //          size zero

        return false;
    }

    if(optionalNumOfRows * optionalNumOfCols != numOfDataPoints)
    {
        optionalNumOfRows = 1;
        optionalNumOfCols = numOfDataPoints;
    }

    // Now we create the
    // empty image of the
    // correct size

    if(this->size1() != optionalNumOfRows || this->size2() != optionalNumOfCols)
    {
        // Here we try
        // to create
        // an image of
        // the correct
        // size

        if(!this->create(optionalNumOfRows,optionalNumOfCols))
        {
            // Error -- We tried to create
            //          an image of the
            //          correct size,
            //          but failed to do
            //          so

            return false;
        }
    }

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // Now that we have
    // the empty image
    // we'll just copy
    // the vector value
    // by value

    for(int i = 0; i < this->size(); ++i)
    {
        (*this)(i) = staticArray[i];
    }

    // We're done

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfRows,int numOfCols>
inline bool blImage5<blDataType>::wrap2DArray(blDataType (&staticArray)[numOfRows][numOfCols])
{
    // First we check the
    // size of the passed
    // matrix

    if(numOfRows <= 0 || numOfCols <= 0)
    {
        // Error -- Tried to wrap a
        //          static array of
        //          size zero

        return false;
    }

    // We need to create
    // an IplImage header
    // and have its data
    // pointer point to
    // the passed static
    // array

    IplImage* NewImageHeader = NULL;
    NewImageHeader = cvCreateImageHeader(cvSize(numOfCols,numOfRows),
                                         this->getDepth(),
                                         this->getNumOfChannels());

    // Let's check if
    // the image header
    // was successfully
    // created

    if(!NewImageHeader)
    {
        // Error -- The new image header
        //          was not successfully
        //          created

        return false;
    }

    // Now that we have
    // the image header,
    // we make its data
    // pointer point to
    // the passed static
    // array

    NewImageHeader->imageData = reinterpret_cast<char*>(&staticArray[0][0]);

    // Finally we create the
    // shared pointer so that
    // it points to the image
    // header but does not
    // release it once it runs
    // out of scope

    this->m_imageSharedPtr = get_shared_ptr(NewImageHeader);

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // We're done

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2,int numOfRows,int numOfCols>
inline bool blImage5<blDataType>::clone2DArray(const blDataType2 (&staticArray)[numOfRows][numOfCols])
{
    // First we check the
    // size of the passed
    // matrix

    if(numOfRows <= 0 || numOfCols <= 0)
    {
        // Error -- Tried to clone a
        //          static array of
        //          size zero

        return false;
    }

    // Now we make
    // sure that
    // this blImage
    // object is of
    // the correct size

    if(this->size1() != numOfRows || this->size2() != numOfCols)
    {
        // Here we try
        // to create
        // an image of
        // the correct
        // size

        if(!this->create(numOfRows,numOfCols))
        {
            // Error -- We tried to create
            //          an image of the
            //          correct size,
            //          but failed to do
            //          so

            return false;
        }
    }

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // Now we just copy the
    // data one value at a
    // time

    for(int i = 0; i < numOfRows; ++i)
    {
        for(int j = 0; j < numOfCols; ++j)
        {
            (*this)(i,j) = staticArray[i][j];
        }
    }

    // We're done

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename vectorType>
inline bool blImage5<blDataType>::wrapVector(vectorType& vectorToWrap)
{
    // We need to create
    // an IplImage header
    // and have its data
    // pointer point to
    // the passed vector

    // NOTE:  The size of
    // vector will be (1xSize)

    IplImage* NewImageHeader = NULL;
    NewImageHeader = cvCreateImageHeader(cvSize(vectorToWrap.size(),1),
                                         this->getDepth(),
                                         this->getNumOfChannels());

    // Let's check if
    // the image header
    // was successfully
    // created

    if(!NewImageHeader)
    {
        // Error -- The new image header
        //          was not successfully
        //          created

        return false;
    }

    // Now that we have
    // the image header,
    // we make its data
    // pointer point to
    // the passed static
    // array

    NewImageHeader->imageData = reinterpret_cast<char*>(&vectorToWrap[0]);

    // Finally we create the
    // shared pointer so that
    // it points to the image
    // header but does not
    // release it once it runs
    // out of scope

    this->m_imageSharedPtr = get_shared_ptr(NewImageHeader);

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // We're done

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename vectorType>
inline bool blImage5<blDataType>::cloneVector(const vectorType& vectorToClone)
{
    // We make sure that
    // the image is of
    // the correct size

    // NOTE:  The vector is assumed
    // to be a 1d vector of size (1xsize)

    if(this->size() != vectorToClone.size())
    {
        // Here we try
        // to create
        // an image of
        // the correct
        // size

        if(!this->create(1,vectorToClone.size()))
        {
            // Error -- We tried to create
            //          an image of the
            //          correct size,
            //          but failed to do
            //          so

            return false;
        }
    }

    // We always set the ROI
    // so that when we check the
    // ROI we can do so very quickly
    // without checking for
    // pointer validity

    cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

    // Now we just copy the
    // data one value at a
    // time

    for(int i = 0; i < this->size(); ++i)
    {
        (*this)(i) = vectorToClone[i];
    }

    // We're done

    return true;
}
//-------------------------------------------------------------------


#endif // BL_IMAGE5_HPP
