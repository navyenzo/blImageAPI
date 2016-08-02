#ifndef BL_BUFFERALGORITHMS2_HPP
#define BL_BUFFERALGORITHMS2_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms2.hpp
// CLASS:           None
// BASE CLASS:      None
//
// PURPOSE:         A collection of simple algorithms
//                  I developed to parse data buffers
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
// DATE CREATED:    Jun/13/2011
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
// FUNCTION:            blStreamLength
//
// ARGUMENTS:           - Stream
//
// TEMPLATE ARGUMENTS:  None
//
// PURPOSE:             This overload function returns the length
//                      of the stream (Input or Output stream)
//
// DEPENDENCIES:        - std::istream
//                      - std::ostream
//
// NOTES:
//-------------------------------------------------------------------
inline int blStreamLength(std::istream& Stream)
{
    Stream.seekg(0,Stream.end);
    int Length = Stream.tellg();
    Stream.seekg(0,Stream.beg);

    return Length;
}

inline int blStreamLength(std::ostream& Stream)
{
    Stream.seekp(0,Stream.end);
    int Length = Stream.tellp();
    Stream.seekp(0,Stream.beg);

    return Length;
}

inline int blStreamLength(std::fstream& Stream)
{
    Stream.seekg(0,Stream.end);
    int Length = Stream.tellg();
    Stream.seekg(0,Stream.beg);

    return Length;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blIsPartiallyEqual
//
// ARGUMENTS:           - Begin1
//                      - End1
//                      - Begin2
//                      - End2
//                      - PredicateFunctor (Second version)
//
// TEMPLATE ARGUMENTS:  - blIterator1Type
//                      - blIterator2Type
//                      - PredicateFunctorType (Second version)
//
// PURPOSE:             - This function compares two buffers for
//                        partial equality, meaning it keeps
//                        comparing the buffers until it reaches
//                        the end of one of the buffers, and if
//                        they are equal up to that point, then
//                        it returns true.
//
// DEPENDENCIES:        None
//
// NOTES:
//-------------------------------------------------------------------
template<typename blBuffer1IteratorType,
         typename blBuffer2IteratorType>
inline bool blIsPartiallyEqual(blBuffer1IteratorType Begin1,
                               blBuffer1IteratorType End1,
                               blBuffer2IteratorType Begin2,
                               blBuffer2IteratorType End2)
{
    // Loop through every
    // element until we
    // reach the end of
    // either buffer or
    // until two elements
    // don't match

    while((Begin1 != End1) && (Begin2 != End2))
    {
        if(!(*Begin1 == *Begin2))
        {
            return false;
        }

        ++Begin1;
        ++Begin2;
    }

    // If the first buffer
    // hasn't reached its
    // end, then it's not
    // partially equal

    if(Begin1 != End1)
        return false;
    else
        return true;
}


template<typename blBuffer1IteratorType,
         typename blBuffer2IteratorType,
         typename blPredicateFunctorType>
inline bool blIsPartiallyEqual(blBuffer1IteratorType Begin1,
                               blBuffer1IteratorType End1,
                               blBuffer2IteratorType Begin2,
                               blBuffer2IteratorType End2,
                               const blPredicateFunctorType& PredicateFunctor)
{
    // Loop through every
    // element until we
    // reach the end of
    // either buffer or
    // until two elements
    // don't match

    while((Begin1 != End1) && (Begin2 != End2))
    {
        if(!PredicateFunctor(*Begin1,*Begin2))
        {
            return false;
        }

        ++Begin1;
        ++Begin2;
    }

    // If the first buffer
    // hasn't reached its
    // end, then it's not
    // partially equal

    if(Begin1 != End1)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blIsEqual
//
// ARGUMENTS:           - Begin1
//                      - End1
//                      - Begin2
//                      - End2
//                      - PredicateFunctor (Second version)
//
// TEMPLATE ARGUMENTS:  - blIterator1Type
//                      - blIterator2Type
//                      - PredicateFunctorType (SecondVersion)
//
// PURPOSE:             - This function compares two buffers for
//                        total equality, meaning the buffers have
//                        to match in everything including their
//                        lengths.
//
// DEPENDENCIES:        None
//
// NOTES:
//-------------------------------------------------------------------
template<typename blBuffer1IteratorType,
         typename blBuffer2IteratorType>
inline bool blIsEqual(blBuffer1IteratorType Begin1,
                      blBuffer1IteratorType End1,
                      blBuffer2IteratorType Begin2,
                      blBuffer2IteratorType End2)
{
    // Loop through every
    // element until we
    // reach the end of
    // either buffer.
    // If we don't reach
    // the ends of both
    // buffers at the same
    // time, then they are
    // not equal

    while((Begin1 != End1) && (Begin2 != End2))
    {
        if(!(*Begin1 == *Begin2))
        {
            return false;
        }

        ++Begin1;
        ++Begin2;
    }

    // Let's make sure that
    // the ends of both
    // buffers have been
    // reached, otherwise
    // we return false.

    if((Begin1 == End1) && (Begin2 == End2))
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<typename blBuffer1IteratorType,
         typename blBuffer2IteratorType,
         typename blPredicateFunctorType>
inline bool blIsEqual(blBuffer1IteratorType Begin1,
                      blBuffer1IteratorType End1,
                      blBuffer2IteratorType Begin2,
                      blBuffer2IteratorType End2,
                      const blPredicateFunctorType& PredicateFunctor)
{
    // Loop through every
    // element until we
    // reach the end of
    // either buffer.
    // If we don't reach
    // the ends of both
    // buffers at the same
    // time, then they are
    // not equal
    while((Begin1 != End1) && (Begin2 != End2))
    {
        if(!PredicateFunctor(*Begin1,*Begin2))
        {
            return false;
        }

        ++Begin1;
        ++Begin2;
    }

    // Let's make sure that
    // the ends of both
    // buffers have been
    // reached, otherwise
    // we return false.
    if((Begin1 == End1) && (Begin2 == End2))
    {
        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Specialized versions of blIsPartiallyEqual and blIsEqual to
// make it easier to work with strings
//-------------------------------------------------------------------
template<typename blStringType,
         typename blBuffer2IteratorType>
inline bool blIsPartiallyEqual(const blStringType& TheString,
                               blBuffer2IteratorType Begin2,
                               blBuffer2IteratorType End2)
{
    for(int i = 0;
        (i < TheString.length()) &&
        Begin2 != End2;
        ++i,++Begin2)
    {
        if(!(TheString[i] == (*Begin2)))
            return false;
    }

    return true;
}

template<typename blStringType,
         typename blBuffer2IteratorType,
         typename blPredicateFunctorType>
inline bool blIsPartiallyEqual(const blStringType& TheString,
                               blBuffer2IteratorType Begin2,
                               blBuffer2IteratorType End2,
                               const blPredicateFunctorType& PredicateFunctor)
{
    for(int i = 0;
        (i < TheString.length()) &&
        Begin2 != End2;
        ++i,++Begin2)
    {
        if(!PredicateFunctor(TheString[i],(*Begin2)))
            return false;
    }

    return true;
}

template<typename blStringType,
         typename blBuffer2IteratorType>
inline bool blIsEqual(const blStringType& TheString,
                      blBuffer2IteratorType Begin2,
                      blBuffer2IteratorType End2)
{
    int i = 0;
    for(;
        (i < TheString.length()) &&
        Begin2 != End2;
        ++i,++Begin2)
    {
        if(!(TheString[i] == (*Begin2)))
            return false;
    }

    if(i == TheString.length() && (Begin2 == End2))
        return true;
    else
        return false;
}

template<typename blStringType,
         typename blBuffer2IteratorType,
         typename blPredicateFunctorType>
inline bool blIsEqual(const blStringType& TheString,
                      blBuffer2IteratorType Begin2,
                      blBuffer2IteratorType End2,
                      const blPredicateFunctorType& PredicateFunctor)
{
    int i = 0;
    for(;
        (i < TheString.length()) &&
        Begin2 != End2;
        ++i,++Begin2)
    {
        if(!PredicateFunctor(TheString[i],(*Begin2)))
            return false;
    }

    if(i == TheString.length() && (Begin2 == End2))
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindLast
//
// ARGUMENTS:           - BeginIter
//                      - EndIter
//                      - Token
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//
// PURPOSE:             This function looks for the last occurrence
//                      of a specified token in a buffer.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenType,
         typename blBufferIteratorType>
inline blBufferIteratorType blFindLast(const blBufferIteratorType& BeginIter,
                                       const blBufferIteratorType& EndIter,
                                       const blTokenType& Token)
{
    blBufferIteratorType CurrentIter = BeginIter;

    blBufferIteratorType IterToLastOccurrence = CurrentIter;

    while(CurrentIter != EndIter)
    {
        if((*CurrentIter) == Token)
            IterToLastOccurrence = CurrentIter;

        ++CurrentIter;

        if(CurrentIter == BeginIter)
            break;
    }

    if((*IterToLastOccurrence) == Token)
        return IterToLastOccurrence;
    else
        return EndIter;
}


template<typename blTokenType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blFindLast(const blBufferIteratorType& BeginIter,
                                       const blBufferIteratorType& EndIter,
                                       const blTokenType& Token,
                                       const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType CurrentIter = BeginIter;

    blBufferIteratorType IterToLastOccurrence = CurrentIter;

    while(CurrentIter != EndIter)
    {
        if(PredicateFunctor((*CurrentIter),Token))
            IterToLastOccurrence = CurrentIter;

        ++CurrentIter;

        if(CurrentIter == BeginIter)
            break;
    }

    if((*IterToLastOccurrence) == Token)
        return IterToLastOccurrence;
    else
        return EndIter;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFind
//
// ARGUMENTS:           - TheTokenToFind
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//
// PURPOSE:             This function looks for a specified token
//                      in a buffer, and can do it in a circular
//                      way.
//                      The function returns a boolean indicating
//                      if the token was found
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenType,
         typename blBufferIteratorType>
inline blBufferIteratorType blFind(const blTokenType& TheTokenToFind,
                                   const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                   const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                   const blBufferIteratorType& IteratorWhereToStartLooking,
                                   const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the token

    // We'll loop circularly
    // if asked to do so

    do
    {
        if(*IteratorToWhereTheTokenWasFoundInTheBuffer == TheTokenToFind)
        {
            // We have found
            // the token
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        }
        else
        {
            // We haven't found
            // the token yet, so
            // we move forward
            // in the buffer
            ++IteratorToWhereTheTokenWasFoundInTheBuffer;
        }

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the token
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFind
//
// ARGUMENTS:           - TheTokenToFind
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//                      - PredicateFunctor
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//                      - blPredicateFunctorType
//
// PURPOSE:             This function looks for a specified token
//                      in a buffer, and can do it in a circular
//                      way, just like the above function but using
//                      a specified predicate functor to test for
//                      equality
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blFind(const blTokenType& TheTokenToFind,
                                   const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                   const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                   const blBufferIteratorType& IteratorWhereToStartLooking,
                                   const bool& ShouldDataBufferBeSearchedCircularly,
                                   const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the token

    // We'll loop circularly
    // if asked to do so

    do
    {
        if(PredicateFunctor(*IteratorToWhereTheTokenWasFoundInTheBuffer,TheTokenToFind))
        {
            // We have found
            // the token
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        }
        else
        {
            // We haven't found
            // the token yet, so
            // we move forward
            // in the buffer
            ++IteratorToWhereTheTokenWasFoundInTheBuffer;
        }

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the token
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFind2
//
// ARGUMENTS:           - TheTokenToFind
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - IteratorWhereToStopLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//
// PURPOSE:             This function looks for a specified token
//                      in a buffer, and can do it in a circular
//                      way.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenType,
         typename blBufferIteratorType>
inline blBufferIteratorType blFind2(const blTokenType& TheTokenToFind,
                                    const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                    const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                    const blBufferIteratorType& IteratorWhereToStartLooking,
                                    const blBufferIteratorType& IteratorWhereToStopLooking,
                                    const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    if(ShouldDataBufferBeSearchedCircularly &&
       IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
    {
        IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
    }

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking &&
          IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer)
    {
        if(*IteratorToWhereTheTokenWasFoundInTheBuffer == TheTokenToFind)
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        else
            ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        if(ShouldDataBufferBeSearchedCircularly &&
           IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}


template<typename blTokenType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blFind2(const blTokenType& TheTokenToFind,
                                    const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                    const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                    const blBufferIteratorType& IteratorWhereToStartLooking,
                                    const blBufferIteratorType& IteratorWhereToStopLooking,
                                    const bool& ShouldDataBufferBeSearchedCircularly,
                                    const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    if(ShouldDataBufferBeSearchedCircularly &&
       IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
    {
        IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
    }

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking &&
          IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer)
    {
        if(PredicateFunctor(*IteratorToWhereTheTokenWasFoundInTheBuffer,TheTokenToFind))
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        else
            ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        if(ShouldDataBufferBeSearchedCircularly &&
           IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blReverseFind2
//
// ARGUMENTS:           - TheTokenToFind
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - IteratorWhereToStopLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//
// PURPOSE:             This function looks for a specified token
//                      in a buffer, but going in the reverse
//                      direction, and can do it in a circular
//                      way.
//
// DEPENDENCIES:        - blBufferIteratorType has to define the
//                        operator--() and operator--(int) operators
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenType,
         typename blBufferIteratorType>
inline blBufferIteratorType blReverseFind2(const blTokenType& TheTokenToFind,
                                           const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                           const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                           const blBufferIteratorType& IteratorWhereToStartLooking,
                                           const blBufferIteratorType& IteratorWhereToStopLooking,
                                           const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    if(ShouldDataBufferBeSearchedCircularly &&
       IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
    {
        IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
    }

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking &&
          IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer)
    {
        if(*IteratorToWhereTheTokenWasFoundInTheBuffer == TheTokenToFind)
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        else
            --IteratorToWhereTheTokenWasFoundInTheBuffer;

        if(ShouldDataBufferBeSearchedCircularly &&
           IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}


template<typename blTokenType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blReverseFind2(const blTokenType& TheTokenToFind,
                                           const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                           const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                           const blBufferIteratorType& IteratorWhereToStartLooking,
                                           const blBufferIteratorType& IteratorWhereToStopLooking,
                                           const bool& ShouldDataBufferBeSearchedCircularly,
                                           const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;

    if(ShouldDataBufferBeSearchedCircularly &&
       IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
    {
        IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
    }

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking &&
          IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer)
    {
        if(PredicateFunctor(*IteratorToWhereTheTokenWasFoundInTheBuffer,TheTokenToFind))
            return IteratorToWhereTheTokenWasFoundInTheBuffer;
        else
            --IteratorToWhereTheTokenWasFoundInTheBuffer;

        if(ShouldDataBufferBeSearchedCircularly &&
           IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindIfNot
//
// ARGUMENTS:           - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//                      - PredicateFunctor
//
// TEMPLATE ARGUMENTS:  - blTokenType
//                      - blBufferIteratorType
//                      - blPredicateFunctorType
//
// PURPOSE:             This function looks for the first "Not" data
//                      element in a buffer based on a specified
//                      predicate functor
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blFindIfNot(const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                        const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                        const blBufferIteratorType& IteratorWhereToStartLooking,
                                        const bool& ShouldDataBufferBeSearchedCircularly,
                                        const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheResultWasFoundInTheBuffer = IteratorWhereToStartLooking;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first "Not"
    // data element

    // We'll loop circularly
    // if asked to do so

    do
    {
        if(!PredicateFunctor(*IteratorToWhereTheResultWasFoundInTheBuffer))
        {
            // We have found
            // the "Not" data
            // element
            return IteratorToWhereTheResultWasFoundInTheBuffer;
        }
        else
        {
            // We haven't found
            // the token yet, so
            // we move forward
            // in the buffer
            ++IteratorToWhereTheResultWasFoundInTheBuffer;
        }

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheResultWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheResultWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheResultWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheResultWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the "Not"
    // data element,
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheResultWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindFirstOf
//
// ARGUMENTS:           - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//
// PURPOSE:             This function looks for the first one of
//                      many tokens specified by the user in a
//                      buffer, and can do it in a circular way.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType>
inline blBufferIteratorType blFindFirstOf(const blTokenIteratorType& IteratorToBeginningOfTokens,
                                          const blTokenIteratorType& IteratorToEndOfTokens,
                                          const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                          const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                          const blBufferIteratorType& IteratorWhereToStartLooking,
                                          const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokens;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    do
    {
        for(IteratorToToken = IteratorToBeginningOfTokens; IteratorToToken != IteratorToEndOfTokens; ++IteratorToToken)
        {
            if(*IteratorToWhereTheTokenWasFoundInTheBuffer == *IteratorToToken)
            {
                // We have found
                // the token
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer
        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the "Not"
    // data element,
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindFirstOf
//
// ARGUMENTS:           - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//                      - PredicateFunctor
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//                      - blPredicateFunctorType
//
// PURPOSE:             This function looks for the first one of
//                      many tokens specified by the user in a
//                      buffer, and can do it in a circular way,
//                      just like the above function but uses
//                      a predicate functor to check for equality
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blFindFirstOf(const blTokenIteratorType& IteratorToBeginningOfTokens,
                                          const blTokenIteratorType& IteratorToEndOfTokens,
                                          const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                          const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                          const blBufferIteratorType& IteratorWhereToStartLooking,
                                          const bool& ShouldDataBufferBeSearchedCircularly,
                                          const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokens;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    do
    {
        for(IteratorToToken = IteratorToBeginningOfTokens; IteratorToToken != IteratorToEndOfTokens; ++IteratorToToken)
        {
            if(PredicateFunctor(*IteratorToWhereTheTokenWasFoundInTheBuffer,*IteratorToToken))
            {
                // We have found
                // the token
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer
        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the "Not"
    // data element,
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSearch
//
// ARGUMENTS:           - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//
// PURPOSE:             This function finds the next occurence
//                      of the specified sequence of data tokens
//                      in a buffer in a circular way
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType>
inline blBufferIteratorType blSearch(const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                     const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                     const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                     const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                     const blBufferIteratorType& IteratorWhereToStartLooking,
                                     const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blBufferIteratorType MyBufferIter2 = IteratorWhereToStartLooking;

    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokenSequence;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    do
    {
        while(*MyBufferIter2 == *IteratorToToken)
        {
            ++IteratorToToken;
            ++MyBufferIter2;

            if(IteratorToToken == IteratorToEndOfTokenSequence)
            {
                // We have found
                // the token
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
            else if((MyBufferIter2 == IteratorToEndOfDataBuffer) && ShouldDataBufferBeSearchedCircularly)
            {
                MyBufferIter2 = IteratorToBeginningOfDataBuffer;
            }
            else if(MyBufferIter2 == IteratorToEndOfDataBuffer)
                break;
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer
        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the "Not"
    // data element,
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSearch
//
// ARGUMENTS:           - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - ShouldDataBufferBeSearchedCircularly
//                      - PredicateFunctor
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//                      - blPredicateFunctorType
//
// PURPOSE:             This function finds the next occurence
//                      of the specified sequence of data tokens
//                      in a buffer in a circular way, just like
//                      the function above but uses a specified
//                      predicate functor to test for equality
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blSearch(const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                     const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                     const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                     const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                     const blBufferIteratorType& IteratorWhereToStartLooking,
                                     const bool& ShouldDataBufferBeSearchedCircularly,
                                     const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blBufferIteratorType MyBufferIter2 = IteratorWhereToStartLooking;

    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokenSequence;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    do
    {
        while(PredicateFunctor(*MyBufferIter2,*IteratorToToken))
        {
            ++IteratorToToken;
            ++MyBufferIter2;

            if(IteratorToToken == IteratorToEndOfTokenSequence)
            {
                // We have found
                // the token
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
            else if((MyBufferIter2 == IteratorToEndOfDataBuffer) && ShouldDataBufferBeSearchedCircularly)
            {
                MyBufferIter2 = IteratorToBeginningOfDataBuffer;
            }
            else if(MyBufferIter2 == IteratorToEndOfDataBuffer)
                break;
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer
        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search
        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
    }
    while(((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorToEndOfDataBuffer) && !ShouldDataBufferBeSearchedCircularly) ||
          ((IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStartLooking) && ShouldDataBufferBeSearchedCircularly));

    // By now, we know
    // that we didn't
    // find the "Not"
    // data element,
    // so we return the
    // iterator where we
    // stopped
    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSearch2
//
// ARGUMENTS:           - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - IteratorWhereToStopLooking
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//
// PURPOSE:             This function finds the next occurence
//                      of the specified sequence of data tokens
//                      in a buffer in a circular way, but in
//                      this version the user can specify where
//                      to stop searching for the token
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType>
inline blBufferIteratorType blSearch2(const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                      const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                      const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                      const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                      const blBufferIteratorType& IteratorWhereToStartLooking,
                                      const blBufferIteratorType& IteratorWhereToStopLooking,
                                      const bool& ShouldDataBufferBeSearchedCircularly)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blBufferIteratorType MyBufferIter2 = IteratorWhereToStartLooking;

    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokenSequence;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking)
    {
        IteratorToToken = IteratorToBeginningOfTokenSequence;

        while(*MyBufferIter2 == *IteratorToToken)
        {
            ++IteratorToToken;
            ++MyBufferIter2;

            if(IteratorToToken == IteratorToEndOfTokenSequence)
            {
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
            else if((MyBufferIter2 == IteratorToEndOfDataBuffer) && ShouldDataBufferBeSearchedCircularly)
            {
                MyBufferIter2 = IteratorToBeginningOfDataBuffer;
            }
            else if(MyBufferIter2 == IteratorToEndOfDataBuffer)
                break;
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer

        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search

        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
        else if(IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
            break;
    }

    // By now, we know
    // that we didn't
    // find the token
    // so we return the
    // iterator where we
    // stopped

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSearch2
//
// ARGUMENTS:           - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - IteratorToBeginningOfDataBuffer
//                      - IteratorToEndOfDataBuffer
//                      - IteratorWhereToStartLooking
//                      - IteratorWhereToStopLooking
//                      - ShouldDataBufferBeSearchedCircularly
//                      - PredicateFunctor
//
// TEMPLATE ARGUMENTS:  - blTokenIteratorType
//                      - blBufferIteratorType
//
// PURPOSE:             This function finds the next occurence
//                      of the specified sequence of data tokens
//                      in a buffer in a circular way, but in
//                      this version the user can specify where
//                      to stop searching for the token.
//                      This is the version that uses a predicate
//                      functor to check for equality.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blTokenIteratorType,
         typename blBufferIteratorType,
         typename blPredicateFunctorType>
inline blBufferIteratorType blSearch2(const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                      const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                      const blBufferIteratorType& IteratorToBeginningOfDataBuffer,
                                      const blBufferIteratorType& IteratorToEndOfDataBuffer,
                                      const blBufferIteratorType& IteratorWhereToStartLooking,
                                      const blBufferIteratorType& IteratorWhereToStopLooking,
                                      const bool& ShouldDataBufferBeSearchedCircularly,
                                      const blPredicateFunctorType& PredicateFunctor)
{
    blBufferIteratorType IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorWhereToStartLooking;
    blBufferIteratorType MyBufferIter2 = IteratorWhereToStartLooking;

    blTokenIteratorType IteratorToToken = IteratorToBeginningOfTokenSequence;

    // We are going to
    // loop through the
    // entire buffer
    // until we find
    // the first data
    // element

    // We'll loop circularly
    // if asked to do so

    while(IteratorToWhereTheTokenWasFoundInTheBuffer != IteratorWhereToStopLooking)
    {
        IteratorToToken = IteratorToBeginningOfTokenSequence;

        while(PredicateFunctor(*MyBufferIter2,*IteratorToToken))
        {
            ++IteratorToToken;
            ++MyBufferIter2;

            if(IteratorToToken == IteratorToEndOfTokenSequence)
            {
                return IteratorToWhereTheTokenWasFoundInTheBuffer;
            }
            else if((MyBufferIter2 == IteratorToEndOfDataBuffer) && ShouldDataBufferBeSearchedCircularly)
            {
                MyBufferIter2 = IteratorToBeginningOfDataBuffer;
            }
            else if(MyBufferIter2 == IteratorToEndOfDataBuffer)
                break;
        }

        // We haven't found
        // one of the tokens
        // yet, so we move
        // forward in the
        // buffer

        ++IteratorToWhereTheTokenWasFoundInTheBuffer;

        // Check if we have
        // reached the end
        // so we can loop
        // around to the
        // beginning if we're
        // doing a circular
        // search

        if((IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer) &&
           ShouldDataBufferBeSearchedCircularly)
        {
            IteratorToWhereTheTokenWasFoundInTheBuffer = IteratorToBeginningOfDataBuffer;
        }
        else if(IteratorToWhereTheTokenWasFoundInTheBuffer == IteratorToEndOfDataBuffer)
            break;
    }

    // By now, we know
    // that we didn't
    // find the token
    // so we return the
    // iterator where we
    // stopped

    return IteratorToWhereTheTokenWasFoundInTheBuffer;
}
//-------------------------------------------------------------------


#endif // BL_BUFFERALGORITHMS2_HPP
