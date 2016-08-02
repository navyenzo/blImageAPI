#ifndef BL_IMAGE_HPP
#define BL_IMAGE_HPP


//-------------------------------------------------------------------
// FILE:            blImage.hpp
// CLASS:           blImage
// BASE CLASS:      blImageX
//
// PURPOSE:         Based on blImageX, (where X is the highest
//                  number in the blImage series), this class
//                  adds constructors and assignment operators,
//                  while allowing us to add more functionality
//                  at a later point by adding more blImageX classes
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LICENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - blImageX and all its dependencies (X being the
//                    highest number in the blImage series)
//
// NOTES:
//
// DATE CREATED:    Jan/30/2012
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
class blImage : public blImage6<blDataType>
{
public: // Constructors and destructors

    // Default constructor

    explicit blImage(const int& rows = 1,
                     const int& cols = 1);

    // Constructor that initializes
    // the image pixel with an initial
    // value

    explicit blImage(const int& rows,
                     const int& cols,
                     const blDataType& initialValue);

    // Copy constructor

    blImage(const blImage<blDataType>& image);

    // Copy constructor from
    // a different type image

    template<typename blDataType2>
    blImage(const blImage<blDataType2>& image,const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Constructor from a
    // raw IplImage* pointer

    blImage(const IplImage* image,const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Constructor from a
    // raw CvMat* pointer

    blImage(const CvMat* image,const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);

    // Construct image from
    // a static 2D array

    template<int numOfRows,int numOfCols>
    blImage(blDataType (&staticArray)[numOfRows][numOfCols]);
    template<int numOfRows,int numOfCols>
    blImage(const blDataType (&staticArray)[numOfRows][numOfCols]);

    // Construct image from
    // a static 2D array of
    // a different type

    template<typename blDataType2,int numOfRows,int numOfCols>
    blImage(const blDataType2 (&staticArray)[numOfRows][numOfCols]);

    // Destructor

    ~blImage()
    {
    }

public: // Assignment operators

    blImage<blDataType>&                            operator=(const blImage<blDataType>& image);
    template<typename blDataType2>
    blImage<blDataType>&                            operator=(const blImage<blDataType2>& image);
    template<int numOfRows,int numOfCols>
    blImage<blDataType>&                            operator=(blDataType (&staticArray)[numOfRows][numOfCols]);
    template<int numOfRows,int numOfCols>
    blImage<blDataType>&                            operator=(const blDataType (&staticArray)[numOfRows][numOfCols]);
    template<typename blDataType2,int numOfRows,int numOfCols>
    blImage<blDataType>&                            operator=(const blDataType2 (&staticArray)[numOfRows][numOfCols]);
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>::blImage(const int& rows,
                                    const int& cols)
                                    : blImage6<blDataType>()
{
    // We try to create
    // an image with the
    // specified number of
    // rows and cols

    this->create(rows,cols);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>::blImage(const int& rows,
                                    const int& cols,
                                    const blDataType& initialValue)
                                    : blImage6<blDataType>()
{
    // We try to create
    // an image with the
    // specified number of
    // rows and cols and
    // initialize it with
    // the specified value

    this->create(rows,cols,initialValue);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>::blImage(const blImage<blDataType>& image)
                                    : blImage6<blDataType>(image)
{
    // We save the image
    // pointer without
    // copying the image

    if(this->m_imageSharedPtr != image.getImageSharedPtr())
        this->m_imageSharedPtr = image.getImageSharedPtr();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline blImage<blDataType>::blImage(const blImage<blDataType2>& image,
                                    const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                    : blImage6<blDataType>()
{
    // We clone the passed
    // image

    this->clone(image,inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>::blImage(const IplImage* image,
                                    const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                    : blImage6<blDataType>()
{
    // We clone the passed
    // image

    this->clone(image,inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>::blImage(const CvMat* image,
                                    const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
                                    : blImage6<blDataType>()
{
    // We clone the passed
    // matrix

    this->clone(image,inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfRows,int numOfCols>
inline blImage<blDataType>::blImage(blDataType (&staticArray)[numOfRows][numOfCols])
                                    : blImage6<blDataType>()
{
    // Since the user passed
    // a non-const static array
    // we wrap it with this blImage
    // object

    this->wrap(staticArray);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfRows,int numOfCols>
inline blImage<blDataType>::blImage(const blDataType (&staticArray)[numOfRows][numOfCols])
                                    : blImage6<blDataType>()
{
    // Since the user passed
    // a "const" static array
    // we clone it

    this->clone(staticArray);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2,int numOfRows,int numOfCols>
inline blImage<blDataType>::blImage(const blDataType2 (&staticArray)[numOfRows][numOfCols])
                                    : blImage6<blDataType>()
{
    // We clone the passed
    // static array

    this->clone(staticArray);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage<blDataType>& blImage<blDataType>::operator=(const blImage<blDataType>& image)
{
    // We save the image
    // pointer without
    // copying the image

    if(this->m_imageSharedPtr != image.getImageSharedPtr())
        this->m_imageSharedPtr = image.getImageSharedPtr();

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline blImage<blDataType>& blImage<blDataType>::operator=(const blImage<blDataType2>& image)
{
    // We clone the passed
    // image

    this->clone(image,0);

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfRows,int numOfCols>
inline blImage<blDataType>& blImage<blDataType>::operator=(blDataType (&staticArray)[numOfRows][numOfCols])
{
    // Since the user passed
    // a non-const static array
    // we wrap it with this blImage
    // object

    this->wrap(staticArray);

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<int numOfRows,int numOfCols>
inline blImage<blDataType>& blImage<blDataType>::operator=(const blDataType (&staticArray)[numOfRows][numOfCols])
{
    // Since the user passed
    // a "const" static array
    // we clone it

    this->clone(staticArray);

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2,int numOfRows,int numOfCols>
inline blImage<blDataType>& blImage<blDataType>::operator=(const blDataType2 (&staticArray)[numOfRows][numOfCols])
{
    // We clone the passed
    // static array

    this->clone(staticArray);

    return (*this);
}
//-------------------------------------------------------------------


#endif // BL_IMAGE_HPP
