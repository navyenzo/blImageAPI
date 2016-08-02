#ifndef BL_CONVERSIONS_HPP
#define BL_CONVERSIONS_HPP


//-------------------------------------------------------------------
// FILE:            blImageConversions.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of functions used to
//                  simplify converting images from one
//                  form to another
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    -- Look at the individual functions
//
// NOTES:
//
// DATE CREATED:    Jan/06/2012
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
// The following functions return
// the minimum and maximum possible
// values for each possible image depth
//-------------------------------------------------------------------
inline double rangeMin(const int& depth)
{
    switch(depth)
    {
    case IPL_DEPTH_1U:
        return std::numeric_limits<bool>::min();
    case IPL_DEPTH_8U:
        return std::numeric_limits<unsigned char>::min();
    case IPL_DEPTH_8S:
        return std::numeric_limits<char>::min();
    case IPL_DEPTH_16U:
        return std::numeric_limits<unsigned short>::min();
    case IPL_DEPTH_16S:
        return std::numeric_limits<short>::min();
    case IPL_DEPTH_32S:
        return std::numeric_limits<int>::min();
    case IPL_DEPTH_32F:
        return 0;
    case IPL_DEPTH_64F:
        return 0;
    default:
        return std::numeric_limits<unsigned char>::min();
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double rangeMax(const int& depth)
{
    switch(depth)
    {
    case IPL_DEPTH_1U:
        return 1;
    case IPL_DEPTH_8U:
        return std::numeric_limits<unsigned char>::max();
    case IPL_DEPTH_8S:
        return std::numeric_limits<char>::max();
    case IPL_DEPTH_16U:
        return std::numeric_limits<unsigned short>::max();
    case IPL_DEPTH_16S:
        return std::numeric_limits<short>::max();
    case IPL_DEPTH_32S:
        return std::numeric_limits<int>::max();
    case IPL_DEPTH_32F:
        return 1;
    case IPL_DEPTH_64F:
        return 1;
    default:
        return std::numeric_limits<unsigned char>::max();
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void rangeMinMax(const int& depth,
                        double& rangeMin,
                        double& rangeMax)
{
    switch(depth)
    {
    case IPL_DEPTH_1U:
        rangeMin = std::numeric_limits<bool>::min();
        rangeMax = std::numeric_limits<bool>::max();
        break;

    case IPL_DEPTH_8U:
        rangeMin = std::numeric_limits<unsigned char>::min();
        rangeMax = std::numeric_limits<unsigned char>::max();
        break;

    case IPL_DEPTH_8S:
        rangeMin = std::numeric_limits<char>::min();
        rangeMax = std::numeric_limits<char>::max();
        break;

    case IPL_DEPTH_16U:
        rangeMin = std::numeric_limits<unsigned short>::min();
        rangeMax = std::numeric_limits<unsigned short>::max();
        break;

    case IPL_DEPTH_16S:
        rangeMin = std::numeric_limits<short>::min();
        rangeMax = std::numeric_limits<short>::max();
        break;

    case IPL_DEPTH_32S:
        rangeMin = std::numeric_limits<int>::min();
        rangeMax = std::numeric_limits<int>::max();
        break;

    case IPL_DEPTH_32F:
        rangeMin = 0;
        rangeMax = 1;
        break;

    case IPL_DEPTH_64F:
        rangeMin = 0;
        rangeMax = 1;
        break;

    default:

        // We should never end up here,
        // but just in case, we default
        // to the unsigned char values

        rangeMin = std::numeric_limits<unsigned char>::min();
        rangeMax = std::numeric_limits<unsigned char>::max();
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function returns the
// scale/shift values to convert from
// one image depth type to another
//-------------------------------------------------------------------
inline void getScaleShiftConversionParameters(const int& srcDepth,
                                              const int& dstDepth,
                                              double& scaleParameter,
                                              double& shiftParameter)
{
    // To properly scale the images
    // when changing depths, we need
    // to know the min/max values
    // (black/white) of pixels for
    // the source and destination
    // images

    double srcMin = rangeMin(srcDepth);
    double srcMax = rangeMax(srcDepth);
    double dstMin = rangeMin(dstDepth);
    double dstMax = rangeMax(dstDepth);

    // Now that we know the min/max
    // values for both images, we
    // calculate how to scale/offset
    // the source image to fit it
    // into the new depth space

    scaleParameter = (dstMax - dstMin)/(srcMax - srcMin);
    shiftParameter = dstMin - scaleParameter * srcMin;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function converts
// between two images of different
// depths, using the correct scale/shift
// values knowing that the "upper" and "lower"
// (white and black) values are different
// for different depths
//
// For example:     unsigned char -- white = 255
//                                   black = 0
//                  float         -- white = 1
//                                   black = 0
//-------------------------------------------------------------------
inline void convertScale(const IplImage* srcImage,
                         IplImage* dstImage)
{
    // Check for NULL pointers

    if(srcImage == NULL ||
       dstImage == NULL)
    {
        // Error -- Either the source
        //          or the destination
        //          or both image pointers
        //          are NULL, so we quit

        return;
    }

    // First get the scale and
    // shift parameters

    double scaleParameter = 1;
    double shiftParameter = 0;

    getScaleShiftConversionParameters(srcImage->depth,
                                      dstImage->depth,
                                      scaleParameter,
                                      shiftParameter);

    // Then finally convert the
    // image

    cvConvertScale(srcImage,dstImage,scaleParameter,shiftParameter);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions convert
// individual pixels without checking
// for pointer validity or errors, which
// are left to the programmer that uses
// the functions
//-------------------------------------------------------------------
template<typename blDataType>

inline void convertFrom2ChannelsTo1Channel(const IplImage* srcImage,
                                           const int& row,
                                           const int& col,
                                           blDataType& dstValue,
                                           const double& scaleParameter,
                                           const double& shiftParameter,
                                           const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_8U:
    default:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_8S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_16U:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_16S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_32S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_32F:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_64F:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue = blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue = std::sqrt(
                                 blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                 blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                 blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                );
        }
        else
        {
            dstValue = blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;
    }
}


template<typename blDataType>

inline void convertFrom2ChannelsTo3or4Channels(const IplImage* srcImage,
                                               const int& row,
                                               const int& col,
                                               blDataType& dstValue1,
                                               blDataType& dstValue2,
                                               blDataType& dstValue3,
                                               const double& scaleParameter,
                                               const double& shiftParameter,
                                               const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_8U:
    default:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_8S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_16U:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_16S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_32S:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_32F:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;

    case IPL_DEPTH_64F:

        if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 1)
        {
            dstValue1 = blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter );
        }
        else if(inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 == 2)
        {
            dstValue1 = std::sqrt(
                                  blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter ) +
                                  blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter ) *
                                  blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter )
                                 );
        }
        else
        {
            dstValue1 = blDataType( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter );
        }

        break;
    }

    dstValue2 = dstValue1;
    dstValue3 = dstValue1;
}


template<typename blDataType>

inline void convertFrom3or4ChannelsTo1Channel(const IplImage* srcImage,
                                              const int& row,
                                              const int& col,
                                              blDataType& dstValue,
                                              const double& scaleParameter,
                                              const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        dstValue = (0.114*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8U:
    default:

        dstValue = (0.114*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8S:

        dstValue = (0.114*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16U:

        dstValue = (0.114*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16S:

        dstValue = (0.114*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32S:

        dstValue = (0.114*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32F:

        dstValue = (0.114*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_64F:

        dstValue = (0.114*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                    0.587*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                    0.299*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                   scaleParameter + shiftParameter;

        break;
    }
}


template<typename blDataType>

inline void convertFrom3or4ChannelsTo2Channels(const IplImage* srcImage,
                                               const int& row,
                                               const int& col,
                                               blDataType& dstValue1,
                                               blDataType& dstValue2,
                                               const double& scaleParameter,
                                               const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        dstValue1 = (0.114*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_8U:
    default:

        dstValue1 = (0.114*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_8S:

        dstValue1 = (0.114*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_16U:

        dstValue1 = (0.114*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_16S:

        dstValue1 = (0.114*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_32S:

        dstValue1 = (0.114*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_32F:

        dstValue1 = (0.114*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;

    case IPL_DEPTH_64F:

        dstValue1 = (0.114*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels))) +
                     0.587*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1))) +
                     0.299*( *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)))) *
                    scaleParameter + shiftParameter;

        dstValue2 = dstValue1;

        break;
    }
}


template<typename blDataType>

inline void convertFrom3ChannelsTo4ChannelsAndViceversa(const IplImage* srcImage,
                                                        const int& row,
                                                        const int& col,
                                                        blDataType& dstValue1,
                                                        blDataType& dstValue2,
                                                        blDataType& dstValue3,
                                                        const double& scaleParameter,
                                                        const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        dstValue1 = *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8U:
    default:

        dstValue1 = *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8S:

        dstValue1 = *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16U:

        dstValue1 = *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16S:

        dstValue1 = *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32S:

        dstValue1 = *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32F:

        dstValue1 = *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_64F:

        dstValue1 = *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;
        dstValue2 = *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 1)) * scaleParameter + shiftParameter;
        dstValue3 = *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels + 2)) * scaleParameter + shiftParameter;

        break;
    }
}


template<typename blDataType>

inline void convertFrom1ChannelTo2Channels(const IplImage* srcImage,
                                           const int& row,
                                           const int& col,
                                           blDataType& dstValue1,
                                           blDataType& dstValue2,
                                           const double& scaleParameter,
                                           const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        dstValue1 = *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8U:
    default:

        dstValue1 = *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8S:

        dstValue1 = *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16U:

        dstValue1 = *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16S:

        dstValue1 = *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32S:

        dstValue1 = *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32F:

        dstValue1 = *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_64F:

        dstValue1 = *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;
    }

    dstValue2 = dstValue1;
}


