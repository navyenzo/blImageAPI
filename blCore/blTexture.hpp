#ifndef BL_TEXTURE_HPP
#define BL_TEXTURE_HPP


//-------------------------------------------------------------------
// file:            blTexture.hpp
// CLASS:           blTexture
// BASE CLASS:      None
// PURPOSE:         A wrapper for an opengl texture I wrote that
//                  generate textures from opencv IplImage or
//                  from image files
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
// DEPENDENCIES:    blImage -- To load opencv images to opengl textures
// NOTES:
// DATE CREATED:    Jun/03/2010
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
class blTexture
{
public: // Constructors and destructors

    // Default constructor

    blTexture();

    // copy constructor

    blTexture(const blTexture& texture);

    // Destructor

    ~blTexture()
    {
        // Release the texture

        this->releaseTexture();
    }

public: // Public functions

    // Assignment operator

    blTexture&                              operator=(const blTexture& texture);

    // Functions used to
    // bind/unbind to/from
    // this texture

    void                                    bind()const;
    static void                             unbind();

    // Function used to check
    // whether the texture we're
    // holding is a valid openGL
    // texture object

    bool                                    isTexture()const;

    // Function used to
    // get/set the texture
    // object id (used by
    // opengl as the texture
    // handle)

    const GLuint&                           getTexture()const;
    void                                    setTexture(const GLuint& texture);

    // Function used to release
    // the texture object

    void                                    releaseTexture();

    // Functions used to get
    // the texture properties

    int                                     getNumOfTextureLevels()const;
    int                                     getWidth(const int& levelOfDetail = 0)const;
    int                                     getHeight(const int& levelOfDetail = 0)const;
    int                                     getDepthFormat(const int& levelOfDetail = 0)const;
    int                                     getChannelFormat(const int& levelOfDetail = 0)const;

    // Function used to
    // get the maximum
    // allowable size for
    // the texture

    GLint                                   getMaximumSize()const;

    // Function used to
    // create an empty
    // texture

    template<typename blDataType = blColor3<float> >

    void                                    createEmptyTexture(const int& width,
                                                               const int& height,
                                                               const bool& areMipmapsToBeGenerated = false,
                                                               const GLenum& textureWrapS = GL_REPEAT,
                                                               const GLenum& textureWrapT = GL_REPEAT,
                                                               const GLenum& textureMagFilter = GL_LINEAR,
                                                               const GLenum& textureMinFilter = GL_LINEAR);

    // Function used to
    // load an image to
    // a texture

    template<typename blDataType = blColor3<float> >

    bool                                    loadImagetoTexture(const blImage<blDataType>& image,
                                                               const bool& isTextureToBeReused = false,
                                                               const bool& areMipmapsToBeGenerated = false,
                                                               const GLenum& textureWrapS = GL_REPEAT,
                                                               const GLenum& textureWrapT = GL_REPEAT,
                                                               const GLenum& textureMagFilter = GL_LINEAR,
                                                               const GLenum& textureMinFilter = GL_LINEAR);

    template<typename blDataType = blColor3<float> >

    bool                                    loadImagetoTexture2(const blImage<blDataType>& image,
                                                                const int& textureWidth,
                                                                const int& textureHeight,
                                                                const bool& isTextureToBeReused = false,
                                                                const bool& areMipmapsToBeGenerated = false,
                                                                const int& resizingInterpolationMethod = CV_INTER_LINEAR,
                                                                const GLenum& textureWrapS = GL_REPEAT,
                                                                const GLenum& textureWrapT = GL_REPEAT,
                                                                const GLenum& textureMagFilter = GL_LINEAR,
                                                                const GLenum& textureMinFilter = GL_LINEAR);

    // Function used to
    // load an image
    // from a file to
    // a texture

    template<typename blDataType = blColor3<float> >

    bool                                    loadImageFromFileToTexture(const char* filename,
                                                                       const bool& isTextureToBeReused = false,
                                                                       const bool& areMipmapsToBeGenerated = false,
                                                                       const GLenum& textureWrapS = GL_REPEAT,
                                                                       const GLenum& textureWrapT = GL_REPEAT,
                                                                       const GLenum& textureMagFilter = GL_LINEAR,
                                                                       const GLenum& textureMinFilter = GL_LINEAR,
                                                                       const bool& shouldImageBeFlipped = false,
                                                                       const int& imageFlipMode = 0,
                                                                       const int& howToReadImageColorAndDepth =
                                                                       CV_LOAD_IMAGE_ANYDEPTH |
                                                                       CV_LOAD_IMAGE_ANYCOLOR);

