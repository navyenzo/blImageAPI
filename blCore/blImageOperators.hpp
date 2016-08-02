#ifndef BL_IMAGEOPERATORS_HPP
#define BL_IMAGEOPERATORS_HPP


//-------------------------------------------------------------------
// FILE:            blImageOperators.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of overloaded operators and functions
//                  I wrote to handle images just like matrices, so
//                  as to make code easier to read
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage and its dependencies
//
// NOTES:
//
// DATE CREATED:    Jun/03/2010
//
// DATE UPDATED:    Jan/13/2015 -- Deleted unnecessary operators and
//                                 made the functions more efficient
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
// The following function converts
// a blImage to a cv::Mat structure
//-------------------------------------------------------------------
template<typename blDataType>

inline cv::Mat blImageToMat(const blImage<blDataType>& img,
                            const bool& copyData = false,
                            const bool& allowND = true,
                            const int& coiMode = 0)
{
    return ( cvarrToMat(img.getImagePtr(),copyData,allowND,coiMode) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following are overloaded stream operators to
// easily output our structures of interest to/from
// an output/input stream
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os,const CvPoint& point)
{
    os << point.x << " " << point.y;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline std::istream& operator>>(std::istream& is,CvPoint& point)
{
    if(!(is >> point.x))
    {
        return is;
    }

    if(!(is >> point.y))
    {
        return is;
    }

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os,const CvRect& rect)
{
    os << rect.x << " " << rect.y << " " << rect.width << " " << rect.height;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline std::istream& operator>>(std::istream& is,CvRect& rect)
{
    if(!(is >> rect.x))
    {
        return is;
    }

    if(!(is >> rect.y))
    {
        return is;
    }

    if(!(is >> rect.width))
    {
        return is;
    }

    if(!(is >> rect.height))
    {
        return is;
    }

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::ostream& operator<<(std::ostream& os,const blImage<blDataType>& img)
{
    // We output the image
    // using a simple format:

    // Rows Cols
    // COI (Channel Of Interest)
    // yROI xROI heightROI widthROI
    // Data

    int rows = img.size1();
    int cols = img.size2();

    os << rows << " " << cols << "\n";
    os << img.getCOI() << "\n";
    os << img.yROI() << " " << img.xROI() << " " << img.size1ROI() << " " << img.size2ROI() << "\n";

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            os << img(i,j) << " ";
        }

        os << "\n";
    }

    os << "\n";

    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::istream& operator>>(std::istream& is,blImage<blDataType>& img)
{
    // Here we assume we're at the
    // point where we can simply read
    // the image following our super simple
    // format

    int rows,cols;
    int COI;
    CvRect ROI;

    if(!(is >> rows))
    {
        // Error -- Could not read the
        //          number of rows

        return is;
    }

    if(!(is >> cols))
    {
        // Error -- Could not read the
        //          number of columns

        return is;
    }

    if(!(is >> COI))
    {
        // Error -- Could not read the
        //          Channel of Interest

        return is;
    }

    if(!(is >> ROI))
    {
        // Error -- Could not read the
        //          Region of Interest

        return is;
    }

    // Here we size img to
    // make sure that its
    // size matches the
    // input image

    if(img.size1() != rows || img.size2() != cols)
        img.Create(rows,cols);

    // We also set its
    // COI and ROI to
    // match the input
    // image

    img.setCOI(COI);
    img.setROI(ROI);

    // Now we read the data points

    for(auto pixelIter = img.begin(); pixelIter != img.end(); ++pixelIter)
    {
        if(!(is >> (*pixelIter)))
        {
            // Error == Could not read
            //          data point

            return is;
        }
    }

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions are used to
// define what we mean by minimum and
// maximum when dealing with any type of
// image, including images of multiple
// channels
//-------------------------------------------------------------------
template<typename blDataType>
inline void min(const blDataType& value1,
                const blDataType& value2,
                blDataType& minValue)
{
    minValue = std::min(value1,value2);
}


template<typename blDataType>
inline void min(const std::complex< blDataType >& value1,
                const std::complex< blDataType >& value2,
                std::complex< blDataType >& minValue)
{
    minValue.real(std::min(value1.real(),value2.real()));
    minValue.imag(std::min(value1.imag(),value2.imag()));
}


inline void min(const CvPoint& value1,
                const CvPoint& value2,
                CvPoint& minValue)
{
    minValue.x = std::min(value1.x,value2.x);
    minValue.y = std::min(value1.y,value2.y);
}


inline void min(const CvPoint2D32f& value1,
                const CvPoint2D32f& value2,
                CvPoint2D32f& minValue)
{
    minValue.x = std::min(value1.x,value2.x);
    minValue.y = std::min(value1.y,value2.y);
}


inline void min(const CvPoint2D64f& value1,
                const CvPoint2D64f& value2,
                CvPoint2D64f& minValue)
{
    minValue.x = std::min(value1.x,value2.x);
    minValue.y = std::min(value1.y,value2.y);
}


inline void min(const CvPoint3D32f& value1,
                const CvPoint3D32f& value2,
                CvPoint3D32f& minValue)
{
    minValue.x = std::min(value1.x,value2.x);
    minValue.y = std::min(value1.y,value2.y);
    minValue.z = std::min(value1.z,value2.z);
}


inline void min(const CvPoint3D64f& value1,
                const CvPoint3D64f& value2,
                CvPoint3D64f& minValue)
{
    minValue.x = std::min(value1.x,value2.x);
    minValue.y = std::min(value1.y,value2.y);
    minValue.z = std::min(value1.z,value2.z);
}


template<typename blDataType>
inline void min(const blImage<blDataType>& img,
                blDataType& minValue)
{
    if(img.sizeROI() <= 0)
        return;

    int rows = img.size1ROI();
    int cols = img.size2ROI();

    int yROI = img.yROI();
    int xROI = img.xROI();

    minValue = img(yROI,xROI);

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            min(img(i,j),minValue,minValue);
        }
    }
}


template<typename blDataType>
inline void min(const blImage<blDataType>& img1,
                const blImage<blDataType>& img2,
                blImage<blDataType>& minImg)
{
    int rows1 = img1.size1ROI();
    int cols1 = img1.size2ROI();
    int yROI1 = img1.yROI();
    int xROI1 = img1.xROI();

    int rows2 = img2.size1ROI();
    int cols2 = img2.size2ROI();
    int yROI2 = img2.yROI();
    int xROI2 = img2.xROI();

    int rowsMin = minImg.size1ROI();
    int colsMin = minImg.size2ROI();
    int yROIMin = minImg.yROI();
    int xROIMin = minImg.xROI();

    for(int i1 = yROI1, i2 = yROI2, iMin = yROIMin;
        i1 < (rows1 +yROI1) && i2 < (rows2 + yROI2) && iMin < (rowsMin + yROIMin);
        ++i1, ++i2, ++iMin)
    {
        for(int j1 = xROI1, j2 = xROI2, jMin = xROIMin;
            j1 < (cols1 + xROI1) && j2 < (cols2 + xROI2) && jMin < (colsMin + xROIMin);
            ++j1, ++j2, ++jMin)
        {
            min(img1(i1,j1),img2(i2,j2),minImg(iMin,jMin));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void max(const blDataType& value1,
                const blDataType& value2,
                blDataType& maxValue)
{
    maxValue = std::max(value1,value2);
}


template<typename blDataType>
inline void max(const std::complex< blDataType >& value1,
                const std::complex< blDataType >& value2,
                std::complex< blDataType >& maxValue)
{
    maxValue.real(std::max(value1.real(),value2.real()));
    maxValue.imag(std::max(value1.imag(),value2.imag()));
}


inline void max(const CvPoint& value1,
                const CvPoint& value2,
                CvPoint& maxValue)
{
    maxValue.x = std::max(value1.x,value2.x);
    maxValue.y = std::max(value1.y,value2.y);
}


inline void max(const CvPoint2D32f& value1,
                const CvPoint2D32f& value2,
                CvPoint2D32f& maxValue)
{
    maxValue.x = std::max(value1.x,value2.x);
    maxValue.y = std::max(value1.y,value2.y);
}


inline void max(const CvPoint2D64f& value1,
                const CvPoint2D64f& value2,
                CvPoint2D64f& maxValue)
{
    maxValue.x = std::max(value1.x,value2.x);
    maxValue.y = std::max(value1.y,value2.y);
}


inline void max(const CvPoint3D32f& value1,
                const CvPoint3D32f& value2,
                CvPoint3D32f& maxValue)
{
    maxValue.x = std::max(value1.x,value2.x);
    maxValue.y = std::max(value1.y,value2.y);
    maxValue.z = std::max(value1.z,value2.z);
}


inline void max(const CvPoint3D64f& value1,
                const CvPoint3D64f& value2,
                CvPoint3D64f& maxValue)
{
    maxValue.x = std::max(value1.x,value2.x);
    maxValue.y = std::max(value1.y,value2.y);
    maxValue.z = std::max(value1.z,value2.z);
}


template<typename blDataType>
inline void max(const blImage<blDataType>& img,
                blDataType& maxValue)
{
    if(img.sizeROI() <= 0)
        return;

    int rows = img.size1ROI();
    int cols = img.size2ROI();

    int yROI = img.yROI();
    int xROI = img.xROI();

    maxValue = img(yROI,xROI);

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            max(img(i,j),maxValue,maxValue);
        }
    }
}


template<typename blDataType>
inline void max(const blImage<blDataType>& img1,
                const blImage<blDataType>& img2,
                blImage<blDataType>& maxImg)
{
    int rows1 = img1.size1ROI();
    int cols1 = img1.size2ROI();
    int yROI1 = img1.yROI();
    int xROI1 = img1.xROI();

    int rows2 = img2.size1ROI();
    int cols2 = img2.size2ROI();
    int yROI2 = img2.yROI();
    int xROI2 = img2.xROI();

    int rowsMax = maxImg.size1ROI();
    int colsMax = maxImg.size2ROI();
    int yROIMax = maxImg.yROI();
    int xROIMax = maxImg.xROI();

    for(int i1 = yROI1, i2 = yROI2, iMax = yROIMax;
        i1 < (rows1 +yROI1) && i2 < (rows2 + yROI2) && iMax < (rowsMax + yROIMax);
        ++i1, ++i2, ++iMax)
    {
        for(int j1 = xROI1, j2 = xROI2, jMax = xROIMax;
            j1 < (cols1 + xROI1) && j2 < (cols2 + xROI2) && jMax < (colsMax + xROIMax);
            ++j1, ++j2, ++jMax)
        {
            max(img1(i1,j1),img2(i2,j2),maxImg(iMax,jMax));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function finds the
// minimum and maximum values simultaneously
//-------------------------------------------------------------------
template<typename blDataType>
inline void minmax(const blImage<blDataType>& img,
                   blDataType& minValue,
                   blDataType& maxValue)
{
    if(img.sizeROI() <= 0)
        return;

    int rows = img.size1ROI();
    int cols = img.size2ROI();

    int yROI = img.yROI();
    int xROI = img.xROI();

    minValue = img(yROI,xROI);
    maxValue = img(yROI,xROI);

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            min(img(i,j),minValue,minValue);
            max(img(i,j),maxValue,maxValue);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions return
// an identity matrix/image of
// specified dimension
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> eye(const int& rows,
                               const int& cols,
                               const blDataType& oneValue = blDataType(1),
                               const blDataType& zeroValue = blDataType(0))
{
    blImage<blDataType> identityMatrix(rows,cols,zeroValue);

    int dimension = std::min(rows,cols);

    for(int i = 0; i < dimension; ++i)
    {
        identityMatrix(i,i) = oneValue;
    }

    return identityMatrix;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> eye(const int& rows,
                               const blDataType& oneValue = blDataType(1),
                               const blDataType& zeroValue = blDataType(0))
{
    blImage<blDataType> identityMatrix(rows,rows,zeroValue);

    for(int i = 0; i < rows; ++i)
    {
        identityMatrix(i,i) = oneValue;
    }

    return identityMatrix;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void eye(blImage<blDataType>& img,
                const blDataType& oneValue = blDataType(1))
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();

    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI, j = xROI; i < rows + yROI && j < cols + xROI; ++i, ++j)
    {
        img(i,j) = oneValue;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function
// calculates the trace
// of a matrix (image)
//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType trace(const blImage<blDataType>& srcImage)
{
    if(srcImage.sizeROI() <= 0)
        return blDataType(0);

    int rows = srcImage.size1ROI();
    int cols = srcImage.size2ROI();

    int yROI = srcImage.yROI();
    int xROI = srcImage.xROI();

    blDataType result = srcImage(yROI,xROI);

    for(int i = yROI + 1, j = xROI + 1; i < rows + yROI && j < cols + xROI; ++i, ++j)
    {
        result += srcImage(i,j);
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions
// invert an image
//-------------------------------------------------------------------
template<typename blDataType>

inline void negateImage(const blImage<blDataType>& srcImg,
                        blImage<blDataType>& dstImg)
{
    // Get the min/max range for
    // the depth of the image

    blDataType minColor = blDataType(rangeMin(srcImg.getDepth()));
    blDataType maxColor = blDataType(rangeMax(srcImg.getDepth()));

    // Get the size of
    // the images' ROIs

    int srcRows = srcImg.size1ROI();
    int srcCols = srcImg.size2ROI();
    int srcyROI = srcImg.yROI();
    int srcxROI = srcImg.xROI();

    int dstRows = dstImg.size1ROI();
    int dstCols = dstImg.size2ROI();
    int dstyROI = dstImg.yROI();
    int dstxROI = dstImg.xROI();

    // Step through the image
    // ROI and negate the values

    for(int iSrc = srcyROI, iDst = dstyROI;
        iSrc < srcRows + srcyROI && iDst < dstRows + dstyROI;
        ++iSrc, ++iDst)
    {
        for(int jSrc = srcxROI, jDst = dstxROI;
            jSrc < srcCols +srcxROI && jDst < dstCols + dstxROI;
            ++jSrc, ++jDst)
        {
            dstImg(iDst,jDst) = maxColor - srcImg(iSrc,jSrc) + minColor;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType> operator!(const blImage<blDataType>& srcImage)
{
    blImage<blDataType> negatedImage(srcImage.size1ROI(),srcImage.size2ROI());

    negateImage(srcImage,negatedImage);

    return negatedImage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions carry out
// element by element addition,
// subtraction, multiplication,
// division and other operations
// of two images
//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blDataType perElementAddition(const blDataType& src1,
                                     const blDataType2& src2)
{
    return (src1 + src2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blDataType perElementSubtraction(const blDataType& src1,
                                        const blDataType2& src2)
{
    return (src1 - src2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blDataType perElementMultiplication(const blDataType& src1,
                                           const blDataType2& src2)
{
    return (src1 * src2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blDataType perElementDivision(const blDataType& src1,
                                     const blDataType2& src2)
{
    return (src1 / src2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blDataType perElementSquareOfDifference(const blDataType& src1,
                                               const blDataType2& src2)
{
    return ( (src1 - src2) * (src1 - src2) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementAddition(const blDataType& src1,
                               const blDataType2& src2,
                               blDataType3& dst)
{
    dst = src1 + src2;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementSubtraction(const blDataType& src1,
                                  const blDataType2& src2,
                                  blDataType3& dst)
{
    dst = src1 - src2;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementMultiplication(const blDataType& src1,
                                     const blDataType2& src2,
                                     blDataType3& dst)
{
    dst = src1 * src2;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementDivision(const blDataType& src1,
                               const blDataType2& src2,
                               blDataType3& dst)
{
    dst = src1 / src2;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline blDataType perElementSquareOfDifference(const blDataType& src1,
                                               const blDataType2& src2,
                                               blDataType3& dst)
{
    dst = ( (src1 - src2) * (src1 - src2) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void perElementAddition(blImage<blDataType>& img,
                               const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            perElementAddition(img(i,j),
                               scalar,
                               img(i,j));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void perElementSubtraction(blImage<blDataType>& img,
                                  const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            perElementSubtraction(img(i,j),
                                  scalar,
                                  img(i,j));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void perElementMultiplication(blImage<blDataType>& img,
                                     const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            perElementMultiplication(img(i,j),
                                     scalar,
                                     img(i,j));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void perElementDivision(blImage<blDataType>& img,
                               const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            perElementDivision(img(i,j),
                               scalar,
                               img(i,j));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void perElementSquareOfDifference(blImage<blDataType>& img,
                                         const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            perElementSquareOfDifference(img(i,j),
                                         scalar,
                                         img(i,j));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementAddition(const blImage<blDataType>& srcImage1,
                               const blImage<blDataType2>& srcImage2,
                               blImage<blDataType3>& dstImage)
{
    int srcRows1 = srcImage1.size1ROI();
    int srcCols1 = srcImage1.size2ROI();
    int srcyROI1 = srcImage1.yROI();
    int srcxROI1 = srcImage1.xROI();

    int srcRows2 = srcImage2.size1ROI();
    int srcCols2 = srcImage2.size2ROI();
    int srcyROI2 = srcImage2.yROI();
    int srcxROI2 = srcImage2.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc1 = srcyROI1, iSrc2 = srcyROI2, iDst = dstyROI;
        iSrc1 < srcRows1 + srcyROI1 && iSrc2 < srcRows2 + srcyROI2 && iDst < dstRows + dstyROI;
        ++iSrc1, ++iSrc2, ++iDst)
    {
        for(int jSrc1 = srcxROI1, jSrc2 = srcxROI2, jDst = dstxROI;
            jSrc1 < srcCols1 + srcxROI1 && jSrc2 < srcCols2 + srcxROI2 && jDst < dstCols + dstxROI;
            ++jSrc1, ++jSrc2, ++jDst)
        {
            perElementAddition(srcImage1(iSrc1,jSrc1),
                               srcImage2(iSrc2,jSrc2),
                               dstImage(iDst,jDst));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementSubtraction(const blImage<blDataType>& srcImage1,
                                  const blImage<blDataType2>& srcImage2,
                                  blImage<blDataType3>& dstImage)
{
    int srcRows1 = srcImage1.size1ROI();
    int srcCols1 = srcImage1.size2ROI();
    int srcyROI1 = srcImage1.yROI();
    int srcxROI1 = srcImage1.xROI();

    int srcRows2 = srcImage2.size1ROI();
    int srcCols2 = srcImage2.size2ROI();
    int srcyROI2 = srcImage2.yROI();
    int srcxROI2 = srcImage2.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc1 = srcyROI1, iSrc2 = srcyROI2, iDst = dstyROI;
        iSrc1 < srcRows1 + srcyROI1 && iSrc2 < srcRows2 + srcyROI2 && iDst < dstRows + dstyROI;
        ++iSrc1, ++iSrc2, ++iDst)
    {
        for(int jSrc1 = srcxROI1, jSrc2 = srcxROI2, jDst = dstxROI;
            jSrc1 < srcCols1 + srcxROI1 && jSrc2 < srcCols2 + srcxROI2 && jDst < dstCols + dstxROI;
            ++jSrc1, ++jSrc2, ++jDst)
        {
            perElementSubtraction(srcImage1(iSrc1,jSrc1),
                                  srcImage2(iSrc2,jSrc2),
                                  dstImage(iDst,jDst));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementMultiplication(const blImage<blDataType>& srcImage1,
                                     const blImage<blDataType2>& srcImage2,
                                     blImage<blDataType3>& dstImage)
{
    int srcRows1 = srcImage1.size1ROI();
    int srcCols1 = srcImage1.size2ROI();
    int srcyROI1 = srcImage1.yROI();
    int srcxROI1 = srcImage1.xROI();

    int srcRows2 = srcImage2.size1ROI();
    int srcCols2 = srcImage2.size2ROI();
    int srcyROI2 = srcImage2.yROI();
    int srcxROI2 = srcImage2.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc1 = srcyROI1, iSrc2 = srcyROI2, iDst = dstyROI;
        iSrc1 < srcRows1 + srcyROI1 && iSrc2 < srcRows2 + srcyROI2 && iDst < dstRows + dstyROI;
        ++iSrc1, ++iSrc2, ++iDst)
    {
        for(int jSrc1 = srcxROI1, jSrc2 = srcxROI2, jDst = dstxROI;
            jSrc1 < srcCols1 + srcxROI1 && jSrc2 < srcCols2 + srcxROI2 && jDst < dstCols + dstxROI;
            ++jSrc1, ++jSrc2, ++jDst)
        {
            perElementMultiplication(srcImage1(iSrc1,jSrc1),
                                     srcImage2(iSrc2,jSrc2),
                                     dstImage(iDst,jDst));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementDivision(const blImage<blDataType>& srcImage1,
                               const blImage<blDataType2>& srcImage2,
                               blImage<blDataType3>& dstImage)
{
    int srcRows1 = srcImage1.size1ROI();
    int srcCols1 = srcImage1.size2ROI();
    int srcyROI1 = srcImage1.yROI();
    int srcxROI1 = srcImage1.xROI();

    int srcRows2 = srcImage2.size1ROI();
    int srcCols2 = srcImage2.size2ROI();
    int srcyROI2 = srcImage2.yROI();
    int srcxROI2 = srcImage2.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc1 = srcyROI1, iSrc2 = srcyROI2, iDst = dstyROI;
        iSrc1 < srcRows1 + srcyROI1 && iSrc2 < srcRows2 + srcyROI2 && iDst < dstRows + dstyROI;
        ++iSrc1, ++iSrc2, ++iDst)
    {
        for(int jSrc1 = srcxROI1, jSrc2 = srcxROI2, jDst = dstxROI;
            jSrc1 < srcCols1 + srcxROI1 && jSrc2 < srcCols2 + srcxROI2 && jDst < dstCols + dstxROI;
            ++jSrc1, ++jSrc2, ++jDst)
        {
            perElementDivision(srcImage1(iSrc1,jSrc1),
                               srcImage2(iSrc2,jSrc2),
                               dstImage(iDst,jDst));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2,
         typename blDataType3>

inline void perElementSquareOfDifference(const blImage<blDataType>& srcImage1,
                                         const blImage<blDataType2>& srcImage2,
                                         blImage<blDataType3>& dstImage)
{
    int srcRows1 = srcImage1.size1ROI();
    int srcCols1 = srcImage1.size2ROI();
    int srcyROI1 = srcImage1.yROI();
    int srcxROI1 = srcImage1.xROI();

    int srcRows2 = srcImage2.size1ROI();
    int srcCols2 = srcImage2.size2ROI();
    int srcyROI2 = srcImage2.yROI();
    int srcxROI2 = srcImage2.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc1 = srcyROI1, iSrc2 = srcyROI2, iDst = dstyROI;
        iSrc1 < srcRows1 + srcyROI1 && iSrc2 < srcRows2 + srcyROI2 && iDst < dstRows + dstyROI;
        ++iSrc1, ++iSrc2, ++iDst)
    {
        for(int jSrc1 = srcxROI1, jSrc2 = srcxROI2, jDst = dstxROI;
            jSrc1 < srcCols1 + srcxROI1 && jSrc2 < srcCols2 + srcxROI2 && jDst < dstCols + dstxROI;
            ++jSrc1, ++jSrc2, ++jDst)
        {
            perElementSquareOfDifference(srcImage1(iSrc1,jSrc1),
                                         srcImage2(iSrc2,jSrc2),
                                         dstImage(iDst,jDst));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blImage<blDataType> perElementAddition(const blImage<blDataType>& srcImage1,
                                              const blImage<blDataType2>& srcImage2)
{
    blImage<blDataType> result(std::min(srcImage1.size1ROI(),srcImage2.size1ROI()),
                               std::min(srcImage1.size2ROI(),srcImage2.size2ROI()));

    perElementAddition(srcImage1,
                       srcImage2,
                       result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blImage<blDataType> perElementSubtraction(const blImage<blDataType>& srcImage1,
                                                 const blImage<blDataType2>& srcImage2)
{
    blImage<blDataType> result(std::min(srcImage1.size1ROI(),srcImage2.size1ROI()),
                               std::min(srcImage1.size2ROI(),srcImage2.size2ROI()));

    perElementSubtraction(srcImage1,
                          srcImage2,
                          result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blImage<blDataType> perElementMultiplication(const blImage<blDataType>& srcImage1,
                                                    const blImage<blDataType2>& srcImage2)
{
    blImage<blDataType> result(std::min(srcImage1.size1ROI(),srcImage2.size1ROI()),
                               std::min(srcImage1.size2ROI(),srcImage2.size2ROI()));

    perElementMultiplication(srcImage1,
                             srcImage2,
                             result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blImage<blDataType> perElementDivision(const blImage<blDataType>& srcImage1,
                                              const blImage<blDataType2>& srcImage2)
{
    blImage<blDataType> result(std::min(srcImage1.size1ROI(),srcImage2.size1ROI()),
                               std::min(srcImage1.size2ROI(),srcImage2.size2ROI()));

    perElementDivision(srcImage1,
                       srcImage2,
                       result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline blImage<blDataType> perElementSquareOfDifference(const blImage<blDataType>& srcImage1,
                                                        const blImage<blDataType2>& srcImage2)
{
    blImage<blDataType> result(std::min(srcImage1.size1ROI(),srcImage2.size1ROI()),
                               std::min(srcImage1.size2ROI(),srcImage2.size2ROI()));

    perElementSquareOfDifference(srcImage1,
                                 srcImage2,
                                 result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions
// calculate the mean, variance
// and standard deviation of
// a matrix (image)
// The functions work with any data
// type that defines the appropriate
// member functions and operators
//-------------------------------------------------------------------
template<typename blDataType>

inline void mean(const blImage<blDataType>& srcImage,
                 blDataType& meanValue)
{
    if(srcImage.sizeROI() <= 0)
    {
        meanValue = blDataType(0);
        return;
    }

    int rows = srcImage.size1ROI();
    int cols = srcImage.size2ROI();

    int yROI = srcImage.yROI();
    int xROI = srcImage.xROI();

    meanValue = srcImage(yROI,xROI);

    for(int i = yROI; i < yROI + rows; ++i)
    {
        for(int j = xROI; j < xROI + cols; ++j)
        {
            if(i != yROI || j != xROI)
            {
                perElementAddition(meanValue,srcImage(i,j),meanValue);
            }
        }
    }

    perElementDivision(meanValue,blDataType(rows*cols),meanValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType mean(const blImage<blDataType>& srcImage)
{
    blDataType meanValue = blDataType(0);

    mean(srcImage,meanValue);

    return meanValue;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType variance(const blImage<blDataType>& srcImage,
                           const blDataType& meanValue)
{
    if(srcImage.sizeROI() <= 1)
        return blDataType(0);

    int rows = srcImage.size1ROI();
    int cols = srcImage.size2ROI();

    int yROI = srcImage.yROI();
    int xROI = srcImage.xROI();

    blDataType result = blDataType(0);

    for(int i = yROI; i < yROI + rows; ++i)
    {
        for(int j = xROI; j < xROI + cols; ++j)
        {
            perElementAddition(result,perElementSquareOfDifference(srcImage(i,j),meanValue),result);
        }
    }

    perElementDivision(result,blDataType(rows*cols - 1),result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType variance(const blImage<blDataType>& srcImage)
{
    auto meanValue = mean(srcImage);

    return variance(srcImage,meanValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType stdDev(const blImage<blDataType>& srcImage,
                         const blDataType& meanValue)
{
    return ( sqrt(variance(srcImage,meanValue)) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blDataType stdDev(const blImage<blDataType>& srcImage)
{
    return ( sqrt(variance(srcImage)) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following overloaded operators
// make code easier to read especially
// when using blImage in mathematical
// expressions
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline CvPoint operator-(const CvPoint& P1)
{
    return cvPoint(-P1.x,P1.y);
}
inline CvPoint operator+(const CvPoint& P1,const CvPoint& P2)
{
    return cvPoint(P1.x + P2.x,P1.y + P2.y);
}
inline CvPoint operator-(const CvPoint& P1,const CvPoint& P2)
{
    return cvPoint(P1.x - P2.x,P1.y - P2.y);
}
inline CvPoint operator*(const CvPoint& P1,const CvPoint& P2)
{
    return cvPoint(P1.x * P2.x,P1.y * P2.y);
}
inline CvPoint operator/(const CvPoint& P1,const CvPoint& P2)
{
    return cvPoint(P1.x / P2.x,P1.y / P2.y);
}
inline CvPoint operator%(const CvPoint& P1,const CvPoint& P2)
{
    return cvPoint(P1.x % P2.x,P1.y % P2.y);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator-(const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = -img(i + yROI,j + xROI);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> transpose(const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = img(j + xROI,i + yROI);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage< std::complex<blDataType> > transpose(const blImage< std::complex<blDataType> >& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage< std::complex<blDataType> > result(rows,cols);

    // We transpose the matrix
    // while also changing the sign
    // of the imaginary values

    for(int i = 0; i < result.size1(); ++i)
    {
        for(int j = 0; j < result.size2(); ++j)
        {
            result(i,j).real(img(j + xROI,i + yROI).real());
            result(i,j).imag(-img(j + xROI,i + yROI).imag());
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> inv(const blImage<blDataType>& img,
                               int flags = cv::DECOMP_SVD)
{
    blImage<blDataType> result(img.size2ROI(),img.size1ROI());

    cvInvert(img,result,flags);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator+(const blImage<blDataType>& img,
                                     const blDataType2& dataPoint)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = img(i + yROI,j + xROI) + dataPoint;
        }
    }

    return result;
}


template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator+(const blDataType2& dataPoint,
                                     const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = dataPoint + img(i + yROI,j + xROI);
        }
    }

    return result;
}


template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator-(const blImage<blDataType>& img,
                                     const blDataType2& dataPoint)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = img(i + yROI,j + xROI) - dataPoint;
        }
    }

    return result;
}


template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator-(const blDataType2& dataPoint,
                                     const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = dataPoint - img(i + yROI,j + xROI);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator+(const blImage<blDataType>& img1,
                                     const blImage<blDataType2>& img2)
{
    int rows1 = img1.size1ROI();
    int cols1 = img1.size2ROI();
    int yROI1 = img1.yROI();
    int xROI1 = img1.xROI();

    int rows2 = img2.size1ROI();
    int cols2 = img2.size2ROI();
    int yROI2 = img2.yROI();
    int xROI2 = img2.xROI();

    int rowsMin = std::min(rows1,rows2);
    int colsMin = std::min(cols1,cols2);

    if(rows1*cols1 == 1)
        return ( img1(yROI1,xROI1) + img2 );
    else if(rows2*cols2 == 1)
        return ( img1 + img2(yROI2,xROI2) );

    blImage<blDataType> result(rowsMin,colsMin);

    for(int i = 0, i1 = yROI1, i2 = yROI2; i < rowsMin; ++i, ++i1, ++i2)
    {
        for(int j = 0, j1 = xROI1, j2 = xROI2; j < colsMin; ++j, ++j1, ++j2)
        {
            result(i,j) = img1(i1,j1) + img2(i2,j2);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>

inline blImage<blDataType> operator-(const blImage<blDataType>& img1,
                                     const blImage<blDataType2>& img2)
{
    int rows1 = img1.size1ROI();
    int cols1 = img1.size2ROI();
    int yROI1 = img1.yROI();
    int xROI1 = img1.xROI();

    int rows2 = img2.size1ROI();
    int cols2 = img2.size2ROI();
    int yROI2 = img2.yROI();
    int xROI2 = img2.xROI();

    int rowsMin = std::min(rows1,rows2);
    int colsMin = std::min(cols1,cols2);

    if(rows1*cols1 == 1)
        return ( img1(yROI1,xROI1) + img2 );
    else if(rows2*cols2 == 1)
        return ( img1 + img2(yROI2,xROI2) );

    blImage<blDataType> result(rowsMin,colsMin);

    for(int i = 0, i1 = yROI1, i2 = yROI2; i < rowsMin; ++i, ++i1, ++i2)
    {
        for(int j = 0, j1 = xROI1, j2 = xROI2; j < colsMin; ++j, ++j1, ++j2)
        {
            result(i,j) = img1(i1,j1) - img2(i2,j2);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions multiply
// a scalar by an image and viceversa
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator*(const blImage<blDataType>& img,
                                     const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = img(i + yROI,j + xROI) * scalar;
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator*(const blDataType& scalar,
                                     const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = scalar * img(i + yROI,j + xROI);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            operator*
//
// ARGUMENTS:           - img1
//                      - img2
//
// TEMPLATE ARGUMENTS:  blDataType
//
// PURPOSE:             Generalized multiplication of two
//                      imges img1 and img2 using OpenCV
//
// DEPENDENCIES:        - blImage
//                      - cvScale
//                      - cvGEMM
//
// NOTES:               The function checks for the size of the
//                      matrices to make sure that they are
//                      correct
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator*(const blImage<blDataType>& img1,
                                     const blImage<blDataType>& img2)
{
    // First we get the sizes
    // of the matrices

    int rows1 = img1.size1ROI();
    int cols1 = img1.size2ROI();
    int rows2 = img2.size1ROI();
    int cols2 = img2.size2ROI();

    // Let's check the sizes and
    // make sure that the matrices
    // can indeed be multiplied together

    if(cols1 != rows2)
    {
        // This means that the images
        // don't have the correct sizes
        // for matrix multiplication, so
        // let's check if one of them is
        // a scalar

        if(rows1 == cols1 && rows1 == 1)
        {
            return ( img1.atROI(0,0) * img2 );
        }
        else if(rows2 == cols2 && rows2 == 1)
        {
            return ( img1 * img2.atROI(0,0) );
        }

        // If we got here then this
        // means that the matrices are
        // the wrong sizes and that we
        // cannot perform multiplication,
        // so we record the error and
        // return img1 as the result

        // Error
        return img1;
    }

    // If we made it to this point
    // then this means that the
    // matrices are the correct
    // sizes, so we just call OpenCV's
    // cvGEMM generalized matrix
    // multiplication algorithm

    blImage<blDataType> result(rows1,cols2);
    cvGEMM(img1,img2,1,NULL,0,result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator*(const blImage<blDataType>& img1,
                                     const blImage<blDataType2>& img2)
{
    // Copy the different
    // type image

    blImage<blDataType> newImg2 = img2;

    // Multiply the
    // two images

    return (img1*newImg2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions extend the
// concept of multiplication of images
// to the division of one image by another
// by use of the inverse
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator/(const blImage<blDataType>& img,
                                     const blDataType& scalar)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = img(i + yROI,j + xROI) / scalar;
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> operator/(const blDataType& scalar,
                                     const blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blImage<blDataType> result(rows,cols);

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            result(i,j) = scalar / img(i + yROI,j + xROI);
        }
    }

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline blImage<blDataType> operator/(const blImage<blDataType>& img1,
                                     const blImage<blDataType2>& img2)
{
    if(img1.sizeROI() <= 0 || img2.sizeROI() <= 0)
        return img1;

    // First we check if
    // the second image is
    // a 1x1 image

    if(img2.sizeROI() == 1)
        return ( img1 / img2.atROI(0,0) );

    // We divide two images by
    // multiplying the first image
    // img1 by the inverse of the
    // second image img2

    return ( img1 * inv(img2) );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function calculates
// the rank of an image matrix
//-------------------------------------------------------------------
template<typename blDataType>
inline int rank(blImage<blDataType>& img,
                const blDataType& definitionOfZero = blDataType(0))
{
    int imgSize = std::min(img.size1ROI(),img.size2ROI());

    // To calculate the rank, we perform
    // a singular value decomposition and
    // count the non zero singular values

    if(imgSize <= 0)
    {
        // The rank of a zero
        // sized image is
        // defaulted to zero

        return 0;
    }

    // Create the vector
    // to hold the singular
    // values

    blImage<blDataType> singularValues(imgSize,1);

    // Calculate the SVD, but
    // we do not want to modify
    // the source image

    cvSVD(img,singularValues);

    // Finally, knowing that the
    // singular values are organized
    // in a descending order, we
    // step through and count all
    // the non-zero values and return
    // the rank

    int imageRank = 0;

    while(imageRank < imgSize)
    {
        if(singularValues(imageRank,0) > definitionOfZero)
            ++imageRank;
        else
            return imageRank;
    }

    return imageRank;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions copy
// individual rows and columns
// from one image into another
//-------------------------------------------------------------------
template<typename blDataType>
inline void copyRow(const blImage<blDataType>& srcImg,
                    blImage<blDataType>& dstImg,
                    const int& srcRowIndex,
                    const int& dstRowIndex)
{
    int srcCols = srcImg.size2ROI();
    int srcyROI = srcImg.yROI();
    int srcxROI = srcImg.xROI();

    int dstCols = dstImg.size2ROI();
    int dstyROI = dstImg.yROI();
    int dstxROI = dstImg.xROI();

    for(int jSrc = srcxROI, jDst = dstxROI;
        jSrc < srcCols + srcxROI && jDst < dstCols + dstxROI;
        ++jSrc, ++jDst)
    {
        dstImg(dstRowIndex + dstyROI,jDst) = srcImg(srcRowIndex + srcyROI,jSrc);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void copyCol(const blImage<blDataType>& srcImg,
                    blImage<blDataType>& dstImg,
                    const int& srcColIndex,
                    const int& dstColIndex)
{
    int srcRows = srcImg.size1ROI();
    int srcyROI = srcImg.yROI();
    int srcxROI = srcImg.xROI();

    int dstRows = dstImg.size1ROI();
    int dstyROI = dstImg.yROI();
    int dstxROI = dstImg.xROI();

    for(int iSrc = srcyROI, iDst = dstyROI;
        iSrc < srcRows + srcyROI && iDst < dstRows + dstyROI;
        ++iSrc, ++iDst)
    {
        dstImg(iDst,dstColIndex + dstxROI) = srcImg(iSrc,srcColIndex + srcxROI);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions
// copy data from one image
// into another taking into
// account both images ROIs
// The ROIs don't have to have
// the same size and the
// images don't have to be
// of the same type, the function
// converts from source type
// to destination type if possible
//-------------------------------------------------------------------
template<typename blDataType,
         typename blDataType2>

inline void copyImage(const blImage<blDataType>& srcImg,
                      blImage<blDataType2>& dstImg)
{
    int srcRows = srcImg.size1ROI();
    int srcCols = srcImg.size2ROI();
    int srcyROI = srcImg.yROI();
    int srcxROI = srcImg.xROI();

    int dstRows = dstImg.size1ROI();
    int dstCols = dstImg.size2ROI();
    int dstyROI = dstImg.yROI();
    int dstxROI = dstImg.xROI();

    for(int iSrc = srcyROI, iDst = dstyROI;
        iSrc < srcRows + srcyROI && iDst < dstRows + dstyROI;
        ++iSrc, ++iDst)
    {
        for(int jSrc = srcxROI, jDst = dstxROI;
            jSrc < srcCols + srcxROI && jDst < dstCols + dstxROI;
            ++jSrc, ++jDst)
        {
            dstImg(iDst,jDst) = blDataType(srcImg(iSrc,jSrc));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions are
// used to quickly extract a row or
// column from an image
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> copyRow(const blImage<blDataType>& img,
                                   const int& indexOfRowToCopy)
{
    blImage<blDataType> extractedRow(1,img.size2ROI());

    copyRow(img,extractedRow,indexOfRowToCopy);

    return extractedRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> copyCol(const blImage<blDataType>& img,
                                      const int& indexOfColToCopy)
{
    blImage<blDataType> extractedCol(img.size1ROI(),1);

    copyRow(img,extractedCol,indexOfColToCopy);

    return extractedCol;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the norm of an image
//-------------------------------------------------------------------
template<typename blDataType>
inline void calculateNorm(const blImage<blDataType>& srcImg1,
                          blDataType& normValue,
                          const int& normType = cv::NORM_L2)
{
    if(srcImg1.size1ROI() <= 0)
        return;

    int rows = srcImg1.size1ROI();
    int cols = srcImg1.size2ROI();
    int yROI = srcImg1.yROI();
    int xROI = srcImg1.xROI();

    blDataType absValue = std::abs(srcImg1(yROI,xROI));
    normValue = absValue;

    switch(normType)
    {
    case cv::NORM_INF:

        for(int i = yROI; i < rows + yROI; ++i)
        {
            for(int j = xROI; j < cols + xROI; ++j)
            {
                absValue = std::abs(srcImg1(i,j));

                if(absValue > normValue)
                    normValue = absValue;
            }
        }

        break;

    case cv::NORM_L1:

        normValue = blDataType(0);

        for(int i = yROI; i < rows + yROI; ++i)
        {
            for(int j = xROI; j < cols + xROI; ++j)
            {
                normValue += std::abs(srcImg1(i,j));
            }
        }

        break;

    case cv::NORM_L2:
    default:

        normValue = blDataType(0);

        for(int i = yROI; i < rows + yROI; ++i)
        {
            for(int j = xROI; j < cols + xROI; ++j)
            {
                absValue = srcImg1(i,j);

                normValue += (absValue * absValue);
            }
        }

        normValue = std::sqrt(normValue);

        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType norm(const blImage<blDataType>& srcImg1,
                       const int& normType = cv::NORM_L2)
{
    blDataType normValue;

    calculateNorm(srcImg1,normValue,normType);

    return normValue;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType norm(const blImage<blDataType>& srcImg1,
                       const blImage<blDataType>& srcImg2,
                       const int& normType = cv::NORM_L2)
{
    /*// In this function we
    // expect both source
    // images to have the
    // same size ROIs

    if( srcImg1.size1ROI() <= 0 || (srcImg1.size1ROI() != srcImg2.size2ROI()) )
        return blDataType(0);

    int rows = srcImg1.size1ROI();
    int cols = srcImg1.size2ROI();

    int yROI1 = srcImg1.yROI();
    int xROI1 = srcImg1.xROI();

    int yROI2 = srcImg2.yROI();
    int xROI2 = srcImg2.xROI();

    blDataType absValue = std::abs(srcImg1(yROI1,xROI1) - srcImg2(yROI2,xROI2));
    blDataType normValue = absValue;

    switch(normType)
    {
    case cv::NORM_INF:

        for(int i1 = yROI1, i2 = yROI2; i1 < rows; ++i)
        {
            for(int j = xROI; j < cols; ++j)
            {
                absValue = std::abs(srcImg1(i,j));

                if(absValue > normValue)
                    normValue = absValue;
            }
        }

        break;

    case cv::NORM_L1:

        normValue = blDataType(0);

        for(int i = yROI; i < rows; ++i)
        {
            for(int j = xROI; j < cols; ++j)
            {
                normValue += std::abs(srcImg1(i,j));
            }
        }

        break;

    case cv::NORM_L2:
    default:

        normValue = blDataType(0);

        for(int i = yROI; i < rows; ++i)
        {
            for(int j = xROI; j < cols; ++j)
            {
                absValue = srcImg1(i,j);

                normValue += (absValue * absValue);
            }
        }

        normValue = std::sqrt(normValue);

        break;
    }

    return normValue;*/
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the magnitude of specified row
// and column vectors of an image
//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType rowMagnitude(const blImage<blDataType>& img,
                               const int& rowIndex)
{
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blDataType result = blDataType(0);

    for(int j = xROI; j < cols + xROI; ++j)
    {
        result += ( img(rowIndex + yROI,j) * img(rowIndex + yROI,j) );
    }

    result = std::sqrt(result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blDataType colMagnitude(const blImage<blDataType>& img,
                               const int& colIndex)
{
    int rows = img.size1ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    blDataType result = blDataType(0);

    for(int i = yROI; i < rows + yROI; ++i)
    {
        result += ( img(i,colIndex + xROI) * img(i,colIndex + xROI) );
    }

    result = std::sqrt(result);

    return result;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions are used to
// normalize individual row or column
// vectors of an image
//-------------------------------------------------------------------
template<typename blDataType>
inline void normalizeRowVectors(blImage<blDataType>& img)
{
    blDataType mag = blDataType(0);

    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = 0; i < rows; ++i)
    {
        mag = rowMagnitude(img,i);

        for(int j = xROI; j < cols + xROI; ++j)
        {
            img(i + yROI,j) /= mag;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void normalizeColVectors(blImage<blDataType>& img)
{
    blDataType mag = blDataType(0);

    int rows = img.size1ROI();
    int cols = img.size2ROI();
    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int j = 0; j < cols; ++j)
    {
        mag = colMagnitude(img,j);

        for(int i = yROI; i < rows + yROI; ++i)
        {
            img(i,j + xROI) /= mag;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the projection of one image vector
// onto another (one for row vectors
// and the other for column vectors)
//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> projectionOfRowVectors(const blImage<blDataType>& vec1,
                                                  const blImage<blDataType>& vec2)
{
    return ( ( (vec1*transpose(vec2))/(vec1*transpose(vec1)) ) * vec1);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType> projectionOfColVectors(const blImage<blDataType>& vec1,
                                                  const blImage<blDataType>& vec2)
{
    return ( ((transpose(vec1)*vec2)/(transpose(vec1)*vec1)) * vec1);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions orthonormalize
// an image using the modified Gram-Schmidt process
// which is numerically stable
// The first orthonormalizes the rows
// and the other orthonormalizes the columns
//-------------------------------------------------------------------
template<typename blDataType>
inline void orthoNormalizeRows(blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();

    // Create column vectors
    // used in the calculation

    blImage<blDataType> vi(1,cols);
    blImage<blDataType> vj(1,cols);

    // Step through the column
    // vectors of the image
    // and ortho-normalize them

    for(int i = 0; i < rows; ++i)
    {
        copyRow(img,vi,i,0);

        for(int j = i-1; j >= 0; --j)
        {
            copyRow(img,vj,j,0);

            vi = vi - projectionOfRowVectors(vj,vi);
        }

        normalizeRowVectors(vi);
        copyRow(vi,img,0,i);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void orthoNormalizeCols(blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();

    // Create column vectors
    // used in the calculation

    blImage<blDataType> vi(rows,1);
    blImage<blDataType> vj(rows,1);

    // Step through the column
    // vectors of the image
    // and ortho-normalize them

    for(int i = 0; i < cols; ++i)
    {
        copyCol(img,vi,i,0);

        for(int j = i-1; j >= 0; --j)
        {
            copyCol(img,vj,j,0);

            vi = vi - projectionOfColVectors(vj,vi);
        }

        normalizeColVectors(vi);
        copyCol(vi,img,0,i);
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGEOPERATORS_HPP
