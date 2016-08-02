#ifndef BL_BLENDING_HPP
#define BL_BLENDING_HPP


//-------------------------------------------------------------------
// FILE:            blBlending.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of algorithms I wrote to blend
//                  two images that emulate adobe photoshop's
//                  blending modes
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:
//
// NOTES:           - So far the following methods are available:
//                      - BlendMultiply
//                      - BlendScreen
//                      - BlendDarken
//                      - BlendLighten
//                      - BlendDifference
//                      - BlendLinearDodge
//                      - BlendOverlay
//                      - BlendHardLight
//                      - BlendLinearBurn
//
// DATE CREATED:    - Nov/30/2010
// DATE UPDATED:    - Aug/09/2013 -- Created one function that gets
//                                   passed a "blending mode" functor
//                                   to blend the image pixels
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blBlendPixel
//
// ARGUMENTS:           - BottomImagePixel
//                      - TopImagePixel
//                      - DstImagePixel
//                      - MinValue
//                      - Range
//                      - BlendingFunctor
//
// TEMPLATE ARGUMENTS:  - blBlendingFunctorType
//                      - blDataType
//
// PURPOSE:             Function used to blend two pixels
//                      using a specified functor
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blBlendingFunctorType,typename blDataType>
class blBlendPixel
{
public:
    void operator()(const blDataType& BottomImagePixel,
                    const blDataType& TopImagePixel,
                    blDataType& DstImagePixel,
                    const double& MinValue,
                    const double& Range,
                    const blBlendingFunctorType& BlendingFunctor)const
    {
        BlendingFunctor(BottomImagePixel,TopImagePixel,DstImagePixel,MinValue,Range);
    }
};

template<typename blBlendingFunctorType,typename blDataType>
class blBlendPixel< blBlendingFunctorType,std::complex<blDataType> >
{
public:
    void operator()(const std::complex<blDataType>& BottomImagePixel,
                    const std::complex<blDataType>& TopImagePixel,
                    std::complex<blDataType>& DstImagePixel,
                    const double& MinValue,
                    const double& Range,
                    const blBlendingFunctorType& BlendingFunctor)const
    {
        BlendingFunctor(BottomImagePixel.real(),TopImagePixel.real(),DstImagePixel.real(),MinValue,Range);
        BlendingFunctor(BottomImagePixel.imag(),TopImagePixel.imag(),DstImagePixel.imag(),MinValue,Range);
    }
};

template<typename blBlendingFunctorType>
class blBlendPixel< blBlendingFunctorType,CvPoint>
{
public:
    void operator()(const CvPoint& BottomImagePixel,
                    const CvPoint& TopImagePixel,
                    CvPoint& DstImagePixel,
                    const double& MinValue,
                    const double& Range,
                    const blBlendingFunctorType& BlendingFunctor)const
    {
        BlendingFunctor(BottomImagePixel.x,TopImagePixel.x,DstImagePixel.x,MinValue,Range);
        BlendingFunctor(BottomImagePixel.y,TopImagePixel.y,DstImagePixel.y,MinValue,Range);
    }
};

template<typename blBlendingFunctorType,typename blDataType>
class blBlendPixel< blBlendingFunctorType,blColor3<blDataType> >
{
public:
    void operator()(const blColor3<blDataType>& BottomImagePixel,
                    const blColor3<blDataType>& TopImagePixel,
                    blColor3<blDataType>& DstImagePixel,
                    const double& MinValue,
                    const double& Range,
                    const blBlendingFunctorType& BlendingFunctor)const
    {
        BlendingFunctor(BottomImagePixel.m_Blue,TopImagePixel.m_Blue,DstImagePixel.m_Blue,MinValue,Range);
        BlendingFunctor(BottomImagePixel.m_Green,TopImagePixel.m_Green,DstImagePixel.m_Green,MinValue,Range);
        BlendingFunctor(BottomImagePixel.m_Red,TopImagePixel.m_Red,DstImagePixel.m_Red,MinValue,Range);
    }
};

