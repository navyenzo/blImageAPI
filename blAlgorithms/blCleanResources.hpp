#ifndef BL_CLEANRESOURCES_HPP
#define BL_CLEANRESOURCES_HPP


//-------------------------------------------------------------------
// FILE:            blCleanResources.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         Special functions needed to handle the
//                  automatic release of various resources
//                  such as OpenCV's IplImage when used with
//                  shared_ptr pointers
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    IplImage -- Image structure from opencv
//                  cvReleaseImage -- To release the image
//                  CvCapture -- Capture device structure from opencv
//                  cvReleaseCapture -- To release the capture device
//                  CvMemStorage -- Memory storage structure from opencv
//                  cvReleaseMemStorage -- To realase the memory storage
//
// NOTES:           - These functors can be used with shared_ptr to
//                    automatically release opencv's memory objects
//                    on pointer destruction
//
// DATE CREATED:    May/30/2010
// DATE UPDATED:
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
// Functor used to release an IplImage
//-------------------------------------------------------------------
class releaseImage
{
public:

    // Overloaded operator
    // used to release an
    // IplImage

    void operator()(IplImage*& img)
    {
        // Check if we have
        // an image

        if(!img)
            return;

        // Release the image

        cvReleaseImage(&img);

        // Nullify the pointer

        img = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV Capture Device
//-------------------------------------------------------------------
class releaseCaptureDevice
{
public:

    // Overloaded operator
    // used to release a
    // capture device

    void operator()(CvCapture*& captureDevice)
    {
        // Check if we have
        // a capture device

        if(!captureDevice)
            return;

        // Release the capture
        // device

        cvReleaseCapture(&captureDevice);

        // Nullify the pointer

        captureDevice = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV Video Writer
//-------------------------------------------------------------------
class releaseVideoWriter
{
public:

    // Overloaded operator
    // used to release a
    // video writer

    void operator()(CvVideoWriter*& videoWriter)
    {
        // Check if we have
        // a video writer

        if(!videoWriter)
            return;

        // Release the video
        // writer

        cvReleaseVideoWriter(&videoWriter);

        // Nullify the pointer

        videoWriter = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV memory storage
//-------------------------------------------------------------------
class releaseMemStorage
{
public:

    // Overloaded operator
    // used to release a
    // memory storage

    void operator()(CvMemStorage*& memoryStorage)
    {
        // Check if we have
        // a memory storage

        if(!memoryStorage)
            return;

        // Release the memory
        // storage

        cvReleaseMemStorage(&memoryStorage);

        // Nullify the pointer

        memoryStorage = NULL;
    }
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Functor used to release an OpenCV file storage
//-------------------------------------------------------------------
class releaseFileStorage
{
public:

    // Overloaded operator
    // used to release a
    // file storage

    void operator()(CvFileStorage*& FileStorage)
    {
        // Check if we have
        // a file storage

        if(!FileStorage)
            return;

        // Release the file
        // storage

        cvReleaseFileStorage(&FileStorage);

        // Nullify the pointer

        FileStorage = NULL;
    }
};
//-------------------------------------------------------------------


#endif // BL_CLEANRESOURCES_HPP
