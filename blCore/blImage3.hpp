#ifndef BL_IMAGE3_HPP
#define BL_IMAGE3_HPP


//-------------------------------------------------------------------
// FILE:            blImage3.hpp
// CLASS:           blImage3
// BASE CLASS:      blImage2
//
// PURPOSE:         - Based on blImage2, this class adds functions to
//                    clone other images, as well as functions to
//                    wrap IplImage images with blImage objects without
//                    making copies of the image data
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
// DEPENDENCIES:    blImage2 and all its dependencies
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
class blImage3 : public blImage2<blDataType>
{
protected: // Protected typedefs

    typedef blImageCircularIterator<blDataType>                     iteratorROI;
    typedef blImageCircularIterator<const blDataType>               const_iteratorROI;

    typedef typename blImage2<blDataType>::blImagePtr               blImagePtr;

public: // Constructors and destructors

    // Default constructor
    blImage3();

    // Copy constructor
    blImage3(const blImage3<blDataType>& image3);

    // Destructor
    ~blImage3()
    {
    }

public: // Public functions

    // Function used to
    // clone an IplImage
    // or CvMat into this
    // object

    bool                                    clone(const IplImage* imageToClone,
                                                  const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    bool                                    clone(const CvMat* matrixToClone,
                                                  const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    #ifdef SFML_IMAGE_HPP

    // Function used to clone an sfml
    // image (from the sfml library)

    bool                                    clone(const sf::Image& srcImage);

    // Function used to load a blImage
    // into an sfml image

    bool                                    loadImageIntoSFMLImage(sf::Image& dstImage);

    #endif

    // Function used to
    // wrap an IplImage
    // pointer with a
    // blImage object
    // with options of
    // whether the blImage
    // is in charge of
    // releasing the
    // IplImage.
    // Also if the blImage
    // object is in charge
    // of releasing the
    // IplImage, then
    // the IplImage
    // pointer is Nullified

    bool                                    wrap(IplImage*& imageToWrap,
                                                 const bool& isThisObjectInChargeOfReleasingIplImage);

    bool                                    wrap(CvMat* imageToWrap,
                                                 const bool& isThisObjectInChargeOfReleasingIplImage);

    // Function used to
    // create a blImage
    // object that points
    // to the same image
    // data as the srcImage
    // blImage object

    void                                    wrap(const blImage3<blDataType>& srcImage);

private: // Private functions

    // Function used to
    // clone an IplImage
    // of the same depth
    // and number of channels

    void                                    cloneFromImageOfSameType(const IplImage* srcImage);

public: // Circular iterators

    // Functions used to
    // get circular iterators to
    // the beginning and end of
    // the ROI
    // (They go through the ROI
    // just once, they're useful
    // in having an stl algorithm
    // go through the ROI because
    // the ROI is not a contiguous
    // space in memory)

    iteratorROI                             begin_ROI();
    iteratorROI                             end_ROI();

    const_iteratorROI                       cbegin_ROI()const;
    const_iteratorROI                       cend_ROI()const;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage3<blDataType>::blImage3() : blImage2<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage3<blDataType>::blImage3(const blImage3<blDataType>& image3) : blImage2<blDataType>(image3)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage3<blDataType>::wrap(const blImage3<blDataType>& srcImage)
{
    // Here we just
    // copy the shared
    // pointer of the
    // source image

    this->m_imageSharedPtr = srcImage.getImageSharedPtr();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline bool blImage3<blType>::wrap(CvMat* imageToWrap,
                                   const bool& isThisObjectInChargeOfReleasingIplImage)
{
    // First we need
    // to check the
    // validity of the
    // CvMat* pointer

    if(!imageToWrap)
    {
        // Error -- Tried to wrap a
        //          CvMat* structure
        //          into this image
        //          using an invalild
        //          pointer.

        return false;
    }

    // Now that we know
    // we have a valid
    // pointer, we create
    // an IplImage header
    // from the CvMat image

    IplImage matToImg;
    cvGetImage(imageToWrap,&matToImg);

    // Now we can
    // finally wrap
    // the image

    bool wereWeSuccessfull = this->wrap(matToImg,isThisObjectInChargeOfReleasingIplImage);

    // If this object
    // is in in charge
    // of releasing
    // the image, then
    // we NULLify the
    // passed CvMat pointer

    if(isThisObjectInChargeOfReleasingIplImage)
        imageToWrap = NULL;

    // Now we return
    // whether the
    // image was
    // successfully
    // wrapped

    return wereWeSuccessfull;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage3<blDataType>::wrap(IplImage*& imageToWrap,
                                       const bool& isThisObjectInChargeOfReleasingIplImage)
{
    // First we have
    // to check if the
    // passed image
    // pointer valid

    if(!imageToWrap)
    {
        // Error -- Tried to wrap an
        //          IplImage pointer
        //          that was not valid

        return false;
    }

    // Since blImage has
    // a statically
    // determined depth
    // and number of
    // channels, if we
    // want to wrap
    // an existing
    // IplImage pointer,
    // we have to check
    // if both the depth
    // and number of
    // channels match this
    // blImage

    if(imageToWrap->depth != this->getDepth() ||
       imageToWrap->nChannels != this->getNumOfChannels())
    {
        // In this case
        // they don't match,
        // therefore we
        // can't simply wrap
        // the IplImage
        // pointer with this
        // blImage.
        // So we simply quit

        return false;
    }
    else
    {
        // In this case
        // they match,
        // therefore
        // we can wrap
        // the IplImage
        // pointer

        if(isThisObjectInChargeOfReleasingIplImage)
        {
            // In this case,
            // we're in charge
            // of releasing
            // the image, so we
            // wrap the IplImage
            // and NULLify the
            // IplImage pointer

            this->m_imageSharedPtr = blImagePtr(imageToWrap,releaseImage());
            imageToWrap = NULL;
        }
        else
        {
            // In this case,
            // we're not in
            // charge of releasing
            // the IplImage pointer,
            // so we wrap it using
            // a null_deleter

            this->m_imageSharedPtr = blIteratorAPI::get_shared_ptr(imageToWrap);
        }

        return true;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage3<blDataType>::cloneFromImageOfSameType(const IplImage* srcImage)
{
    // NOTE:  This function
    //        does not check
    //        the validity
    //        of the source image
    //        pointer

    // First we make
    // sure this image is
    // the same size as the
    // source image

    this->create(srcImage->height,srcImage->width);

    // Now that we
    // know that the
    // sizes match, we
    // simply copy
    // the data from
    // the source image

    std::copy(
              srcImage->imageData,
              srcImage->imageData + srcImage->imageSize,
              &((*this)(0))
             );

    // We also copy
    // the region of
    // interest (ROI)

    this->setROI(cvGetImageROI(srcImage));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline bool blImage3<blType>::clone(const CvMat* imageToClone,
                                    const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // First we need
    // to check the
    // validity of the
    // CvMat* pointer

    if(!imageToClone)
    {
        // Error -- Tried to load a
        //          CvMat* structure
        //          into this image
        //          using a NULL pointer.

        return false;
    }

    // Now that we
    // know we don't
    // have an invaild
    // pointer, we
    // create an IplImage
    // header from the
    // CvMat image

    IplImage matToImg;
    cvGetImage(imageToClone,&matToImg);

    // Now we can
    // finally load
    // the matrix
    // into this image

    return (this->clone(&matToImg,inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2));
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blImage3<blDataType>::clone(const IplImage* imageToClone,
                                        const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Check if the
    // image pointer
    // is not valid

    if(!imageToClone)
    {
        // Error -- Tried to clone an
        //          invalid image

        return false;
    }

    // Because this
    // blImage has a
    // pre-determined
    // depth and number
    // of channels, we
    // have to do different
    // things depending
    // on the depth and
    // number of channels
    // of the user specified
    // image

    if(imageToClone->depth == this->getDepth() &&
       imageToClone->nChannels == this->getNumOfChannels())
    {
        // In this case,
        // both the depth
        // and number of
        // channels match

        cloneFromImageOfSameType(imageToClone);

        return true;
    }

    // If we made
    // it this far,
    // it is because
    // either the depths
    // or number of
    // channels don't
    // match so we have
    // to do some extra
    // work

    // First we make
    // sure that the
    // size of this image
    // matches the size
    // of the source
    // image to clone

    if(!this->create(imageToClone->height,imageToClone->width))
    {
        // Error -- We have failed to resize
        //          the image to the correct
        //          size so we quit

        return false;
    }

    // We also copy
    // the region of
    // interest (ROI)

    cvSetImageROI((*this),cvGetImageROI(imageToClone));

    // First we check
    // if the number
    // of channels match

    if(this->getNumOfChannels() == imageToClone->nChannels)
    {
        // In this case,
        // we know that
        // the number of
        // channels match,
        // but that the
        // depths are different,
        // so we use the
        // convertScale
        // function to
        // convert the image
        // to the correct depth

        convertScale(imageToClone,(*this));

        // We're done

        return true;
    }

    // By now, we know
    // that the number
    // of channels don't
    // match, so we have
    // to do some extra
    // work to convert
    // the image

    // First we get the scale/shift
    // parameters

    double scale = 1;
    double shift = 0;

    getScaleShiftConversionParameters(imageToClone->depth,this->getDepth(),scale,shift);

    // There are many cases:

    // Case 1:  - imageToClone has 2 channels
    //          - this image has 1 channel

    if(this->getNumOfChannels() == 1 && imageToClone->nChannels == 2)
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom2ChannelsTo1Channel(imageToClone,
                                               i,j,
                                               this->getImagePtr(),
                                               i,j,
                                               scale,
                                               shift,
                                               inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
            }
        }

        // We're done
        return true;
    }

    // Case 2:  - imageToClone has 2 channels
    //          - this image has 3 channels

    if(this->getNumOfChannels() == 3 && imageToClone->nChannels == 2)
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom2ChannelsTo3Channels(imageToClone,
                                                i,j,
                                                this->getImagePtr(),
                                                i,j,
                                                scale,
                                                shift,
                                                inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
            }
        }

        // We're done
        return true;
    }

    // Case 3:  - imageToClone has 2 channels
    //          - this image has 4 channels

    if(this->getNumOfChannels() == 4 && imageToClone->nChannels == 2)
    {
        double alphaValue = rangeMax(this->getDepth());

        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom2ChannelsTo4Channels(imageToClone,
                                                i,j,
                                                this->getImagePtr(),
                                                i,j,
                                                scale,
                                                shift,
                                                inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2,
                                                alphaValue);
            }
        }

        // We're done
        return true;
    }

    // Case 4:  - imageToClone has 3 or 4 channels
    //          - this image has 1 channels

    if(this->getNumOfChannels() == 1 && (imageToClone->nChannels == 3 || imageToClone->nChannels == 4))
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom3or4ChannelsTo1Channel(imageToClone,
                                                  i,j,
                                                  this->getImagePtr(),
                                                  i,j,
                                                  scale,
                                                  shift);
            }
        }

        // We're done
        return true;
    }