    template<typename blDataType = blColor3<float> >
    bool                                    loadImageFromFileToTexture2(const char* filename,
                                                                        const int& textureWidth,
                                                                        const int& textureHeight,
                                                                        const bool& isTextureToBeReused = false,
                                                                        const bool& areMipmapsToBeGenerated = false,
                                                                        const int& resizingInterpolationMethod = CV_INTER_LINEAR,
                                                                        const GLenum& textureWrapS = GL_REPEAT,
                                                                        const GLenum& textureWrapT = GL_REPEAT,
                                                                        const GLenum& textureMagFilter = GL_LINEAR,
                                                                        const GLenum& textureMinFilter = GL_LINEAR,
                                                                        const bool& shouldImageBeFlipped = false,
                                                                        const int& imageFlipMode = 0,
                                                                        const int& howToReadImageColorAndDepth =
                                                                        CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

    // Function used to
    // save a texture to
    // an image

    template<typename blDataType = blColor3<float> >
    void                                    saveTextureToImage(blImage<blDataType>& image,
                                                               const GLint& level = 0);

    // Function used to
    // save a texture to
    // a file

    template<typename blDataType = blColor3<float> >
    void                                    saveTextureToFile(const char* filepath,
                                                              const GLint& level = 0);

    // Functions used to render to
    // this texture object

    void                                    copyBufferToThisTextureAtASpecifiedLevel(const int& levelOfDetail);
    void                                    copyBufferToThisTextureAtAllLevels();

protected: // Protected variables

    // texture object

    GLuint                                  m_texture;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blTexture::blTexture()
{
    m_texture = 0;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blTexture::blTexture(const blTexture& texture)
{
    setTexture(texture.getTexture());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blTexture& blTexture::operator=(const blTexture& texture)
{
    setTexture(texture.getTexture());

    return(*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::releaseTexture()
{
    // Delete the texture
    // if it has been
    // created

    if(glIsTexture(m_texture))
        glDeleteTextures(1,&m_texture);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline bool blTexture::isTexture()const
{
    return glIsTexture(m_texture);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::bind()const
{
    glBindTexture(GL_TEXTURE_2D,m_texture);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::unbind()
{
    glBindTexture(GL_TEXTURE_2D,0);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const GLuint& blTexture::getTexture()const
{
    return m_texture;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::setTexture(const GLuint& texture)
{
    m_texture = texture;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blTexture::createEmptyTexture(const int& width,
                                          const int& height,
                                          const bool& areMipmapsToBeGenerated,
                                          const GLenum& textureWrapS,
                                          const GLenum& textureWrapT,
                                          const GLenum& textureMagFilter,
                                          const GLenum& textureMinFilter)
{
    // Let's create an empty
    // image of the correct size

    blImage< blColor3<float> > tempImage(height,width);

    // Let's create the texture
    // from the image

    loadImagetoTexture<blDataType>(tempImage,
                                   true,
                                   areMipmapsToBeGenerated,
                                   textureWrapS,
                                   textureWrapT,
                                   textureMagFilter,
                                   textureMinFilter);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blTexture::saveTextureToFile(const char* filepath,
                                         const GLint& level)
{
    // We first create a temporary
    // image and save this texture
    // to that image

    blImage<blDataType> tempImage;
    this->saveTextureToImage(tempImage,level);

    // Finally we save the image
    // at the file path passed to
    // this function

    cvSaveImage(filepath,tempImage);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void blTexture::saveTextureToImage(blImage<blDataType>& image,
                                          const GLint& level)
{
    // Step 1:  Check if this
    //          texture is a valid
    //          texture object

    if(!glIsTexture(m_texture))
    {
        // Error -- This texture is
        //          not a valid texture
        //          object, so we simply
        //          quit

        return;
    }

    // Step 2:  bind to this
    //          texture object

    glBindTexture(GL_TEXTURE_2D,m_texture);

    // Step 3:  Determine the
    //          pixel format

    GLenum channelFormat;

    // Depending on the
    // pixel format, we
    // pick the channel
    // format of the
    // image

    switch(image.getNumOfChannels())
    {
    case 1:
        channelFormat = GL_LUMINANCE;
        break;

    case 3:
        channelFormat = GL_BGR_EXT;
        break;

    case 4:
        channelFormat = GL_BGRA_EXT;
        break;

    default:
        // Error -- Did not understand  the
        //          format, so we default
        //          to GL_LUMINANCE

        channelFormat = GL_LUMINANCE;
        break;
    }

    // Step 4:  Determine the
    //          depth format

    GLenum depthFormat;

    switch(image.getDepth())
    {
    case IPL_DEPTH_8U:
        depthFormat = GL_UNSIGNED_BYTE;
        break;

    case IPL_DEPTH_8S:
        depthFormat = GL_BYTE;
        break;

    case IPL_DEPTH_16U:
        depthFormat = GL_UNSIGNED_SHORT;
        break;

    case IPL_DEPTH_16S:
        depthFormat = GL_SHORT;
        break;

    case IPL_DEPTH_32S:
        depthFormat = GL_INT;
        break;

    case IPL_DEPTH_32F:
        depthFormat = GL_FLOAT;
        break;

    case IPL_DEPTH_64F:

        // Error -- Most GPUs don't support
        //          double and so the texture
        //          won't load, for this we
        //          make the depth format float.
        //          Note that the texture will
        //          be streched

        depthFormat = GL_FLOAT;
        break;

    default:

        // Error -- Did not understand format,
        //          so we default the format
        //          to GL_UNSIGNED_BYTE

        depthFormat = GL_UNSIGNED_BYTE;
        break;
    }

    // Step 5:  We need to make sure that
    //          the image is the correct size
    if(image.size1() != this->getHeight() ||
       image.size2() != this->getWidth())
    {
        image.CreateImage(this->getHeight(),
                          this->getWidth());
    }

    // Step 6:  Finally we get the
    //          image data into the
    //          image itself

    glGetTexImage(GL_TEXTURE_2D,
                  level,
                  channelFormat,
                  depthFormat,
                  image[0]);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blTexture::loadImagetoTexture(const blImage<blDataType>& image,
                                          const bool& isTextureToBeReused,
                                          const bool& areMipmapsToBeGenerated,
                                          const GLenum& textureWrapS,
                                          const GLenum& textureWrapT,
                                          const GLenum& textureMagFilter,
                                          const GLenum& textureMinFilter)
{
    // Check if the image
    // is of size zero

    if(image.size() <= 0)
    {
        // Error -- Tried to upload an
        //          image into a texture
        //          using an image of size
        //          zero

        return false;
    }

    // set the color
    // format of the
    // texture depending
    // on the number
    // of channels
    // of the image

    GLenum channelFormat;

    switch(image.getNumOfChannels())
    {
    case 1:

        channelFormat = GL_LUMINANCE;
        break;

    case 3:

        channelFormat = GL_BGR;
        break;

    case 4:

        channelFormat = GL_BGRA;
        break;

    default:

        // Error -- Did not understand
        //          format, so we default
        //          to GL_LUMINANCE

        channelFormat = GL_LUMINANCE;
        break;
    }

    // set the format of
    // the data sent to
    // to the texture
    // depending on the
    // image depth

    GLenum depthFormat;

    switch(image.getDepth())
    {
    case IPL_DEPTH_8U:

        depthFormat = GL_UNSIGNED_BYTE;
        break;

    case IPL_DEPTH_8S:

        depthFormat = GL_BYTE;
        break;

    case IPL_DEPTH_16U:

        depthFormat = GL_UNSIGNED_SHORT;
        break;

    case IPL_DEPTH_16S:

        depthFormat = GL_SHORT;
        break;

    case IPL_DEPTH_32S:

        depthFormat = GL_INT;
        break;

    case IPL_DEPTH_32F:

        depthFormat = GL_FLOAT;
        break;

    case IPL_DEPTH_64F:

        // Error -- Most GPUs don't
        //          support double and
        //          so the texture won't
        //          load, for this we
        //          make the depth format
        //          float.
        //          Note that the texture
        //          will be streched

        depthFormat = GL_FLOAT;
        break;

    default:

        // Error -- Did not understand
        //          format, so we default
        //          to GL_UNSIGNED_BYTE

        depthFormat = GL_UNSIGNED_BYTE;
        break;
    }

    // Enable texturing

    glEnable(GL_TEXTURE_2D);

    // If the user has
    // specified to NOT
    // reuse the existing
    // texture object, or
    // if we don't have an
    // existing texture object,
    // then we generate a
    // new one

    if(!isTextureToBeReused || m_texture == 0)
    {
        // If the current
        // texture object
        // is a valid texture
        // then delete it

        if(glIsTexture(m_texture))
            glDeleteTextures(1,&m_texture);

        // Now we generate
        // the new texture
        // object

        glGenTextures(1,&m_texture);
    }
    else
    {
        // In this case,
        // we know that
        // we want to reuse
        // the current
        // texture object,
        // but if it's not
        // a valid texture
        // object, then we
        // have to generate
        // a new one anyway

        if(!glIsTexture(m_texture))
        {
            glGenTextures(1,&m_texture);
        }
    }

    // bind to the
    // texture object

    glBindTexture(GL_TEXTURE_2D,m_texture);

    // set the texturing
    // environment variables

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glPixelStorei(GL_UNPACK_ALIGNMENT,2);
    glTexEnvi(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // get the total number
    // of available texture
    // levels

    int numOfTextureLevels = getNumOfTextureLevels();

    if(isTextureToBeReused)
    {
        if((areMipmapsToBeGenerated && (numOfTextureLevels > 0)) ||
           (!areMipmapsToBeGenerated))
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,textureWrapS);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,textureWrapT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,textureMagFilter);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,textureMinFilter);

            if(numOfTextureLevels > 0)
            {
                int textureWidth = 0;
                int textureHeight = 0;

                // We load the image
                // for each available
                // texture level

                for(int level = 0; level < numOfTextureLevels; ++level)
                {
                    textureWidth = getWidth(level);
                    textureHeight = getHeight(level);

                    if(textureWidth <= image.size2() &&
                       textureHeight <= image.size1())
                    {
                        // Make sure that
                        // the texture has
                        // the correct size
                        // depth and channel
                        // formats

                        glTexSubImage2D(GL_TEXTURE_2D,
                                        level,
                                        0,0,
                                        textureWidth,
                                        textureHeight,
                                        channelFormat,
                                        depthFormat,
                                        &image(0));
                    }
                    else
                    {
                        glTexImage2D(GL_TEXTURE_2D,
                                     level,
                                     GL_RGBA,
                                     image.size2(),
                                     image.size1(),
                                     0,
                                     channelFormat,
                                     depthFormat,
                                     &image(0));
                    }
                }
            }
            else
            {
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGBA,
                             image.size2(),
                             image.size1(),
                             0,
                             channelFormat,
                             depthFormat,
                             &image(0));
            }
        }
        else
        {
            gluBuild2DMipmaps(GL_TEXTURE_2D,
                              GL_RGBA,
                              image.size2(),
                              image.size1(),
                              channelFormat,
                              depthFormat,
                              &image(0));
        }
    }
    else
    {
        if(areMipmapsToBeGenerated)
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,textureWrapS);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,textureWrapT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,textureMagFilter);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,textureMinFilter);
            gluBuild2DMipmaps(GL_TEXTURE_2D,
                              GL_RGBA,
                              image.size2(),
                              image.size1(),
                              channelFormat,
                              depthFormat,
                              &image(0));
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,textureWrapS);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,textureWrapT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,textureMagFilter);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,textureMinFilter);
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_RGBA,
                         image.size2(),
                         image.size1(),
                         0,
                         channelFormat,
                         depthFormat,
                         &image(0));
        }
    }

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blTexture::loadImagetoTexture2(const blImage<blDataType>& image,
                                           const int& textureWidth,
                                           const int& textureHeight,
                                           const bool& isTextureToBeReused,
                                           const bool& areMipmapsToBeGenerated,
                                           const int& resizingInterpolationMethod,
                                           const GLenum& textureWrapS,
                                           const GLenum& textureWrapT,
                                           const GLenum& textureMagFilter,
                                           const GLenum& textureMinFilter)
{
    if(image.size1() != textureHeight ||
       image.size2() != textureWidth)
    {
        // The image does
        // not match the
        // specified size,
        // so we resize it

        blImage<blDataType> resizedImage(textureHeight,textureWidth);

        cvResize(image,resizedImage,resizingInterpolationMethod);

        return ( loadImagetoTexture(resizedImage,
                                    isTextureToBeReused,
                                    areMipmapsToBeGenerated,
                                    textureWrapS,
                                    textureWrapT,
                                    textureMagFilter,
                                    textureMinFilter) );
    }
    else
    {
        return ( loadImagetoTexture(image,
                                    isTextureToBeReused,
                                    areMipmapsToBeGenerated,
                                    textureWrapS,
                                    textureWrapT,
                                    textureMagFilter,
                                    textureMinFilter) );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blTexture::loadImageFromFileToTexture(const char* filename,
                                                  const bool& isTextureToBeReused,
                                                  const bool& areMipmapsToBeGenerated,
                                                  const GLenum& textureWrapS,
                                                  const GLenum& textureWrapT,
                                                  const GLenum& textureMagFilter,
                                                  const GLenum& textureMinFilter,
                                                  const bool& shouldImageBeFlipped,
                                                  const int& imageFlipMode,
                                                  const int& howToReadImageColorAndDepth)
{
    // Let's create
    // an empty image

    blImage<blDataType> image;

    // Let's try to
    // load the image
    // from the specified
    // location

    if( image.loadImageFromFile(filename,
                                shouldImageBeFlipped,
                                imageFlipMode,
                                howToReadImageColorAndDepth) )
    {
        // Now that we have
        // loaded an image
        // we just use the
        // above function to
        // load it into this
        // texture object

        return ( loadImagetoTexture(image,
                                    isTextureToBeReused,
                                    areMipmapsToBeGenerated,
                                    textureWrapS,
                                    textureWrapT,
                                    textureMagFilter,
                                    textureMinFilter) );
    }
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blTexture::loadImageFromFileToTexture2(const char* filename,
                                                   const int& textureWidth,
                                                   const int& textureHeight,
                                                   const bool& isTextureToBeReused,
                                                   const bool& areMipmapsToBeGenerated,
                                                   const int& resizingInterpolationMethod,
                                                   const GLenum& textureWrapS,
                                                   const GLenum& textureWrapT,
                                                   const GLenum& textureMagFilter,
                                                   const GLenum& textureMinFilter,
                                                   const bool& shouldImageBeFlipped,
                                                   const int& imageFlipMode,
                                                   const int& howToReadImageColorAndDepth)
{
    // Let's create
    // an empty image

    blImage<blDataType> image;

    // Let's try to
    // load the image
    // from the specified
    // location

    if( image.loadImageFromFile(filename,
                                shouldImageBeFlipped,
                                imageFlipMode,
                                howToReadImageColorAndDepth) )
    {
        // Now that we have
        // loaded an image
        // we just use the
        // above function to
        // load it into this
        // texture object

        return ( loadImagetoTexture2(image,
                                     isTextureToBeReused,
                                     areMipmapsToBeGenerated,
                                     textureWidth,
                                     textureHeight,
                                     resizingInterpolationMethod,
                                     textureWrapS,
                                     textureWrapT,
                                     textureMagFilter,
                                     textureMinFilter) );
    }
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blTexture::getWidth(const int& levelOfDetail)const
{
    // First we check
    // if we have a
    // valid texture
    // object
    if(glIsTexture(m_texture))
    {
        // We then bind to
        // the texture object
        glBindTexture(GL_TEXTURE_2D,m_texture);

        // Finally we obtain
        // its width
        int width;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,
                                 levelOfDetail,
                                 GL_TEXTURE_WIDTH,
                                 &width);

        return width;
    }
    else
    {
        // In this case we
        // did not have a
        // valid texture
        // object, so we
        // return 0 as its
        // width

        return 0;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blTexture::getHeight(const int& levelOfDetail)const
{
    // First we check
    // if we have a
    // valid texture
    // object
    if(glIsTexture(m_texture))
    {
        // We then bind to
        // the texture object
        glBindTexture(GL_TEXTURE_2D,m_texture);

        // Finally we obtain
        // its height
        int height;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,levelOfDetail,GL_TEXTURE_HEIGHT,&height);

        return height;
    }
    else
    {
        // In this case we
        // did not have a
        // valid texture
        // object, so we
        // return 0 as its
        // height

        return 0;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blTexture::getDepthFormat(const int& levelOfDetail)const
{
    // First we check
    // if we have a
    // valid texture
    // object

    if(glIsTexture(m_texture))
    {
        // We then bind to
        // the texture object

        glBindTexture(GL_TEXTURE_2D,m_texture);

        // Finally we obtain
        // its depth format

        int depthFormat;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,levelOfDetail,GL_TEXTURE_DEPTH,&depthFormat);

        return depthFormat;
    }
    else
    {
        // In this case we
        // did not have a
        // valid texture
        // object, so we
        // return 0 as its
        // depth format

        return 0;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blTexture::getChannelFormat(const int& levelOfDetail)const
{
    // First we check
    // if we have a
    // valid texture
    // object

    if(glIsTexture(m_texture))
    {
        // We then bind to
        // the texture object

        glBindTexture(GL_TEXTURE_2D,m_texture);

        // Finally we obtain
        // its channel format

        int channelFormat;
        glGetTexLevelParameteriv(GL_TEXTURE_2D,levelOfDetail,GL_TEXTURE_INTERNAL_FORMAT,&channelFormat);

        return channelFormat;
    }
    else
    {
        // In this case we
        // did not have a
        // valid texture
        // object, so we
        // return 0 as its
        // channel format

        return 0;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline GLint blTexture::getMaximumSize()const
{
    GLint maximumSize;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximumSize);

    return maximumSize;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blTexture::getNumOfTextureLevels()const
{
    // We are going to
    // loop through the
    // levels of this
    // texture until we get
    // to a level with
    // zero width or height
    // and that is the total
    // number of levels
    // for this texture

    int levelOfDetail = 0;

    while(this->getWidth(levelOfDetail) > 0 &&
          this->getHeight(levelOfDetail) > 0)
    {
        ++levelOfDetail;
    }

    return levelOfDetail;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::copyBufferToThisTextureAtASpecifiedLevel(const int& levelOfDetail)
{
    // NOTE:    We're going to copy
    //          the current graphics
    //          buffer into this
    //          texture object at
    //          the specified level
    //          of the texture object

    // Check if the
    // texture is valid

    if(!glIsTexture(m_texture))
    {
        // Error -- Tried to render to
        //          a texture object
        //          without first
        //          initializing it

        return;
    }

    // bind to the
    // texture object

    glBindTexture(GL_TEXTURE_2D,m_texture);

    // We have to copy
    // the buffer image
    // to the specified
    // level of detail

    int width = this->getWidth(levelOfDetail);
    int height = this->getHeight(levelOfDetail);

    // copy the specified
    // level if its height
    // and width are greater
    // than zero

    if(width > 0 && height > 0)
    {
        // Store the rendered
        // data into the texture
        // object level of detail

        glCopyTexSubImage2D(GL_TEXTURE_2D,      // Type of texture
                            levelOfDetail,      // level of detail (0 is base image)
                            0,                  // texture x starting point
                            0,                  // texture y starting point
                            0,                  // Window starting x coord
                            0,                  // Window starting y coord
                            width,              // width of image to copy
                            height              // height of image to copy
                           );
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blTexture::copyBufferToThisTextureAtAllLevels()
{
    // NOTE:    We're going to copy
    //          the current graphics
    //          buffer into this
    //          texture object at
    //          all the levels of
    //          the texture object

    // Check if the
    // texture is valid

    if(!glIsTexture(m_texture))
    {
        // Error -- Tried to render to
        //          a texture object
        //          without first
        //          initializing it

        return;
    }

    // bind to the
    // texture object

    glBindTexture(GL_TEXTURE_2D,m_texture);

    // We have to copy
    // the buffer image
    // to all the levels
    // of the texture

    int levelOfDetail = 0;
    int width = this->getWidth(levelOfDetail);
    int height = this->getHeight(levelOfDetail);

    // Keep copying until
    // a level has a width
    // or height of zero

    while(width > 0 && height > 0)
    {
        // Store the rendered data
        // into the texture object
        // level of detail

        glCopyTexSubImage2D(GL_TEXTURE_2D,      // Type of texture
                            levelOfDetail,      // level of detail (0 is base image)
                            0,                  // texture x starting point
                            0,                  // texture y starting point
                            0,                  // Window starting x coord
                            0,                  // Window starting y coord
                            width,              // width of image to copy
                            height              // height of image to copy
                           );

        // Increase the level
        // and get its width
        // and height

        ++levelOfDetail;
        width = this->getWidth(levelOfDetail);
        height = this->getHeight(levelOfDetail);
    }
}
//-------------------------------------------------------------------


#endif // BL_TEXTURE_HPP
