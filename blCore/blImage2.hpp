#ifndef BL_IMAGE2_HPP
#define BL_IMAGE2_HPP


//-------------------------------------------------------------------
// FILE:            blImage2.hpp
// CLASS:           blImage2
// BASE CLASS:      blImage1
//
// PURPOSE:         - Based on blImage1, this class adds functions
//                    used to create a new image of a specified size
//                    and to initialize it to a specified value
//                  - It also adds functions to handle ROIs (Regions
//                    Of Interest)
//                  - It also adds random access forward and reverse
//                    begin/end iterators
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage1 and all its dependencies
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


//-------------------------------------------------------------------
template<typename blDataType>
class blImage2 : public blImage1<blDataType>
{
public: // Public typedefs

    typedef typename blImage1<blDataType>::iterator                                             iterator;
    typedef typename blImage1<blDataType>::const_iterator                                       const_iterator;
    typedef typename blImage1<blDataType>::reverse_iterator                                     reverse_iterator;
    typedef typename blImage1<blDataType>::const_reverse_iterator                               const_reverse_iterator;

    typedef typename blImage1<blDataType>::blImagePtr                                           blImagePtr;

public: // Constructors and destructors

    // Default constructor

    blImage2();

    // Copy constructor

    blImage2(const blImage2<blDataType>& Image2);

    // Destructor

    ~blImage2()
    {
    }

public: // Public functions

    // Functions used to create
    // the image specifying size
    // and initial value

    bool                                    create(int numOfRows,int numOfCols);

    bool                                    create(const int& numOfRows,
                                                   const int& numOfCols,
                                                   const blDataType& initialValue);

    // Function used to set the
    // image to a specified value

    void                                    set(const blDataType& specifiedValue);

    // Functions used to access
    // image componenets based
    // on the image ROI

    blDataType&                             atROI(const int& rowIndex,const int& colIndex);
    const blDataType&                       atROI(const int& rowIndex,const int& colIndex)const;

    blDataType&                             atROI(const int& imageElementIndex);
    const blDataType&                       atROI(const int& imageElementIndex)const;

    // Functions used to
    // circularly access
    // image pixels from
    // the ROI

    blDataType&                             circ_atROI(const int& rowIndex,const int& colIndex);
    const blDataType&                       circ_atROI(const int& rowIndex,const int& colIndex)const;

    blDataType&                             circ_atROI(const int& imageElementIndex);
    const blDataType&                       circ_atROI(const int& imageElementIndex)const;

    // Functions used to
    // get the data index
    // based on circular
    // lookup

    int                                     getDataIndex_circ_atROI(const int& imageElementIndex)const;
    int                                     getDataIndex_circ_atROI(const int& rowIndex,const int& colIndex)const;

    // Function used to get
    // whether an index points
    // to a pixel within the
    // image ROI.

    bool                                    doesIndexPointToPixelInImageROI(const int& rowIndex,
                                                                            const int& colIndex)const;

    // Functions used to
    // get the size of the
    // image ROI.
    //
    // (size1 = ROI height = ROI's rows)
    // (size2 = ROI width = ROI's cols)
    // (size = total number of
    // pixels/elements in the ROI)
    // (xROI = x offset of ROI)
    // (yROI = y offset of ROI)
    // (iROI = where ROI begins when
    // treating image as 1D array)

    int                                     size1ROI()const;
    int                                     size2ROI()const;
    int                                     sizeROI()const;
    int                                     xROI()const;
    int                                     yROI()const;
    int                                     iROI()const;


    // Functions used to
    // set/get the ROI
    // of the image

    _IplROI*                                getROI()const;

    CvRect                                  getROIRect()const;

    void                                    resetROI();

