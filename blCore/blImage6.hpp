#ifndef BL_IMAGE6_HPP
#define BL_IMAGE6_HPP


//-------------------------------------------------------------------
// FILE:            blImage6.hpp
// CLASS:           blImage6
// BASE CLASS:      blImage5
//
// PURPOSE:         Based on blImage5, this class adds functions
//                  used to copy individual row and column vectors
//                  from the image or to copy a sub-image into
//                  another one
//                  It also adds a function to sample the image
//                  at specified intervals into a smaller or
//                  bigger image without any interpolation
//                  It also adds functions to add/subtract/multiply
//                  and divide the image by a datapoint or a datapoint
//                  by an image (Individual pixels)
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage5 and all its dependencies
//
// NOTES:
//
// DATE CREATED:    Feb/01/2012
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
class blImage6 : public blImage5<blDataType>
{
public: // Constructors and destructors

    // Default constructor
    blImage6();

    // Copy constructor
    blImage6(const blImage6<blDataType>& Image6);

    // Destructor
    ~blImage6()
    {
    }

public: // Public functions

    // Functions used to
    // add, subtract,
    // multiply and divide
    // the image by a data
    // point (Individual
    // pixels)

    template<typename blDataType2>
    const blImage6<blDataType>&         addDataPointToImagePixels(const blDataType2& dataPoint);

    template<typename blDataType2>
    const blImage6<blDataType>&         subtractDataPointFromImagePixels(const blDataType2& dataPoint);

    template<typename blDataType2>
    const blImage6<blDataType>&         multiplyImagePixelsByDataPoint(const blDataType2& dataPoint);

    template<typename blDataType2>
    const blImage6<blDataType>&         divideImagePixelsByDataPoint(const blDataType2& dataPoint);

    template<typename blDataType2>
    const blImage6<blDataType>&         subtractImagePixelsFromDataPoint(const blDataType2& dataPoint);

    template<typename blDataType2>
    const blImage6<blDataType>&         divideDataPointByImagePixels(const blDataType2& dataPoint);

    // Functions used to
    // partially copy a
    // row of another image.
    //
    // NOTE:    This function
    //          does not check
    //          the validity of
    //          the specified indeces

    void                                copyRowVector(const blImage6<blDataType>& srcImage,
                                                      const int& whichRowToCopyFromSrcImage,
                                                      const int& whichColToStartCopyingFromSrcImage,
                                                      const int& howManyColsToCopyFromSrcImage,
                                                      const int& whichRowToWriteInto,
                                                      const int& whichColToStartWritingInto);

    // Functions used to
    // partially copy a
    // column of another
    // image.
    //
    // NOTE:    This function
    //          does not check
    //          the validity of
    //          the specified indeces

    void                                copyColVector(const blImage6<blDataType>& srcImage,
                                                      const int& whichColToCopyFromSrcImage,
                                                      const int& whichRowToStartCopyingFromSrcImage,
                                                      const int& howManyRowsToCopyFromSrcImage,
                                                      const int& whichRowToWriteInto,
                                                      const int& whichColToStartWritingInto);

    // Function used to
    // copy a portion of
    // another image
    // without the cumbersome
    // use of ROIs
    //
    // NOTE:    This function
    //          does not check
    //          the validity of
    //          the specified indeces

    template<typename blDataType2>
    void                                copyImage(const blImage6<blDataType2>& srcImage,
                                                  const int& whichRowToStartCopyingFrom,
                                                  const int& whichColToStartCopyingFrom,
                                                  const int& howManyRowsToCopy,
                                                  const int& howManyColsToCopy,
                                                  const int& whichRowToStartWritingInto,
                                                  const int& whichColToStartWritingInto);

    // Function used to
    // sample the image
    // at a specified
    // interval.  The
    // image is sampled
    // without interpolating
    // the pixel values
    //
    // NOTE:    This function
    //          does not check
    //          the validity of
    //          the specified indeces

    void                                sampleImageWithoutInterpolating(const blImage6<blDataType>& srcImage,
                                                                        const int& whichRowToStartSamplingFrom,
                                                                        const int& whichColToStartSamplingFrom,
                                                                        const int& howManyRowsToSample,
                                                                        const int& howManyColsToSample);
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage6<blDataType>::blImage6() : blImage5<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage6<blDataType>::blImage6(const blImage6<blDataType>& Image6) : blImage5<blDataType>(Image6)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Define the basic
// arithmetic functions
// here
//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::addDataPointToImagePixels(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) += dataPoint;
    }

    return (*this);
}


template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::subtractDataPointFromImagePixels(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) -= dataPoint;
    }

    return (*this);
}


