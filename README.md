# blImageAPI

## What is it and Why?

[blImageAPI](https://github.com/navyenzo/blImageAPI.git) is a library that wraps opencv's IplImage* images with a std::shared_ptr smart pointer and makes IplImage* images work with any type of data, work similar to std::vector and work with STL algorithms

* It was started started before OpenCV introduced the c++ version Mat images, and allowed the use of IplImage* images, CvCapture* devices and more with automatic memory release avoiding memory leaks

* The library contains many useful tools and algorithms geared toward making the use of opencv easier.

* It adds special iterators to IplImage* images making them usable in STL algorithms, including Circula>r ROI iterators that can iterate over an image's ROI and can be used with STL algorithms

* The library also adds a few algorithms that ease the use of IplImage* images in algorithms geared towards scientific use

* It is an ongoing project where I collect all the various image analysis related tools I've used in the past and keep using today

* Do with the source what you want, use it in a project, or use it as material

## How do I use it?

* The library is a header only template library that you can use by just including its header as follows: `#include <blImage.hpp>`

* It defines everything inside the `blImageAPI` namespace

* The `blImage<T>` can be used interchangibly with IplImage* and cv::Mat in existing OpenCV algorithms

* For example, creating a 10x10 image/matrix of double values, initializing its entries to -7.3:

..* `blImageAPI::blImage<double> myMatrix(10,10,-7.3);`

* Or using it in an STL algorithm such as `std::copy`:

⋅⋅* `// Create and initialize an unsigned char image

     blImageAPI::blImage<unsigned char> myWord(1,1000,'\0');
    
     // A string we will copy into the unsigned char image

     std::string hello = "hello world\n";
    
     // Copying the string

     std::copy(hello.begin(),hello.end(),myWord.begin());
    
     // Outputting the image to the standard console

     std::cout << myWord << "\n\n";`



## Dependencies

* [OpenCV library](http://opencv.org/)

* [blMathAPI library (another one of my very light weight header only libraries)](https://github.com/navyenzo/blMathAPI.git)

* <memory> from the STL library (used for std::shared_ptr smart pointers)

## License?

MIT License

Do what you like with the source, use it however you like, maybe even as a reference or learning material