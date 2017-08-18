#blImageAPI

[blImageAPI](https://github.com/navyenzo/blImageAPI.git) is a library that wraps opencv's IplImage* images with a std::shared_ptr smart pointer and makes IplImage* images work with any type of data, work similar to std::vector and work with STL algorithms

>-- It was started started before OpenCV introduced the c++ version Mat images, and allowed the use of IplImage* images, CvCapture* devices and more with automatic memory release avoiding memory leaks

>-- The library contains many useful tools and algorithms geared toward making the use of opencv easier.

>-- It adds special iterators to IplImage* images making them usable in STL algorithms, including Circula>r ROI iterators that can iterate over an image's ROI and can be used with STL algorithms

>-- The library also adds a few algorithms that ease the use of IplImage* images in algorithms geared towards scientific use

>-- It is an ongoing project where I collect all the various image analysis related tools I've used in the past and keep using today

>-- Do with the source what you want, use it in a project, or use it as material