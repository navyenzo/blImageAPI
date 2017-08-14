#ifndef BL_CAPTUREDEVICE_HPP
#define BL_CAPTUREDEVICE_HPP


//-------------------------------------------------------------------
// FILE:            blCaptureDevice.hpp
// CLASS:           blCaptureDevice
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's CvCapture
//                  class with a smart shared_ptr pointer
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blImage
//                  std::shared_ptr -- From boost c++ libraries
//
// NOTES:
//
// DATE CREATED:    Sep/22/2009
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
class blCaptureDevice
{
public: // Constructors and destructors

    // Default constructor
    blCaptureDevice();

    // Copy constructor
    blCaptureDevice(const blCaptureDevice& CaptureDevice);

    // Virtual destructor
    virtual ~blCaptureDevice()
    {
    }

public: // Public functions

    // Function used to
    // connect to a webcam

    bool                                    connectToCamera(const int& whichCamera,
                                                            const int& width = 320,
                                                            const int& height = 240);

    // Function used to
    // connect to an AVI file

    bool                                    connectToAVIFile(const std::string& AVIFileName);

    // Function used to disconnect
    // the capture device

    void                                    disconnectDevice();

    // Function used to know if
    // capture device is connected

    bool                                    isConnected()const;

    // Funtions used to query
    // images from a video
    // source directly into
    // a blImage structure

    template<typename blDataType>
    bool                                    queryFrame(blImage<blDataType>& frame)const;

    // Functions used to get/set the
    // properties of the capture
    // device (both camera and movie file)

    double                                  getFrameWidth()const;
    double                                  getFrameHeight()const;
    CvSize                                  getFrameSize()const;
    double                                  getFPS()const;

    int                                     setFrameWidth(const double& width);
    int                                     setFrameHeight(const double& height);
    void                                    setFrameSize(const CvSize& size);
    int                                     setFPS(const double& FPS);

    // Functions used to get the
    // properties of the capture
    // device (camera only)

    double                                  getBrightness()const;
    double                                  getContrast()const;
    double                                  getSaturation()const;
    double                                  getHue()const;

    int                                     setBrightness(const double& brightness);
    int                                     setContrast(const double& contrast);
    int                                     setSaturation(const double& saturation);
    int                                     setHue(const double& hue);

    // Functions used to get the
    // properties of the capture
    // device (movie file only)

    double                                  getTotalNumOfFrames()const;
    double                                  getCurrentPositionInMilliseconds()const;
    double                                  getNextFrameIndex()const;
    double                                  getCurrentRelativePosition()const;
    double                                  getFourCharacterCodeOfCodec()const;

    int                                     setCurrentPositionInMilliseconds(const double& CurrentPositionInMilliseconds);
    int                                     setNextFrameIndex(const double& NextFrameIndex);
    int                                     setCurrentRelativePosition(const double& CurrentRelativePosition);
    int                                     setFourCharacterCodeOfCodec(const double& FourCharacterCodeOfCodec);

    // Function used to get
    // the capture device

    const std::shared_ptr<CvCapture>&       getCaptureDevice()const;

    // Function used to convert
    // this object to a pure OpenCV
    // CvCapture device

    operator CvCapture*()
    {
        if(!m_captureDevice)
            return NULL;
        else
            return m_captureDevice.get();
    }

    // Function used to convert
    // this const object
    // to a pure OpenCV
    // CvCapture device

    operator const CvCapture*()const
    {
        if(!m_captureDevice)
            return NULL;
        else
            return m_captureDevice.get();
    }

protected: // Protected variables

    // Smart pointer holding
    // the connected device