template<typename blBlendingFunctorType,typename blDataType>
class blBlendPixel< blBlendingFunctorType,blColor4<blDataType> >
{
public:
    void operator()(const blColor4<blDataType>& BottomImagePixel,
                    const blColor4<blDataType>& TopImagePixel,
                    blColor4<blDataType>& DstImagePixel,
                    const double& MinValue,
                    const double& Range,
                    const blBlendingFunctorType& BlendingFunctor)const
    {
        BlendingFunctor(BottomImagePixel.m_Blue,TopImagePixel.m_Blue,DstImagePixel.m_Blue,MinValue,Range);
        BlendingFunctor(BottomImagePixel.m_Green,TopImagePixel.m_Green,DstImagePixel.m_Green,MinValue,Range);
        BlendingFunctor(BottomImagePixel.m_Red,TopImagePixel.m_Red,DstImagePixel.m_Red,MinValue,Range);
        BlendingFunctor(BottomImagePixel.m_Alpha,TopImagePixel.m_Alpha,DstImagePixel.m_Alpha,MinValue,Range);
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blBlendIgnoringROIs
//
// ARGUMENTS:           - BottomImage
//                      - TopImage
//                      - DstImage
//                      - BlendingFunctor
//
// TEMPLATE ARGUMENTS:  - blDataType
//                      - blBlendingFunctorType
//                      - blCalculationType
//
// PURPOSE:             This function blends the top image with the
//                      bottom image into a destination image using
//                      a user specified "blending functor".
//                      The function assumes all images are aligned
//                      at their top-left corner, and only parses
//                      as many pixels as the smallest image size
//
// DEPENDENCIES:        - blImage
//                      - std::min
//
// NOTES:               - In case the images are of different
//                        sizes, the function only parses the
//                        smallest size.
//                      - The function takes into account the
//                        current ROIs of the three images
//                      - The function assumes that all images
//                        are aligned at the top-left corner
//                      - The function does ignores the ROIs
//                      - The user specifies the type of
//                        variable used to do the calculations,
//                        for example:  float or double or whatever.
//-------------------------------------------------------------------
template<typename blDataType,typename blBlendingFunctorType>
inline void blBlendIgnoringROIs(const blImage<blDataType>& BottomImage,
                                const blImage<blDataType>& TopImage,
                                blImage<blDataType>& DstImage,
                                const blBlendingFunctorType& BlendingFunctor)
{
    // Get the min and max values
    // representable by the image
    // type
    double MinValue = rangeMin(DstImage.GetDepth());
    double MaxValue = rangeMax(DstImage.GetDepth());
    double Range = MaxValue - MinValue;

    // Calculate the number of
    // rows and columns to parse
    int Rows = std::min(DstImage.size1(),std::min(BottomImage.size1(),TopImage.size1()));
    int Cols = std::min(DstImage.size2(),std::min(BottomImage.size2(),TopImage.size2()));

    // Create the blending
    // pixel functor
    blBlendPixel<blBlendingFunctorType,blDataType> MyBlendingFunctor;

    // Step through the destination
    // image and calculate the
    // corresponsind pixels
    for(int i = 0; i < Rows; ++i)
    {
        for(int j = 0; j < Cols; ++j)
        {
            MyBlendingFunctor(BottomImage[i][j],
                              TopImage[i][j],
                              DstImage[i][j],
                              MinValue,
                              Range,
                              BlendingFunctor);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blBlend
//
// ARGUMENTS:           - BottomImage
//                      - TopImage
//                      - DstImage
//                      - BlendingFunctor
//                      - BottomImageOffsetToDstImage
//                      - TopImageOffsetToDstImage
//
// TEMPLATE ARGUMENTS:  - blDataType
//                      - blBlendingFunctorType
//                      - blCalculationType
//
// PURPOSE:             This function blends the top image with the
//                      bottom image using a "blending functor"
//                      specified by the programmer.
//                      The programmer specifies the image offsets
//                      between the top image and the destination
//                      image, and between the bottom image and
//                      the destination image.
//
// DEPENDENCIES:        - blImage
//                      - std::min
//                      - std::max
//
// NOTES:               - In case the images are of different
//                        sizes, the function only parses the
//                        smallest size.
//                      - The function takes into account the
//                        current ROIs of the three images
//                      - The top-left corner of the destination
//                        image is the (0,0) point, and the offsets
//                        of the top and bottom images are expressed
//                        relative to it.
//                      - The user specifies the type of
//                        variable used to do the calculations,
//                        for example:  float or double or whatever.
//-------------------------------------------------------------------
template<typename blDataType,typename blBlendingFunctorType>
inline void blBlend(const blImage<blDataType>& BottomImage,
                    const blImage<blDataType>& TopImage,
                    blImage<blDataType>& DstImage,
                    const blBlendingFunctorType& BlendingFunctor,
                    const CvPoint& BottomImageOffsetToDstImage = cvPoint(0,0),
                    const CvPoint& TopImageOffsetToDstImage = cvPoint(0,0))
{
    // Get the min and max values
    // representable by the image
    // type
    double MinValue = rangeMin(DstImage.GetDepth());
    double MaxValue = rangeMin(DstImage.GetDepth());
    double Range = MaxValue - MinValue;

    // Find the ROI of the
    // destination image
    CvRect DstROI = DstImage.GetROI();

    // Step through the destination
    // image ROI and calculate the
    // corresponding pixels
    for(int i = DstROI.y; i < DstROI.height; ++i)
    {
        for(int j = DstROI.x; j < DstROI.width; ++j)
        {
            if(BottomImage.DoesIndexPointToPixelInImageROI(i + BottomImageOffsetToDstImage.y,j + BottomImageOffsetToDstImage.x) &&
               TopImage.DoesIndexPointToPixelInImageROI(i + TopImageOffsetToDstImage.y,j + TopImageOffsetToDstImage.x))
            {
                // In this case, the current
                // index is pointing at pixels
                // from both the top and bottom
                // images, therefore we calculate
                // the blended pixel
                BlendingFunctor(BottomImage[i + BottomImageOffsetToDstImage.y][j + BottomImageOffsetToDstImage.x],
                                TopImage[i + TopImageOffsetToDstImage.y][j + TopImageOffsetToDstImage.x],
                                DstImage[i][j],
                                MinValue,
                                Range);
            }
            else if(BottomImage.DoesIndexPointToPixelInImageROI(i + BottomImageOffsetToDstImage.y,j + BottomImageOffsetToDstImage.x))
            {
                // In this case, only the
                // bottom image is being indexed
                // therefore, we simply assign
                // its pixel to the destination
                // image
                DstImage[i][j] = BottomImage[i + BottomImageOffsetToDstImage.y][j + BottomImageOffsetToDstImage.x];
            }
            else if(BottomImage.DoesIndexPointToPixelInImageROI(i + BottomImageOffsetToDstImage.y,j + BottomImageOffsetToDstImage.x))
            {
                // In this case, only the
                // top image is being indexed
                // therefore, we simply assign
                // its pixel to the destination
                // image
                DstImage[i][j] = TopImage[i + TopImageOffsetToDstImage.y][j + TopImageOffsetToDstImage.x];
            }

            // If none of the above cases is
            // true, we simply leave the
            // destination image pixel as is
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTIONS:           Blending functors used to blend
//                      two pixels into a third one
//-------------------------------------------------------------------

//----------------------------------
// Multiply blend
//----------------------------------
template<typename blDataType>
class blBlendMultiplyFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = ((double(BottomImagePixel) - MinValue) * (double(TopImagePixel) - MinValue) / Range) + MinValue;
    }
};

//----------------------------------
// Screen blend
//----------------------------------
template<typename blDataType>
class blBlendScreenFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = Range - (Range - (double(BottomImagePixel) - MinValue)) * (Range - (double(TopImagePixel) - MinValue)) / Range + MinValue;
    }
};

//----------------------------------
// Overlay blend
//----------------------------------
template<typename blDataType>
class blBlendOverlayFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        if(double(BottomImagePixel) < ((Range/2.0) + MinValue))
            DstImagePixel = 2.0*((double(BottomImagePixel) - MinValue) * (double(TopImagePixel) - MinValue) / Range) + MinValue;
        else
            DstImagePixel = Range - 2.0*(Range - (double(BottomImagePixel) - MinValue)) * (Range - (double(TopImagePixel) - MinValue)) / Range + MinValue;
    }
};

//----------------------------------
// Hard light blend
//----------------------------------
template<typename blDataType>
class blBlendHardLightFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        if(double(TopImagePixel) < ((Range/2.0) + MinValue))
            DstImagePixel = 2.0*((double(TopImagePixel) - MinValue) * (double(BottomImagePixel) - MinValue) / Range) + MinValue;
        else
            DstImagePixel = Range - 2.0*(Range - (double(TopImagePixel) - MinValue)) * (Range - (double(BottomImagePixel) - MinValue)) / Range + MinValue;
    }
};

