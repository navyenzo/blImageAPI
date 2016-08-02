#ifndef BL_IMAGE4_HPP
#define BL_IMAGE4_HPP


//-------------------------------------------------------------------
// FILE:            blImage4.hpp
// CLASS:           blImage4
// BASE CLASS:      blImage3
//
// PURPOSE:         Based on blImage3, this class adds functions
//                  that can load/save images from/to files
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage3 and all its dependencies
//
// NOTES:
//
// DATE CREATED:    Jan/24/2012
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
class blImage4 : public blImage3<blDataType>
{
public: // Constructors and destructors

    // Default constructor
    blImage4();

    // Copy constructor
    blImage4(const blImage4<blDataType>& Image4);

    // Destructor
    ~blImage4()
    {
    }

public: // Public functions

    // Function used to load
    // an image from file

    bool            loadImageFromFile(const char* filename,
                                      const bool& shouldImageBeFlipped = false,
                                      const int& imageFlipMode = 0,
                                      const int& howToReadImageColorAndDepth = CV_LOAD_IMAGE_UNCHANGED,
                                      const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2 = 0);


};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage4<blDataType>::blImage4() : blImage3<blDataType>()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blImage4<blDataType>::blImage4(const blImage4<blDataType>& Image4) : blImage3<blDataType>(Image4)
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline bool blImage4<blType>::loadImageFromFile(const char* filename,
                                                const bool& shouldImageBeFlipped,
                                                const int& imageFlipMode,
                                                const int& howToReadImageColorAndDepth,
                                                const int& inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2)
{
    // Try to load
    // image from file

    IplImage* loadedImage = NULL;
    loadedImage = cvLoadImage(filename,
                              howToReadImageColorAndDepth);

    // Check if the image
    // successfully loaded

    if(loadedImage == NULL)
    {
        // Error -- Tried to load an image
        //          from file but failed

        return false;
    }

    // Flip image if
    // being asked to

    if(shouldImageBeFlipped)
        cvFlip(loadedImage,
               loadedImage,
               imageFlipMode);

    // We try to wrap
    // the loaded image
    // with this blImage

    if(this->wrap(loadedImage,true))
    {
        // In this case we
        // successfully
        // wrapped the
        // loaded image, so
        // we return true
        // The loaded image
        // is released by
        // the Wrap function

        return true;
    }
    else
    {
        // In this case, the loaded
        // image differs in either
        // depth or number of channels
        // from this blImage object, so
        // we clone the image and release
        // it

        bool wereWeSuccessfull = this->clone(loadedImage,inCaseOfComplexToRealConversionDoYouWantReal_0_Imaginary_1_OrAbsoluteValue_2);

        // Release the image that
        // was loaded from file

        cvReleaseImage(&loadedImage);

        return wereWeSuccessfull;
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGE4_HPP