    std::shared_ptr<CvCapture>              m_captureDevice;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blCaptureDevice::blCaptureDevice()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blCaptureDevice::blCaptureDevice(const blCaptureDevice& captureDevice)
{
    // We link to the same capture
    // device of the passed object
    // That means this object holds
    // a pointer to the same capture
    // device

    m_captureDevice = captureDevice.getCaptureDevice();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const std::shared_ptr<CvCapture>& blCaptureDevice::getCaptureDevice()const
{
    return m_captureDevice;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blCaptureDevice::disconnectDevice()
{
    // To disconnect the currently
    // connected device and release
    // the capture device, we simply
    // invalidate the shared_ptr pointer

    std::shared_ptr<CvCapture> disconnectedCaptureDevice;
    m_captureDevice = disconnectedCaptureDevice;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blCaptureDevice::connectToCamera(const int& whichCamera,
                                             const int& width,
                                             const int& height)
{
    // Create a capture device
    // and connect it to the camera

    CvCapture* captureDevice = NULL;
    captureDevice = cvCaptureFromCAM(whichCamera);

    // Check if the capture device
    // was connected successfully

    if(captureDevice != NULL)
    {
        // The capture device was
        // successfully connected to
        // the AVI file

        m_captureDevice = std::shared_ptr<CvCapture>(captureDevice,releaseCaptureDevice());

        // Set the desired resolution
        // of the webcam capture

        cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_WIDTH,width);
        cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT,height);

        return true;
    }
    else
    {
        // Error connecting to the camera

        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blCaptureDevice::connectToAVIFile(const std::string& AVIFileName)
{
    // Create a capture device
    // and connect it to the AVI file

    CvCapture* tempCaptureDevice = NULL;
    tempCaptureDevice = cvCaptureFromAVI(AVIFileName.c_str());

    // Check if the capture device
    // was connected successfully

    if(tempCaptureDevice)
    {
        // The capture device was
        // successfully connected
        // to the AVI file

        m_captureDevice = std::shared_ptr<CvCapture>(tempCaptureDevice,releaseCaptureDevice());

        return true;
    }
    else
    {
        // Error -- For some reason, OpenCV
        //          did not successfully load
        //          the file, so we quit

        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blCaptureDevice::isConnected()const
{
    if(!m_captureDevice)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blCaptureDevice::queryFrame(blImage<blDataType>& frame)const
{
    // First check if the
    // capture device has
    // been connected

    if(this->isConnected())
    {
        frame.clone(cvQueryFrame(m_captureDevice.get()),0);
        return true;
    }
    else
    {
        // Error -- The device is not
        //          currently connected
        //          so we simply quit

        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getFrameWidth()const
{
    // If the device is connected
    // query for the frame width
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_WIDTH);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getFrameHeight()const
{
    // If the device is connected
    // query for the frame height
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline CvSize blCaptureDevice::getFrameSize()const
{
    // If the device is connected
    // query for the frame height
    // otherwise return 0

    if(this->isConnected())
    {
        int Width = int(cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_WIDTH));
        int Height = int(cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT));
        return cvSize(Width,Height);
    }
    else
        return cvSize(0,0);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getFPS()const
{
    // If the device is connected
    // query for the frames per second
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FPS);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getBrightness()const
{
    // If the device is connected
    // query for the brightness
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_BRIGHTNESS);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getContrast()const
{
    // If the device is connected
    // query for the contrast
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_CONTRAST);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getSaturation()const
{
    // If the device is connected
    // query for the saturation
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_SATURATION);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getHue()const
{
    // If the device is connected
    // query for the saturation
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_HUE);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getTotalNumOfFrames()const
{
    // If the device is connected
    // query for the total number
    // of frames, otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_COUNT);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getCurrentPositionInMilliseconds()const
{
    // If the device is connected
    // query for current position
    // in miiliseconds, otherwise
    // return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_MSEC);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getNextFrameIndex()const
{
    // If the device is connected
    // query for next frame index
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_FRAMES);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getCurrentRelativePosition()const
{
    // If the device is connected
    // query for the relative position
    // in the movie, where 0 is at the
    // start and 1 is at the end,
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_AVI_RATIO);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline double blCaptureDevice::getFourCharacterCodeOfCodec()const
{
    // If the device is connected
    // query for the Codec code
    // used for this movie file,
    // otherwise return 0

    if(this->isConnected())
        return cvGetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FOURCC);
    else
        return 0.0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setFrameWidth(const double& width)
{
    if(this->isConnected())
        return cvSetCaptureProperty((*this),CV_CAP_PROP_FRAME_WIDTH,width);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setFrameHeight(const double& height)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT,height);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blCaptureDevice::setFrameSize(const CvSize& size)
{
    if(this->isConnected())
    {
        cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_WIDTH,double(size.width));
        cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FRAME_HEIGHT,double(size.height));
        return;
    }
    else
        return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setFPS(const double& FPS)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FPS,FPS);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setBrightness(const double& brightness)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_BRIGHTNESS,brightness);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setContrast(const double& contrast)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_CONTRAST,contrast);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setSaturation(const double& saturation)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_SATURATION,saturation);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setHue(const double& hue)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_HUE,hue);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setCurrentPositionInMilliseconds(const double& currentPositionInMilliseconds)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_MSEC,currentPositionInMilliseconds);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setNextFrameIndex(const double& nextFrameIndex)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_FRAMES,nextFrameIndex);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setCurrentRelativePosition(const double& currentRelativePosition)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_POS_AVI_RATIO,currentRelativePosition);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blCaptureDevice::setFourCharacterCodeOfCodec(const double& fourCharacterCodeOfCodec)
{
    if(this->isConnected())
        return cvSetCaptureProperty(m_captureDevice.get(),CV_CAP_PROP_FOURCC,fourCharacterCodeOfCodec);
    else
        return 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int countNumberOfAvailableCameras(const int& maxNumberOfCamerasToCount = 10)
{
    int cameraCount = 0;

    blCaptureDevice myDevice;

    for(int i = 0; i < maxNumberOfCamerasToCount; ++i)
    {
        if(myDevice.connectToCamera(i))
        {
            ++cameraCount;
            myDevice.disconnectDevice();
        }
        else
            return cameraCount;
    }
}
//-------------------------------------------------------------------


#endif // BL_CAPTUREDEVICE_HPP