template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::multiplyImagePixelsByDataPoint(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) *= dataPoint;
    }

    return (*this);
}


template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::divideImagePixelsByDataPoint(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) /= dataPoint;
    }

    return (*this);
}


template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::subtractImagePixelsFromDataPoint(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) = dataPoint - (*pixel);
    }

    return (*this);
}


template<typename blDataType>
template<typename blDataType2>
inline const blImage6<blDataType>& blImage6<blDataType>::divideDataPointByImagePixels(const blDataType2& dataPoint)
{
    for(auto pixel = this->begin();
        pixel != this->end();
        ++pixel)
    {
        (*pixel) = dataPoint / (*pixel);
    }

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage6<blDataType>::copyRowVector(const blImage6<blDataType>& srcImage,
                                                const int& whichRowToCopyFromSrcImage,
                                                const int& whichColToStartCopyingFromSrcImage,
                                                const int& howManyColsToCopyFromSrcImage,
                                                const int& whichRowToWriteInto,
                                                const int& whichColToStartWritingInto)
{
    // NOTE:  This function does not
    //        check the validity of
    //        the specified indeces
    //        nor does it check to
    //        make sure this image
    //        is of the correct size

    CopyData(&(srcImage[whichRowToCopyFromSrcImage][whichColToStartCopyingFromSrcImage]),
             &(srcImage[whichRowToCopyFromSrcImage][whichColToStartCopyingFromSrcImage]) + howManyColsToCopyFromSrcImage,
             &(*this)[whichRowToWriteInto][whichColToStartWritingInto],
             &(*this)[whichRowToWriteInto][whichColToStartWritingInto] + howManyColsToCopyFromSrcImage,
             &(*this)[whichRowToWriteInto][whichColToStartWritingInto],
             false,
             true);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage6<blDataType>::copyColVector(const blImage6<blDataType>& srcImage,
                                                const int& whichColToCopyFromSrcImage,
                                                const int& whichRowToStartCopyingFromSrcImage,
                                                const int& howManyRowsToCopyFromSrcImage,
                                                const int& whichColToWriteInto,
                                                const int& whichRowToStartWritingInto)
{
    // NOTE:  This function does not
    //        check the validity of
    //        the specified indeces
    //        nor does it check to
    //        make sure this image
    //        is of the correct size

    for(int i = 0;
        i < howManyRowsToCopyFromSrcImage;
        ++i)
    {
        (*this)[i + whichRowToStartWritingInto][whichColToWriteInto] = srcImage[i + whichRowToStartCopyingFromSrcImage][whichColToCopyFromSrcImage];
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline void blImage6<blDataType>::copyImage(const blImage6<blDataType2>& srcImage,
                                            const int& whichRowToStartCopyingFrom,
                                            const int& whichColToStartCopyingFrom,
                                            const int& howManyRowsToCopy,
                                            const int& howManyColsToCopy,
                                            const int& whichRowToStartWritingInto,
                                            const int& whichColToStartWritingInto)
{
    // NOTE:  This function does not
    //        check the validity of
    //        the specified indeces
    //        nor does it check to
    //        make sure this image
    //        is of the correct size

    // We simply loop and
    // copy individual rows

    for(int i = 0; i < howManyRowsToCopy; ++i)
        copyRowVector(srcImage,
                      whichRowToStartCopyingFrom + i,
                      whichColToStartCopyingFrom,
                      howManyColsToCopy,
                      whichRowToStartWritingInto + i,
                      whichColToStartWritingInto);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blImage6<blDataType>::sampleImageWithoutInterpolating(const blImage6<blDataType>& srcImage,
                                                                  const int& whichRowToStartSamplingFrom,
                                                                  const int& whichColToStartSamplingFrom,
                                                                  const int& howManyRowsToSample,
                                                                  const int& howManyColsToSample)
{
    // Calculate the row and
    // column steps used to
    // sample the original
    // image

    double rowStep = double(howManyRowsToSample)/double(this->size1());
    double colStep = double(howManyColsToSample)/double(this->size2());

    // The indeces used to
    // index the source image

    int rowIndex = 0;
    int colIndex = 0;

    // Loop through this image
    // and assign the sampled
    // values from the source
    // image

    for(int i = 0; i < this->size1(); ++i)
    {
        // Calculate the
        // row index

        rowIndex = int(double(i)*rowStep + double(whichRowToStartSamplingFrom));

        for(int j = 0; j < this->size2(); ++j)
        {
            // Calculate the
            // column index

            colIndex = int(double(j)*colStep + double(whichColToStartSamplingFrom));

            // Assign the value

            (*this)[i][j] = srcImage[rowIndex][colIndex];
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGE6_HPP