//----------------------------------
// Photoshop's Soft light blend
//----------------------------------
template<typename blDataType>
class blBlendPhotoshopSoftLightFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        if(double(TopImagePixel) < ((Range/2.0) + MinValue))
            DstImagePixel = 2.0*((double(BottomImagePixel) - MinValue) * (double(TopImagePixel) - MinValue) / Range) + MinValue +
                    ((double(BottomImagePixel) - MinValue) * (double(BottomImagePixel) - MinValue) / Range)*(Range - 2.0*(double(TopImagePixel) - MinValue)) + MinValue;
        else
            DstImagePixel = 2.0*((double(BottomImagePixel) - MinValue) * (Range - (double(TopImagePixel) - MinValue)))/Range + MinValue +
                    sqrt((double(BottomImagePixel) - MinValue)/Range)*Range * (2.0*(double(TopImagePixel) - MinValue) - Range) + MinValue;
    }
};

//----------------------------------
// Difference blend
//----------------------------------
template<typename blDataType>
class blBlendDifferenceFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        if(TopImagePixel > BottomImagePixel)
            DstImagePixel = TopImagePixel - BottomImagePixel + MinValue;
        else
            DstImagePixel = BottomImagePixel - TopImagePixel + MinValue;
    }
};

//----------------------------------
// Addition blend
//----------------------------------
template<typename blDataType>
class blBlendAdditionFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = std::min(MinValue + Range,TopImagePixel + BottomImagePixel);
    }
};

