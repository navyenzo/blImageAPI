#ifndef BL_BUFFERALGORITHMS1_HPP
#define BL_BUFFERALGORITHMS1_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms1.hpp
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
// FUNCTION:            - blCopy
//
// ARGUMENTS:           - ReadBegin
//                      - ReadStart
//                      - ReadStop
//                      - ReadEnd
//                      - WriteBegin
//                      - WriteStart
//                      - WriteStop
//                      - WriteEnd
//                      - ShouldReadingBeDoneCircularly
//                      - ShouldWritingBeDoneCircularly
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             Copies data from a Read buffer into
//                      Write buffer, but in a circular way,
//                      and return the total number of data
//                      points copied.
//
// DEPENDENCIES:        - Iterators have to define the
//                        operator++ operation
//
// NOTES:
//-------------------------------------------------------------------
template<typename blReadIteratorType,
         typename blWriteIteratorType>
inline size_t blCopy(const blReadIteratorType& ReadBegin,
                     const blReadIteratorType& ReadStart,
                     const blReadIteratorType& ReadStop,
                     const blReadIteratorType& ReadEnd,
                     const blWriteIteratorType& WriteBegin,
                     const blWriteIteratorType& WriteStart,
                     const blWriteIteratorType& WriteStop,
                     const blWriteIteratorType& WriteEnd,
                     const bool& ShouldReadingBeDoneCircularly,
                     const bool& ShouldWritingBeDoneCircularly)
{
    blReadIteratorType ReadIndex = ReadStart;
    blWriteIteratorType WriteIndex = WriteStart;

    int NumberOfDataPointsCopied = 0;

    while((ReadIndex != ReadStop && WriteIndex != WriteStop) ||
          ( (ReadIndex != ReadEnd && !ShouldReadingBeDoneCircularly) &&
            (WriteIndex != WriteEnd && !ShouldWritingBeDoneCircularly) )
         )
    {
        (*WriteIndex) = (*ReadIndex);

        ++ReadIndex;
        ++WriteIndex;

        ++NumberOfDataPointsCopied;

        if(ReadIndex == ReadEnd && ReadIndex != ReadStop && ShouldReadingBeDoneCircularly)
            ReadIndex = ReadBegin;
        if(WriteIndex == WriteEnd && WriteIndex != WriteStop && ShouldWritingBeDoneCircularly)
            WriteIndex = WriteBegin;
    }

    return NumberOfDataPointsCopied;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            - blCopy2
//
// ARGUMENTS:           - ReadBegin
//                      - ReadStart
//                      - ReadStop
//                      - ReadEnd
//                      - WriteBegin
//                      - WriteStart
//                      - WriteStop
//                      - WriteEnd
//                      - ShouldReadingBeDoneCircularly
//                      - ShouldWritingBeDoneCircularly
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             Like the above function but uses a
//                      do/while loop so that if the Start
//                      and Stop iterator match, the function
//                      loops around until it reach the stop.
//
// DEPENDENCIES:        - Iterators have to define the
//                        operator++ operation
//
// NOTES:
//-------------------------------------------------------------------
template<typename blReadIteratorType,
         typename blWriteIteratorType>
inline size_t blCopy2(const blReadIteratorType& ReadBegin,
                      const blReadIteratorType& ReadStart,
                      const blReadIteratorType& ReadStop,
                      const blReadIteratorType& ReadEnd,
                      const blWriteIteratorType& WriteBegin,
                      const blWriteIteratorType& WriteStart,
                      const blWriteIteratorType& WriteStop,
                      const blWriteIteratorType& WriteEnd,
                      const bool& ShouldReadingBeDoneCircularly,
                      const bool& ShouldWritingBeDoneCircularly)
{
    blReadIteratorType ReadIndex = ReadStart;
    blWriteIteratorType WriteIndex = WriteStart;

    int NumberOfDataPointsCopied = 0;

    do
    {
        (*WriteIndex) = (*ReadIndex);

        ++ReadIndex;
        ++WriteIndex;

        ++NumberOfDataPointsCopied;

        if(ReadIndex == ReadEnd && ReadIndex != ReadStop && ShouldReadingBeDoneCircularly)
            ReadIndex = ReadBegin;
        if(WriteIndex == WriteEnd && WriteIndex != WriteStop && ShouldWritingBeDoneCircularly)
            WriteIndex = WriteBegin;
    }
    while((ReadIndex != ReadStop && WriteIndex != WriteStop) ||
          ( (ReadIndex != ReadEnd && !ShouldReadingBeDoneCircularly) &&
            (WriteIndex != WriteEnd && !ShouldWritingBeDoneCircularly) )
         );

    return NumberOfDataPointsCopied;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTIONS:           - blSequentialCopyingFunctor
//                      - blBulkCopyingFunctor
//                      - blSingleDataPointCopyFunctor
//
// ARGUMENTS:
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             These lambdas can be passed to a data
//                      parsing function to copy data from one
//                      buffer to another.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
class blSequentialCopyingFunctor
{
public:
    blSequentialCopyingFunctor(){}
    ~blSequentialCopyingFunctor(){}

    template<typename blIteratorType1,
             typename blIteratorType2>

    void operator()(blIteratorType1 WhereToStartReading,
                    blIteratorType1 WhereToEndReading,
                    blIteratorType2 WhereToStartWriting)const
    {
        std::copy(WhereToStartReading,WhereToEndReading,WhereToStartWriting);
    }
};
blSequentialCopyingFunctor MySequentialCopyingFunctor;


class blBulkCopyingFunctor
{
public:
    blBulkCopyingFunctor(){}
    ~blBulkCopyingFunctor(){}

    template<typename blIteratorType1,
             typename blIteratorType2,
             typename blIntegerType>

    void operator()(blIteratorType2 WhereToStartWriting,
                    blIteratorType1 WhereToStartReading,
                    const blIntegerType& NumberOfBytesToCopy)const
    {
        std::memcpy(WhereToStartWriting,WhereToStartReading,NumberOfBytesToCopy);
    }
};
blBulkCopyingFunctor MyBulkCopyingFunctor;


class blSingleDataPointCopyFunctor
{
public:
    blSingleDataPointCopyFunctor(){}
    ~blSingleDataPointCopyFunctor(){}

    template<typename blDataType,
             typename blDataType2>

    void operator()(blDataType& ObjectToWriteTo,
                    const blDataType2& ObjectToReadFrom)const
    {
        ObjectToWriteTo = ObjectToReadFrom;
    }
};
blSingleDataPointCopyFunctor MySingleDataPointCopyFunctor;
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blParseData
//
// ARGUMENTS:           - SequentialParser
//                      - BulkParser
//                      - IteratorToBeginningOfReadBuffer
//                      - IteratorToEndOfReadBuffer
//                      - IteratorToBeginningOfWriteBuffer
//                      - IteratorToEndOfWriteBuffer
//                      - IteratorWhereToStartWriting
//                      - ShouldParsedDataBeWrittenCircularly
//                      - AreBothTheReadAndWriteBuffersContiguous
//                      - NumOfDataPointsActuallyParsed
//                      - IteratorAfterTheLastReadElementInTheReadBuffer,
//                      - IteratorAfterTheLastParsedElementInTheWriteBuffer
//
// TEMPLATE ARGUMENTS:  - blSequentialParsingFunctorType
//                      - blBulkParsingFunctorType
//                      - blReadIteratorType
//                      - blParseIteratorType
//
// PURPOSE:             This function parses data from a Read
//                      Buffer into a Write Buffer and returns
//                      the amount of data actually parsed.
//                      The function can stop when the end iterator
//                      is reached or it can loop around circularly.
//                      The user can specify whether or not the read
//                      and write buffers are contiguous.
//                      The user also specifies whether the data
//                      should be parsed to the write buffer
//                      circularly.
//                      The function also saves that amount of data
//                      that was actually parsed, the iterator in the
//                      Read buffer pointing to the position after the
//                      last read element, and the iterator in the
//                      Write buffer pointing to the position after
//                      the last parsed element.
//                      The user supplies the function with a
//                      functor to parse data sequentially and a
//                      functor to parse data in bulk.
//
// DEPENDENCIES:
//
// NOTES:
//-------------------------------------------------------------------
template<typename blSequentialParsingFunctorType,
         typename blBulkParsingFunctorType,
         typename blReadIteratorType,
         typename blWriteIteratorType>
inline void blParseData(const blSequentialParsingFunctorType& SequentialParser,
                        const blBulkParsingFunctorType& BulkParser,
                        const blReadIteratorType& IteratorToBeginningOfReadBuffer,
                        const blReadIteratorType& IteratorToEndOfReadBuffer,
                        const blWriteIteratorType& IteratorToBeginningOfWriteBuffer,
                        const blWriteIteratorType& IteratorToEndOfWriteBuffer,
                        const blWriteIteratorType& IteratorWhereToStartWriting,
                        const bool& ShouldParsedDataBeWrittenCircularly,
                        const bool& AreBothTheReadAndWriteBuffersContiguous,
                        int& NumOfDataPointsActuallyParsed,
                        blReadIteratorType& IteratorAfterTheLastReadElementInTheReadBuffer,
                        blWriteIteratorType& IteratorAfterTheLastParsedElementInTheWriteBuffer)
{
    // Initialize the returns
    NumOfDataPointsActuallyParsed = 0;
    IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;
    IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorWhereToStartWriting;

    // Now we check if the writing
    // is to be done circularly
    if(ShouldParsedDataBeWrittenCircularly)
    {
        // In this case the writing
        // is done circularly.

        // We first calculate the
        // lengths of the read buffer
        // and the write buffer

        int ReadBufferLength = std::distance(IteratorToBeginningOfReadBuffer,IteratorToEndOfReadBuffer);
        int WriteBufferLength = std::distance(IteratorToBeginningOfWriteBuffer,IteratorToEndOfWriteBuffer);

        if(ReadBufferLength <= 0 || WriteBufferLength <= 0)
        {
            // Error -- In this case the user
            //          passed zero length read
            //          or write buffers
            return;
        }

        // We then calculate the
        // distance from the writing
        // point in the write buffer
        // to the end of the write buffer
        int RemainingWriteBufferDistance = std::distance(IteratorWhereToStartWriting,IteratorToEndOfWriteBuffer);
        if(RemainingWriteBufferDistance < 0)
            RemainingWriteBufferDistance = 0;

        blReadIteratorType IteratorWhereToStartReading = IteratorToBeginningOfReadBuffer;

        // Let's do the copying of
        // the data right before
        // circularly going around
        // the last time
        if(ReadBufferLength >= WriteBufferLength)
        {
            // Now we calculate how much
            // data from the read buffer
            // would be written to the
            // write buffer after having
            // circularly written the data
            int AA = (ReadBufferLength - RemainingWriteBufferDistance) / WriteBufferLength;
            int BB = (ReadBufferLength - RemainingWriteBufferDistance) % WriteBufferLength;

            std::advance(IteratorWhereToStartReading,RemainingWriteBufferDistance + AA * WriteBufferLength);

            // First we copy the part
            // of the reading buffer that
            // would have wrapped around
            // the writing buffer right
            // before finishing copying
            if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
            {
                // In this case the data types
                // pointed to by the different
                // iterators is of the same size
                // and the read and write buffers
                // are contiguous, so we simply
                // copy the entire memory block
                // at once
                BulkParser(IteratorToBeginningOfWriteBuffer,
                           IteratorWhereToStartReading,
                           sizeof(*IteratorToBeginningOfReadBuffer) * (ReadBufferLength - (RemainingWriteBufferDistance + AA * WriteBufferLength)));
            }
            else
            {
                SequentialParser(IteratorWhereToStartReading,
                                 IteratorToEndOfReadBuffer,
                                 IteratorToBeginningOfWriteBuffer);
            }

            // Update the number of data
            // points that were copied
            NumOfDataPointsActuallyParsed += ReadBufferLength - (RemainingWriteBufferDistance + AA * WriteBufferLength);

            IteratorWhereToStartReading = IteratorToBeginningOfReadBuffer;
            std::advance(IteratorWhereToStartReading,
                         RemainingWriteBufferDistance + AA * WriteBufferLength - (WriteBufferLength - BB));

            IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToBeginningOfWriteBuffer;
            std::advance(IteratorAfterTheLastParsedElementInTheWriteBuffer,BB);

            blReadIteratorType IteratorWhereToStopReading = IteratorWhereToStartReading;
            std::advance(IteratorWhereToStopReading,RemainingWriteBufferDistance + AA * WriteBufferLength);

            // Now copy the data that
            // would have been copied
            // right before wrapping
            // around the write buffer
            // the last time around
            if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
            {
                // In this case the data types
                // pointed to by the different
                // iterators is of the same size
                // so we simply copy the entire
                // memory block at once
                BulkParser(IteratorAfterTheLastParsedElementInTheWriteBuffer,
                           IteratorWhereToStartReading,
                           sizeof(*IteratorToBeginningOfReadBuffer) * (WriteBufferLength - BB));
            }
            else
            {
                SequentialParser(IteratorWhereToStartReading,
                                 IteratorWhereToStopReading,
                                 IteratorAfterTheLastParsedElementInTheWriteBuffer);
            }

            // Update the number of data
            // points that were copied
            NumOfDataPointsActuallyParsed += WriteBufferLength - BB;
        }
        else
        {
            if(ReadBufferLength > RemainingWriteBufferDistance)
            {
                if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
                {
                    // In this case the data types
                    // pointed to by the different
                    // iterators is of the same size
                    // so we simply copy the entire
                    // memory block at once
                    BulkParser(IteratorWhereToStartWriting,
                               IteratorToBeginningOfReadBuffer,
                               sizeof(*IteratorToBeginningOfReadBuffer) * RemainingWriteBufferDistance);

                    BulkParser(IteratorToBeginningOfWriteBuffer,
                               IteratorToBeginningOfReadBuffer + RemainingWriteBufferDistance,
                               sizeof(*IteratorToBeginningOfReadBuffer) * (ReadBufferLength - RemainingWriteBufferDistance));
                }
                else
                {
                    SequentialParser(IteratorToBeginningOfReadBuffer,
                                     IteratorToBeginningOfReadBuffer + RemainingWriteBufferDistance,
                                     IteratorWhereToStartWriting);

                    SequentialParser(IteratorToBeginningOfReadBuffer + RemainingWriteBufferDistance,
                                     IteratorToEndOfReadBuffer,
                                     IteratorToBeginningOfWriteBuffer);
                }

                // Update the number of data
                // points that were copied
                NumOfDataPointsActuallyParsed += ReadBufferLength - RemainingWriteBufferDistance;

                IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorWhereToStartWriting;
                std::advance(IteratorAfterTheLastParsedElementInTheWriteBuffer,RemainingWriteBufferDistance);

                IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;
                std::advance(IteratorAfterTheLastReadElementInTheReadBuffer,RemainingWriteBufferDistance);
            }
            else
            {
                if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
                {
                    // In this case the data types
                    // pointed to by the different
                    // iterators is of the same size
                    // so we simply copy the entire
                    // memory block at once
                    BulkParser(IteratorWhereToStartWriting,
                               IteratorToBeginningOfReadBuffer,
                               sizeof(*IteratorToBeginningOfReadBuffer) * ReadBufferLength);
                }
                else
                {
                    SequentialParser(IteratorToBeginningOfReadBuffer,
                                     IteratorToEndOfReadBuffer,
                                     IteratorWhereToStartWriting);
                }

                // Update the number of data
                // points that were copied
                NumOfDataPointsActuallyParsed += ReadBufferLength;

                IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorWhereToStartWriting;
                std::advance(IteratorAfterTheLastParsedElementInTheWriteBuffer,ReadBufferLength);

                IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;
                std::advance(IteratorAfterTheLastReadElementInTheReadBuffer,ReadBufferLength);
            }
        }
    }
    else
    {
        // In this case the writing
        // is non circular.

        // Calculate the read buffer
        // length and the remaining
        // write buffer length
        int ReadBufferLength = std::distance(IteratorToBeginningOfReadBuffer,IteratorToEndOfReadBuffer);
        int RemainingWriteBufferDistance = std::distance(IteratorWhereToStartWriting,IteratorToEndOfWriteBuffer);

        if(ReadBufferLength <= 0 || RemainingWriteBufferDistance <= 0)
        {
            // Error -- In this case the user
            //          passed zero length read
            //          or write buffers
            return;
        }

        if(ReadBufferLength > RemainingWriteBufferDistance)
        {
            IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;
            std::advance(IteratorAfterTheLastReadElementInTheReadBuffer,RemainingWriteBufferDistance);
            IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToEndOfWriteBuffer;

            if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
            {
                BulkParser(IteratorWhereToStartWriting,
                           IteratorToBeginningOfReadBuffer,
                           sizeof(*IteratorToBeginningOfReadBuffer) * RemainingWriteBufferDistance);
            }
            else
            {
                SequentialParser(IteratorToBeginningOfReadBuffer,
                                 IteratorAfterTheLastReadElementInTheReadBuffer,
                                 IteratorWhereToStartWriting);
            }

            // Update the number of data
            // points that were copied
            NumOfDataPointsActuallyParsed += RemainingWriteBufferDistance;
        }
        else
        {
            IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;
            IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToEndOfWriteBuffer;
            std::advance(IteratorAfterTheLastParsedElementInTheWriteBuffer,ReadBufferLength);

            // In this case the writing
            // is non circular.
            if((sizeof(*IteratorToBeginningOfReadBuffer) == sizeof(*IteratorToBeginningOfWriteBuffer)) && AreBothTheReadAndWriteBuffersContiguous)
            {
                BulkParser(IteratorWhereToStartWriting,
                           IteratorToBeginningOfReadBuffer,
                           sizeof(*IteratorToBeginningOfReadBuffer) * ReadBufferLength);
            }
            else
            {
                SequentialParser(IteratorToBeginningOfReadBuffer,
                                 IteratorToEndOfReadBuffer,
                                 IteratorToBeginningOfWriteBuffer);
            }

            // Update the number of data
            // points that were copied
            NumOfDataPointsActuallyParsed += ReadBufferLength;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blParseData2
//
// ARGUMENTS:           - SequentialParser
//                      - BulkParser
//                      - HowManyDataElemenetsToParse
//                      - IteratorToBeginningOfReadBuffer
//                      - IteratorToEndOfReadBuffer
//                      - IteratorWhereToStartReading,
//                      - IteratorToBeginningOfWriteBuffer
//                      - IteratorToEndOfWriteBuffer
//                      - IteratorWhereToStartWriting
//                      - ShouldReadingBeDoneCircularly
//                      - ShouldWritingBeDoneCircularly
//                      - AreBothTheReadAndWriteBuffersContiguous
//                      - NumOfDataPointsActuallyCopied
//                      - IteratorAfterTheLastReadElementInTheReadBuffer,
//                      - teratorAfterTheLastWrittenElementInTheWriteBuffer
//
// TEMPLATE ARGUMENTS:  - blSequentialParsingFunctorType
//                      - typename blBulkParsingFunctorType
//                      - typename blSingleDataPointParsingFunctorType
//                      - blReadIteratorType
//                      - blWriteIteratorType
//
// PURPOSE:             Just like the function above, this
//                      function parses data from a Read buffer
//                      into a Write buffer, but this function
//                      can do the reading circularly.
//                      The function returns the amount of data
//                      that was actually parsed.
//                      The function can stop when the end iterator
//                      is reached or it can loop around circularly,
//                      for both the Read and Write buffers.
//                      The user can specify whether or not the read
//                      and write buffers are contiguous
//                      The user also specifies whether the data
//                      should be read/written circularly
//
// DEPENDENCIES:        - std::distance
//                      - std::memcpy
//                      - std::copy
//
// NOTES:
//-------------------------------------------------------------------
template<typename blSequentialParsingFunctorType,
         typename blBulkParsingFunctorType,
         typename blSingleDataPointParsingFunctorType,
         typename blReadIteratorType,
         typename blWriteIteratorType>
inline void blParseData2(const blSequentialParsingFunctorType& SequentialParser,
                         const blBulkParsingFunctorType& BulkParser,
                         const blSingleDataPointParsingFunctorType& SingleDataPointParser,
                         const int& HowManyDataElemenetsToParse,
                         const blReadIteratorType& IteratorToBeginningOfReadBuffer,
                         const blReadIteratorType& IteratorToEndOfReadBuffer,
                         const blReadIteratorType& IteratorWhereToStartReading,
                         const blWriteIteratorType& IteratorToBeginningOfWriteBuffer,
                         const blWriteIteratorType& IteratorToEndOfWriteBuffer,
                         const blWriteIteratorType& IteratorWhereToStartWriting,
                         const bool& ShouldReadingBeDoneCircularly,
                         const bool& ShouldWritingBeDoneCircularly,
                         const bool& AreBothTheReadAndWriteBuffersContiguous,
                         int& NumOfDataPointsActuallyParsed,
                         blReadIteratorType& IteratorAfterTheLastReadElementInTheReadBuffer,
                         blWriteIteratorType& IteratorAfterTheLastParsedElementInTheWriteBuffer)
{
    // In this case the user has
    // specified that the reading
    // should be done circularly

    // First we calculate the
    // distances in the Read
    // buffer
    int RemainingReadBufferDistance = std::distance(IteratorWhereToStartReading,IteratorToEndOfReadBuffer);
    if(RemainingReadBufferDistance < 0)
        RemainingReadBufferDistance = 0;

    if(!ShouldReadingBeDoneCircularly)
    {
        if(HowManyDataElemenetsToParse >= RemainingReadBufferDistance)
        {
            // In this case, we just
            // call the function above
            blParseData(SequentialParser,
                        BulkParser,
                        IteratorWhereToStartReading,
                        IteratorToEndOfReadBuffer,
                        IteratorToBeginningOfWriteBuffer,
                        IteratorToEndOfWriteBuffer,
                        IteratorWhereToStartWriting,
                        ShouldWritingBeDoneCircularly,
                        AreBothTheReadAndWriteBuffersContiguous,
                        NumOfDataPointsActuallyParsed,
                        IteratorAfterTheLastReadElementInTheReadBuffer,
                        IteratorAfterTheLastParsedElementInTheWriteBuffer);

            return;
        }
        else
        {
            // In this case, we also
            // call the above function
            // but we set the appropriate
            // iterator to end of Read buffer
            // to only read the requested amount
            // of data
            blReadIteratorType NewIteratorToEndOfReadBuffer = IteratorWhereToStartReading;
            std::advance(NewIteratorToEndOfReadBuffer,HowManyDataElemenetsToParse);

            blParseData(SequentialParser,
                        BulkParser,
                        IteratorWhereToStartReading,
                        NewIteratorToEndOfReadBuffer,
                        IteratorToBeginningOfWriteBuffer,
                        IteratorToEndOfWriteBuffer,
                        IteratorWhereToStartWriting,
                        ShouldWritingBeDoneCircularly,
                        AreBothTheReadAndWriteBuffersContiguous,
                        NumOfDataPointsActuallyParsed,
                        IteratorAfterTheLastReadElementInTheReadBuffer,
                        IteratorAfterTheLastParsedElementInTheWriteBuffer);

            return;
        }
    }
    else
    {
        if(HowManyDataElemenetsToParse <= RemainingReadBufferDistance)
        {
            // In this case, we also
            // call the above function
            // but we set the appropriate
            // iterator to end of Read buffer
            // to only read the requested amount
            // of data
            blReadIteratorType NewIteratorToEndOfReadBuffer = IteratorWhereToStartReading;
            std::advance(NewIteratorToEndOfReadBuffer,HowManyDataElemenetsToParse);

            blParseData(SequentialParser,
                        BulkParser,
                        IteratorWhereToStartReading,
                        NewIteratorToEndOfReadBuffer,
                        IteratorToBeginningOfWriteBuffer,
                        IteratorToEndOfWriteBuffer,
                        IteratorWhereToStartWriting,
                        ShouldWritingBeDoneCircularly,
                        AreBothTheReadAndWriteBuffersContiguous,
                        NumOfDataPointsActuallyParsed,
                        IteratorAfterTheLastReadElementInTheReadBuffer,
                        IteratorAfterTheLastParsedElementInTheWriteBuffer);

            return;
        }
        else
        {
            // In this case we have to
            // copy more elements than
            // there are remaining in the
            // Read buffer, so we circle
            // around the Read buffer and
            // parse

            // TODO:    This part of this
            //          function can be further
            //          optimized

            // Calculate the Read Buffer
            // and Write buffer lengths
            int ReadBufferLength = std::distance(IteratorToBeginningOfReadBuffer,IteratorToEndOfReadBuffer);
            int WriteBufferLength = std::distance(IteratorToBeginningOfWriteBuffer,IteratorToEndOfWriteBuffer);

            if(ReadBufferLength <= 0 || WriteBufferLength <= 0)
            {
                // Error -- In this case, we
                //          tried to parse data
                //          from/to a zero length
                //          buffer
                NumOfDataPointsActuallyParsed = 0;
                IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToEndOfReadBuffer;
                IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToEndOfWriteBuffer;
                return;
            }
            else
            {
                // The iterators that we'll
                // use to iterate throught the
                // data
                blReadIteratorType IteratorToReadBufferPosition = IteratorWhereToStartReading;
                blWriteIteratorType IteratorToWriteBufferPosition = IteratorWhereToStartWriting;

                int RemainingWriteBufferDistance = std::distance(IteratorWhereToStartWriting,IteratorToEndOfWriteBuffer);

                if(ShouldWritingBeDoneCircularly)
                {
                    // Let's make sure that the
                    // iterators don't point to
                    // the end of the data
                    if(RemainingReadBufferDistance <= 0)
                        IteratorToReadBufferPosition = IteratorToBeginningOfReadBuffer;

                    if(RemainingWriteBufferDistance <= 0)
                        IteratorToWriteBufferPosition = IteratorToBeginningOfWriteBuffer;

                    // Circle throught the
                    // data and parse
                    for(int i = 0;
                        i < HowManyDataElemenetsToParse;
                        ++i,
                        ++IteratorToReadBufferPosition,
                        ++IteratorToWriteBufferPosition)
                    {
                        // Check the iterators
                        // against their respective
                        // end iterators
                        if(IteratorToReadBufferPosition == IteratorToEndOfReadBuffer)
                            IteratorToReadBufferPosition = IteratorToBeginningOfReadBuffer;
                        if(IteratorToWriteBufferPosition == IteratorToEndOfWriteBuffer)
                            IteratorToWriteBufferPosition = IteratorToBeginningOfWriteBuffer;

                        // Parse the data element
                        SingleDataPointParser(*IteratorToWriteBufferPosition,*IteratorToReadBufferPosition);

                    }

                    NumOfDataPointsActuallyParsed = HowManyDataElemenetsToParse;
                    IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToReadBufferPosition;
                    IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToWriteBufferPosition;

                    if(IteratorAfterTheLastReadElementInTheReadBuffer != IteratorToEndOfReadBuffer)
                        ++IteratorAfterTheLastReadElementInTheReadBuffer;
                    else
                        IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;

                    if(IteratorAfterTheLastParsedElementInTheWriteBuffer != IteratorToEndOfWriteBuffer)
                        ++IteratorAfterTheLastParsedElementInTheWriteBuffer;
                    else
                        IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToBeginningOfWriteBuffer;
                }
                else if(RemainingWriteBufferDistance > 0)
                {
                    for(int i = 0;
                        ((i < HowManyDataElemenetsToParse) && (IteratorToWriteBufferPosition != IteratorToEndOfWriteBuffer));
                        ++i,
                        ++IteratorToReadBufferPosition,
                        ++IteratorToWriteBufferPosition)
                    {
                        // Check the Read iterator
                        // against its end iterator
                        if(IteratorToReadBufferPosition == IteratorToEndOfReadBuffer)
                            IteratorToReadBufferPosition = IteratorToBeginningOfReadBuffer;

                        // Parse the data element
                        SingleDataPointParser(*IteratorToWriteBufferPosition,*IteratorToReadBufferPosition);
                    }

                    NumOfDataPointsActuallyParsed = HowManyDataElemenetsToParse;
                    IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToReadBufferPosition;
                    IteratorAfterTheLastParsedElementInTheWriteBuffer = IteratorToWriteBufferPosition;

                    if(IteratorAfterTheLastReadElementInTheReadBuffer != IteratorToEndOfReadBuffer)
                        ++IteratorAfterTheLastReadElementInTheReadBuffer;
                    else
                        IteratorAfterTheLastReadElementInTheReadBuffer = IteratorToBeginningOfReadBuffer;

                    if(IteratorAfterTheLastParsedElementInTheWriteBuffer != IteratorToEndOfWriteBuffer)
                        ++IteratorAfterTheLastParsedElementInTheWriteBuffer;
                }
            }
        }
    }
}
//-------------------------------------------------------------------


#endif // BL_BUFFERALGORITHMS1_HPP
