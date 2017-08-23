#ifndef BL_IMAGEPYRAMIDS_HPP
#define BL_IMAGEPYRAMIDS_HPP



//-------------------------------------------------------------------
// FILE:            blImagePyramids.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of algorithms I wrote to facilitate
//                  the generation of image pyramids
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// These functions calculate the maximum number
// of pyramid levels available for a specified
// source image
//-------------------------------------------------------------------
inline int calculateMaxNumberOfPyramidLevels(const int& size1ROI,const int& size2ROI)
{
    // When using cvPyrDown, the size
    // of the down pyramid is calculated
    // as Size((src.cols+1)/2, (src.rows+1)/2)

    // Therefore we use the following
    // formula to calculate the max
    // number of levels

    // (n + 1) = 2^m
    // where n is the image size
    // m is the max number of levels

    // which is translated to:
    // m = ln(n+1)/ln(2)

    return ( std::log(double(std::min(size1ROI,size2ROI))) /
             std::log(double(2.0)) );
}



template<typename blDataType>

inline int calculateMaxNumberOfPyramidLevels(const blImage<blDataType>& srcImage)
{
    return calculateMaxNumberOfPyramidLevels(srcImage.size1ROI(),srcImage.size2ROI());
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This function sets the ROI in a pyramid image to
// point to a specified pyramid level in the pyramid
// image
// This function assumes that the pyramid image is
// formatted so that the level 0 image is on the left
// side and all the higher levels are on the right
// side aligned vertically
//-------------------------------------------------------------------
template<typename blDataType>

inline void setROIofPyramidforSpecifiedLevel(blImage<blDataType>& pyramidImage,
                                             const int& level,
                                             const int& pyramidImageStartingYROI,
                                             const int& pyramidImageStartingXROI,
                                             const int& srcImageRows,
                                             const int& srcImageCols)
{
    if(level <= 0)
    {
        pyramidImage.setROI(pyramidImageStartingYROI,
                            pyramidImageStartingXROI,
                            srcImageRows,
                            srcImageCols,
                            true);
    }
    else
    {
        int rows = srcImageRows;
        int cols = srcImageCols;

        int yROI = pyramidImageStartingYROI;

        for(int i = 0; i < level; ++i)
        {
            if(i > 0)
                yROI += rows;

            rows = (rows + 1)/2;
            cols = (cols + 1)/2;
        }

        pyramidImage.setROI(yROI,
                            pyramidImageStartingXROI + srcImageCols,
                            rows,
                            cols,
                            true);
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This function builds a gaussian pyramid from a specified
// source image onto a specified guassian pyramid image, laying
// level 0 image on the left side and the higher level images
// on the right side aligned vertically
//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType>& buildGaussianPyramid(const blImage<blDataType>& srcImage,
                                                 blImage<blDataType>& workerImage,
                                                 blImage<blDataType>& gaussianPyramidImage,
                                                 const int& filterType = CV_GAUSSIAN_5x5)
{
    // Get the source image ROI size

    int srcImageRows = srcImage.size1ROI();
    int srcImageCols = srcImage.size2ROI();



    // Count the number of possible
    // pyramid levels

    int maxNumberOfPyramidLevels = calculateMaxNumberOfPyramidLevels(srcImageRows,srcImageCols);



    // Get the starting coordinates of the
    // pyramid image ROI

    int gaussian_starting_yROI = gaussianPyramidImage.yROI();
    int gaussian_starting_xROI = gaussianPyramidImage.xROI();



    // The first image (level 0) in the
    // gaussian pyramid is the source image
    // which we copy to the left/top
    // side of the gaussian pyramid

    setROIofPyramidforSpecifiedLevel(gaussianPyramidImage,
                                     0,
                                     gaussian_starting_yROI,
                                     gaussian_starting_xROI,
                                     srcImageRows,
                                     srcImageCols);

    cvCopy(srcImage,gaussianPyramidImage);



    // Now that we have copied the
    // source image, we move to the
    // right of it and start creating
    // the increasigly smaller gaussian
    // images



    // The starting pyramid level

    int level = 1;



    // Let's loop and create the gaussian images

    while(level < maxNumberOfPyramidLevels)
    {
        // The first thing is copy the previous
        // level gaussian to our worker image

        workerImage.setROI(0,0,gaussianPyramidImage.size1ROI(),gaussianPyramidImage.size2ROI(),true);
        cvCopy(gaussianPyramidImage,workerImage);



        // We then set the ROI in the pyramid
        // image for the current level

        setROIofPyramidforSpecifiedLevel(gaussianPyramidImage,
                                         level,
                                         gaussian_starting_yROI,
                                         gaussian_starting_xROI,
                                         srcImageRows,
                                         srcImageCols);



        // Here we downsample the worker image
        // into the pyramid image

        cvPyrDown(workerImage,gaussianPyramidImage,filterType);



        // Let's move on to the next level

        ++level;
    }



    // Let's not forget to reset the
    // ROI coordinates for the pyramid
    // image

    gaussianPyramidImage.setROI(gaussian_starting_yROI,
                                gaussian_starting_xROI,
                                srcImageRows,
                                srcImageCols + (srcImageCols + 1)/2,
                                false);



    // Let's return the pyramid image

    return gaussianPyramidImage;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
// This function builds a laplacian pyramid from a specified
// source image and a specified gaussian pyramid, laying
// level 0 image on the left side and the higher level images
// on the right side aligned vertically
//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType>& buildLaplacianPyramid(const blImage<blDataType>& srcImage,
                                                  blImage<blDataType>& workerImage,
                                                  blImage<blDataType>& gaussianPyramidImage,
                                                  blImage<blDataType>& laplacianPyramidImage,
                                                  const int& filterType = CV_GAUSSIAN_5x5)
{
    // Get the source image ROI size

    int srcImageRows = srcImage.size1ROI();
    int srcImageCols = srcImage.size2ROI();



    // Count the number of possible
    // pyramid levels

    int maxNumberOfPyramidLevels = calculateMaxNumberOfPyramidLevels(srcImageRows,srcImageCols);



    // Get the starting coordinates of the
    // pyramid images ROIs

    int gaussian_starting_yROI = gaussianPyramidImage.yROI();
    int gaussian_starting_xROI = gaussianPyramidImage.xROI();

    int laplacian_starting_yROI = laplacianPyramidImage.yROI();
    int laplacian_starting_xROI = laplacianPyramidImage.xROI();



    // The starting pyramid level

    int level = 0;



    // Let's loop and create the gaussian images

    while(level < (maxNumberOfPyramidLevels - 1))
    {
        // The first thing we do is set the ROIs
        // for the laplacian pyramid

        setROIofPyramidforSpecifiedLevel(laplacianPyramidImage,
                                         level,
                                         laplacian_starting_yROI,
                                         laplacian_starting_yROI,
                                         srcImageRows,
                                         srcImageCols);



        // Next we have to make sure the worker
        // image has the same ROI

        workerImage.setROI(0,0,laplacianPyramidImage.size1ROI(),laplacianPyramidImage.size2ROI());



        // We then set the ROI for the gaussian
        // pyramid (it has to be one level up so
        // we can upsample it)

        setROIofPyramidforSpecifiedLevel(gaussianPyramidImage,
                                         level + 1,
                                         gaussian_starting_yROI,
                                         gaussian_starting_xROI,
                                         srcImageRows,
                                         srcImageCols);



        // Here we upsample the current gaussian
        // image into the worker image

        cvPyrUp(gaussianPyramidImage,workerImage);



        // Now once again we have to change the
        // ROI of the gaussian pyramid and set
        // it to the current pyramid level

        setROIofPyramidforSpecifiedLevel(gaussianPyramidImage,
                                         level,
                                         gaussian_starting_yROI,
                                         gaussian_starting_xROI,
                                         srcImageRows,
                                         srcImageCols);



        // Finally we subtract the current
        // level gaussian image with the
        // upsampled one and save the result
        // into the laplacian pyramid

        cvSub(gaussianPyramidImage,
              workerImage,
              laplacianPyramidImage,
              NULL);



        // Let's move on to the next level

        ++level;
    }



    // For the last pyramid level, we just
    // set the laplacian image equal to the
    // guassian image at that last level



    setROIofPyramidforSpecifiedLevel(laplacianPyramidImage,
                                     maxNumberOfPyramidLevels - 1,
                                     laplacian_starting_yROI,
                                     laplacian_starting_yROI,
                                     srcImageRows,
                                     srcImageCols);

    setROIofPyramidforSpecifiedLevel(gaussianPyramidImage,
                                     maxNumberOfPyramidLevels - 1,
                                     gaussian_starting_yROI,
                                     gaussian_starting_xROI,
                                     srcImageRows,
                                     srcImageCols);

    cvCopy(gaussianPyramidImage,laplacianPyramidImage);



    // Let's not forget to reset the
    // ROI coordinates for both of the
    // pyramid images

    gaussianPyramidImage.setROI(gaussian_starting_yROI,
                                gaussian_starting_xROI,
                                srcImageRows,
                                srcImageCols + (srcImageCols + 1)/2,
                                false);

    laplacianPyramidImage.setROI(laplacian_starting_yROI,
                                 laplacian_starting_xROI,
                                 srcImageRows,
                                 srcImageCols + (srcImageCols + 1)/2,
                                 false);



    // Let's return the pyramid image

    return gaussianPyramidImage;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blDataType>

inline std::vector< blImage<blDataType> >& initializePyramidVector(const blImage<blDataType>& srcImage,
                                                                   std::vector< blImage<blDataType> >& imageVector)
{
    // We count the number of possible
    // image pyramid levels and make sure
    // the image vector has enough images
    // matching the source image ROI

    int maxNumberOfPyramidLevels = calculateMaxNumberOfPyramidLevels(srcImage);

    if(imageVector.size() < maxNumberOfPyramidLevels)
    {
        // First we reserve the right
        // number of places in the vector

        imageVector.reserve(maxNumberOfPyramidLevels);



        // We then first make sure that the
        // images that were already in the image
        // vector have the correct ROIs

        auto ROI = srcImage.getROIRect();

        for(int i = 0; i < imageVector.size(); ++i)
        {
            imageVector[i].setROI(ROI,true);
        }



        // Finally we add the other images the
        // image vector didn't have yet

        int numberOfExtraImagesNeeded = maxNumberOfPyramidLevels - imageVector.size();

        for(int i = 0; i < numberOfExtraImagesNeeded; ++i)
        {
            blImage<blDataType> newImg(srcImage.size1ROI(),srcImage.size2ROI());

            imageVector.push_back(newImg);
        }
    }

    return imageVector;
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blDataType,
         typename blPyramidIteratorType>

inline void buildGaussianPyramids(blImage<blDataType>& srcImage,
                                  blPyramidIteratorType iteratorToBeginningOfGaussianPyramid,
                                  const blPyramidIteratorType& iteratorToEndOfGaussianPyramid,
                                  const int& filterType = CV_GAUSSIAN_5x5)
{
    if(iteratorToBeginningOfGaussianPyramid == iteratorToEndOfGaussianPyramid)
    {
        // The gaussian pyramid is
        // of zero length, therefore
        // we quit

        return;
    }

    // First we calculate the maximum
    // allowable pyramid levels based
    // on the size of the source image

    int maxNumberOfPyramidLevels = calculateMaxNumberOfPyramidLevels(srcImage);

    // The first image (level 0)
    // is the source image

    iteratorToBeginningOfGaussianPyramid->clone(srcImage);

    auto iteratorToPreviousImage = iteratorToBeginningOfGaussianPyramid;

    ++iteratorToBeginningOfGaussianPyramid;

    CvRect ROIrect;

    ROIrect.y = 0;
    ROIrect.x = 0;
    ROIrect.height = (srcImage.size1ROI() + 1)/2;
    ROIrect.width = (srcImage.size2ROI() + 1)/2;

    int level = 1;

    // Finally we calculate
    // the pyramids and size
    // the images if necessary

    while(level < maxNumberOfPyramidLevels &&
          ROIrect.height > 0 &&
          ROIrect.width > 0 &&
          iteratorToBeginningOfGaussianPyramid != iteratorToEndOfGaussianPyramid)
    {
        iteratorToBeginningOfGaussianPyramid->setROI(ROIrect,true);

        ROIrect.height = (ROIrect.height + 1) / 2;
        ROIrect.width = (ROIrect.width + 1) / 2;

        cvPyrDown((*iteratorToPreviousImage),
                  (*iteratorToBeginningOfGaussianPyramid),
                  filterType);

        iteratorToPreviousImage = iteratorToBeginningOfGaussianPyramid;
        ++iteratorToBeginningOfGaussianPyramid;
        ++level;
    }
}
//-------------------------------------------------------------------



//-------------------------------------------------------------------
template<typename blGaussianPyramidIteratorType,
         typename blLaplacianPyramidIteratorType>

inline void buildLaplacianPyramids(const blGaussianPyramidIteratorType& iteratorToBeginningOfGaussianPyramid,
                                   const blGaussianPyramidIteratorType& iteratorToEndOfGaussianPyramid,
                                   blLaplacianPyramidIteratorType iteratorToBeginningOfLaplacianPyramid,
                                   blLaplacianPyramidIteratorType iteratorToEndOfLaplacianPyramid,
                                   const int& filterType = CV_GAUSSIAN_5x5)
{
    if(iteratorToBeginningOfGaussianPyramid == iteratorToEndOfGaussianPyramid ||
       iteratorToBeginningOfLaplacianPyramid == iteratorToEndOfLaplacianPyramid)
    {
        // Either or both of the
        // pyramids are of zero
        // length and therefore
        // we quit`

        return;
    }

    // The laplacian pyramid is
    // built by upsampling the
    // gaussian pyramid images
    // and taking the difference
    // between the bigger image
    // and the upsampled one

    auto iteratorToPreviousGaussianImage = iteratorToBeginningOfGaussianPyramid;
    auto iteratorToCurrentGaussianImage = iteratorToBeginningOfGaussianPyramid;
    ++iteratorToCurrentGaussianImage;

    auto iteratorToCurrentLaplacianImage = iteratorToBeginningOfLaplacianPyramid;

    for(;
        iteratorToCurrentGaussianImage != iteratorToEndOfGaussianPyramid &&
        iteratorToCurrentLaplacianImage != iteratorToEndOfLaplacianPyramid;
        ++iteratorToCurrentGaussianImage,
        ++iteratorToCurrentLaplacianImage)
    {
        // First we size the
        // laplacian image if
        // needed

        iteratorToCurrentLaplacianImage->setROI(iteratorToPreviousGaussianImage->getROIRect(),true);

        // Then we upsample the
        // gaussian image

        cvPyrUp(*iteratorToCurrentGaussianImage,
                *iteratorToCurrentLaplacianImage,
                filterType);

        // Finally we take the
        // difference between
        // the bigger gaussian
        // image and the upsampled
        // one

        cvSub(*iteratorToPreviousGaussianImage,
              *iteratorToCurrentLaplacianImage,
              *iteratorToCurrentLaplacianImage,
              NULL);

        iteratorToPreviousGaussianImage = iteratorToCurrentGaussianImage;
    }
}
//-------------------------------------------------------------------



#endif // BL_IMAGEPYRAMIDS_HPP