    // Case 5:  - imageToClone has 3 or 4 channels
    //          - this image has 2 channels

    if(this->getNumOfChannels() == 2 && (imageToClone->nChannels == 3 || imageToClone->nChannels == 4))
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom3or4ChannelsTo2Channels(imageToClone,
                                                   i,j,
                                                   this->getImagePtr(),
                                                   i,j,
                                                   scale,
                                                   shift);
            }
        }

        // We're done
        return true;
    }

    // Case 6:  - imageToClone has 4 channels
    //          - this image has 3 channels

    if(this->getNumOfChannels() == 3 && imageToClone->nChannels == 4)
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom4ChannelsTo3Channels(imageToClone,
                                                i,j,
                                                this->getImagePtr(),
                                                i,j,
                                                scale,
                                                shift);
            }
        }

        // We're done
        return true;
    }

    // Case 7:  - imageToClone has 3 channels
    //          - this image has 4 channels

    if(this->getNumOfChannels() == 4 && imageToClone->nChannels == 3)
    {
        double alphaValue = rangeMax(this->getDepth());

        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom3ChannelsTo4Channels(imageToClone,
                                                i,j,
                                                this->getImagePtr(),
                                                i,j,
                                                scale,
                                                shift,
                                                alphaValue);
            }
        }

        // We're done
        return true;
    }

    // Case 8:  - imageToClone has 1 channel
    //          - this image has 2 channels

    if(this->getNumOfChannels() == 2 && imageToClone->nChannels == 1)
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom1ChannelTo2Channels(imageToClone,
                                               i,j,
                                               this->getImagePtr(),
                                               i,j,
                                               scale,
                                               shift);
            }
        }

        // We're done
        return true;
    }

    // Case 9:  - imageToClone has 1 channel
    //          - this image has 3 channels

    if(this->getNumOfChannels() == 3 && imageToClone->nChannels == 1)
    {
        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom1ChannelTo3Channels(imageToClone,
                                               i,j,
                                               this->getImagePtr(),
                                               i,j,
                                               scale,
                                               shift);
            }
        }

        // We're done
        return true;
    }

    // Case 10: - imageToClone has 1 channel
    //          - this image has 4 channels

    if(this->getNumOfChannels() == 4 && imageToClone->nChannels == 1)
    {
        double alphaValue = rangeMax(this->getDepth());

        for(int i = 0; i < imageToClone->height; ++i)
        {
            for(int j = 0; j < imageToClone->width; ++j)
            {
                convertFrom1ChannelTo4Channels(imageToClone,
                                               i,j,
                                               this->getImagePtr(),
                                               i,j,
                                               scale,
                                               shift,
                                               alphaValue);
            }
        }

        // We're done
        return true;
    }

    // If we got here, it's because
    // we encountered a case that
    // we did not originally think of.
    // So we simply return false

    return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage3<blDataType>::iteratorROI blImage3<blDataType>::begin_ROI()
{
    return iteratorROI(*this,0,1);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage3<blDataType>::iteratorROI blImage3<blDataType>::end_ROI()
{
    return iteratorROI(*this,this->sizeROI(),0);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage3<blDataType>::const_iteratorROI blImage3<blDataType>::cbegin_ROI()const
{
    return const_iteratorROI(*this,0,1);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline typename blImage3<blDataType>::const_iteratorROI blImage3<blDataType>::cend_ROI()const
{
    return const_iteratorROI(*this,this->sizeROI(),0);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
#ifdef SFML_IMAGE_HPP

template<typename blDataType>
inline bool blImage3<blDataType>::clone(const sf::Image& srcImage)
{
    // First we create an unsigned
    // char image of the correct
    // size to load the src image
    // into

    blImage3< blColor4<unsigned char> > img;

    // Then we load the source
    // image into it

    bool wasOperationSuccessful = img.clone(srcImage);

    if(wasOperationSuccessful)
    {
        // Finally we clone the
        // loaded image, which takes
        // care of converting it
        // to this specific data type

        wasOperationSuccessful = this->clone(img);
    }

    return wasOperationSuccessful;
}


// Template specialization for when
// the image is of type blColor4<unsigned char>

template<>
inline bool blImage3< blColor4<unsigned char> >::clone(const sf::Image& srcImage)
{
    // Get the size of
    // the source image

    int rows = srcImage.getSize().y;
    int cols = srcImage.getSize().x;

    if((rows * cols) <= 0)
    {
        // There's nothing to copy
        // so we can return

        return false;
    }

    // Let's make sure to
    // size the image correctly

    auto wasImageCreatedSuccessfully = this->create(rows,cols);

    if(!wasImageCreatedSuccessfully)
    {
        // We were not able to
        // create an appropriately
        // sized image

        return false;
    }

    // Then we assign each individual
    // pixel, one by one

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            (*this)(i,j) = srcImage.getPixel(j,i);
        }
    }

    // We're done

    return true;
}

#endif
//-------------------------------------------------------------------





//-------------------------------------------------------------------
#ifdef SFML_IMAGE_HPP

template<typename blDataType>
inline bool blImage3<blDataType>::loadImageIntoSFMLImage(sf::Image& dstImage)
{
    // First we convert this image
    // into one of type blColor4<unsigned char>

    blImage3< blColor4<unsigned char> > img;

    img.clone(*this);

    // Then we load the newly
    // converted image into the
    // destination image

    auto wasOperationSuccessful = img.loadImageIntoSFMLImage(dstImage);

    // We're done

    return wasOperationSuccessful;
}


// Template specialization for when
// the image is of type blColor4<unsigned char>

template<>
inline bool blImage3< blColor4<unsigned char> >::loadImageIntoSFMLImage(sf::Image& dstImage)
{
    int rows = this->size1();
    int cols = this->size2();

    // First we make sure that
    // the destination image
    // is of the correct size

    if(int(dstImage.getSize().y) != rows ||
       int(dstImage.getSize().x) != cols)
    {
        dstImage.create(cols,rows);

        if(int(dstImage.getSize().y) != rows ||
           int(dstImage.getSize().x) != cols)
        {
            // Image creation failed

            return false;
        }
    }

    // We now have the correctly sized
    // destination image, so we asssign
    // each individual pixel one by one

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            dstImage.setPixel(j,i,(*this)(i,j));
        }
    }

    // We're done

    return true;
}

#endif
//-------------------------------------------------------------------


#endif // BL_IMAGE3_HPP
