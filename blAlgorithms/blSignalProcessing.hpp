#ifndef BL_SIGNALPROCESSING_HPP
#define BL_SIGNALPROCESSING_HPP


//-------------------------------------------------------------------
// FILE:            blSignalProcessing.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of algorithms I wrote to use
//                  the blImageAPI for signal processing
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file and sub-files
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums used for this file and sub-files
//-------------------------------------------------------------------

enum blFilterTypeEnum {BL_LOWPASS,
                       BL_HIGHPASS,
                       BL_BANDPASS,
                       BL_BANDREJECT};

enum blFilterFunctionTypeEnum {BL_BUTTERWORTH,
                               BL_GAUSSIAN};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// This function returns the index
// pointing to a position in an array
// containing a signal in the frequency
// domain that corresponds to the
// specified frequency
//-------------------------------------------------------------------
inline int frequencyIndex(const double& frequencyOfInterest,
                          const int& numberOfSamples,
                          const double& samplingFrequency,
                          const bool& hasfftImageBeenShiftedToTheMiddle)
{
    // The corresponding frequency
    // of each "bin" in the fft image
    // is:  Freq = n * Fs / N
    //
    // where Freq = Frequency
    //       n = nth bin
    //       Fs = Sampling Frequency
    //       N = sample size (number of bins)

    int index = 0;

    if(hasfftImageBeenShiftedToTheMiddle)
    {
        index = (double(numberOfSamples) / double(2)) - frequencyOfInterest * double(numberOfSamples) / samplingFrequency;
    }
    else
    {
        index = int(frequencyOfInterest * double(numberOfSamples) / samplingFrequency);
    }

    if(index < 0)
        index = 0;
    else if(index >= numberOfSamples)
        index = numberOfSamples - 1;

    return index;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// This function generates the
// x-axis of an fft representing
// the fft frequencies (Hz)
//
// The function assumes that the
// size of the ROI of the axis
// image corresponds to the
// number of samples
//-------------------------------------------------------------------
template<typename blDataType>

inline void generateFrequencyAxis(blImage<blDataType>& frequencyAxis,
                                  const double& samplingRate,
                                  const bool& hasfftImageBeenShiftedToTheMiddle)
{
    // The corresponding frequency
    // of each "bin" in the fft image
    // is:  freq = i * fs / n
    // where freq = Frequency
    //       i = ith bin
    //       fs = Sampling Frequency
    //       n = sample size (number of bins)



    int rows = frequencyAxis.size1ROI();
    int cols = frequencyAxis.size2ROI();
    int yROI = frequencyAxis.yROI();
    int xROI = frequencyAxis.xROI();

    int iLimit = yROI + rows;
    int jLimit = xROI + cols;



    int numberOfSamples = rows*cols;

    double rateStep = samplingRate / static_cast<double>(numberOfSamples);



    double currentBin = 0;



    if(hasfftImageBeenShiftedToTheMiddle)
    {
        double halfNumberOfSamples = static_cast<double>(numberOfSamples/2);

        for(int i = yROI; i < iLimit; ++i)
        {
            for(int j = xROI; j < jLimit; ++j,++currentBin)
            {
                frequencyAxis(i,j) = -(currentBin - halfNumberOfSamples) * rateStep;
            }
        }
    }
    else
    {
        for(int i = yROI; i < iLimit; ++i)
        {
            for(int j = xROI; j < jLimit; ++j,++currentBin)
            {
                frequencyAxis(i,j) = currentBin * rateStep;
            }
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function shifts
// the DFT output to center the
// DC frequency in the middle of
// the image by multiplying the
// image by (-1)^(rows + cols)
//-------------------------------------------------------------------
template<typename blDataType>

inline void shiftImageForFourierTransform(blImage<blDataType>& img)
{
    int rows = img.size1ROI();
    int cols = img.size2ROI();

    int yROI = img.yROI();
    int xROI = img.xROI();

    for(int i = yROI; i < rows + yROI; ++i)
    {
        for(int j = xROI; j < cols + xROI; ++j)
        {
            if((i + j) % 2)
                img(i,j) *= static_cast<blDataType>(-1);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline void shiftImageForFourierTransform(const blImage<blDataType>& srcImg,
                                          blImage<blDataType>& dstImg)
{
    int srcRows = srcImg.size1ROI();
    int srcCols = srcImg.size2ROI();
    int srcyROI = srcImg.yROI();
    int srcxROI = srcImg.xROI();

    int dstRows = dstImg.size1ROI();
    int dstCols = dstImg.size2ROI();
    int dstyROI = dstImg.yROI();
    int dstxROI = dstImg.xROI();



    for(int iSrc = srcyROI, iDst = dstyROI;
        iSrc < srcRows + srcyROI && iDst < dstRows + dstyROI;
        ++iSrc, ++iDst)
    {
        for(int jSrc = srcxROI, jDst = dstxROI;
            jSrc < srcCols + srcxROI && jDst < dstCols + dstxROI;
            ++jSrc, ++jDst)
        {
            if((iSrc + jSrc) % 2)
                dstImg(iDst,jDst) = srcImg(iSrc,jSrc) * blDataType(-1);
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions take the
// forward fast fourier transform
// of an image
//-------------------------------------------------------------------
template<typename blDataType>

inline void fft2(const blImage<blDataType>& srcImage,
                 blImage< std::complex<blDataType> >& dstImage,
                 const bool& shouldfftBeDoneOnIndividualRows)
{
    // Let's make sure that
    // the destination image
    // is of the correct size

    if(dstImage.size1ROI() != srcImage.size1ROI() ||
       dstImage.size2ROI() != srcImage.size2ROI())
    {
        // We can't take the forward
        // fft of the image due to the
        // sizes being different

        return;
    }

    if(shouldfftBeDoneOnIndividualRows)
        cvDFT(srcImage,dstImage,CV_DXT_FORWARD|CV_DXT_ROWS);
    else
        cvDFT(srcImage,dstImage,CV_DXT_FORWARD);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage< std::complex<blDataType> > fft2(const blImage<blDataType>& srcImage,
                                                const bool& shouldfftBeDoneOnIndividualRows)
{
    blImage< std::complex<blDataType> > fft2Image(srcImage.size1ROI(),srcImage.size2ROI());

    fft2(srcImage,fft2Image,shouldfftBeDoneOnIndividualRows);

    return fft2Image;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions take the
// inverse fast fourier transform of
// an image
//-------------------------------------------------------------------
template<typename blDataType>

inline void ifft2(const blImage< std::complex<blDataType> >& srcImage,
                  blImage<blDataType>& dstImage,
                  const bool& shouldifftBeDoneOnIndividualRows)
{
    // Let's make sure that
    // the destination image
    // is of the correct size

    if(dstImage.size1ROI() != srcImage.size1ROI() ||
       dstImage.size2ROI() != srcImage.size2ROI())
    {
        // We can't take the forward
        // fft of the image due to the
        // sizes being different

        return;
    }


    // Compute the inverse fft

    if(shouldifftBeDoneOnIndividualRows)
        cvDFT(srcImage,dstImage,CV_DXT_INV_SCALE|CV_DXT_ROWS);
    else
        cvDFT(srcImage,dstImage,CV_DXT_INV_SCALE);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType> ifft2(const blImage< std::complex<blDataType> >& srcImage,
                                 const bool& shouldifftBeDoneOnIndividualRows)
{
    blImage<blDataType> ifft2Image(srcImage.size1ROI(),srcImage.size2ROI());

    ifft2(srcImage,ifft2Image,shouldifftBeDoneOnIndividualRows);

    return ifft2Image;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the power spectrum in the frequency
// domain
//
// NOTE:    This function works well if
//          the images are floating point
//          (float or double)
//-------------------------------------------------------------------
template<typename blDataType>

inline void fftPowerSpectrum(const blImage< std::complex<blDataType> >& srcImage,
                             blImage<blDataType>& dstImage,
                             const bool& shouldResultsBePlottedOnALogScale,
                             const bool& shouldResultsBeNormalized)
{
    // We step through the source
    // image, calculate the power
    // and save it into the destination
    // image, therefore the images
    // don't have to have the same
    // ROI, we just stop whenever
    // we encounter the end of either
    // image's ROI

    int srcRows = srcImage.size1ROI();
    int srcCols = srcImage.size2ROI();
    int srcyROI = srcImage.yROI();
    int srcxROI = srcImage.xROI();

    int dstRows = dstImage.size1ROI();
    int dstCols = dstImage.size2ROI();
    int dstyROI = dstImage.yROI();
    int dstxROI = dstImage.xROI();

    for(int iSrc = srcyROI, iDst = dstyROI; iSrc < (srcRows + srcyROI) && iDst < (dstRows + dstyROI); ++iSrc, ++iDst)
    {
        for(int jSrc = srcxROI, jDst = dstxROI; jSrc < (srcCols + srcxROI) && jDst < (dstCols + dstxROI); ++jSrc, ++jDst)
        {
            // Calculate the magnitude

            dstImage(iDst,jDst) = (srcImage(iSrc,jSrc).real() * srcImage(iSrc,jSrc).real() +
                                   srcImage(iSrc,jSrc).imag() * srcImage(iSrc,jSrc).imag());

            // Calculate the log if
            // we need to

            if(shouldResultsBePlottedOnALogScale)
            {
                dstImage(iDst,jDst) = std::log(blDataType(1) + dstImage(iDst,jDst));
            }
        }
    }

    // Normalize the results
    // if we need to

    if(shouldResultsBeNormalized)
    {
        auto normValueFS = norm(dstImage,cv::NORM_INF);
        perElementDivision(dstImage,normValueFS);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType> fftPowerSpectrum(const blImage< std::complex<blDataType> >& srcImage,
                                            const bool& shouldResultsBePlottedOnALogScale,
                                            const bool& shouldResultsBeNormalized)
{
    blImage<blDataType> powerSpectrumImage(srcImage.size1ROI(),srcImage.size2ROI());

    fftPowerSpectrum(srcImage,
                     powerSpectrumImage,
                     shouldResultsBePlottedOnALogScale,
                     shouldResultsBeNormalized);

    return powerSpectrumImage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the fourier spectrum in the frequency
// domain
//
// NOTE:    This function works well if
//          the images are floating point
//          (float or double)
//-------------------------------------------------------------------
template<typename blDataType>

inline void fftFourierSpectrum(const blImage< std::complex<blDataType> >& fftComplexImage,
                               blImage<blDataType>& fourierSpectrumImage,
                               const bool& shouldResultsBePlottedOnALogScale,
                               const bool& shouldResultsBeNormalized)
{
    // We step through the source
    // image, calculate the fourier
    // spectrum and save it into
    // the destination image,
    // therefore the images
    // don't have to have the same
    // ROI, we just stop whenever
    // we encounter the end of either
    // image's ROI

    int fsRows = fourierSpectrumImage.size1ROI();
    int fsCols = fourierSpectrumImage.size2ROI();
    int fsyROI = fourierSpectrumImage.yROI();
    int fsxROI = fourierSpectrumImage.xROI();

    int fftRows = fftComplexImage.size1ROI();
    int fftCols = fftComplexImage.size2ROI();
    int fftyROI = fftComplexImage.yROI();
    int fftxROI = fftComplexImage.xROI();

    for(int iFS = fsyROI, iFFT = fftyROI;
        iFS < (fsRows + fsyROI) && iFFT < (fftRows + fftyROI);
        ++iFS, ++iFFT)
    {
        for(int jFS = fsxROI, jFFT = fftxROI;
            jFS < (fsCols + fsxROI) && jFFT < (fftCols + fftxROI);
            ++jFS, ++jFFT)
        {
            fourierSpectrumImage(iFS,jFS) = std::sqrt(fftComplexImage(iFFT,jFFT).real() * fftComplexImage(iFFT,jFFT).real() +
                                                      fftComplexImage(iFFT,jFFT).imag() * fftComplexImage(iFFT,jFFT).imag());

            // Calculate the log if
            // we need to

            if(shouldResultsBePlottedOnALogScale)
            {
                fourierSpectrumImage(iFS,jFS) = std::log(blDataType(1) + fourierSpectrumImage(iFS,jFS));
            }
        }
    }

    // Normalize the results
    // if we need to

    if(shouldResultsBeNormalized)
    {
        auto normValueFS = norm(fourierSpectrumImage,cv::NORM_INF);
        perElementDivision(fourierSpectrumImage,normValueFS);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType> fftFourierSpectrum(const blImage< std::complex<blDataType> >& fftComplexImage,
                                              const bool& shouldResultsBePlottedOnALogScale,
                                              const bool& shouldResultsBeNormalized)
{
    blImage<blDataType> fourierSpectrum(fftComplexImage.size1ROI(),fftComplexImage.size2ROI());

    fftFourierSpectrum(fftComplexImage,
                       fourierSpectrum,
                       shouldResultsBePlottedOnALogScale,
                       shouldResultsBeNormalized);

    return fourierSpectrum;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the phase angle in the frequency
// domain
//
// NOTE:    This function works well if
//          the images are floating point
//          (float or double)
//-------------------------------------------------------------------
template<typename blDataType>

inline void fftPhaseAngle(const blImage< std::complex<blDataType> >& fftComplexImage,
                          blImage<blDataType>& phaseAngleImage,
                          const bool& shouldResultsBePlottedOnALogScale,
                          const bool& shouldResultsBeNormalized)
{
    // We step through the source
    // image, calculate the phase
    // angle and save it into
    // the destination image,
    // therefore the images
    // don't have to have the same
    // ROI, we just stop whenever
    // we encounter the end of either
    // image's ROI

    int paRows = phaseAngleImage.size1ROI();
    int paCols = phaseAngleImage.size2ROI();
    int payROI = phaseAngleImage.yROI();
    int paxROI = phaseAngleImage.xROI();

    int fftRows = fftComplexImage.size1ROI();
    int fftCols = fftComplexImage.size2ROI();
    int fftyROI = fftComplexImage.yROI();
    int fftxROI = fftComplexImage.xROI();

    for(int iPA = payROI, iFFT = fftyROI;
        iPA < (paRows + payROI) && iFFT < (fftRows + fftyROI);
        ++iPA, ++iFFT)
    {
        for(int jPA = paxROI, jFFT = fftxROI;
            jPA < (paCols + paxROI) && jFFT < (fftCols + fftxROI);
            ++jPA, ++jFFT)
        {
            phaseAngleImage(iPA,jPA) = std::atan2(fftComplexImage(iFFT,jFFT).imag(),fftComplexImage(iFFT,jFFT).real());

            // Calculate the log if
            // we need to

            if(shouldResultsBePlottedOnALogScale)
            {
                phaseAngleImage(iPA,jPA) = std::log(blDataType(1) + phaseAngleImage(iPA,jPA));
            }
        }
    }

    // Normalize the results
    // if we need to

    if(shouldResultsBeNormalized)
    {
        auto normValueFS = norm(phaseAngleImage,cv::NORM_INF);
        perElementDivision(phaseAngleImage,normValueFS);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage<blDataType> fftPhaseAngle(const blImage< std::complex<blDataType> >& fftComplexImage,
                                         const bool& shouldResultsBePlottedOnALogScale,
                                         const bool& shouldResultsBeNormalized)
{
    blImage<blDataType> phaseAngleImg(fftComplexImage.size1ROI(),fftComplexImage.size2ROI());

    fftPhaseAngle(fftComplexImage,
                  phaseAngleImg,
                  shouldResultsBePlottedOnALogScale,
                  shouldResultsBeNormalized);

    return phaseAngleImg;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following function combines both
// functions above to calculate the
// fourier spectrum and phase angle of
// an image at the same time
//
// NOTE:    This function works well if
//          the images are floating point
//          (float or double)
//-------------------------------------------------------------------
template<typename blDataType>

inline void fftFourierSpectrumAndPhaseAngle(const blImage< std::complex<blDataType> >& fftComplexImage,
                                            blImage<blDataType>& fourierSpectrumImage,
                                            blImage<blDataType>& phaseAngleImage,
                                            const bool& shouldResultsBePlottedOnALogScale,
                                            const bool& shouldFourierSpectrumResultsBeNormalized,
                                            const bool& shouldPhaseAngleResultsBeNormalized)
{
    // We step through the source
    // image, calculate the fourier
    // spectrum and phase angle
    // and save them into their appropriate
    // images, therefore the images
    // don't have to have the same
    // ROI, we just stop whenever
    // we encounter the end of either
    // image's ROI

    int fsRows = fourierSpectrumImage.size1ROI();
    int fsCols = fourierSpectrumImage.size2ROI();
    int fsyROI = fourierSpectrumImage.yROI();
    int fsxROI = fourierSpectrumImage.xROI();

    int paRows = phaseAngleImage.size1ROI();
    int paCols = phaseAngleImage.size2ROI();
    int payROI = phaseAngleImage.yROI();
    int paxROI = phaseAngleImage.xROI();

    int fftRows = fftComplexImage.size1ROI();
    int fftCols = fftComplexImage.size2ROI();
    int fftyROI = fftComplexImage.yROI();
    int fftxROI = fftComplexImage.xROI();

    for(int iFS = fsyROI, iPA = payROI, iFFT = fftyROI;
        iFS < (fsRows + fsyROI) && iPA < (paRows + payROI) && iFFT < (fftRows + fftyROI);
        ++iFS, ++iPA, ++iFFT)
    {
        for(int jFS = fsxROI, jPA = paxROI, jFFT = fftxROI;
            jFS < (fsCols + fsxROI) && jPA < (paCols + paxROI) && jFFT < (fftCols + fftxROI);
            ++jFS, ++jPA, ++jFFT)
        {
            // Calculate the magnitude

            fourierSpectrumImage(iFS,jFS) = std::sqrt(fftComplexImage(iFFT,jFFT).real() * fftComplexImage(iFFT,jFFT).real() +
                                                      fftComplexImage(iFFT,jFFT).imag() * fftComplexImage(iFFT,jFFT).imag());

            // Calculate the phase

            phaseAngleImage(iPA,jPA) = std::atan2(fftComplexImage(iFFT,jFFT).imag(),fftComplexImage(iFFT,jFFT).real());

            // Let's take the log
            // if we need to

            if(shouldResultsBePlottedOnALogScale)
            {
                fourierSpectrumImage(iFS,jFS) = std::log(blDataType(1) + fourierSpectrumImage(iFS,jFS));
                phaseAngleImage(iPA,jPA) = std::log(blDataType(1) + phaseAngleImage(iPA,jPA));
            }
        }
    }

    // Normalize the fourier
    // spectrum results
    // if we need to

    if(shouldFourierSpectrumResultsBeNormalized)
    {
        auto normValueFS = norm(fourierSpectrumImage,cv::NORM_INF);
        perElementDivision(fourierSpectrumImage,normValueFS);
    }

    // Normalize the phase
    // angle results
    // if we need to

    if(shouldPhaseAngleResultsBeNormalized)
    {
        auto normValuePA = norm(fourierSpectrumImage,cv::NORM_INF);
        perElementDivision(phaseAngleImage,normValuePA);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions calculate
// the fourier transform of an image
// from its fourier spectrum and phase
// angle
//
// NOTE:    This function works well if
//          the images are floating point
//          (float or double)
//-------------------------------------------------------------------
template<typename blDataType>

inline void fftComplexImageFromFourierSpectrumAndPhaseAngle(const blImage<blDataType>& fourierSpectrumImage,
                                                            const blImage<blDataType>& phaseAngleImage,
                                                            blImage< std::complex<blDataType> >& fftComplexImage)
{
    // We step through the source
    // image, calculate the fft
    // complex image from its
    // fourier spectrum and phase
    // angle components.
    // The images don't have to
    // have the same ROI, we just
    // stop whenever we encounter
    // the end of either image's ROI

    int fsRows = fourierSpectrumImage.size1ROI();
    int fsCols = fourierSpectrumImage.size2ROI();
    int fsyROI = fourierSpectrumImage.yROI();
    int fsxROI = fourierSpectrumImage.xROI();

    int paRows = phaseAngleImage.size1ROI();
    int paCols = phaseAngleImage.size2ROI();
    int payROI = phaseAngleImage.yROI();
    int paxROI = phaseAngleImage.xROI();

    int fftRows = fftComplexImage.size1ROI();
    int fftCols = fftComplexImage.size2ROI();
    int fftyROI = fftComplexImage.yROI();
    int fftxROI = fftComplexImage.xROI();

    for(int iFS = fsyROI, iPA = payROI, iFFT = fftyROI;
        iFS < (fsRows + fsyROI) && iPA < (paRows + payROI) && iFFT < (fftRows + fftyROI);
        ++iFS, ++iPA, ++iFFT)
    {
        for(int jFS = fsxROI, jPA = paxROI, jFFT = fftxROI;
            jFS < (fsCols + fsxROI) && jPA < (paCols + paxROI) && jFFT < (fftCols + fftxROI);
            ++jFS, ++jPA, ++jFFT)
        {
            fftComplexImage(iFFT,jFFT).real(fourierSpectrumImage(iFS,jFS) * std::cos(phaseAngleImage(iPA,jPA)));
            fftComplexImage(iFFT,jFFT).imag(fourierSpectrumImage(iFS,jFS) * std::sin(phaseAngleImage(iPA,jPA)));
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline blImage< std::complex<blDataType> > fftComplexImageFromFourierSpectrumAndPhaseAngle(const blImage<blDataType>& fourierSpectrumImage,
                                                                                           const blImage<blDataType>& phaseAngleImage)
{
    blImage< std::complex<blDataType> > fftComplexImage(fourierSpectrumImage.size1ROI(),fourierSpectrumImage.size2ROI());

    fftComplexImageFromFourierSpectrumAndPhaseAngle(fourierSpectrumImage,
                                                    phaseAngleImage,
                                                    fftComplexImage);

    return fftComplexImage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// - The following functions draw
//   an ellipse on a mask to create
//   Butterworth or Gaussian filters
//
// - The functions expect a floating
//   point image to draw the mask on
//
// - The functions assume that the
//   zero frequency is in the middle
//   of the image (height/2,width/2)
//-------------------------------------------------------------------
template<typename blDataType>

inline void filterMask(blImage<blDataType>& dstImage,
                       const int& filterOrder,
                       const double& cutoffFrequencyAsARatioOfImageRadius,
                       const double& bandwidthAsARatioOfImageRadius,
                       const blFilterTypeEnum& filterType = BL_LOWPASS,
                       const blFilterFunctionTypeEnum& filterFunctionType = BL_BUTTERWORTH)
{
    // Calculate the radial
    // bounds of the filter
    // mask

    int rows = dstImage.size1ROI();
    int cols = dstImage.size2ROI();
    int yROI = dstImage.yROI();
    int xROI = dstImage.xROI();

    double yMiddle = double(rows)/2.0;
    double xMiddle = double(cols)/2.0;

    // The cutoff radii
    // in the x and y
    // directions

    // Cutoff radii in x
    // and y directions

    double yR = cutoffFrequencyAsARatioOfImageRadius * double(rows)/2.0;
    double xR = cutoffFrequencyAsARatioOfImageRadius * double(cols)/2.0;

    double yBandwidth = bandwidthAsARatioOfImageRadius * double(rows/2.0);
    double xBandwidth = bandwidthAsARatioOfImageRadius * double(cols/2.0);
    double bandwidth = 0;

    double cutoffRadius = 0;
    double distanceRadius = 0;

    double sinTheta = 0;
    double cosTheta = 0;

    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            distanceRadius = std::sqrt((double(i) - yMiddle)*(double(i) - yMiddle) + (double(j) - xMiddle)*(double(j) - xMiddle));

            if(distanceRadius > 0)
            {
                sinTheta = std::abs(double(i) - yMiddle)/distanceRadius;
                cosTheta = std::abs(double(j) - xMiddle)/distanceRadius;

                cutoffRadius = (yR*xR) / std::sqrt( (yR*cosTheta)*(yR*cosTheta) + (xR*sinTheta)*(xR*sinTheta) );

                switch(filterType)
                {
                default:
                case BL_LOWPASS:

                    if(filterFunctionType == BL_BUTTERWORTH)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 / ( 1.0 + std::pow(distanceRadius/cutoffRadius,2.0*double(filterOrder)) ) );

                    else if(filterFunctionType == BL_GAUSSIAN)
                        dstImage(i + yROI,j + xROI) = blDataType( std::exp(-0.5 * (distanceRadius*distanceRadius)/(cutoffRadius*cutoffRadius)) );

                    break;

                case BL_HIGHPASS:

                    if(filterFunctionType == BL_BUTTERWORTH)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 / ( 1.0 + std::pow(cutoffRadius/distanceRadius,2.0*double(filterOrder)) ) );

                    else if(filterFunctionType == BL_GAUSSIAN)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 - std::exp(-0.5 * (distanceRadius*distanceRadius)/(cutoffRadius*cutoffRadius)) );

                    break;

                case BL_BANDPASS:
                case BL_BANDREJECT:

                    bandwidth = (yBandwidth*xBandwidth) / std::sqrt( (yBandwidth*cosTheta)*(yBandwidth*cosTheta) + (xBandwidth*sinTheta)*(xBandwidth*sinTheta) );

                    if(filterFunctionType == BL_BUTTERWORTH)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 / (1.0 + std::pow((distanceRadius*bandwidth)/(distanceRadius*distanceRadius - cutoffRadius*cutoffRadius),2.0*double(filterOrder))) );

                    else if(filterFunctionType == BL_GAUSSIAN)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 - std::exp( -std::pow((distanceRadius*distanceRadius - cutoffRadius*cutoffRadius)/(distanceRadius*bandwidth),2.0) ) );

                    if(filterType == BL_BANDPASS)
                        dstImage(i + yROI,j + xROI) = blDataType( 1.0 - dstImage(i + yROI,j + xROI) );

                    break;
                }
            }
        }
    }

    // We did not set what
    // the value in the center
    // of the image, so we
    // assign it an average
    // of the four surrounding
    // pixels left, right, up and down

    if(rows >= 3)
    {
        if(cols >= 3)
        {
            dstImage(rows/2 + yROI,cols/2 + xROI) = ( dstImage(rows/2 + yROI - 1,cols/2 + xROI) +           // Pixel above
                                                      dstImage(rows/2 + yROI + 1,cols/2 + xROI) +           // Pixel below
                                                      dstImage(rows/2 + yROI,cols/2 + xROI - 1) +           // Pixel left
                                                      dstImage(rows/2 + yROI,cols/2 + xROI + 1) ) / 4.0;    // Pixel right
        }
        else
        {
            dstImage(rows/2 + yROI,cols/2 + xROI) = ( dstImage(rows/2 + yROI - 1,cols/2 + xROI) +           // Pixel above
                                                      dstImage(rows/2 + yROI + 1,cols/2 + xROI) ) / 2.0;    // Pixel below
        }
    }
    else if(cols >= 3)
    {
        dstImage(rows/2 + yROI,cols/2 + xROI) = ( dstImage(rows/2 + yROI,cols/2 + xROI - 1) +               // Pixel left
                                                  dstImage(rows/2 + yROI,cols/2 + xROI + 1) ) / 4.0;        // Pixel right
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// The following functions build
// vectors of images used for storing
// gaussian/laplacian pyramids
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>

inline int calculateMaxNumberOfPyramidLevel(const blImage<blDataType>& srcImage)
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

    return ( std::log(double(std::min(srcImage.size1ROI(),srcImage.size2ROI()))) /
             std::log(double(2.0)) );
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

    int maxNumberOfPyramidLevels = calculateMaxNumberOfPyramidLevel(srcImage);

    // The first image (level 0)
    // is the source image

    iteratorToBeginningOfGaussianPyramid->wrap(srcImage);

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
                                   const blLaplacianPyramidIteratorType& iteratorToEndOfLaplacianPyramid,
                                   const int& filterType = CV_GAUSSIAN_5x5)
{
    if(iteratorToBeginningOfGaussianPyramid == iteratorToEndOfGaussianPyramid ||
       iteratorToBeginningOfLaplacianPyramid == iteratorToEndOfLaplacianPyramid)
    {
        // Either or both of the
        // pyramids are of zero
        // length and therefore
        // we quit

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

        iteratorToCurrentLaplacianImage->setROI(iteratorToPreviousGaussianImage->getROI(),true);

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


#endif // BL_SIGNALPROCESSING_HPP
