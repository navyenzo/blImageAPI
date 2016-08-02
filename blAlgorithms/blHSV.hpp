#ifndef BL_HSV_HPP
#define BL_HSV_HPP


//-------------------------------------------------------------------
// FILE:            blHSV.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions I created to
//                  convert images from/to the HSV color space
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - blColor3 -- To represent color
//                  - blImage -- The image structure
//                  - cvCvtColor -- Used to convert unsigned char
//                                  images from BGR to HSV and
//                                  viceversa.  The cvCvtColor
//                                  function is a little faster
//                                  for unsigned char images
//
// NOTES:
// DATE CREATED:    Nov/02/2010
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
// FUNCTION:            FromBGRtoHSVColor
// ARGUMENTS:           srcColor
//                      dstColor
//                      SrcRangeMin
//                      SrcRange
//                      srcImageDepth
//                      DstRangeMin
//                      DstRange
//                      dstImageDept
// TEMPLATE ARGUMENTS:  None
// PURPOSE:             Convert a color from the BGR space to
//                      the HSV space
// DEPENDENCIES:        blColor3
//-------------------------------------------------------------------
inline void FromBGRtoHSVColor(blColor3<double> srcColor,
                              blColor3<double>& dstColor,
                              const double& SrcRangeMin,
                              const double& SrcRange,
                              const int& srcImageDepth,
                              const double& DstRangeMin,
                              const double& DstRange,
                              const int& dstImageDepth)
{
    // Map the color to [0,1]

    srcColor.m_blue += SrcRangeMin;
    srcColor.m_green += SrcRangeMin;
    srcColor.m_red += SrcRangeMin;
    srcColor /= SrcRange;

    // Calculate the V value

    double V = std::max(std::max(srcColor.m_blue,srcColor.m_green),srcColor.m_red);

    // Calculate the min value

    double Min = std::min(std::min(srcColor.m_blue,srcColor.m_green),srcColor.m_red);

    // Calculate the Chroma Value

    double Chroma = V - Min;

    // Calculate the Saturation value

    double Sat = 0;
    if(Chroma != 0)
        Sat = Chroma/V;

    // Calculate the Hue Value

    double Hue = 0;
    if(Chroma != 0)
    {
        if(V == srcColor.m_red)
        {
            Hue = 60.0*(srcColor.m_green - srcColor.m_blue)/Chroma;

            // In this case Hue could
            // be negative, so we
            // add 360 to it

            if(Hue < 0)
                Hue += 360.0;
        }

        else if(V == srcColor.m_green)
            Hue = 120.0 + 60.0*(srcColor.m_blue - srcColor.m_red)/Chroma;

        else
            Hue = 240 + 60.0*(srcColor.m_red - srcColor.m_green)/Chroma;
    }

    // Now that we know the HSV values we
    // save them into the destination image
    // being careful about the depth of the
    // destination image

    // H is stored as Blue
    // S is stored as Green
    // V is stored as Red

    // The H value will be stored differently
    // depending on the depth of the image

    // We do not account for unsigned char nor
    // for float as those will be taken care
    // of in template specializations

    switch(dstImageDepth)
    {
    case IPL_DEPTH_8U:
        Hue /= 2.0;
        break;

    case IPL_DEPTH_8S:
        Hue -= Hue/2.0;
        Hue /= 2.0;
        break;

    case IPL_DEPTH_16S:
        Hue -= Hue/2.0;
        break;

    case IPL_DEPTH_32S:
        Hue -= Hue/2.0;
        break;
    }

    // We have to modify S and V due to depth

    Sat = Sat*DstRange + DstRangeMin;
    V = V*DstRange + DstRangeMin;

    // Now we create the HSV color

    dstColor.m_blue = Hue;
    dstColor.m_green = Sat;
    dstColor.m_red = V;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromHSVtoBGRColor
// ARGUMENTS:           srcColor
//                      dstColor
//                      SrcRangeMin
//                      SrcRange
//                      srcImageDepth
//                      DstRangeMin
//                      DstRange
//                      dstImageDept
// TEMPLATE ARGUMENTS:  None
// PURPOSE:             Convert a color from the HSV space to
//                      the BGR space
// DEPENDENCIES:        blColor3
//-------------------------------------------------------------------
inline void FromHSVtoBGRColor(blColor3<double> srcColor,
                              blColor3<double>& dstColor,
                              const double& SrcRangeMin,
                              const double& SrcRange,
                              const int& srcImageDepth,
                              const double& DstRangeMin,
                              const double& DstRange,
                              const int& dstImageDepth)
{
    // The H S V values are stored in
    // srcColor as Blue,Green and Red
    // respectively

    double Hue = srcColor.m_blue;
    double Sat = srcColor.m_green;
    double V = srcColor.m_red;

    // Remap S and V back to [0,1]

    Sat -= SrcRangeMin;
    Sat /= SrcRange;
    V -= SrcRangeMin;
    V /= SrcRange;

    // Remap Hue back to [0,6]

    switch(srcImageDepth)
    {
    case IPL_DEPTH_8U:
        Hue *= 2.0;
        break;

    case IPL_DEPTH_8S:
        Hue *= 2.0;
        Hue += Hue/2.0;
        break;

    case IPL_DEPTH_16S:
        Hue += Hue/2.0;
        break;

    case IPL_DEPTH_32S:
        Hue += Hue/2.0;
        break;
    }

    Hue /= 60.0;

    // Calculate the integral
    // and fractional part of Hue

    int Integral = int(Hue);

    double Fractional = Hue - Integral;

    // Calculate the Colors
    double p = V*(1.0 - Sat);
    double q = V*(1.0 - Sat*Fractional);
    double t = V*(1.0 - (Sat*(1.0 - Fractional)));

    // Depending on the integral
    // part of the Hue, we assign
    // the colors differently

    switch(Integral)
    {
    case 0:
        dstColor.m_blue = p;
        dstColor.m_green = t;
        dstColor.m_red = V;
        break;

    case 1:
        dstColor.m_blue = p;
        dstColor.m_green = V;
        dstColor.m_red = q;
        break;

    case 2:
        dstColor.m_blue = t;
        dstColor.m_green = V;
        dstColor.m_red = p;
        break;

    case 3:
        dstColor.m_blue = V;
        dstColor.m_green = q;
        dstColor.m_red = p;
        break;

    case 4:
        dstColor.m_blue = V;
        dstColor.m_green = p;
        dstColor.m_red = t;
        break;

    default:
        dstColor.m_blue = q;
        dstColor.m_green = p;
        dstColor.m_red = V;
        break;
    }

    // We have to remap the color
    // to its correct depth

    dstColor.m_blue = dstColor.m_blue*DstRange + DstRangeMin;
    dstColor.m_green = dstColor.m_green*DstRange + DstRangeMin;
    dstColor.m_red = dstColor.m_red*DstRange + DstRangeMin;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromBGRtoHSV
// ARGUMENTS:           srcImage,dstImage
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Convert a generic three-channel BGR color
//                      image into a three-channel HSV color image
// DEPENDENCIES:        blColor3
//                      blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void FromBGRtoHSV(const blImage< blColor3<blType> >& srcImage,
                         blImage< blColor3<blType2> >& dstImage)
{
    // Get the size of the source image
    int rows = srcImage.size1();
    int cols = srcImage.size2();

    // Let's make sure that the destination image
    // is the correct size
    if(dstImage.size1() != rows || dstImage.size2() != cols)
        dstImage.Create(rows,cols);

    // Get the min and max possible
    // values for the source image
    double SrcRangeMin = srcImage.GetRangeMin();
    double SrcRangeMax = srcImage.GetRangeMax();
    double SrcRange = SrcRangeMax - SrcRangeMin;
    int srcImageDepth = srcImage.GetDepth();

    // Get the min and max possible
    // values for the destination image
    double DstRangeMin = dstImage.GetRangeMin();
    double DstRangeMax = dstImage.GetRangeMax();
    double DstRange = DstRangeMax - SrcRangeMin;
    int dstImageDepth = dstImage.GetDepth();

    // Temporary color variable
    blColor3<double> srcColor;
    blColor3<double> dstColor;
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            srcColor = srcImage[i][j];
            FromBGRtoHSVColor(srcColor,
                              dstColor,
                              SrcRangeMin,
                              SrcRange,
                              srcImageDepth,
                              DstRangeMin,
                              DstRange,
                              dstImageDepth);
            dstImage[i][j] = dstColor;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specialization for unsigned char images
// -- Since cvCvtColor is faster for unsigned char images
//-------------------------------------------------------------------
template<>
inline void FromBGRtoHSV(const blImage< blColor3<unsigned char> >& srcImage,
                         blImage< blColor3<unsigned char> >& dstImage)
{
    // Get the size of the
    // source image

    int rows = srcImage.size1();
    int cols = srcImage.size2();

    // Let's make sure that
    // the destination image
    // is the correct size

    if(dstImage.size1() != rows || dstImage.size2() != cols)
        dstImage.create(rows,cols);

    // Now that we made sure
    // that the images are of
    // the same size, we just use
    // the cvCvtColor function

    cvCvtColor(srcImage,dstImage,CV_BGR2HSV);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FromHSVtoBGR
// ARGUMENTS:           srcImage,dstImage
// TEMPLATE ARGUMENTS:  blType,blType2
// PURPOSE:             Convert a generic three-channel HSV color
//                      image back to a three-channel BGR color image
// DEPENDENCIES:        blColor3
//                      blImage
//-------------------------------------------------------------------
template<typename blType,typename blType2>
inline void fromHSVtoBGR(const blImage< blColor3<blType> >& srcImage,
                         blImage< blColor3<blType2> >& dstImage)
{
    // Get the size of the
    // source image

    int rows = srcImage.size1();
    int cols = srcImage.size2();

    // Let's make sure that
    // the destination image
    // is the correct size

    if(dstImage.size1() != rows || dstImage.size2() != cols)
        dstImage.Create(rows,cols);

    // Get the min and max possible
    // values for the source image

    double SrcRangeMin = srcImage.GetRangeMin();
    double SrcRangeMax = srcImage.GetRangeMax();
    double SrcRange = SrcRangeMax - SrcRangeMin;
    int srcImageDepth = srcImage.GetDepth();

    // Get the min and max possible
    // values for the destination image

    double DstRangeMin = dstImage.GetRangeMin();
    double DstRangeMax = dstImage.GetRangeMax();
    double DstRange = DstRangeMax - SrcRangeMin;
    int dstImageDepth = dstImage.GetDepth();

    // Temporary color variable

    blColor3<double> srcColor;
    blColor3<double> dstColor;

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            srcColor = srcImage[i][j];
            FromHSVtoBGRColor(srcColor,
                              dstColor,
                              SrcRangeMin,
                              SrcRange,
                              srcImageDepth,
                              DstRangeMin,
                              DstRange,
                              dstImageDepth);
            dstImage[i][j] = dstColor;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Template specialization for unsigned char images
// -- Since cvCvtColor is faster for unsigned char images
//-------------------------------------------------------------------
template<>
inline void fromHSVtoBGR(const blImage< blColor3<unsigned char> >& srcImage,
                         blImage< blColor3<unsigned char> >& dstImage)
{
    // Get the size of the
    // source image

    int rows = srcImage.size1();
    int cols = srcImage.size2();

    // Let's make sure that
    // the destination image
    // is the correct size

    if(dstImage.size1() != rows || dstImage.size2() != cols)
        dstImage.create(rows,cols);

    // Now that we made sure
    // that the images
    // are of the same
    // size, we just use
    // the cvCvtColor function

    cvCvtColor(srcImage,dstImage,CV_HSV2BGR);
}
//-------------------------------------------------------------------


#endif // BL_HSV_HPP
