#ifndef BL_CHANNELALGORITHMS_HPP
#define BL_CHANNELALGORITHMS_HPP


//-------------------------------------------------------------------
// FILE:            blChannelAlgorithms.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions I created
//                  to play with image channels
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    - blImage -- The image structure
//                  - cvSplit -- To split an image into its channels
//                  - cvMerge -- To combine channels into an image
//
// NOTES:
//
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
// The following functions grab a channel
// from a multi-channel image into another
//-------------------------------------------------------------------
template<typename blDataType>

inline void queryChannel(const blImage<blDataType>& srcImage,
                         blImage<blDataType2>& dstImageChannel,
                         const int& whichChannel)
{
    // If the channel image is not the
    // correct size then resize it

    if(DstImageChannel.size1() != SrcImage.size1() || DstImageChannel.size2() != SrcImage.size2())
        DstImageChannel.Create(SrcImage.size1(),SrcImage.size2());

    // Call the opencv function to get the individual channel image
    switch(WhichChannel)
    {
    case 0:
        // Blue Channel
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;

    case 1:
        // Green Channel
        cvSplit(SrcImage,NULL,DstImageChannel,NULL,NULL);
        break;

    case 2:
        // Red Channel
        cvSplit(SrcImage,NULL,NULL,DstImageChannel,NULL);
        break;

    case 3:
        // Alpha Channel
        cvSplit(SrcImage,NULL,NULL,NULL,DstImageChannel);
        break;

    default:
        // Error
        // In this case the wrong channel index
        // was specified and thus we pass the
        // first channel as default
        cvSplit(SrcImage,DstImageChannel,NULL,NULL,NULL);
        break;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blInsertChannel
//
// ARGUMENTS:           - SrcChannel
//                      - DstImage
//                      - WhichChannel
//
// TEMPLATE ARGUMENTS:  - blDataType
//                      - blDataType2
//
// PURPOSE:             Insert a channel into a destination image
//
// DEPENDENCIES:        - blImage
//                      - cvMerge
//
// NOTES:               - This function leaves it up to the user
//                        to make sure that both the source and
//                        the destination images have the same depth,
//                        and that the source image is a single
//                        channel image while the destination image
//                        is a multiple channel image
//                      - The channel and destination image don't
//                        have to have the same size
//-------------------------------------------------------------------
template<typename blDataType,typename blDataType2>
inline void blInsertChannel(const blImage<blDataType>& SrcChannel,
                            blImage<blDataType2>& DstImage,
                            const int& WhichChannel)
{
    if(SrcChannel.GetDepth() == DstImage.GetDepth() &&
       SrcChannel.size1() == DstImage.size1() &&
       SrcChannel.size2() == DstImage.size2() &&
       SrcChannel.IsDataTypeNativelySupported() &&
       DstImage.IsDataTypeNativelySupported())
    {
        // In this case the depths are
        // the same, the sizes match,
        // and the data types are
        // natively supported so we use
        // opencv's cvMerge function to
        // insert the channel

        switch(WhichChannel)
        {
        case 0:
            cvMerge(SrcChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 1:
            cvMerge(NULL,
                    SrcChannel,
                    NULL,
                    NULL,
                    DstImage);
            break;

        case 2:
            cvMerge(NULL,
                    NULL,
                    SrcChannel,
                    NULL,
                    DstImage);
            break;

        case 3:
            cvMerge(NULL,
                    NULL,
                    NULL,
                    SrcChannel,
                    DstImage);
            break;

        default:

            // Error
            // In this case the wrong channel index
            // was specified and thus we insert
            // the channel as the blue channel
            cvMerge(SrcChannel,
                    NULL,
                    NULL,
                    NULL,
                    DstImage);
            break;
        }
    }
    else
    {
        // In this case the depths are
        // not the same, or the sizes
        // don't match or the data
        // types are not natively
        // supported by opencv

        if(SrcChannel.IsDataTypeNativelySupported() &&
           DstImage.IsDataTypeNativelySupported())
        {
            // In this case the data types
            // are natively supported by
            // opencv, so we simply loop
            // through the pixels and
            // individually assign them

            int Rows = std::min(SrcChannel.size1(),DstImage.size1());
            int Cols = std::min(SrcChannel.size2(),DstImage.size1());
            int DepthSize = DstImage.GetSizeOfDepthInBytes();

            for(int i = 0; i < Rows; ++i)
            {
                for(int j = 0; j < Cols; ++j)
                {
                    (*(reinterpret_cast<blDataType2*>(reinterpret_cast<char*>(&DstImage[i][j]) + DepthSize*WhichChannel))) = SrcChannel[i][j];
                }
            }
        }
        else
        {
            // In this case, the data types
            // are not natively supported

            // TODO:  Support this function for
            //        when the data types are
            //        not natively supported
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           RealImageChannel
//                      ImaginaryImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blDataType
// PURPOSE:             Combine two single image channels into
//                      one two-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//                      std::complex
//-------------------------------------------------------------------
template<typename blDataType>
inline void CombineChannels(const blImage<blDataType>& RealImageChannel,
                            const blImage<blDataType>& ImaginaryImageChannel,
                            blImage< std::complex<blDataType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = RealImageChannel.size1();
    int Cols = RealImageChannel.size2();

    if(Rows == ImaginaryImageChannel.size1() &&
       Cols == ImaginaryImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(RealImageChannel,
                ImaginaryImageChannel,
                NULL,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Calculate the minimum
        // number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(Rows,ImaginaryImageChannel.size1());
        ColsMin = std::min(Cols,ImaginaryImageChannel.size2());

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary complex variable
        std::complex<blDataType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.real() = RealImageChannel[i][j];
                Color.imag() = ImaginaryImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           BlueImageChannel
//                      GreenImageChannel
//                      RedImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blDataType
// PURPOSE:             Combine three single image channels into
//                      one three-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//-------------------------------------------------------------------
template<typename blDataType>
inline void CombineChannels(const blImage<blDataType>& BlueImageChannel,
                            const blImage<blDataType>& GreenImageChannel,
                            const blImage<blDataType>& RedImageChannel,
                            blImage< blColor3<blDataType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = BlueImageChannel.size1();
    int Cols = BlueImageChannel.size2();

    if(Rows == GreenImageChannel.size1() &&
       Cols == GreenImageChannel.size2() &&
       Rows == RedImageChannel.size1() &&
       Cols == RedImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(BlueImageChannel,
                GreenImageChannel,
                RedImageChannel,
                NULL,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Find the size of each image channel
        int RowsBlue = BlueImageChannel.size1();
        int ColsBlue = BlueImageChannel.size2();
        int RowsGreen = GreenImageChannel.size1();
        int ColsGreen = GreenImageChannel.size2();
        int RowsRed = RedImageChannel.size1();
        int ColsRed = RedImageChannel.size2();

        // Now that we know the size of each channel, we
        // have to find the minimum number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(RowsBlue,RowsGreen);
        RowsMin = std::min(RowsMin,RowsRed);

        ColsMin = std::min(ColsBlue,ColsGreen);
        ColsMin = std::min(ColsMin,ColsRed);

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary color variable
        blColor3<blDataType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.m_Blue = BlueImageChannel[i][j];
                Color.m_Green = GreenImageChannel[i][j];
                Color.m_Red = RedImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            CombineChannels
// ARGUMENTS:           BlueImageChannel
//                      GreenImageChannel
//                      RedImageChannel
//                      AlphaImageChannel
//                      DstImage
// TEMPLATE ARGUMENTS:  blDataType
// PURPOSE:             Combine four single image channels into
//                      one four-channel colored image.  If the
//                      channels are of different size, then DstImage
//                      will be of size (minRows,minCols), and the
//                      channels will be copied into the DstImage
//                      pixel by pixel (slower than if the
//                      channels are the same size)
// DEPENDENCIES:        blImage
//                      cvMerge
//                      std::min
//-------------------------------------------------------------------
template<typename blDataType>
inline void CombineChannels(const blImage<blDataType>& BlueImageChannel,
                            const blImage<blDataType>& GreenImageChannel,
                            const blImage<blDataType>& RedImageChannel,
                            const blImage<blDataType>& AlphaImageChannel,
                            blImage< blColor4<blDataType> >& DstImage)
{
    // Check the channels sizes and make
    // sure that they're all the same
    int Rows = BlueImageChannel.size1();
    int Cols = BlueImageChannel.size2();

    if(Rows == GreenImageChannel.size1() &&
       Cols == GreenImageChannel.size2() &&
       Rows == RedImageChannel.size1() &&
       Cols == RedImageChannel.size2() &&
       Rows == AlphaImageChannel.size1() &&
       Cols == AlphaImageChannel.size2())
    {
        // Now that we know the image channels are
        // all of equal size, we check to make
        // sure the destination image is of
        // the correct size
        if(DstImage.size1() != Rows || DstImage.size2() != Cols)
            DstImage.CreateImage(Rows,Cols);

        // Now we can combine the channels into the DstImage
        cvMerge(BlueImageChannel,
                GreenImageChannel,
                RedImageChannel,
                AlphaImageChannel,
                DstImage);

        return;
    }
    else
    {
        // In this case, the image channels are not of
        // equal size, and thus we have to do more work

        // Find the size of each image channel
        int RowsBlue = BlueImageChannel.size1();
        int ColsBlue = BlueImageChannel.size2();
        int RowsGreen = GreenImageChannel.size1();
        int ColsGreen = GreenImageChannel.size2();
        int RowsRed = RedImageChannel.size1();
        int ColsRed = RedImageChannel.size2();
        int RowsAlpha = AlphaImageChannel.size1();
        int ColsAlpha = AlphaImageChannel.size2();

        // Now that we know the size of each channel, we
        // have to find the minimum number of rows and cols
        int RowsMin,ColsMin;

        RowsMin = std::min(RowsBlue,RowsGreen);
        RowsMin = std::min(RowsMin,RowsRed);
        RowsMin = std::min(RowsMin,RowsAlpha);

        ColsMin = std::min(ColsBlue,ColsGreen);
        ColsMin = std::min(ColsMin,ColsRed);
        ColsMin = std::min(ColsMin,ColsAlpha);

        // If the DstImage is not already the
        // correct size, then resize it
        if(DstImage.size1() != RowsMin || DstImage.size2() != ColsMin)
            DstImage.CreateImage(RowsMin,ColsMin);

        // Temporary color variable
        blColor3<blDataType> Color;

        // Step through and assign the values
        // to the corresponding channels
        for(int i = 0; i < RowsMin; ++i)
        {
            for(int j = 0; j < ColsMin; ++j)
            {
                Color.m_Blue = BlueImageChannel[i][j];
                Color.m_Green = GreenImageChannel[i][j];
                Color.m_Red = RedImageChannel[i][j];
                Color.m_Alpha = AlphaImageChannel[i][j];

                DstImage[i][j] = Color;
            }
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_CHANNELALGORITHMS_HPP
