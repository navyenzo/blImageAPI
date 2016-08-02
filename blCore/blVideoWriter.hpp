#ifndef BL_VIDEOWRITER_HPP
#define BL_VIDEOWRITER_HPP


//-------------------------------------------------------------------
// FILE:            blVideoWriter.hpp
// CLASS:           blVideoWriter
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's
//                  CvVideoWriter class with a smart
//                  std::shared_ptr pointer
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
class blVideoWriter
{
public: // Constructors and destructors

    // Default constructor
    blVideoWriter();

    // Copy constructor
    blVideoWriter(const blVideoWriter& VideoWriter);

    // Virtual destructor
    virtual ~blVideoWriter()
    {
    }

public: // Public functions

    // Function used to create
    // the video writer
    bool                                        Create(const std::string& Filename,
                                                       const int& FourCharacterCodecCode,
                                                       const double& FPS,
                                                       const CvSize& FrameSize,
                                                       const int& IsColor = 1);

    // Function used to close
    // the currently open
    // video writer
    void                                        Close();

    // Function used to write
    // a frame to the video
    // writer
    template<typename blType>
    int                                         WriteFrame(const blImage<blType>& Frame);

    // Function used to query
    // whether the video writer
    // is valid
    bool                                        IsVideoWriterValid()const;

    // Function used to get
    // the std::shared_ptr to the
    // video writer
    const std::shared_ptr<CvVideoWriter>&       GetVideoWriter()const;

    // Function used to convert this object
    // to a pure OpenCV CvVideoWriter device
    operator CvVideoWriter*()
    {
        if(!m_VideoWriter)
            return NULL;
        else
            return m_VideoWriter.get();
    }

    // Function used to convert this const object
    // to a pure OpenCV CvVideoWriter device
    operator const CvVideoWriter*()const
    {
        if(!m_VideoWriter)
            return NULL;
        else
            return m_VideoWriter.get();
    }

protected: // Protected variables

    // Image variable
    std::shared_ptr<CvVideoWriter>              m_VideoWriter;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blVideoWriter::blVideoWriter()
{
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blVideoWriter::blVideoWriter(const blVideoWriter& VideoWriter)
{
    // We link to the same capture
    // device of the passed object
    // That means this object holds
    // a pointer to the same capture
    // device
    m_VideoWriter = VideoWriter.GetVideoWriter();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const std::shared_ptr<CvVideoWriter>& blVideoWriter::GetVideoWriter()const
{
    return m_VideoWriter;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blVideoWriter::IsVideoWriterValid()const
{
    if(!m_VideoWriter)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blVideoWriter::Create(const std::string& Filename,
                                  const int& FourCharacterCodecCode,
                                  const double& FPS,
                                  const CvSize& FrameSize,
                                  const int& IsColor)
{
    // First we try to
    // create the video
    // writer
    CvVideoWriter* MyVideoWriter = cvCreateVideoWriter(Filename.c_str(),
                                                       FourCharacterCodecCode,
                                                       FPS,
                                                       FrameSize,
                                                       IsColor);

    if(MyVideoWriter == NULL)
    {
        // Error -- We failed to create a
        //          valide video writer, so
        //          we quit and return false
        return false;
    }
    else
    {
        // In this case, we successfully
        // got a valid video writer, so
        // we store it into the std::shared_ptr
        // pointer and return true
        m_VideoWriter = std::shared_ptr<CvVideoWriter>(MyVideoWriter,releaseVideoWriter());
        return true;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blVideoWriter::Close()
{
    // To release/close the video writer
    // we simply release the std::shared_ptr
    // pointer
    std::shared_ptr<CvVideoWriter> CloseVideoWriter;
    m_VideoWriter = CloseVideoWriter;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blType>
inline int blVideoWriter::WriteFrame(const blImage<blType>& Frame)
{
    return cvWriteFrame((*this),Frame);
}
//-------------------------------------------------------------------


#endif // BL_VIDEOWRITER_HPP