template<typename blDataType>

inline void convertFrom1ChannelTo3or4Channels(const IplImage* srcImage,
                                              const int& row,
                                              const int& col,
                                              blDataType& dstValue1,
                                              blDataType& dstValue2,
                                              blDataType& dstValue3,
                                              const double& scaleParameter,
                                              const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        dstValue1 = *(reinterpret_cast<bool*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8U:
    default:

        dstValue1 = *(reinterpret_cast<unsigned char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_8S:

        dstValue1 = *(reinterpret_cast<char*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16U:

        dstValue1 = *(reinterpret_cast<unsigned short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_16S:

        dstValue1 = *(reinterpret_cast<short*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32S:

        dstValue1 = *(reinterpret_cast<int*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_32F:

        dstValue1 = *(reinterpret_cast<float*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;

    case IPL_DEPTH_64F:

        dstValue1 = *(reinterpret_cast<double*>(srcImage->imageData + srcImage->widthStep * row + col * srcImage->nChannels)) * scaleParameter + shiftParameter;

        break;
    }

    dstValue2 = dstValue1;
    dstValue3 = dstValue1;
}


inline void convertFrom2ChannelsTo1Channel(const IplImage* srcImage,
                                           const int& srcRow,
                                           const int& srcCol,
                                           IplImage* dstImage,
                                           const int& dstRow,
                                           const int& dstCol,
                                           const double& scaleParameter,
                                           const double& shiftParameter,
                                           const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    switch(dstImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_8S:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_16U:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_16S:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_32S:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_32F:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_64F:

        convertFrom2ChannelsTo1Channel(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       scaleParameter,
                                       shiftParameter,
                                       inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;
    }
}


inline void convertFrom2ChannelsTo3Channels(const IplImage* srcImage,
                                            const int& srcRow,
                                            const int& srcCol,
                                            IplImage* dstImage,
                                            const int& dstRow,
                                            const int& dstCol,
                                            const double& scaleParameter,
                                            const double& shiftParameter,
                                            const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    switch(dstImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_8S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_16U:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_16S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_32S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_32F:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;

    case IPL_DEPTH_64F:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        break;
    }
}


inline void convertFrom2ChannelsTo4Channels(const IplImage* srcImage,
                                            const int& srcRow,
                                            const int& srcCol,
                                            IplImage* dstImage,
                                            const int& dstRow,
                                            const int& dstCol,
                                            const double& scaleParameter,
                                            const double& shiftParameter,
                                            const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2,
                                            const double& alphaValue)
{
    switch(dstImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16U:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32S:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32F:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_64F:

        convertFrom2ChannelsTo3or4Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                           scaleParameter,
                                           shiftParameter,
                                           inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;
    }
}


inline void convertFrom3or4ChannelsTo1Channel(const IplImage* srcImage,
                                              const int& srcRow,
                                              const int& srcCol,
                                              IplImage* dstImage,
                                              const int& dstRow,
                                              const int& dstCol,
                                              const double& scaleParameter,
                                              const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_8S:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_16U:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_16S:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_32S:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_32F:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_64F:

        convertFrom3or4ChannelsTo1Channel(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          scaleParameter,
                                          shiftParameter);

        break;
    }
}


inline void convertFrom3or4ChannelsTo2Channels(const IplImage* srcImage,
                                               const int& srcRow,
                                               const int& srcCol,
                                               IplImage* dstImage,
                                               const int& dstRow,
                                               const int& dstCol,
                                               const double& scaleParameter,
                                               const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_8S:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_16U:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_16S:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_32S:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_32F:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;

    case IPL_DEPTH_64F:

        convertFrom3or4ChannelsTo2Channels(srcImage,
                                           srcRow,
                                           srcCol,
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                           *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                           scaleParameter,
                                           shiftParameter);

        break;
    }
}


inline void convertFrom4ChannelsTo3Channels(const IplImage* srcImage,
                                            const int& srcRow,
                                            const int& srcCol,
                                            IplImage* dstImage,
                                            const int& dstRow,
                                            const int& dstCol,
                                            const double& scaleParameter,
                                            const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_8S:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_16U:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_16S:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_32S:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_32F:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;

    case IPL_DEPTH_64F:

        convertFrom3ChannelsTo4ChannelsAndViceversa(srcImage,
                                                    srcRow,
                                                    srcCol,
                                                    *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                                    *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                                    *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                                    scaleParameter,
                                                    shiftParameter);

        break;
    }
}


inline void convertFrom3ChannelsTo4Channels(const IplImage* srcImage,
                                            const int& srcRow,
                                            const int& srcCol,
                                            IplImage* dstImage,
                                            const int& dstRow,
                                            const int& dstCol,
                                            const double& scaleParameter,
                                            const double& shiftParameter,
                                            const double& alphaValue)
{
    convertFrom4ChannelsTo3Channels(srcImage,
                                    srcRow,
                                    srcCol,
                                    dstImage,
                                    dstRow,
                                    dstCol,
                                    scaleParameter,
                                    shiftParameter);

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8U:
    default:

        *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8S:

        *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16U:

        *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16S:

        *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32S:

        *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32F:

        *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_64F:

        *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;
    }
}


inline void convertFrom1ChannelTo2Channels(const IplImage* srcImage,
                                           const int& srcRow,
                                           const int& srcCol,
                                           IplImage* dstImage,
                                           const int& dstRow,
                                           const int& dstCol,
                                           const double& scaleParameter,
                                           const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_8S:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_16U:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_16S:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_32S:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_32F:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;

    case IPL_DEPTH_64F:

        convertFrom1ChannelTo2Channels(srcImage,
                                       srcRow,
                                       srcCol,
                                       *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                       *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                       scaleParameter,
                                       shiftParameter);

        break;
    }
}


inline void convertFrom1ChannelTo3Channels(const IplImage* srcImage,
                                           const int& srcRow,
                                           const int& srcCol,
                                           IplImage* dstImage,
                                           const int& dstRow,
                                           const int& dstCol,
                                           const double& scaleParameter,
                                           const double& shiftParameter)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_8S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_16U:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_16S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_32S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_32F:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;

    case IPL_DEPTH_64F:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        break;
    }
}


