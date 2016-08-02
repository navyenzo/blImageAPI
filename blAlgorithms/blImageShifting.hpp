#ifndef BL_IMAGESHIFTING_HPP
#define BL_IMAGESHIFTING_HPP


//-------------------------------------------------------------------
// FILE:            blImageShifting.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple functions I wrote
//                  to shift images up, down, left, right.
//                  I will be adding more functions as I build them
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
// DATE CREATED:    Jan/01/2011
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
// The following function shifts an
// image by M rows and N columns
//-------------------------------------------------------------------
template<typename blDataType>

inline void shiftImageByNRowsAndMCols(blImage<blDataType>& img,
                                      const int& howManyRowsToShiftBy,
                                      const int& howManyColsToShiftBy)
{
    if(howManyRowsToShiftBy == 0 && howManyColsToShiftBy == 0)
        return;

    blImage<blDataType> tempImg;
    tempImg.clone(img);

    for(int i = 0; i < img.size1ROI(); ++i)
    {
        for(int j = 0; j < img.size2ROI(); ++j)
        {
            img.circ_atROI(i+howManyRowsToShiftBy,j+howManyColsToShiftBy) = tempImg.circ_atROI(i,j);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void shiftImageByNRowsAndMCols(const blImage<blDataType>& srcImage,
                                      blImage<blDataType>& dstImage,
                                      const int& howManyRowsToShiftBy,
                                      const int& howManyColsToShiftBy)
{
    // Check to see if the
    // destination and source
    // images are the same image

    if(srcImage.getImageSharedPtr() == dstImage.getImageSharedPtr())
    {
        shiftImageByNRowsAndMCols(dstImage,
                                  howManyRowsToShiftBy,
                                  howManyColsToShiftBy);

        return;
    }

    for(int i = 0; i < srcImage.size1ROI(); ++i)
    {
        for(int j = 0; j < srcImage.size2ROI(); ++j)
        {
            dstImage.circ_atROI(i+howManyRowsToShiftBy,j+howManyColsToShiftBy) = srcImage.circ_atROI(i,j);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions take
// an image and split it into
// a specified number of images
// stored in a vector
//-------------------------------------------------------------------
template<typename blDataType>

inline void splitImageIntoVectorOfSubImages(const blImage<blDataType>& srcImage,
                                            std::vector< blImage<blDataType> >& vectorOfSplitImages,
                                            int howManyTimesShouldTheImageBeSplitVertically,
                                            int howManyTimesShouldTheImageBeSplitHorizontally)
{
    // Since we are being asked
    // to split an image into
    // vertical and horizontal
    // sections, we cannot have
    // more sections than we have
    // rows or columns in the
    // source image

    howManyTimesShouldTheImageBeSplitVertically = std::min(howManyTimesShouldTheImageBeSplitVertically,srcImage.size1ROI());
    howManyTimesShouldTheImageBeSplitHorizontally = std::min(howManyTimesShouldTheImageBeSplitHorizontally,srcImage.size2ROI());

    // Let's resize the vector
    // if necessary so it can
    // hold all the images

    if(vectorOfSplitImages.size() < (howManyTimesShouldTheImageBeSplitVertically * howManyTimesShouldTheImageBeSplitHorizontally) )
    {
        for(int i = int(vectorOfSplitImages.size());
            i < (howManyTimesShouldTheImageBeSplitVertically * howManyTimesShouldTheImageBeSplitHorizontally);
            ++i)
        {
            blImage<blDataType> newImage;
            vectorOfSplitImages.push_back(newImage);
        }
    }

    // Now we calculate the
    // number of rows and
    // columns for each sub
    // image

    // NOTE:    The images stored
    //          in the last row/col
    //          of the vector of
    //          images have to have
    //          a size to make up
    //          for the uneven splits

    int rows = srcImage.size1ROI() / howManyTimesShouldTheImageBeSplitVertically;
    int cols = srcImage.size2ROI() / howManyTimesShouldTheImageBeSplitHorizontally;

    int remainingRows = srcImage.size1ROI() % howManyTimesShouldTheImageBeSplitVertically;
    int remainingCols = srcImage.size2ROI() % howManyTimesShouldTheImageBeSplitHorizontally;

    int n,m;

    int howManyRowsToCopyInThisLoop = 0;
    int howManyColsToCopyInThisLoop = 0;

    int imageIndex = 0;

    for(int i = 0; i < howManyTimesShouldTheImageBeSplitVertically; ++i)
    {
        for(int j = 0; j < howManyTimesShouldTheImageBeSplitHorizontally; ++j)
        {
            // First we find the size
            // of this current sub image

            if(i < (howManyTimesShouldTheImageBeSplitVertically - 1) && j < (howManyTimesShouldTheImageBeSplitHorizontally - 1))
            {
                howManyRowsToCopyInThisLoop = rows;
                howManyColsToCopyInThisLoop = cols;
            }
            else if(i < (howManyTimesShouldTheImageBeSplitVertically - 1))
            {
                howManyRowsToCopyInThisLoop = rows;
                howManyColsToCopyInThisLoop = cols + remainingCols;
            }
            else
            {
                if(j < (howManyTimesShouldTheImageBeSplitHorizontally - 1))
                {
                    howManyRowsToCopyInThisLoop = rows + remainingRows;
                    howManyColsToCopyInThisLoop = cols;
                }
                else
                {
                    howManyRowsToCopyInThisLoop = rows + remainingRows;
                    howManyColsToCopyInThisLoop = cols + remainingCols;
                }
            }

            // We set the size of
            // the sub image ROI in case
            // it's not the necessary
            // size

            vectorOfSplitImages[imageIndex].setROIheightAndwidth(howManyRowsToCopyInThisLoop,
                                                                 howManyColsToCopyInThisLoop,
                                                                 true);

            for(n = 0; n < howManyRowsToCopyInThisLoop; ++n)
            {
                for(m = 0; m < howManyColsToCopyInThisLoop; ++m)
                {
                    vectorOfSplitImages[imageIndex].atROI(n,m) = srcImage.atROI(i*rows + n,
                                                                                j*cols + m);
                }
            }

            // Move onto the
            // next image in
            // the vector

            ++imageIndex;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void splitImageIntoVectorOfSubImages(const blImage<blDataType>& srcImage,
                                            blImage< blImage<blDataType> >& vectorOfSplitImages,
                                            int howManyTimesShouldTheImageBeSplitVertically,
                                            int howManyTimesShouldTheImageBeSplitHorizontally)
{
    // Since we are being asked
    // to split an image into
    // vertical and horizontal
    // sections, we cannot have
    // more sections than we have
    // rows or columns in the
    // source image

    howManyTimesShouldTheImageBeSplitVertically = std::min(howManyTimesShouldTheImageBeSplitVertically,srcImage.size1ROI());
    howManyTimesShouldTheImageBeSplitHorizontally = std::min(howManyTimesShouldTheImageBeSplitHorizontally,srcImage.size2ROI());

    // Let's resize the vector
    // if necessary so it can
    // hold all the images

    vectorOfSplitImages.setROIheightAndwidth(howManyTimesShouldTheImageBeSplitVertically,howManyTimesShouldTheImageBeSplitHorizontally,true);

    // Now we calculate the
    // number of rows and
    // columns for each sub
    // image

    // NOTE:    The images stored
    //          in the last row/col
    //          of the vector of
    //          images have to have
    //          a size to make up
    //          for the uneven splits

    int rows = srcImage.size1ROI() / howManyTimesShouldTheImageBeSplitVertically;
    int cols = srcImage.size2ROI() / howManyTimesShouldTheImageBeSplitHorizontally;

    int remainingRows = srcImage.size1ROI() % howManyTimesShouldTheImageBeSplitVertically;
    int remainingCols = srcImage.size2ROI() % howManyTimesShouldTheImageBeSplitHorizontally;

    int n,m;

    int howManyRowsToCopyInThisLoop = 0;
    int howManyColsToCopyInThisLoop = 0;

    for(int i = 0; i < howManyTimesShouldTheImageBeSplitVertically; ++i)
    {
        for(int j = 0; j < howManyTimesShouldTheImageBeSplitHorizontally; ++j)
        {
            // First we find the size
            // of this current sub image

            if(i < (howManyTimesShouldTheImageBeSplitVertically - 1) && j < (howManyTimesShouldTheImageBeSplitHorizontally - 1))
            {
                howManyRowsToCopyInThisLoop = rows;
                howManyColsToCopyInThisLoop = cols;
            }
            else if(i < (howManyTimesShouldTheImageBeSplitVertically - 1))
            {
                howManyRowsToCopyInThisLoop = rows;
                howManyColsToCopyInThisLoop = cols + remainingCols;
            }
            else
            {
                if(j < (howManyTimesShouldTheImageBeSplitHorizontally - 1))
                {
                    howManyRowsToCopyInThisLoop = rows + remainingRows;
                    howManyColsToCopyInThisLoop = cols;
                }
                else
                {
                    howManyRowsToCopyInThisLoop = rows + remainingRows;
                    howManyColsToCopyInThisLoop = cols + remainingCols;
                }
            }

            // We set the size of
            // the sub image ROI in case
            // it's not the necessary
            // size

            vectorOfSplitImages.atROI(i,j).setROIheightAndwidth(howManyRowsToCopyInThisLoop,
                                                                howManyColsToCopyInThisLoop,
                                                                true);

            for(n = 0; n < howManyRowsToCopyInThisLoop; ++n)
            {
                for(m = 0; m < howManyColsToCopyInThisLoop; ++m)
                {
                    vectorOfSplitImages.atROI(i,j).atROI(n,m) = srcImage.atROI(i*rows + n,
                                                                               j*cols + m);
                }
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            ShiftImageByMRowsAndNColsWithBackgroundColor
//
// ARGUMENTS:           - SrcImage
//                      - DstImage
//                      - HowManyRowsToShiftBy
//                      - HowManyColsToShiftBy
//
// TEMPLATE ARGUMENTS:  blDataType
//
// PURPOSE:             Takes an image and shifts it by m rows
//                      and n cols, but replaces the moving pixels
//                      with a specified background color
//
// DEPENDENCIES:        - blImage
//-------------------------------------------------------------------
template<typename blDataType>
inline void ShiftImageByNRowsAndMColsWithBackgroundColor(const blImage<blDataType>& SrcImage,
                                                         blImage<blDataType>& DstImage,
                                                         const int& HowManyRowsToShiftBy,
                                                         const int& HowManyColsToShiftBy,
                                                         const blDataType& BackgroundColor)
{
    // Let's get the size
    // of the source image
    int Rows = SrcImage.size1();
    int Cols = SrcImage.size2();

    // Let's make sure that the
    // destination image is the
    // same size as the source
    // image
    if(DstImage.size1() != Rows || DstImage.size2() != Cols)
        DstImage.CreateImage(Rows,Cols);

    // Because the indeces can be
    // negative or bigger than the
    // size of the image, then we
    // do some simple math to "wrap"
    // the indeces around
    int M = HowManyRowsToShiftBy % Rows;
    int N = HowManyColsToShiftBy % Cols;

    // Create the indeces used to
    // refer to the source image
    int RowIndex;
    int ColIndex;

    // If both the destination and
    // source images are the same
    // image, then we need to make
    // a copy of the source image
    // and work off of that
    if(SrcImage.GetImageSharedPointer() == DstImage.GetImageSharedPointer())
    {
        // Let's make a copy of the source
        // image because the destination
        // image could be the same image
        // as the source image, and that
        // would cause problems
        blImage<blDataType> ImageCopy = SrcImage;

        // Now it's just a matter of going
        // through the image and offsetting
        // the indeces by the right amount
        for(int i = M; i < Rows; ++i)
        {
            RowIndex = i % Rows;
            if(RowIndex < 0)
                RowIndex = Rows + RowIndex;

            for(int j = N; j < Cols; ++j)
            {
                ColIndex = j % Cols;
                if(ColIndex < 0)
                    ColIndex = Cols + ColIndex;

                DstImage[i][j] = ImageCopy[RowIndex][ColIndex];
            }
        }

        for(int i = 0; i < M; ++i)
            for(int j = 0; j < N; ++j)
                DstImage[i][j] = BackgroundColor;
    }
    else
    {
        // Now it's just a matter of going
        // through the image and offsetting
        // the indeces by the right amount
        for(int i = M; i < Rows; ++i)
        {
            RowIndex = i % Rows;
            if(RowIndex < 0)
                RowIndex = Rows + RowIndex;

            for(int j = N; j < Cols; ++j)
            {
                ColIndex = j % Cols;
                if(ColIndex < 0)
                    ColIndex = Cols + ColIndex;

                DstImage[i][j] = SrcImage[RowIndex][ColIndex];
            }
        }

        for(int i = 0; i < M; ++i)
            for(int j = 0; j < N; ++j)
                DstImage[i][j] = BackgroundColor;
    }
}
//-------------------------------------------------------------------


#endif // BL_IMAGESHIFTING_HPP
