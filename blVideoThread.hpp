#ifndef BL_VIDEOTHREAD_HPP
#define BL_VIDEOTHREAD_HPP


//-------------------------------------------------------------------
// FILE:            blVideoThread.hpp
// CLASS:           blVideoThread
// BASE CLASS:      sf::Thread,blCaptureDevice
//
// PURPOSE:         Based on sf::Thread and blCaptureDevice, it
//                  allows us to retrieve frames from a video source,
//                  which is usually limited to 30 frames per second,
//                  in a parallel thread, allowing the main application
//                  to run at much faster rates
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    sf::Thread -- Derived from sf::Thread to handle
//                                threading in a cross-platform way
//
//                  blCaptureDevice -- We use a capture device to
//                                     grab frames from a video source
//                  blImage -- We use a blImage to store the
//                             last captured frame
//
// NOTES:
//
// DATE CREATED:    Feb/03/2011
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
class blVideoThread : public blCaptureDevice
{
public: // Constructors and destructors

    // Default constructor
    blVideoThread();

    // Destructor
    ~blVideoThread()
    {
        stopCapturingThread();
    }

public: // Public functions

    // Functions used to start,stop,
    // pause and unpause the thread

    void                                        startCapturingThread();
    void                                        stopCapturingThread();
    void                                        pauseCapturingThread();

    // Function used to know whether
    // there's a new image available

    const bool&                                 isNewFrameAvailable()const;

    // Function used to tell the video
    // thread that we grabbed the frame
    // and the it should go ahead and
    // grab another one

    void                                        letThreadQueryAnotherFrame();

    // Function that gets called
    // when thread is running

    virtual void                                threadLoop();

    // Function used to get
    // the captured frame

    const blImage< blColor3<unsigned char> >&   getFrame()const;

protected: // Protected variables

    // Frame image used in this thread

    blImage< blColor3<unsigned char> >          m_frame;

    // Boolean variable used to
    // continue/terminate the thread

    bool                                        m_isCapturingThreadToBeTerminated;

    // Boolean variable used for
    // pausing the video grabbing

    bool                                        m_isCapturingThreadPaused;

    // Boolean variable used
    // to check if new image
    // is available from this thread

    bool                                        m_isNewFrameAvailable;

    // The thread that will
    // keep running the thread
    // loop function

    sf::Thread                                  m_thread;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blVideoThread::blVideoThread() : m_thread(&blVideoThread::threadLoop,this)
{
    // Default all the booleans
    m_isCapturingThreadToBeTerminated = true;
    m_isCapturingThreadPaused = false;
    m_isNewFrameAvailable = false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::startCapturingThread()
{
    // Let's first make sure that we
    // don't try to re-launch the thread
    // after it's already been started
    if(!m_isCapturingThreadToBeTerminated)
    {
        // Error -- Tried to restart a thread
        //          that had already been started
        return;
    }

    // Check if the capture device has been set
    if(this->isConnected())
    {
        // Since we have a capture device, we start the thread
        m_thread.launch();

        // Set boolean so that thread will run
        m_isCapturingThreadToBeTerminated = false;
        return;
    }

    // Error -- In this case, we don't yet
    // have a capture device so we just return

    // Set the boolean in order
    // to terminate the thread
    m_isCapturingThreadToBeTerminated = true;

    return;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::stopCapturingThread()
{
    // Set the boolean in order
    // to terminate the thread
    m_isCapturingThreadToBeTerminated = true;

    // Here we just terminate
    // the running thread
    m_thread.terminate();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::threadLoop()
{
    while(!m_isCapturingThreadToBeTerminated)
    {
        if(!m_isCapturingThreadPaused &&
           !m_isNewFrameAvailable &&
           !m_isCapturingThreadToBeTerminated)
        {
            // If we cannot successfully query
            // a frame, then we just stop the
            // capturing thread
            if(this->isConnected())
            {
                // Query a new frame and
                // signal that a new frame
                // is available
                this->queryFrame(m_frame);
                m_isNewFrameAvailable = true;
            }
            else
            {
                // The capturing device got
                // disconnected, so we stop
                // this capturing thread
                m_isCapturingThreadToBeTerminated = true;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::pauseCapturingThread()
{
    // If the thread is currently paused, then
    // we unpause otherwise we pause it
    if(m_isCapturingThreadPaused)
        m_isCapturingThreadPaused = false;
    else
        m_isCapturingThreadPaused = true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const blImage< blColor3<unsigned char> >& blVideoThread::getFrame()const
{
    return m_frame;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const bool& blVideoThread::isNewFrameAvailable()const
{
    return m_isNewFrameAvailable;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoThread::letThreadQueryAnotherFrame()
{
    m_isNewFrameAvailable = false;
}
//-------------------------------------------------------------------


#endif // BL_VIDEOTHREAD_HPP