inline void convertFrom1ChannelTo4Channels(const IplImage* srcImage,
                                           const int& srcRow,
                                           const int& srcCol,
                                           IplImage* dstImage,
                                           const int& dstRow,
                                           const int& dstCol,
                                           const double& scaleParameter,
                                           const double& shiftParameter,
                                           const double& alphaValue)
{
    // Depending on the depth of
    // the source image we read
    // the source value as different
    // types

    switch(srcImage->depth)
    {
    case IPL_DEPTH_1U:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<bool*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8U:
    default:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<unsigned char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_8S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<char*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16U:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<unsigned short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_16S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<short*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32S:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<int*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_32F:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<float*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;

    case IPL_DEPTH_64F:

        convertFrom1ChannelTo3or4Channels(srcImage,
                                          srcRow,
                                          srcCol,
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels)),
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 1)),
                                          *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 2)),
                                          scaleParameter,
                                          shiftParameter);

        *(reinterpret_cast<double*>(dstImage->imageData + dstImage->widthStep * dstRow + dstCol * dstImage->nChannels + 3)) = alphaValue;

        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions convert
// an IplImage to a QImage and
// viceversa
//-------------------------------------------------------------------
/*#ifdef QIMAGE_H

void toQImage(IplImage* srcImg,
              QImage* dstImg)
{
    CvRect imageROI = cvGetImageROI(srcImg);

    int channels = srcImg->nChannels;

    char *data = srcImg->imageData;

    for (int y = imageROI.y, yQ = 0;
         y < imageROI.height && yQ < dstImg->height();
         ++y, ++yQ, data += srcImg->widthStep)
    {
        for (int x = imageROI.x, xQ = 0;
             x < imageROI.width && xQ < dstImg->width();
             ++x, ++xQ)
        {
            char r, g, b, a = 0;

            if (channels == 1)
            {
                r = data[x * channels];
                g = data[x * channels];
                b = data[x * channels];
            }
            else if (channels == 3 || channels == 4)
            {
                r = data[x * channels + 2];
                g = data[x * channels + 1];
                b = data[x * channels];
            }

            if (channels == 4)
            {
                a = data[x * channels + 3];
                dstImg->setPixel(xQ,yQ,qRgba(r,g,b,a));
            }
            else
            {
                dstImg->setPixel(xQ,yQ,qRgb(r,g,b));
            }
        }
    }
}


QImage* toQImage(IplImage* srcImg)
{
    CvRect imageROI = cvGetImageROI(srcImg);

    QImage* dstImg = new QImage(imageROI.width,imageROI.height,QImage::Format_ARGB32);

    toQImage(srcImg,dstImg);

    return dstImg;
}

#endif // QImage/IplImage conversions
//-------------------------------------------------------------------*/


#endif // BL_CONVERSIONS_HPP
