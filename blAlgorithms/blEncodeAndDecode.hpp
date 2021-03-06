#ifndef BL_ENCODEANDDECODE_HPP
#define BL_ENCODEANDDECODE_HPP


//-------------------------------------------------------------------
// FILE:            blEncodeAndDecode.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         Very simple functions based on cvEncodeImage and
//                  cvDecodeImage used to encode and decode
//                  an image to/from a compressed format (specified
//                  at run time by user) staying with the blImage
//                  format
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - cvEncodeImage -- Encodes image to
//                                     compressed format
//                  - cvDecodeImage -- Decodes image from
//                                     compressed format
//
// NOTES:
//
// DATE CREATED:    May/25/2011
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
// FUNCTION:            EncodeImage
//
// ARGUMENTS:           SrcImage
//                      DstImage
//                      Extension
//                      QualityParams
//
// TEMPLATE ARGUMENTS:  - blDataType -- For source image
//                      - blDataType2 -- For destination image
//
// PURPOSE:             Encodes/Compresses a source image into
//                      a destination image based on the chosen
//                      extension, and based on the specified
//                      quality (which ranges from 0 to 100)
//
// DEPENDENCIES:        - blImage
//                      - cvEncodeImage
//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline bool EncodeImage(const blImage<blDataType>& SrcImage,
                        blImage<blDataType2>& DstImage,
                        const std::string& Extension = ".jpg",
                        const int* QualityParams = NULL)
{
    // First encode the source
    // image into a CvMat structure
    CvMat* EncodedImage = cvEncodeImage(Extension.c_str(),
                                        SrcImage,
                                        QualityParams);

    // Then clone the CvMat
    // structure into the
    // destination image
    bool WereWeSuccessful = DstImage.Clone(EncodedImage);

    // Now we need to clean up and
    // release the matrix structure
    if(EncodedImage != NULL)
        cvReleaseMat(&EncodedImage);

    return WereWeSuccessful;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            DecodeImage
//
// ARGUMENTS:           SrcImage
//                      DstImage
//                      IsColor
//
// TEMPLATE ARGUMENTS:  - blDataType -- For source image
//                      - blDataType2 -- For destination image
//
// PURPOSE:             Decodes/Uncompresses a source image into
//                      a destination image where the encoded image
//                      is specified as a grayscale or color image
//
// DEPENDENCIES:        - blImage
//                      - cvDecodeImage
//
// NOTES:               - The encoded image has to have one
//                        channel or it's not going to work
//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline bool DecodeImage(const blImage<blDataType>& SrcImage,
                        blImage<blDataType2>& DstImage,
                        const int& IsColor = CV_LOAD_IMAGE_COLOR)
{
    // First we need to create
    // a CvMat header from the
    // source image
    CvMat MatrixHeader;
    cvGetMat(SrcImage,&MatrixHeader);

    // We can now decode the
    // image into an IplImage*
    IplImage* DecodedImage = cvDecodeImage(&MatrixHeader,IsColor);

    // Finally, we load the
    // decoded image into
    // the destination image
    if(DecodedImage != NULL)
    {
        DstImage.Clone(DecodedImage);

        // Of course we have to clean
        // up and release the IplImage
        cvReleaseImage(&DecodedImage);

        // We're done
        return true;
    }
    else
    {
        // Error -- There was an error
        //          trying to decode
        //          the image, probably
        //          due to the user having
        //          tried to decode an image
        //          that has more than one channel
        return false;
    }
}
//-------------------------------------------------------------------


#endif // BL_ENCODEANDDECODE_HPP