    void                                    setROI(const CvRect& ROIrect,
                                                   const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROI(const blMathAPI::blRectangle<int>& ROIrect,
                                                   const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROI(const int& y,
                                                   const int& x,
                                                   const int& height,
                                                   const int& width,
                                                   const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROIx(const int& x,
                                                    const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROIy(const int& y,
                                                    const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROIi(const int& index,
                                                    const bool& shouldImageBeResizedToAccomodateForBiggerROI);

    void                                    setROIwidth(const int& width,
                                                        const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROIheight(const int& height,
                                                         const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);

    void                                    setROIheightAndwidth(const int& height,
                                                                 const int& width,
                                                                 const bool& shouldImageBeResizedToAccomodateForBiggerROI = false);
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage2<blDataType>::blImage2() : blImage1<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage2<blDataType>::blImage2(const blImage2<blDataType>& Image2) : blImage1<blDataType>(Image2)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage2<blDataType>::create(int numOfRows,int numOfCols)
{
    // Let's make sure that the image
    // is not zero sized so that we
    // don't have to check for zero
    // sized images in other algorithms
    // (It speeds up basic functions
    // such as the size() functions)

    if(numOfRows <= 0)
        numOfRows = 1;
    if(numOfCols <= 0)
        numOfCols = 1;

    // Only create the image
    // if the current image
    // is of different size

    if(this->getImageSharedPtr())
    {
        if(this->size1() == numOfRows && this->size2() == numOfCols)
        {
            // In this case the
            // image is already
            // of the correct size
            // so there's no need
            // to create a new one

            return true;
        }
    }

    IplImage* newImage = NULL;

    // We check if the Depth and
    // Number of channels is correct
    // for the specified data type

    if(this->isDataTypeNativelySupported())
    {
        // In this case the size
        // of the depth and the
        // number of channels
        // is correct

        newImage = cvCreateImage(cvSize(numOfCols,numOfRows),
                                 this->getDepth(),
                                 this->getNumOfChannels());
    }
    else
    {
        // In this case the size
        // of the depth and the
        // number of channels are
        // not representative of the
        // specified data type, therefore
        // we adjust the width of each
        // row to have enough bytes
        // to represent the data type
        // correctly

        newImage = cvCreateImage(cvSize(numOfCols*sizeof(blDataType),numOfRows),
                                 this->getDepth(),
                                 this->getNumOfChannels());

        // Some custom data types
        // might contain pointers
        // that if not properly
        // initialized will crash
        // the program

        // To resolve this, let's
        // zero out the entire array

        if(newImage)
        {
            for(int i = 0; i < newImage->imageSize; ++i)
            {
                newImage->imageData[i] = 0;
            }
        }
    }

    // If we successfully created the image
    // we wrap it with the shared pointer
    // otherwise we return false

    if(newImage != NULL)
    {
        this->m_imageSharedPtr = blImagePtr(newImage,releaseImage());

        // We always set the ROI
        // so that when we check the
        // ROI we can do so very quickly
        // without checking for
        // pointer validity

        cvSetImageROI(*this,CvRect(0,0,this->size2(),this->size1()));

        // We are done and signal
        // that the image was created
        // successfully

        return true;
    }
    else
    {
        // In this case there was an error
        // probably due to specifying the wrong
        // size (either negative or too big)

        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage2<blDataType>::create(const int& numOfRows,
                                         const int& numOfCols,
                                         const blDataType& initialValue)
{
    // If we can successfully create
    // the image then we initialize it

    if(this->create(numOfRows,numOfCols))
    {
        set(initialValue);

        return true;
    }
    else
    {
        // In this case we failed to
        // create the image so we
        // return false

        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::set(const blDataType& specifiedValue)
{
    for(int i = 0; i < this->size1ROI(); ++i)
    {
        for(int j = 0; j < this->size2ROI(); ++j)
        {
            this->atROI(i,j) = specifiedValue;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage2<blDataType>::atROI(const int& rowIndex,const int& colIndex)
{
    return (*this)(rowIndex + yROI(),colIndex + xROI());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage2<blDataType>::atROI(const int& rowIndex,const int& colIndex)const
{
    return (*this)(rowIndex + yROI(),colIndex + xROI());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage2<blDataType>::atROI(const int& imageElementIndex)
{
    return this->atROI(int(imageElementIndex / this->size2ROI()),
                       imageElementIndex % this->size2ROI());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage2<blDataType>::atROI(const int& imageElementIndex)const
{
    return this->atROI(int(imageElementIndex / this->size2ROI()),
                       imageElementIndex % this->size2ROI());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage2<blDataType>::circ_atROI(const int& rowIndex,
                                                    const int& colIndex)
{
    auto rows = this->size1ROI();
    auto cols = this->size2ROI();

    return ( atROI( (rows + rowIndex % rows) % rows, (cols + colIndex % cols) % cols) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage2<blDataType>::circ_atROI(const int& rowIndex,
                                                          const int& colIndex)const
{
    auto rows = this->size1ROI();
    auto cols = this->size2ROI();

    return ( atROI( (rows + rowIndex % rows) % rows, (cols + colIndex % cols) % cols) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType& blImage2<blDataType>::circ_atROI(const int& imageElementIndex)
{
    return this->atROI( (this->sizeROI() + imageElementIndex % this->sizeROI()) % this->sizeROI() );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blDataType& blImage2<blDataType>::circ_atROI(const int& imageElementIndex)const
{
    return this->atROI( (this->sizeROI() + imageElementIndex % this->sizeROI()) % this->sizeROI() );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::getDataIndex_circ_atROI(const int& rowIndex,const int& colIndex)const
{
    auto rows = this->size1ROI();
    auto cols = this->size2ROI();

    auto circledRowIndex = (rows + rowIndex % rows) % rows;
    auto circledColIndex = (cols + colIndex % cols) % cols;

    return circledRowIndex * cols + circledColIndex;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::getDataIndex_circ_atROI(const int& imageElementIndex)const
{
    return ( (this->sizeROI() + imageElementIndex % this->sizeROI()) % this->sizeROI() );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage2<blDataType>::doesIndexPointToPixelInImageROI(const int& rowIndex,
                                                                  const int& colIndex)const
{
    if(rowIndex < xROI() || rowIndex >= xROI() + size1ROI())
        return false;

    if(colIndex < yROI() || colIndex >= yROI() + size2ROI())
        return false;

    // At this point we
    // know the that
    // index is valid

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::size1ROI()const
{
    return (this->m_imageSharedPtr->roi->height);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::size2ROI()const
{
    return (this->m_imageSharedPtr->roi->width);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::sizeROI()const
{
    return size1ROI() * size2ROI();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::xROI()const
{
    return (this->m_imageSharedPtr->roi->xOffset);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::yROI()const
{
    return (this->m_imageSharedPtr->roi->yOffset);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline int blImage2<blDataType>::iROI()const
{
    return (xROI() * yROI());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline _IplROI* blImage2<blDataType>::getROI()const
{
    return this->m_imageSharedPtr->roi;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline CvRect blImage2<blDataType>::getROIRect()const
{
    return CvRect(this->m_imageSharedPtr->roi->xOffset,
                  this->m_imageSharedPtr->roi->yOffset,
                  this->m_imageSharedPtr->roi->width,
                  this->m_imageSharedPtr->roi->height);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::resetROI()
{
    // We reset the image
    // ROI to the entire image

    this->setROI(0,0,this->size1(),this->size2());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROI(const CvRect& ROIrect,
                                         const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    if(!shouldImageBeResizedToAccomodateForBiggerROI)
    {
        if(this->size() > 0)
        {
            int xNew = std::min(this->size2() - 1,ROIrect.x);
            int yNew = std::min(this->size1() - 1,ROIrect.y);
            int widthNew = std::min(this->size2() - xNew,ROIrect.width);
            int heightNew = std::min(this->size1() - yNew,ROIrect.height);

            cvSetImageROI((*this),CvRect(xNew,yNew,widthNew,heightNew));
        }
    }
    else
    {
        // In this case, we have to
        // check if the user has set
        // a ROI which would be
        // out of the bounds of the
        // current image, and if that
        // is the case then the user wants
        // us to grow the image, which we
        // do by simply padding the image

        if( (ROIrect.x >= this->size2()) ||
            (ROIrect.x + ROIrect.width > this->size2()) ||
            (ROIrect.y >= this->size1()) ||
            (ROIrect.y + ROIrect.height > this->size1()) )
        {
            // Create the padded
            // image

            blImage2<blDataType> paddedImage;
            paddedImage.create(std::max(ROIrect.y + ROIrect.height,int(this->size1())),
                               std::max(ROIrect.x + ROIrect.width,int(this->size2())));

            // Copy the original
            // image into the padded
            // one

            for(int i = 0; i < this->size1(); ++i)
            {
                for(int j = 0; j < this->size2(); ++j)
                {
                    paddedImage(i,j) = (*this)(i,j);
                }
            }

            paddedImage.setROI(ROIrect);

            // Finally we save the
            // padded image into this
            // image

            this->m_imageSharedPtr = paddedImage.getImageSharedPtr();
        }
        else
        {
            // In this case the ROI is
            // ok for this image

            cvSetImageROI(*this,ROIrect);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROI(const blMathAPI::blRectangle<int>& ROIrect,
                                         const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    this->setROI(CvRect(ROIrect),shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROI(const int& y,
                                         const int& x,
                                         const int& height,
                                         const int& width,
                                         const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    this->setROI(cvRect(x,y,width,height),shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIx(const int& x,
                                          const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.x = x;

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIy(const int& y,
                                          const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.y = y;

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIi(const int& index,
                                          const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.y = index / this->size2();
    ROIRect.x = index % this->size2();

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIwidth(const int& width,
                                              const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.width = width;

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIheight(const int& height,
                                               const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.height = height;

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage2<blDataType>::setROIheightAndwidth(const int& height,
                                                       const int& width,
                                                       const bool& shouldImageBeResizedToAccomodateForBiggerROI)
{
    CvRect ROIRect = this->getROI();

    ROIRect.height = height;
    ROIRect.width = width;

    this->setROI(ROIRect,shouldImageBeResizedToAccomodateForBiggerROI);
}
//-------------------------------------------------------------------


#endif // BL_IMAGE2_HPP
