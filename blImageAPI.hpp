#ifndef BL_IMAGEAPI_HPP
#define BL_IMAGEAPI_HPP


//-------------------------------------------------------------------
// FILE:            blImageAPI.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of data structures and image
//                  algorithms to use for Machine Vision in
//                  robotics experiments
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
// NOTES:
//
// DATE CREATED:    May/30/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------

#include <memory>

#include <blIteratorAPI/blIteratorAPI.hpp>

#include <blMathAPI/blMathAPI.hpp>

#include <opencv.hpp>

//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
namespace blImageAPI
{
    // Special functions needed to handle the
    // automatic release of various resources
    // such as OpenCV's IplImage when used with
    // shared_ptr pointers

    #include "blAlgorithms/blCleanResources.hpp"



    // A collection of functions used to
    // simplify converting images from one
    // form to another

    #include "blAlgorithms/blConversions.hpp"



    // A simple and efficient color structure of three
    // components saved in a Blue,Green,Red sequence

    #include "blCore/blColor3.hpp"



    // Based on blColor3, adds an alpha transparency value

    #include "blCore/blColor4.hpp"



    // A base class used to wrap OpenCV's image
    // structure IplImage with a shared_ptr
    // and provide a fast, generic and easy way
    // to access image pixels, while remaining
    // interchangable with IplImage in Opencv's
    // algorithms

    #include "blCore/blImage0.hpp"



    // These files define basic forward and
    // reverse random access iterators to
    // allow the use of blImage class in stl
    // algorithms

    #include "blCore/blImageIterator.hpp"
    #include "blCore/blImageReverseIterator.hpp"



    // Based on blImage0, this class adds linear,
    // circular as well as reverse linear and
    // reverse circular iterators to facilitate
    // the use of the blImage class in stl-like
    // algorithms

    #include "blCore/blImage1.hpp"



    // Based on blImage1, this class adds functions
    // used to create a new image of a specified size
    // and to initialize it to a specified value

    #include "blCore/blImage2.hpp"



    // This file defines circular forward and
    // reverse random access iterators to allow
    // the use of blImage as a circular buffer
    // in stl algorithms used to deal with
    // non-circular buffers
    // For example, a programmer could let an
    // stl algorithm transverse a blImage n times
    // until it reaches the "end"
    // These iterators could also be used to let
    // stl algorithm transverse the ROI of an image
    // that is a subset of an image simply by
    // passing it these circular iterators

    #include "blCore/blImageCircularIterators.hpp"



    // Based on blImage2, this class adds functions to
    // clone other images, as well as functions to
    // wrap IplImage images with blImage objects without
    // making copies of the image data

    #include "blCore/blImage3.hpp"



    // Based on blImage3, this class adds functions
    // that can load/save images from/to files

    #include "blCore/blImage4.hpp"



    // Based on blImage4, this class adds functions
    // used to wrap or clone a static 2d array with
    // a blImage

    #include "blCore/blImage5.hpp"



    // Based on blImage5, this class adds functions
    // used to copy individual row and column vectors
    // from the image or to copy a sub-image into
    // another one
    // It also adds a function to sample the image
    // at specified intervals into a smaller or
    // bigger image without any interpolation
    // It also adds functions to add/subtract/multiply
    // and divide the image by a datapoint or a datapoint
    // by an image (Individual pixels)

    #include "blCore/blImage6.hpp"



    // Based on blImageX, (where X is the highest
    // number in the blImage series), this class
    // adds constructors and assignment operators,
    // while allowing us to add more functionality
    // at a later point by adding more blImageX classes

    #include "blCore/blImage.hpp"



    // A collection of overloaded operators and functions
    // I developed to handle images just like matrices, so
    // as to make code very readable

    #include "blCore/blImageOperators.hpp"



    // A base class used to wrap OpenCV's CvCapture
    // class with a smart shared_ptr pointer

    #include "blCore/blCaptureDevice.hpp"



    // A base class used to wrap OpenCV's CvMemStorage
    // with boost::shared_ptr

    #include "blCore/blMemStorage.hpp"



    // A base class used to wrap OpenCV's
    // CvVideoWriter class with a smart
    // shared_ptr pointer

    #include "blCore/blVideoWriter.hpp"



    // Very simple functions based on cvEncodeImage and
    // cvDecodeImage used to encode and decode
    // an image to/from a compressed format (specified
    // at run time by user) staying with the blImage
    // format

    #include "blAlgorithms/blEncodeAndDecode.hpp"



    // A collection of functions used to augment
    // images in various ways to make one "big"
    // image.

    #include "blAlgorithms/blTiling.hpp"



    // A collection of simple functions used to
    // serialize/unserialize images to/from a
    // file storage

    #include "blAlgorithms/blSerialize.hpp"



    // A collection of simple functions I created to
    // convert images from and to the HSV color space

    #include "blAlgorithms/blHSV.hpp"



    // A collection of algorithms I wrote to blend
    // two images that emulate adobe photoshop's
    // blending modes

    #include "blAlgorithms/blBlending.hpp"



    // To use the blTexture class you
    // have to define the following macro

    #ifdef USE_BL_TEXTURE

        // A wrapper for an opengl texture I wrote that
        // generate textures from opencv IplImage or
        // from image files

        #include "blCore/blTexture.hpp"

    #endif



    // A simple function used to take an image and
    // make it look like it was sketched using a pencil
    // which can be in B/W or in Color
    //#include "blImagePencilSketch.hpp"



    // A collection of simple functions I wrote
    // to shift images up, down, left, right.
    // I will be adding more functions as I build them

    #include "blAlgorithms/blImageShifting.hpp"



    // A collection of algorithms I wrote to use
    // the blImageAPI for signal processing

    #include "blAlgorithms/blSignalProcessing.hpp"



    // A simple class that wraps CvFont and
    // provides easy to use text function to
    // write on images
    //#include "blImageFont.hpp"

    /*// Based on blImage, this class treats the
    // image data as generic data builds plots
    // of that data
    #include "blPlotPoints.hpp"

    // Based on blPlotPoints, this class adds the
    // ability to translate, rotate and scale the
    // set of plot points.
    #include "blPlotPoints2.hpp"

    // Based on blPlotPoints2, this class adds the
    // ability to "play" through data points,
    // based on the specified sampling period
    #include "blPlotPoints3.hpp"

    // Based on blPlotPoints3, this class adds
    // an image and a texture to draw the plots
    // onto
    #include "blPlotPoints4.hpp"

    // A class used to plot data on an image
    #include "blImagePlot1.hpp"

    // Based on blPlot, it adds the property
    // of sampling period at which the data
    // was acquired.  It also adds the ability
    // to "play" through the data
    #include "blImagePlot2.hpp"

    // A collection of simple functions used
    // to plot data on an image
    #include "blImagePlotting.hpp"

    // A class used to draw grids on an image
    #include "blGrid.hpp"*/



    // To use the blVideoThread class you
    // have to de fine the following macro
    #ifdef USE_BL_VIDEOTHREAD

        // Based on sf::Thread and blCaptureDevice, it
        // allows us to retrieve frames from a video source,
        // which is usually limited to 30 frames per second,
        // in a parallel thread, allowing the main application
        // to run at much faster rates
        #include "blVideoThread.hpp"

        // Based on blVideoThread, this class has a buffer
        // that is always available and the same size,
        // and that new pixel data is fed serially to the
        // frame buffer
        #include "blVideoThread2.hpp"

    #endif
}
//-------------------------------------------------------------------


#endif // BL_IMAGEAPI_HPP