//----------------------------------
// Divide blend
//----------------------------------
template<typename blDataType>
class blBlendDivideFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = (double(BottomImagePixel) - MinValue) / (double(TopImagePixel) - MinValue) + MinValue;

        // Check for infinity or nan
        if((DstImagePixel <= std::numeric_limits<blDataType>::min() && DstImagePixel >= std::numeric_limits<blDataType>::max()) ||
           (DstImagePixel != DstImagePixel))
        {
            // In this case the result
            // is infinite or nan, thus we
            // set it to its maximum value
            DstImagePixel = MinValue + Range;
        }
    }
};

//----------------------------------
// Darken only blend
//----------------------------------
template<typename blDataType>
class blBlendDarkenOnlyFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = std::min(TopImagePixel,BottomImagePixel);
    }
};

//----------------------------------
// Lighten only blend
//----------------------------------
template<typename blDataType>
class blBlendLightenOnlyFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = std::max(TopImagePixel,BottomImagePixel);
    }
};

//----------------------------------
// Linear dodge blend
//----------------------------------
template<typename blDataType>
class blBlendLinearDodgeFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = std::min(MinValue + Range,double(BottomImagePixel) + double(TopImagePixel));
    }
};

//----------------------------------
// Color dodge blend
//----------------------------------
template<typename blDataType>
class blBlendColorDodgeFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = (double(BottomImagePixel) - MinValue) / (Range - double(TopImagePixel) - MinValue) + MinValue;

        // Check for infinity or nan
        if((DstImagePixel <= std::numeric_limits<blDataType>::min() && DstImagePixel >= std::numeric_limits<blDataType>::max()) ||
           (DstImagePixel != DstImagePixel))
        {
            // In this case the result
            // is infinite or nan, thus we
            // set it to its maximum value
            DstImagePixel = MinValue + Range;
        }
    }
};

//----------------------------------
// Color burn blend
//----------------------------------
template<typename blDataType>
class blBlendColorBurnFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = (Range - double(BottomImagePixel) - MinValue) / (double(TopImagePixel) - MinValue) + MinValue;

        // Check for infinity or nan
        if((DstImagePixel <= std::numeric_limits<blDataType>::min() && DstImagePixel >= std::numeric_limits<blDataType>::max()) ||
           (DstImagePixel != DstImagePixel))
        {
            // In this case the result
            // is infinite or nan, thus we
            // set it to its maximum value
            DstImagePixel = MinValue + Range;
        }
    }
};

//----------------------------------
// Linear burn blend
//----------------------------------
template<typename blDataType>
class blBlendLinearBurnFunctor
{
public:
    void operator()(const blDataType& BottomImagePixel,const blDataType& TopImagePixel,blDataType& DstImagePixel,const double& MinValue,const double& Range)const
    {
        DstImagePixel = std::max(MinValue,double(BottomImagePixel) + double(TopImagePixel) - (MinValue + Range));
    }
};
//-------------------------------------------------------------------


#endif // BL_BLENDING_HPP
