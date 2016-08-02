#ifndef BL_BUFFERALGORITHMS_HPP
#define BL_BUFFERALGORITHMS_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms.hpp
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

    template<typename blDataType,typename blDataType2>
    void operator()(blDataType* WhereToStartReading,
                    blDataType* WhereToEndReading,
                    blDataType2* WhereToStartWriting)const
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

    template<typename blDataType,typename blDataType2>
    void operator()(blDataType2* WhereToStartWriting,
                    blDataType* WhereToStartReading,
                    const int& NumberOfBytesToCopy)const
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

    template<typename blDataType,typename blDataType2>
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


//-------------------------------------------------------------------
// FUNCTION:            blCountRowsAndColumnsFromCSVData
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ColToken
//                      - DoubleQuotesToken
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - NumberOfRows
//                      - NumberOfCols
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blDataType -- The type of data
//
// PURPOSE:             This function counts the number of rows
//                      and number of columns in a CSV data array.
//                      The function counts the number of
//                      rows by searching for the "row" token.
//                      At each row, the function counts the
//                      number of columns by searching for the
//                      "column" token.
//                      It returns the number of rows and the
//                      maximum number of columns.
//                      So if each row has a different number
//                      of columns, this function returns the
//                      maximum number of columns.
//                      This function also allows for the CSV
//                      data to have "double quotes" in which
//                      we don't count "row" and "column" tokens
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:               For example:  If we want to count the number
//                                    of rows and columns in a CSV
//                                    file, we would for example
//                                    make the "\n" the row token,
//                                    and the "," the column token.
//                                    If the CSV file contains data
//                                    within double quotes, then
//                                    we would make the "\"" the
//                                    double quotes token.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blDataType>
inline void blCountRowsAndColumnsFromCSVData(const blDataIteratorType& IteratorToBeginningOfData,
                                             const blDataIteratorType& IteratorToEndOfData,
                                             const blDataType& RowToken,
                                             const blDataType& ColToken,
                                             const blDataType& DoubleQuotesToken,
                                             const bool& ShouldZeroSizedRowsBeCounted,
                                             const bool& ShouldZeroSizedColsBeCounted,
                                             int& NumberOfRows,
                                             int& NumberOfCols)
{
    // Check for zero
    // sized data buffer
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // default the number of rows
        // and columns to 0
        NumberOfRows = int(0);
        NumberOfCols = int(0);
        return;
    }

    // Initialize the number
    // of row and column that
    // we're currently parsing
    if(ShouldZeroSizedRowsBeCounted)
        NumberOfRows = 1;
    else
        NumberOfRows = 0;

    if(ShouldZeroSizedColsBeCounted)
        NumberOfCols = 1;
    else
        NumberOfCols = 0;

    // Variable used to know if
    // we're currently parsing
    // inside double quotes
    bool AreWeCurrentlyInDoubleQuotes = false;

    // Variable used to count the
    // number of columns for the
    // current row
    int NumberOfColsForCurrentRow = 0;

    // Iterator pointing to
    // the previously found
    // column token
    blDataIteratorType IteratorToPreviousColToken = IteratorToBeginningOfData;

    // Iterator used to loop
    // through the data
    blDataIteratorType DataIterator = IteratorToBeginningOfData;

    // Iterator to the
    // previously parsed
    // data element
    blDataIteratorType PreviousDataIterator = IteratorToBeginningOfData;

    // Loop through the data
    // one data point at a time
    while(DataIterator != IteratorToEndOfData)
    {
        if(AreWeCurrentlyInDoubleQuotes)
        {
            // Check for double
            // quotes token
            if(*DataIterator == DoubleQuotesToken)
                AreWeCurrentlyInDoubleQuotes = false;
        }
        else
        {
            // Check for double
            // quotes token
            if(*DataIterator == DoubleQuotesToken)
            {
                AreWeCurrentlyInDoubleQuotes = true;
            }

            // Check for
            // row token
            else if(*DataIterator == RowToken)
            {
                if(NumberOfColsForCurrentRow <= 0)
                {
                    if(ShouldZeroSizedRowsBeCounted)
                        ++NumberOfRows;
                }
                else
                    ++NumberOfRows;

                if(NumberOfColsForCurrentRow > NumberOfCols)
                    NumberOfCols = NumberOfColsForCurrentRow;

                if(ShouldZeroSizedColsBeCounted)
                    NumberOfColsForCurrentRow = 1;
                else
                    NumberOfColsForCurrentRow = 0;

                IteratorToPreviousColToken = DataIterator;
            }

            // Check for
            // column token
            else if(*DataIterator == ColToken)
            {
                if(DataIterator == IteratorToPreviousColToken)
                {
                    // In this case, we
                    // have a zero sized
                    // column.
                    if(ShouldZeroSizedColsBeCounted)
                    {
                        ++NumberOfColsForCurrentRow;
                        IteratorToPreviousColToken = DataIterator;
                    }
                }
                else
                {
                    ++NumberOfColsForCurrentRow;
                    IteratorToPreviousColToken = DataIterator;
                }
            }
        }

        // Store the iterator
        // to the previous data
        // element
        PreviousDataIterator = DataIterator;

        // Move along the
        // data array
        ++DataIterator;
    }

    // Check to see if the
    // last data element
    // was a column token
    // or row token
    if(*PreviousDataIterator != ColToken)
        ++NumberOfColsForCurrentRow;
    else if(ShouldZeroSizedColsBeCounted)
        ++NumberOfColsForCurrentRow;

    // Check one more time
    // for the last row
    if(NumberOfColsForCurrentRow > NumberOfCols)
        NumberOfCols = NumberOfColsForCurrentRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindNthRowAndMthColumnFromCSVData
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowIndex
//                      - ColIndex
//                      - RowToken
//                      - ColToken
//                      - DoubleQuotesToken
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - IteratorToNthRowAndMthColumnInData
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blDataType -- The type of data
//
// PURPOSE:             This function finds the Nth row and
//                      Mth column in a CSV data array.
//                      If the specified row and column cannot
//                      be found, it saves the position to the
//                      row/column right after, and return false
//                      to let the user know it did not find it.
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blDataType>
inline bool blFindNthRowAndMthColumnFromCSVData(const blDataIteratorType& IteratorToBeginningOfData,
                                                const blDataIteratorType& IteratorToEndOfData,
                                                const int& RowIndex,
                                                const int& ColIndex,
                                                const blDataType& RowToken,
                                                const blDataType& ColToken,
                                                const blDataType& DoubleQuotesToken,
                                                const bool& ShouldZeroSizedRowsBeCounted,
                                                const bool& ShouldZeroSizedColsBeCounted,
                                                blDataIteratorType& IteratorToNthRowAndMthColumnInData)
{
    // Check for zero
    // sized data buffer
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // quit
        return false;
    }

    // Initialize the number
    // of row and column that
    // we're currently parsing
    int NumberOfRows = 0;
    if(ShouldZeroSizedRowsBeCounted)
        NumberOfRows = 1;
    else
        NumberOfRows = 0;

    int NumberOfCols = 0;
    if(ShouldZeroSizedColsBeCounted)
        NumberOfCols = 1;
    else
        NumberOfCols = 0;

    // Variable used to know if
    // we're currently parsing
    // inside double quotes
    bool AreWeCurrentlyInDoubleQuotes = false;

    // Variable used to count the
    // number of columns for the
    // current row
    int NumberOfColsForCurrentRow = 0;

    // Iterator pointing to
    // the previously found
    // column token
    blDataIteratorType IteratorToPreviousColToken = IteratorToBeginningOfData;

    // Iterator used to loop
    // through the data
    blDataIteratorType DataIterator = IteratorToBeginningOfData;

    // Iterator to the
    // previously parsed
    // data element
    blDataIteratorType PreviousDataIterator = IteratorToBeginningOfData;

    // Loop through the data
    // one data point at a time
    while(DataIterator != IteratorToEndOfData)
    {
        // Let's check to see
        // if we're at the
        // specified place
        if((NumberOfRows == RowIndex) && (NumberOfCols == ColIndex))
        {
            // This means that
            // we have found the
            // specified place in
            // the data buffer, so
            // we quit
            return true;
        }

        if(AreWeCurrentlyInDoubleQuotes)
        {
            // Check for double
            // quotes token
            if(*DataIterator == DoubleQuotesToken)
                AreWeCurrentlyInDoubleQuotes = false;
        }
        else
        {
            // Check for double
            // quotes token
            if(*DataIterator == DoubleQuotesToken)
            {
                AreWeCurrentlyInDoubleQuotes = true;
            }

            // Check for
            // row token
            else if(*DataIterator == RowToken)
            {
                if(NumberOfColsForCurrentRow <= 0)
                {
                    if(ShouldZeroSizedRowsBeCounted)
                        ++NumberOfRows;
                }
                else
                    ++NumberOfRows;

                if(NumberOfColsForCurrentRow > NumberOfCols)
                    NumberOfCols = NumberOfColsForCurrentRow;

                if(ShouldZeroSizedColsBeCounted)
                    NumberOfColsForCurrentRow = 1;
                else
                    NumberOfColsForCurrentRow = 0;

                IteratorToPreviousColToken = DataIterator;
            }

            // Check for
            // column token
            else if(*DataIterator == ColToken)
            {
                if(DataIterator == IteratorToPreviousColToken)
                {
                    // In this case, we
                    // have a zero sized
                    // column.
                    if(ShouldZeroSizedColsBeCounted)
                    {
                        ++NumberOfColsForCurrentRow;
                        IteratorToPreviousColToken = DataIterator;
                    }
                }
                else
                {
                    ++NumberOfColsForCurrentRow;
                    IteratorToPreviousColToken = DataIterator;
                }
            }
        }

        // Store the iterator
        // to the previous data
        // element
        PreviousDataIterator = DataIterator;

        // Move along the
        // data array
        ++DataIterator;
    }

    // Check to see if the
    // last data element
    // was a column token
    // or row token
    if(*PreviousDataIterator != ColToken)
        ++NumberOfColsForCurrentRow;
    else if(ShouldZeroSizedColsBeCounted)
        ++NumberOfColsForCurrentRow;

    // Check one more time
    // for the last row
    if(NumberOfColsForCurrentRow > NumberOfCols)
        NumberOfCols = NumberOfColsForCurrentRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsLookingForRowToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ShouldZeroLengthRowsBeCounted
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenType
//
// PURPOSE:             This function counts the number of
//                      data rows from a data buffer by
//                      looking for a user specified "row token".
//                      This function allows the user to specify
//                      whether or not to count rows of zero length.
//
// DEPENDENCIES:        - std::find
//
// NOTES:               For example: If we want to count the
//                                   number of lines from a text
//                                   string, we would look for
//                                   the newline "\n" character.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blCountDataRowsLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                const blDataIteratorType& IteratorToEndOfData,
                                                const blTokenType& RowToken,
                                                const bool& ShouldZeroLengthRowsBeCounted)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case there
        // are no rows
        return size_t(0);
    }

    // Iterators used to
    // find the tokens in
    // the data buffer
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The total number of rows
    // found by this function
    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find(FirstTokenIterator,
                                        IteratorToEndOfData,
                                        RowToken);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row
            ++FirstTokenIterator;
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We're done counting
    return TotalNumberOfRows;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsLookingForSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - ShouldZeroLengthRowsBeCounted
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenIteratorType
//
// PURPOSE:             This function counts the number of
//                      data rows from a data array by searching
//                      for one of many user specified "new row"
//                      tokens.
//                      This function allows the user to specify
//                      whether or not to count rows of zero length.
//
// DEPENDENCIES:        - std::find_first_of
//
// NOTES:               For example: If we want to count the
//                                   number of lines from a text
//                                   string, we would look for
//                                   the newline "\n" character.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blCountDataRowsLookingForSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                        const blDataIteratorType& IteratorToEndOfData,
                                                        const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                        const blTokenIteratorType& IteratorToEndOfTokens,
                                                        const bool& ShouldZeroLengthRowsBeCounted)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case there
        // are no rows
        return size_t(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we
        // have no tokens
        // to look for
        if(ShouldZeroLengthRowsBeCounted)
            return size_t(1);
        else
            return size_t(0);
    }

    // Iterators used to
    // find the tokens in
    // the data buffer
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The total number of rows
    // found by this function
    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,
                                                 IteratorToEndOfData,
                                                 IteratorToBeginningOfTokens,
                                                 IteratorToEndOfTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row
            ++FirstTokenIterator;
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We're done counting
    return TotalNumberOfRows;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsLookingForTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - ShouldZeroLengthRowsBeCounted
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenIteratorType
//
// PURPOSE:             This function counts the number of
//                      data rows from a data array by searching
//                      for a sequence of tokens representing
//                      a "new row".
//                      This function allows the user to specify
//                      whether or not to count rows of zero length.
//
// DEPENDENCIES:        - std::sesarch
//
// NOTES:               For example: If we want to count the
//                                   number of lines from a text
//                                   string, we would look for
//                                   the newline "\n" character.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blCountDataRowsLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                     const blDataIteratorType& IteratorToEndOfData,
                                                     const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                     const blTokenIteratorType& IteratorToEndOfTokens,
                                                     const bool& ShouldZeroLengthRowsBeCounted)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case there
        // are no rows
        return size_t(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we have
        // no sequence of tokens
        if(ShouldZeroLengthRowsBeCounted)
            return size_t(1);
        else
            return size_t(0);
    }

    // Calculate the length
    // of the token sequence
    size_t LengthOfTokenSequence = std::distance(IteratorToBeginningOfTokens,IteratorToEndOfTokens);

    // Iterators used to
    // find the tokens in
    // the data buffer
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The total number of rows
    // found by this function
    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::search(FirstTokenIterator,
                                          IteratorToEndOfData,
                                          IteratorToBeginningOfTokens,
                                          IteratorToEndOfTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row
            AdvanceIterator(FirstTokenIterator,
                            IteratorToEndOfData,
                            LengthOfTokenSequence);
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            AdvanceIterator(FirstTokenIterator,
                            IteratorToEndOfData,
                            LengthOfTokenSequence);
        }
    }

    // We're done counting
    return TotalNumberOfRows;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindLengthOfLongestDataRowLookingForRowToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - IteratorToBeginningOfLongestDataRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function searches for the user
//                      specified "newline" token, and counts
//                      the length of each data row.
//                      It returns the greatest length.
//                      This function also stores an iterator
//                      to the beginning of the longest data row.
//
// DEPENDENCIES:        - std::find
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blFindLengthOfLongestDataRowLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                             const blDataIteratorType& IteratorToEndOfData,
                                                             const blTokenType& RowToken)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we won't
        // be able to find any tokens
        return size_t(0);
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The length of the
    // longest data row
    size_t LongestLength = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find(FirstTokenIterator,
                                        IteratorToEndOfData,
                                        RowToken);

        // Calculate the length of
        // the current data row
        if(FirstTokenIterator != SecondTokenIterator)
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator) - 1;
        else
            LengthOfCurrentRow = 0;

        // Compare the lengths
        if(LengthOfCurrentRow > LongestLength)
        {
            LongestLength = LengthOfCurrentRow;
        }

        FirstTokenIterator = SecondTokenIterator;

        // Advance the iterator
        // if we've not reached
        // the end yet
        if(FirstTokenIterator != IteratorToEndOfData)
            ++FirstTokenIterator;
    }

    // We're done counting
    return LongestLength;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindLengthOfLongestDataRowLookingForRowToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - IteratorToBeginningOfLongestDataRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function searches for the user
//                      specified "newline" token, and counts
//                      the length of each data row.
//                      It returns the greatest length.
//                      This function also stores an iterator
//                      to the beginning of the longest data row.
//
// DEPENDENCIES:        - std::find
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blFindLengthOfLongestDataRowLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                             const blDataIteratorType& IteratorToEndOfData,
                                                             const blTokenType& RowToken,
                                                             blDataIteratorType& IteratorToBeginningOfLongestDataRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we won't
        // be able to find any tokens
        IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;
        return size_t(0);
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The length of the
    // longest data row
    size_t LongestLength = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find(FirstTokenIterator,
                                        IteratorToEndOfData,
                                        RowToken);

        // Calculate the length of
        // the current data row
        if(FirstTokenIterator != SecondTokenIterator)
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator) - 1;
        else
            LengthOfCurrentRow = 0;

        // Compare the lengths
        if(LengthOfCurrentRow > LongestLength)
        {
            LongestLength = LengthOfCurrentRow;
            IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
        }

        FirstTokenIterator = SecondTokenIterator;

        // Advance the iterator
        // if we've not reached
        // the end yet
        if(FirstTokenIterator != IteratorToEndOfData)
            ++FirstTokenIterator;
    }

    // We're done counting
    return LongestLength;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindLengthOfLongestDataRowLookingForSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - IteratorToBeginningOfLongestDataRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function searches for the user
//                      specified "newline" token, and counts
//                      the length of each data row.
//                      It returns the greatest length.
//                      This function also stores an iterator
//                      to the beginning of the longest data row.
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blFindLengthOfLongestDataRowLookingForSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                     const blDataIteratorType& IteratorToEndOfData,
                                                                     const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                     const blTokenIteratorType& IteratorToEndOfTokens,
                                                                     blDataIteratorType& IteratorToBeginningOfLongestDataRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we won't
        // be able to find any tokens
        IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;
        return size_t(0);
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The length of the
    // longest data row
    size_t LongestLength = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,
                                                 IteratorToEndOfData,
                                                 IteratorToBeginningOfTokens,
                                                 IteratorToEndOfTokens);

        // Calculate the length of
        // the current data row
        if(FirstTokenIterator != SecondTokenIterator)
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator) - 1;
        else
            LengthOfCurrentRow = 0;

        // Compare the lengths
        if(LengthOfCurrentRow > LongestLength)
        {
            LongestLength = LengthOfCurrentRow;
            IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
        }

        FirstTokenIterator = SecondTokenIterator;

        // Advance the iterator
        // if we've not reached
        // the end yet
        if(FirstTokenIterator != IteratorToEndOfData)
            ++FirstTokenIterator;
    }

    // We're done counting
    return LongestLength;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindLengthOfLongestDataRowLookingForTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningToken
//                      - IteratorToEndOfToken
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function searches for the user
//                      specified Token Sequence and counts
//                      the length of each data row.
//                      It returns the greatest length.
//                      This function also stores an iterator
//                      to the beginning of the longest data row
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//                      - AdvanceIterator
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blFindLengthOfLongestDataRowLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                  const blDataIteratorType& IteratorToEndOfData,
                                                                  const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                  const blTokenIteratorType& IteratorToEndOfTokens,
                                                                  blDataIteratorType& IteratorToBeginningOfLongestDataRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we won't
        // be able to find any tokens
        IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;
        return size_t(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we won't
        // find any tokens so the
        // longest row is the
        // entire data
        IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;
        return std::distance(IteratorToBeginningOfData,IteratorToEndOfData) - 1;
    }

    // Calculate the length
    // of the token sequence
    size_t LengthOfTokenSequence = std::distance(IteratorToBeginningOfTokens,
                                                 IteratorToEndOfTokens);

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The length of the
    // longest data row
    size_t LongestLength = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    IteratorToBeginningOfLongestDataRow = IteratorToBeginningOfData;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::search(FirstTokenIterator,
                                          IteratorToEndOfData,
                                          IteratorToBeginningOfTokens,
                                          IteratorToEndOfTokens);

        // Calculate the length of
        // the current data row
        if(FirstTokenIterator != SecondTokenIterator)
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator) - 1;
        else
            LengthOfCurrentRow = 0;

        // Compare the lengths
        if(LengthOfCurrentRow > LongestLength)
        {
            LongestLength = LengthOfCurrentRow;
            IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
        }

        FirstTokenIterator = SecondTokenIterator;

        // Advance the iterator
        AdvanceIterator(FirstTokenIterator,
                        IteratorToEndOfData,
                        LengthOfTokenSequence);
    }

    // We're done counting
    return LongestLength;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsAndLongestRowLengthLookingForRowToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ShouldZeroLengthRowsBeCounted,
//                      - TotalNumberOfRows
//                      - LengthOfLongestRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function counts the total number of
//                      rows from a data array and the longest
//                      length of any row.
//                      This function also stores an iterator
//                      to the beginning of the longest row.
//                      This function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::find
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                               const blDataIteratorType& IteratorToEndOfData,
                                                                               const blTokenType& RowToken,
                                                                               const bool& ShouldZeroLengthRowsBeCounted,
                                                                               size_t& TotalNumberOfRows,
                                                                               size_t& LengthOfLongestRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data points
        TotalNumberOfRows = 0;
        LengthOfLongestRow = 0;
        return IteratorToBeginningOfData;
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType IteratorToBeginningOfLongestDataRow = FirstTokenIterator;

    // The length of the
    // longest data row
    LengthOfLongestRow = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    // The total number of rows
    TotalNumberOfRows = 0;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find(FirstTokenIterator,
                                        IteratorToEndOfData,
                                        RowToken);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this we don't count
            // the zero sized row
            ++FirstTokenIterator;
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            // Calculate the length of
            // the current data row
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator);

            // Compare the lengths
            if(LengthOfCurrentRow > LengthOfLongestRow)
            {

                LengthOfLongestRow = LengthOfCurrentRow;
                IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
            }

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    return IteratorToBeginningOfLongestDataRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsAndLongestRowLengthLookingForAnyoneOfSeveralRowTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokens,
//                      - IteratorToEndOfTokens
//                      - ShouldZeroLengthRowsBeCounted,
//                      - TotalNumberOfRows
//                      - LengthOfLongestRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function counts the total number of
//                      rows from a data array and the longest
//                      length of any row.
//                      This function also stores an iterator
//                      to the beginning of the longest row.
//                      This function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForAnyoneOfSeveralRowTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                               const blDataIteratorType& IteratorToEndOfData,
                                                                                               const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                               const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                               const bool& ShouldZeroLengthRowsBeCounted,
                                                                                               size_t& TotalNumberOfRows,
                                                                                               size_t& LengthOfLongestRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data points
        TotalNumberOfRows = 0;
        LengthOfLongestRow = 0;
        return IteratorToBeginningOfData;
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case there are
        // no tokens to find
        TotalNumberOfRows = 1;
        LengthOfLongestRow = std::distance(IteratorToBeginningOfData,IteratorToEndOfData);
        return IteratorToBeginningOfData;
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType IteratorToBeginningOfLongestDataRow = FirstTokenIterator;

    // The length of the
    // longest data row
    LengthOfLongestRow = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    // The total number of rows
    TotalNumberOfRows = 0;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,
                                                 IteratorToEndOfData,
                                                 IteratorToBeginningOfTokens,
                                                 IteratorToEndOfTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this we don't count
            // the zero sized row
            ++FirstTokenIterator;
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            // Calculate the length of
            // the current data row
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator);

            // Compare the lengths
            if(LengthOfCurrentRow > LengthOfLongestRow)
            {

                LengthOfLongestRow = LengthOfCurrentRow;
                IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
            }

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    return IteratorToBeginningOfLongestDataRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsAndLongestRowLengthLookingForTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfToken
//                      - IteratorToEndOfToken
//                      - ShouldZeroLengthRowsBeCounted
//                      - TotalNumberOfRows
//                      - LengthOfLongestRow
//                      - IteratorToBeginningOfLongestDataRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function counts the total number of
//                      rows from a data array and the longest
//                      length of any row.
//                      This function also stores an iterator
//                      to the beginning of the longest row.
//                      This function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                    const blDataIteratorType& IteratorToEndOfData,
                                                                                    const blTokenIteratorType& IteratorToBeginningOfToken,
                                                                                    const blTokenIteratorType& IteratorToEndOfToken,
                                                                                    const bool& ShouldZeroLengthRowsBeCounted,
                                                                                    size_t& TotalNumberOfRows,
                                                                                    size_t& LengthOfLongestRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data points
        TotalNumberOfRows = 0;
        LengthOfLongestRow = 0;
        return IteratorToBeginningOfData;
    }
    else if(IteratorToBeginningOfToken == IteratorToEndOfToken)
    {
        // In this case we won't
        // be able to find any tokens
        // so we say that the
        // longest row is the
        // data array itself
        TotalNumberOfRows = 1;
        LengthOfLongestRow = std::distance(IteratorToBeginningOfData,IteratorToEndOfData);
        return IteratorToBeginningOfData;
    }

    // Calculate the length
    // of the token
    size_t TokenLength = std::distance(IteratorToBeginningOfToken,IteratorToEndOfToken);

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType IteratorToBeginningOfLongestDataRow = FirstTokenIterator;

    // The length of the
    // longest data row
    LengthOfLongestRow = 0;

    // The length of the
    // current data row
    size_t LengthOfCurrentRow = 0;

    // The total number of rows
    TotalNumberOfRows = 0;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::search(FirstTokenIterator,IteratorToEndOfData,
                                          IteratorToBeginningOfToken,IteratorToEndOfToken);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this we don't count
            // the zero sized row
            std::advance(FirstTokenIterator,std::min(TokenLength,std::distance(FirstTokenIterator,IteratorToEndOfData)));
        }
        else
        {
            // Increse the total number of rows
            ++TotalNumberOfRows;

            // Calculate the length of
            // the current data row
            LengthOfCurrentRow = std::distance(FirstTokenIterator,SecondTokenIterator);

            // Compare the lengths
            if(LengthOfCurrentRow > LengthOfLongestRow)
            {
                LengthOfLongestRow = LengthOfCurrentRow;
                IteratorToBeginningOfLongestDataRow = FirstTokenIterator;
            }

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator if
            // we've not reached the
            // end yet
            std::advance(FirstTokenIterator,std::min(TokenLength,std::distance(FirstTokenIterator,IteratorToEndOfData)));
        }
    }

    return IteratorToBeginningOfLongestDataRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindStartingAndEndingPositionsOfNthDataRowLookingForRowToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ShouldZeroLengthRowsBeCounted
//                      - WhichRowToFind
//                      - IteratorToStartingPositionOfNthRow
//                      - IteratorToEndingPositionOfNthRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to the
//                      beginning of the user specified nth row.
//                      The function returns the "actual" row found
//                      in case that there are less than "n" rows.
//                      The function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::find
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blFindStartingAndEndingPositionsOfNthDataRowLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,                                                                           const blDataIteratorType& IteratorToEndOfData,
                                                                             const blTokenType& RowToken,
                                                                             const bool& ShouldZeroLengthRowsBeCounted,
                                                                             const size_t& WhichRowToFind,
                                                                             blDataIteratorType& IteratorToStartingPositionOfNthRow,
                                                                             blDataIteratorType& IteratorToEndingPositionOfNthRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we have
        // no data to look through
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return size_t(0);
    }

    // The iterator to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    IteratorToStartingPositionOfNthRow = FirstTokenIterator;
    IteratorToEndingPositionOfNthRow = FirstTokenIterator;

    // The current data row
    size_t CurrentRow = size_t(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow != WhichRowToFind)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find(FirstTokenIterator,
                                        IteratorToEndOfData,
                                        RowToken);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, the row
            // is of zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else
        {
            ++CurrentRow;

            IteratorToStartingPositionOfNthRow = FirstTokenIterator;
            IteratorToEndingPositionOfNthRow = SecondTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // If we have not reached
            // the end, then we advance
            // the iterator
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We're done
    return CurrentRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindStartingAndEndingPositionsOfNthDataRowLookingForSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfSingleDataTokens
//                      - IteratorToEndOfSingleDataTokens
//                      - ShouldZeroLengthRowsBeCounted
//                      - WhichRowToFind
//                      - IteratorToStartingPositionOfNthRow
//                      - IteratorToEndingPositionOfNthRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to the
//                      beginning of the user specified nth row.
//                      The function returns the "actual" row found
//                      in case that there are less than "n" rows.
//                      The function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::find_first_of
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blFindStartingAndEndingPositionsOfNthDataRowLookingForSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                     const blDataIteratorType& IteratorToEndOfData,
                                                                                     const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                     const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                     const bool& ShouldZeroLengthRowsBeCounted,
                                                                                     const size_t& WhichRowToFind,
                                                                                     blDataIteratorType& IteratorToStartingPositionOfNthRow,
                                                                                     blDataIteratorType& IteratorToEndingPositionOfNthRow)

{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we have
        // no data to look through
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return size_t(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we won't
        // find any tokens
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return size_t(0);
    }

    // The iterator to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    IteratorToStartingPositionOfNthRow = FirstTokenIterator;
    IteratorToEndingPositionOfNthRow = FirstTokenIterator;

    // The current data row
    size_t CurrentRow = size_t(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow != WhichRowToFind)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,
                                                 IteratorToEndOfData,
                                                 IteratorToBeginningOfTokens,
                                                 IteratorToEndOfTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, the row
            // is of zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else
        {
            ++CurrentRow;

            IteratorToStartingPositionOfNthRow = FirstTokenIterator;
            IteratorToEndingPositionOfNthRow = SecondTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // If we have not reached
            // the end, then we advance
            // the iterator
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We're done
    return CurrentRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindStartingAndEndingPositionsOfNthDataRowLookingForTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokens
//                      - IteratorToEndOfTokens
//                      - ShouldZeroLengthRowsBeCounted
//                      - WhichRowToFind
//                      - IteratorToStartingPositionOfNthRow
//                      - IteratorToEndingPositionOfNthRow
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to the
//                      beginning of the user specified nth row.
//                      The function returns the "actual" row found
//                      in case that there are less than "n" rows.
//                      The function allows the user to specify
//                      whether or not to count zero sized rows.
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline size_t blFindStartingAndEndingPositionsOfNthDataRowLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                  const blDataIteratorType& IteratorToEndOfData,
                                                                                  const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                  const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                  const bool& ShouldZeroLengthRowsBeCounted,
                                                                                  const size_t& WhichRowToFind,
                                                                                  blDataIteratorType& IteratorToStartingPositionOfNthRow,
                                                                                  blDataIteratorType& IteratorToEndingPositionOfNthRow)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we have
        // no data to look through
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return size_t(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we won't
        // find any tokens
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return size_t(0);
    }

    // Calculate the token length
    int TokenLength = std::distance(IteratorToBeginningOfTokens,
                                    IteratorToEndOfTokens);

    // The iterator to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    IteratorToStartingPositionOfNthRow = FirstTokenIterator;
    IteratorToEndingPositionOfNthRow = FirstTokenIterator;

    // The current data row
    size_t CurrentRow = size_t(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow != WhichRowToFind)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::search(FirstTokenIterator,
                                          IteratorToEndOfData,
                                          IteratorToBeginningOfTokens,
                                          IteratorToEndOfTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, the row
            // is of zero length and
            // we don't count it
            AdvanceIterator(FirstTokenIterator,IteratorToEndOfData,TokenLength);
        }
        else
        {
            ++CurrentRow;

            IteratorToStartingPositionOfNthRow = FirstTokenIterator;
            IteratorToEndingPositionOfNthRow = SecondTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // If we have not reached
            // the end, then we advance
            // the iterator
            if(FirstTokenIterator != IteratorToEndOfData)
                AdvanceIterator(FirstTokenIterator,IteratorToEndOfData,TokenLength);
        }
    }

    // We're done
    return CurrentRow;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindPositionOfNthDataRowAndMthDataColumnUsingToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ColToken
//                      - ShouldZeroSizedVectorsBeAllowed
//                      - WhichRowToFind
//                      - WhichColToFind
//                      - IteratorToPositionOfNthRowAndMthColumn
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowUsingToken
//
// NOTES:               - For example:  For a text string, where
//                                      rows are the lines of text,
//                                      and columns are the characters
//                                      in each line of text, the user
//                                      can find the position in the
//                                      text string of the Mth character
//                                      in the Nth line.
//
//                      - In case the specified position is not
//                        found, the function finds the position
//                        closest to the one specified.
//
//                      - For example, the user asks for the 100th
//                        character of the 10th line in a text string.
//                        But the 10th line only has 55 characters, then
//                        the function will return the position to
//                        the 55th character of the 10th line.
//
//                      - Line and character positions are based on
//                        zero.  Therefore the first data point
//                        in the data array is the 0th data point
//                        of the 0th row.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline void blFindPositionOfNthDataRowAndMthDataColumnUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                 const blDataIteratorType& IteratorToEndOfData,
                                                                 const blTokenType& RowToken,
                                                                 const blTokenType& ColToken,
                                                                 const bool& ShouldZeroSizedRowsAndColumnsBeCounted,
                                                                 const size_t& WhichRowToFind,
                                                                 const size_t& WhichColToFind,
                                                                 blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                 size_t& ActualRowFound,
                                                                 size_t& ActualColFound)
{
    // We first try to find the
    // starting and ending positions
    // of the specified Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    ActualRowFound = blFindStartingAndEndingPositionsOfNthDataRowLookingForRowToken(IteratorToBeginningOfData,
                                                                                    IteratorToEndOfData,
                                                                                    RowToken,
                                                                                    ShouldZeroSizedRowsAndColumnsBeCounted,
                                                                                    WhichRowToFind,
                                                                                    IteratorToPositionOfNthRowAndMthColumn,
                                                                                    IteratorToEndingPositionOfNthRow);

    // We decrease the row end
    // iterator because it's
    // currently pointing to just
    // after the last column of the
    // row
    --IteratorToEndingPositionOfNthRow;

    // We calculate the actual
    // column that the iterator
    // will be pointing to
    ActualColFound = std::min(WhichColToFind,size_t(std::distance(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow)));

    // Advance the iterator to
    // get it to the Mth column
    // making sure it does not
    // pass the end of the row
    AdvanceIterator(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow,WhichColToFind);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindPositionOfNthDataRowAndMthDataColumnUsingSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfToken
//                      - IteratorToEndOfToken
//                      - ShouldZeroSizedVectorsBeAllowed
//                      - WhichRowToFind
//                      - WhichColToFind
//                      - IteratorToPositionOfNthRowAndMthColumn
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens
//
// NOTES:               - For example:  For a text string, where
//                                      rows are the lines of text,
//                                      and columns are the characters
//                                      in each line of text, the user
//                                      can find the position in the
//                                      text string of the Mth character
//                                      in the Nth line.
//
//                      - In case the specified position is not
//                        found, the function finds the position
//                        closest to the one specified.
//
//                      - For example, the user asks for the 100th
//                        character of the 10th line in a text string.
//                        But the 10th line only has 55 characters, then
//                        the function will return the position to
//                        the 55th character of the 10th line.
//
//                      - Line and character positions are based on
//                        zero.  Therefore the first data point
//                        in the data array is the 0th data point
//                        of the 0th row.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blFindPositionOfNthDataRowAndMthDataColumnUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                            const blDataIteratorType& IteratorToEndOfData,
                                                                            const blTokenIteratorType& IteratorToBeginningOfSingleDataTokens,
                                                                            const blTokenIteratorType& IteratorToEndOfSingleDataTokens,
                                                                            const bool& ShouldZeroSizedVectorsBeAllowed,
                                                                            const size_t& WhichRowToFind,
                                                                            const size_t& WhichColToFind,
                                                                            blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                            size_t& ActualRowFound,
                                                                            size_t& ActualColFound)
{
    // We first try to find the
    // starting and ending positions
    // of the specified Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens(IteratorToBeginningOfData,
                                                                                     IteratorToEndOfData,
                                                                                     IteratorToBeginningOfSingleDataTokens,
                                                                                     IteratorToEndOfSingleDataTokens,
                                                                                     ShouldZeroSizedVectorsBeAllowed,
                                                                                     WhichRowToFind,
                                                                                     IteratorToPositionOfNthRowAndMthColumn,
                                                                                     IteratorToEndingPositionOfNthRow);

    // We decrease the row end
    // iterator because it's
    // currently pointing to just
    // after the last column of the
    // row
    --IteratorToEndingPositionOfNthRow;

    // We calculate the actual
    // column that the iterator
    // will be pointing to
    ActualColFound = std::min(WhichColToFind,size_t(std::distance(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow)));

    // Advance the iterator to
    // get it to the Mth column
    // making sure it does not
    // pass the end of the row
    AdvanceIterator(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow,WhichColToFind);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindPositionOfNthDataRowAndMthDataColumnUsingToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfToken
//                      - IteratorToEndOfToken
//                      - ShouldZeroSizedVectorsBeAllowed
//                      - WhichRowToFind
//                      - WhichColToFind
//                      - IteratorToPositionOfNthRowAndMthColumn
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens
//
// NOTES:               - For example:  For a text string, where
//                                      rows are the lines of text,
//                                      and columns are the characters
//                                      in each line of text, the user
//                                      can find the position in the
//                                      text string of the Mth character
//                                      in the Nth line.
//
//                      - In case the specified position is not
//                        found, the function finds the position
//                        closest to the one specified.
//
//                      - For example, the user asks for the 100th
//                        character of the 10th line in a text string.
//                        But the 10th line only has 55 characters, then
//                        the function will return the position to
//                        the 55th character of the 10th line.
//
//                      - Line and character positions are based on
//                        zero.  Therefore the first data point
//                        in the data array is the 0th data point
//                        of the 0th row.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blFindPositionOfNthDataRowAndMthDataColumnUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                 const blDataIteratorType& IteratorToEndOfData,
                                                                 const blTokenIteratorType& IteratorToBeginningOfSingleDataTokens,
                                                                 const blTokenIteratorType& IteratorToEndOfSingleDataTokens,
                                                                 const bool& ShouldZeroSizedVectorsBeAllowed,
                                                                 const size_t& WhichRowToFind,
                                                                 const size_t& WhichColToFind,
                                                                 blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                 size_t& ActualRowFound,
                                                                 size_t& ActualColFound)
{
    // We first try to find the
    // starting and ending positions
    // of the specified Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowUsingToken(IteratorToBeginningOfData,
                                                                          IteratorToEndOfData,
                                                                          IteratorToBeginningOfSingleDataTokens,
                                                                          IteratorToEndOfSingleDataTokens,
                                                                          ShouldZeroSizedVectorsBeAllowed,
                                                                          WhichRowToFind,
                                                                          IteratorToPositionOfNthRowAndMthColumn,
                                                                          IteratorToEndingPositionOfNthRow);

    // We decrease the row end
    // iterator because it's
    // currently pointing to just
    // after the last column of the
    // row
    --IteratorToEndingPositionOfNthRow;

    // We calculate the actual
    // column that the iterator
    // will be pointing to
    ActualColFound = std::min(WhichColToFind,size_t(std::distance(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow)));

    // Advance the iterator to
    // get it to the Mth column
    // making sure it does not
    // pass the end of the row
    AdvanceIterator(IteratorToPositionOfNthRowAndMthColumn,IteratorToEndingPositionOfNthRow,WhichColToFind);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blGetRowAndColumnNumberOfDataPositionUsingToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - Token
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - IteratorToDataPosition
//                      - RowNumber
//                      - ColNumber
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function finds the corresponding
//                      row and column number of the user
//                      specified data position.
//                      It looks for a user specified token
//                      to count rows.
//
// DEPENDENCIES:        - std::find
//                      - std::distance
//                      - AdvanceIterator
//
// NOTES:               For example:  We have a text string and we
//                                    want to know in which line
//                                    and character position the
//                                    99th character of the text
//                                    lies in.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline void blGetRowAndColumnNumberOfDataPositionUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                            const blDataIteratorType& IteratorToEndOfData,
                                                            const blTokenType& Token,
                                                            const bool& ShouldZeroSizedRowsBeCounted,
                                                            const bool& ShouldZeroSizedColsBeCounted,
                                                            const blDataIteratorType& IteratorToDataPosition,
                                                            int& RowNumber,
                                                            int& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroSizedRowsBeCounted)
        {
            RowNumber = -1;
            ColNumber = -1;
        }
        else
        {
            RowNumber = 0;
            ColNumber = 0;
        }

        return;
    }

    // The iterator to
    // the tokens found
    // in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The starting
    // position of the
    // row we're currently
    // parsing
    blDataIteratorType StartingPositionOfCurrentRow = IteratorToBeginningOfData;

    // We initialize the
    // row number differently
    // depending on whether
    // or not rows of zero size
    // are to be counted
    if(ShouldZeroSizedRowsBeCounted)
        RowNumber = 0;
    else
        RowNumber = -1;

    while(FirstTokenIterator != IteratorToEndOfData &&
          SecondTokenIterator != IteratorToDataPosition)
    {
        // Find the next
        // token in the
        // data buffer
        SecondTokenIterator = std::find(FirstTokenIterator,IteratorToEndOfData,Token);

        // Check to see if
        // the row we found
        // was a zero length
        // row
        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroSizedRowsBeCounted)
        {
            // In this case,
            // the row is of
            // zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else
        {
            ++RowNumber;
            StartingPositionOfCurrentRow = FirstTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // If we have not reached
            // the end, then we advance
            // the iterator
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);

    if(ColNumber == 0 && !ShouldZeroSizedColsBeCounted)
        ColNumber = -1;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blGetRowAndColumnNumberOfDataPositionUsingSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfSingleDataTokens
//                      - IteratorToEndOfSingleDataTokens
//                      - ShouldZeroSizedRowsBeAllowed
//                      - ShouldZeroSizedColsBeAllowed
//                      - IteratorToDataPosition
//                      - RowNumber
//                      - ColNumber
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function finds the corresponding
//                      row and column number of the user
//                      specified data position.
//                      It looks for single data tokens specified
//                      by the user to count rows.
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:               For example:  We have a text string and we
//                                    want to know in which line
//                                    and character position the
//                                    99th character of the text
//                                    lies in.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blGetRowAndColumnNumberOfDataPositionUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                       const blDataIteratorType& IteratorToEndOfData,
                                                                       const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                       const blTokenIteratorType& IteratorToEndOfTokens,
                                                                       const bool& ShouldZeroSizedRowsBeCounted,
                                                                       const bool& ShouldZeroSizedColsBeCounted,
                                                                       const blDataIteratorType& IteratorToDataPosition,
                                                                       int& RowNumber,
                                                                       int& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData ||
       IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroSizedRowsBeCounted)
        {
            RowNumber = -1;
            ColNumber = -1;
        }
        else
        {
            RowNumber = 0;
            ColNumber = 0;
        }

        return;
    }

    // The iterator to
    // the tokens found
    // in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The starting
    // position of the
    // row we're currently
    // parsing
    blDataIteratorType StartingPositionOfCurrentRow = IteratorToBeginningOfData;

    // We initialize the
    // row number differently
    // depending on whether
    // or not rows of zero size
    // are to be counted
    if(ShouldZeroSizedRowsBeCounted)
        RowNumber = 0;
    else
        RowNumber = -1;

    while(FirstTokenIterator != IteratorToEndOfData &&
          SecondTokenIterator != IteratorToDataPosition)
    {
        // Find the next
        // token in the
        // data buffer
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,IteratorToEndOfData,
                                                 IteratorToBeginningOfTokens,IteratorToEndOfTokens);

        // Check to see if
        // the row we found
        // was a zero length
        // row
        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroSizedRowsBeCounted)
        {
            // In this case,
            // the row is of
            // zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else
        {
            ++RowNumber;
            StartingPositionOfCurrentRow = FirstTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // If we have not reached
            // the end, then we advance
            // the iterator
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);

    if(ColNumber == 0 && !ShouldZeroSizedColsBeCounted)
        ColNumber = -1;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blGetRowAndColumnNumberOfDataPositionUsingTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - IteratorToDataPosition
//                      - RowNumber
//                      - ColNumber
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//
// PURPOSE:             This function finds the corresponding
//                      row and column number of the user
//                      specified data position.
//                      It looks for a user specified token
//                      (sequence of data points) to count rows.
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//                      - AdvanceIterator
//
// NOTES:               For example:  We have a text string and we
//                                    want to know in which line
//                                    and character position the
//                                    99th character of the text
//                                    lies in.
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blGetRowAndColumnNumberOfDataPositionUsingTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                    const blDataIteratorType& IteratorToEndOfData,
                                                                    const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                                                    const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                                                    const bool& ShouldZeroSizedRowsBeCounted,
                                                                    const bool& ShouldZeroSizedColsBeCounted,
                                                                    const blDataIteratorType& IteratorToDataPosition,
                                                                    int& RowNumber,
                                                                    int& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData ||
       IteratorToBeginningOfTokenSequence == IteratorToEndOfTokenSequence)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroSizedRowsBeCounted)
        {
            RowNumber = -1;
            ColNumber = -1;
        }
        else
        {
            RowNumber = 0;
            ColNumber = 0;
        }

        return;
    }

    // The iterator to
    // the tokens found
    // in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    // The starting
    // position of the
    // row we're currently
    // parsing
    blDataIteratorType StartingPositionOfCurrentRow = IteratorToBeginningOfData;

    // Calculate the
    // token sequence
    // length
    int TokenSequenceLength = std::distance(IteratorToBeginningOfTokenSequence,IteratorToEndOfTokenSequence);

    // We initialize the
    // row number differently
    // depending on whether
    // or not rows of zero size
    // are to be counted
    if(ShouldZeroSizedRowsBeCounted)
        RowNumber = 0;
    else
        RowNumber = -1;

    while(FirstTokenIterator != IteratorToEndOfData &&
          SecondTokenIterator != IteratorToDataPosition)
    {
        // Find the next
        // token in the
        // data buffer
        SecondTokenIterator = std::search(FirstTokenIterator,IteratorToEndOfData,
                                          IteratorToBeginningOfTokenSequence,IteratorToEndOfTokenSequence);

        // Check to see if
        // the row we found
        // was a zero length
        // row
        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroSizedRowsBeCounted)
        {
            // In this case,
            // the row is of
            // zero length and
            // we don't count it
            std::advance(FirstTokenIterator,
                         std::min(TokenSequenceLength,
                                  std::distance(FirstTokenIterator,
                                                IteratorToEndOfData)));
        }
        else
        {
            ++RowNumber;
            StartingPositionOfCurrentRow = FirstTokenIterator;

            FirstTokenIterator = SecondTokenIterator;

            // We advance
            // the iterator
            std::advance(FirstTokenIterator,
                         std::min(TokenSequenceLength,
                                  std::distance(FirstTokenIterator,
                                                IteratorToEndOfData)));
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);

    if(ColNumber == 0 && !ShouldZeroSizedColsBeCounted)
        ColNumber = -1;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountRowsAndColumnsFromDataUsingSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfSingleDataRowTokens
//                      - IteratorToEndOfSingleDataRowTokens
//                      - IteratorToBeginningOfSingleDataColTokens
//                      - IteratorToEndOfSingleDataColTokens
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - NumberOfRows
//                      - NumberOfCols
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blRowTokenIteratorType -- The type of iterator
//                                                  for the row token
//                      - blColTokenIteratorType -- The type of iterator
//                                                  for the column token
//
// PURPOSE:             This function counts the number of rows
//                      and number of columns in a data array.
//                      The function counts the number of
//                      rows by searching for the "row" tokens.
//                      At each row, the function counts the
//                      number of columns by searching for the
//                      "col" tokens.
//                      It returns the number of rows and the
//                      maximum number of columns.
//                      So if each row has a different number
//                      of columns, this function returns the
//                      maximum number of columns.
//
// DEPENDENCIES:        - std::find_first_of
//                      - std::distance
//
// NOTES:               For example:  If we want to count the number
//                                    of rows and columns in a CSV
//                                    file, we would for example
//                                    make the "\n" the row token,
//                                    and the "," the column token
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blRowTokenIteratorType,
         typename blColTokenIteratorType>
inline void blCountRowsAndColumnsFromDataUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                               const blDataIteratorType& IteratorToEndOfData,
                                                               const blRowTokenIteratorType& IteratorToBeginningOfSingleDataRowTokens,
                                                               const blRowTokenIteratorType& IteratorToEndOfSingleDataRowTokens,
                                                               const blColTokenIteratorType& IteratorToBeginningOfSingleDataColTokens,
                                                               const blColTokenIteratorType& IteratorToEndOfSingleDataColTokens,
                                                               const bool& ShouldZeroSizedRowsBeCounted,
                                                               const bool& ShouldZeroSizedColsBeCounted,
                                                               size_t& NumberOfRows,
                                                               size_t& NumberOfCols)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // default the number of rows
        // and columnsn to -1
        NumberOfRows = size_t(-1);
        NumberOfCols = size_t(-1);
        return;
    }

    // Iterators that point to starting
    // and ending positions of each row
    blDataIteratorType IteratorToStartOfCurrentRow = IteratorToBeginningOfData;
    blDataIteratorType IteratorToEndOfCurrentRow = IteratorToBeginningOfData;
    blDataIteratorType IteratorToBeginningOfData2 = IteratorToBeginningOfData;

    NumberOfRows = 0;
    NumberOfCols = 0;

    size_t NumberOfColsForCurrentRow = 0;
    size_t ActualRowFound = 0;

    while(IteratorToEndOfCurrentRow != IteratorToEndOfData &&
          ActualRowFound != size_t(-1))
    {
        // Find the current row
        ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens(IteratorToBeginningOfData2,
                                                                                         IteratorToEndOfData,
                                                                                         IteratorToBeginningOfSingleDataRowTokens,
                                                                                         IteratorToEndOfSingleDataRowTokens,
                                                                                         ShouldZeroSizedRowsBeCounted,
                                                                                         0,
                                                                                         IteratorToStartOfCurrentRow,
                                                                                         IteratorToEndOfCurrentRow);

        if(ActualRowFound == 0)
        {
            // In this case we found
            // the row, so we count
            // its number of columns
            NumberOfColsForCurrentRow = CountDataRowsUsingSingleDataTokens(IteratorToStartOfCurrentRow,
                                                                           IteratorToEndOfCurrentRow,
                                                                           IteratorToBeginningOfSingleDataColTokens,
                                                                           IteratorToEndOfSingleDataColTokens,
                                                                           ShouldZeroSizedColsBeCounted);

            if(NumberOfColsForCurrentRow > NumberOfCols)
                NumberOfCols = NumberOfColsForCurrentRow;

            ++NumberOfRows;

            IteratorToBeginningOfData2 = IteratorToEndOfCurrentRow;
            ++IteratorToBeginningOfData2;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountRowsAndColumnsFromDataUsingSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfSingleDataRowTokens
//                      - IteratorToEndOfSingleDataRowTokens
//                      - IteratorToBeginningOfSingleDataColTokens
//                      - IteratorToEndOfSingleDataColTokens
//                      - ShouldZeroSizedRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - NumberOfRows
//                      - NumberOfCols
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blRowTokenIteratorType -- The type of iterator
//                                                  for the row token
//                      - blColTokenIteratorType -- The type of iterator
//                                                  for the column token
//
// PURPOSE:             This function counts the number of rows
//                      and number of columns in a data array.
//                      The function counts the number of
//                      rows by searching for the "row" tokens.
//                      At each row, the function counts the
//                      number of columns by searching for the
//                      "col" tokens.
//                      It returns the number of rows and the
//                      maximum number of columns.
//                      So it each row has a different number
//                      of columns, this function returns the
//                      maximum number of columns.
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//
// NOTES:               For example:  If we want to count the number
//                                    of rows and columns in a CSV
//                                    file, we would for example
//                                    make the "\n" the row token,
//                                    and the "," the column token
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blRowTokenIteratorType,
         typename blColTokenIteratorType>
inline void blCountRowsAndColumnsFromDataUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                    const blDataIteratorType& IteratorToEndOfData,
                                                    const blRowTokenIteratorType& IteratorToBeginningOfRowToken,
                                                    const blRowTokenIteratorType& IteratorToEndOfRowToken,
                                                    const blColTokenIteratorType& IteratorToBeginningOfColToken,
                                                    const blColTokenIteratorType& IteratorToEndOfColToken,
                                                    const bool& ShouldZeroSizedRowsBeCounted,
                                                    const bool& ShouldZeroSizedColsBeCounted,
                                                    size_t& NumberOfRows,
                                                    size_t& NumberOfCols)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // default the number of rows
        // and columnsn to -1
        NumberOfRows = size_t(-1);
        NumberOfCols = size_t(-1);
        return;
    }

    // Iterators that point to starting
    // and ending positions of each row
    blDataIteratorType IteratorToStartOfCurrentRow = IteratorToBeginningOfData;
    blDataIteratorType IteratorToEndOfCurrentRow = IteratorToBeginningOfData;
    blDataIteratorType IteratorToBeginningOfData2 = IteratorToBeginningOfData;

    NumberOfRows = 0;
    NumberOfCols = 0;

    size_t NumberOfColsForCurrentRow = 0;
    size_t ActualRowFound = 0;

    while(IteratorToEndOfCurrentRow != IteratorToEndOfData &&
          ActualRowFound != size_t(-1))
    {
        // Find the current row
        ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowUsingToken(IteratorToBeginningOfData2,
                                                                              IteratorToEndOfData,
                                                                              IteratorToBeginningOfRowToken,
                                                                              IteratorToEndOfRowToken,
                                                                              ShouldZeroSizedRowsBeCounted,
                                                                              0,
                                                                              IteratorToStartOfCurrentRow,
                                                                              IteratorToEndOfCurrentRow);

        if(ActualRowFound == 0)
        {
            // In this case we found
            // the row, so we count
            // its number of columns
            NumberOfColsForCurrentRow = CountDataRowsUsingToken(IteratorToStartOfCurrentRow,
                                                                IteratorToEndOfCurrentRow,
                                                                IteratorToBeginningOfColToken,
                                                                IteratorToEndOfColToken,
                                                                ShouldZeroSizedColsBeCounted);

            if(NumberOfColsForCurrentRow > NumberOfCols)
                NumberOfCols = NumberOfColsForCurrentRow;

            ++NumberOfRows;

            IteratorToBeginningOfData2 = IteratorToEndOfCurrentRow;
            ++IteratorToBeginningOfData2;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSplitDataUsingSingleDataTokens
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfSingleDataTokens
//                      - IteratorToEndOfSingleDataTokens
//                      - ShouldZeroSizedVectorsBeAllowed
//                      - SplitDataIterator
//
// TEMPLATE ARGUMENTS:  - blSplitDataContainerType -- The type of
//                                                    container that
//                                                    will hold the
//                                                    individual data
//                                                    points.
//                      - blSplitDataIteratorType -- The type of
//                                                   iterator that will
//                                                   iterate through
//                                                   the split data
//                                                   and push the
//                                                   split data
//                                                   containers.
//                      - blDataIteratorType -- The type of data iterator
//                      - blTokenIteratorType -- The type of token iterator
//
// PURPOSE:             This function splits a data array into
//                      a vector of data vectors using single
//                      data tokens specified by the user.
//                      The function allows the user to specify
//                      whether it should allow zero sized vectors.
//
// DEPENDENCIES:        - blSplitDataIteratorType is assumed to
//                        to act like std::back_insert_iterator or
//                        std::front_insert_iterator
//                      - blSplitDataContainerType is assumed to
//                        have a constructor like the following:
//                        std::vector(IteratorToFirst,IteratorToLast)
//
// NOTES:               - std::find_first_of
//                      - The template parameters all infered
//                        through the function arguments except
//                        for the first one.  The user has to
//                        specify the type of split data container
//                        where the individual data points will
//                        be saved.
//                      - For example:  If the data points are
//                                      doubles, and the split
//                                      data container is an
//                                      std::deque<double>, the
//                                      user has to specify it.
//-------------------------------------------------------------------
template<typename blSplitDataContainerType,
         typename blSplitDataIteratorType,
         typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blSplitDataUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                             const blDataIteratorType& IteratorToEndOfData,
                                             const blTokenIteratorType& IteratorToBeginningOfSingleDataTokens,
                                             const blTokenIteratorType& IteratorToEndOfSingleDataTokens,
                                             const bool& ShouldZeroSizedVectorsBeAllowed,
                                             blSplitDataIteratorType SplitDataIterator)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we
        // have no data
        return;
    }
    else if(IteratorToBeginningOfSingleDataTokens == IteratorToEndOfSingleDataTokens)
    {
        // In this case we won't
        // be able to find any tokens,
        // Therefore the split data
        // will look the same as the
        // original data
        (*SplitDataIterator) = blSplitDataContainerType(IteratorToBeginningOfData,IteratorToEndOfData);
    }

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::find_first_of(FirstTokenIterator,IteratorToEndOfData,
                                                 IteratorToBeginningOfSingleDataTokens,IteratorToEndOfSingleDataTokens);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroSizedVectorsBeAllowed)
        {
            // In this case, the token is the
            // first data element, and since
            // we don't count zero size data
            // vectors, we skip to the next
            // data element
            ++FirstTokenIterator;
        }
        else
        {
            // Split the data
            (*SplitDataIterator++) = blSplitDataContainerType(FirstTokenIterator,SecondTokenIterator);

            FirstTokenIterator = SecondTokenIterator;

            // Advance the token
            // if we've not reched
            // the end yet
            if(FirstTokenIterator != IteratorToEndOfData)
                ++FirstTokenIterator;
        }
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blSplitDataUsingToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfToken
//                      - IteratorToEndOfToken
//                      - ShouldZeroSizedVectorsBeAllowed
//                      - SplitDataIterator
//
// TEMPLATE ARGUMENTS:  - blSplitDataContainerType -- The type of
//                                                    container that
//                                                    will hold the
//                                                    individual data
//                                                    points.
//                      - blSplitDataIteratorType -- The type of
//                                                   iterator that will
//                                                   iterate through
//                                                   the split data
//                                                   and push the
//                                                   split data
//                                                   containers.
//                      - blDataIteratorType -- The type of data iterator
//                      - blTokenIteratorType -- The type of token iterator
//
// PURPOSE:             This function splits a data array into
//                      a vector of data vectors using a token.
//                      The token is a sequence of data points.
//                      The function allows the user to specify
//                      whether it should allow zero sized vectors.
//
// DEPENDENCIES:        - std::search
//                      - std::distance
//                      - AdvanceIterator
//                      - blSplitDataIteratorType is assumed to
//                        to act like std::back_insert_iterator or
//                        std::front_insert_iterator
//                      - blSplitDataContainerType is assumed to
//                        have a constructor like the following:
//                        std::vector(IteratorToFirst,IteratorToLast)
//
// NOTES:               - The template parameters are all infered
//                        through the function arguments except
//                        for the first one.  The user has to
//                        specify the type of split data container
//                        where the individual data points will
//                        be saved.
//                      - For example:  If the data points are
//                                      doubles, and the split
//                                      data container is an
//                                      std::deque<double>, the
//                                      user has to specify it.
//-------------------------------------------------------------------
template<typename blSplitDataContainerType,
         typename blSplitDataIteratorType,
         typename blDataIteratorType,
         typename blTokenIteratorType>
inline void blSplitDataUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                  const blDataIteratorType& IteratorToEndOfData,
                                  const blTokenIteratorType& IteratorToBeginningOfToken,
                                  const blTokenIteratorType& IteratorToEndOfToken,
                                  const bool& ShouldZeroSizedVectorsBeAllowed,
                                  blSplitDataIteratorType SplitDataIterator)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case we
        // have no data
        return;
    }
    else if(IteratorToBeginningOfToken == IteratorToEndOfToken)
    {
        // In this case we won't
        // be able to find any tokens,
        // Therefore the split data
        // will look the same as the
        // original data
        (*SplitDataIterator++) = blSplitDataContainerType(IteratorToBeginningOfData,IteratorToEndOfData);
    }

    // Calculate the token length
    // but remember that the token
    // could be a non-contiguous array,
    // so the iterators might be
    // only "bidirectional" iterators
    // instead of "random access" ones
    size_t TokenLength = std::distance(IteratorToBeginningOfToken,IteratorToEndOfToken);

    // The iterators to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;

    while(FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next token
        // in the data
        SecondTokenIterator = std::search(FirstTokenIterator,IteratorToEndOfData,
                                          IteratorToBeginningOfToken,IteratorToEndOfToken);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroSizedVectorsBeAllowed)
        {
            // In this case, the token is the
            // first data element, and since
            // we don't count zero size data
            // vectors, we skip to the next
            // data element
            AdvanceIterator(FirstTokenIterator,IteratorToEndOfData,TokenLength);
        }
        else
        {
            // Split the data at
            // the token point
            (*SplitDataIterator++) = blSplitDataContainerType(FirstTokenIterator,SecondTokenIterator);
            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator if
            // we've not reached the
            // end yet
            AdvanceIterator(FirstTokenIterator,IteratorToEndOfData,TokenLength);
        }
    }
}
//-------------------------------------------------------------------
/*





//-------------------------------------------------------------------
// FUNCTION:            GetImageFromString
//
// ARGUMENTS:           - DstImage
//                      - Text
//                      - RowDelimiter
//                      - ColDelimiter
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             Reads a table of values from a string
//                      into a blImage of type specified using
//                      a template
//
// DEPENDENCIES:        - blImage
//                      - std::string
//
// NOTES:
//-------------------------------------------------------------------
template<typename blType>
inline void GetImageFromString(blImage<blType>& DstImage,
                               const std::string& Text,
                               const char& RowDelimiter = '\n',
                               const char& ColDelimiter = ',')
{
    // First count the number
    // of rows and columns from
    // the text string
    int Rows,Cols;
    CountRowsAndColumnsFromString(Text,Rows,Cols,RowDelimiter,ColDelimiter);

    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- The text string
        //          did not contain
        //          any values
        return;
    }

    // Now we know the size of
    // the final destination
    // image, so we create it
    if(DstImage.size1() != Rows ||
       DstImage.size2() != Cols)
    {
        DstImage.CreateImage(Rows,Cols);
    }

    // Save the length of the string
    int Length = Text.length();

    // Integers used to know the position
    // of each row and column
    int RowBegin = 0,RowEnd = 0;
    int ColBegin = 0,ColEnd = 0;

    // Integers used to index
    // our position in the image
    int NumOfRowsAlreadyProcessed = 0;
    int NumOfColsAlreadyProcessed = 0;

    int NumOfCharsUsed;

    // Now we parse the text string
    // and read its values into our
    // destination image
    while(RowBegin < Length &&
          RowEnd< Length &&
          ColEnd < Length &&
          NumOfRowsAlreadyProcessed <= Rows)
    {
        // Determine the end of
        // the next row of data
        RowEnd = FindPositionOfNextDelimiter(Text.c_str(),RowBegin,Length - RowBegin,RowDelimiter);

        // Zero out the number of
        // columns already processed
        NumOfColsAlreadyProcessed = 0;

        // Save the new col
        // begin position
        ColBegin = RowBegin;

        while(ColBegin < RowEnd &&
              ColEnd <= RowEnd &&
              NumOfColsAlreadyProcessed <= Cols)
        {
            // Determine the end of
            // the next column of data
            ColEnd = FindPositionOfNextDelimiter(Text.c_str(),ColBegin,Length - ColBegin,ColDelimiter);

            // Convert that data value
            // and store it inside the
            // blImage matrix
            if(ColEnd <= RowEnd)
                DstImage[NumOfRowsAlreadyProcessed][NumOfColsAlreadyProcessed] = ConvertStringToNumber<blType>(Text.c_str(),
                                                                                                               NumOfCharsUsed,
                                                                                                               ColBegin,
                                                                                                               ColEnd - ColBegin);
            else
                DstImage[NumOfRowsAlreadyProcessed][NumOfColsAlreadyProcessed] = ConvertStringToNumber<blType>(Text.c_str(),
                                                                                                               NumOfCharsUsed,
                                                                                                               ColBegin,
                                                                                                               RowEnd - ColBegin);

            // Save the new col
            // begin position
            ColBegin = ColEnd + 1;

            // Increase the number of
            // cols that we've already
            // processed
            ++NumOfColsAlreadyProcessed;
        }

        // Save the new row
        // begin position
        RowBegin = RowEnd + 1;

        // Increase the number of
        // rows that we've already
        // processed
        ++NumOfRowsAlreadyProcessed;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            FillImageValuesFromString
//
// ARGUMENTS:           - DstImage
//                      - RowIndex
//                      - ColIndex
//                      - Text
//                      - TextLength
//                      - PosInTextStringToStartReadingFrom
//                      - PosInTextStringToStartReadingFromNextTime
//                      - RowDelimiter
//                      - ColDelimiter
//
// TEMPLATE ARGUMENTS:  blType
//
// PURPOSE:             Reads a table of values from a string
//                      into a blImage of type specified using
//                      a template
//
// DEPENDENCIES:        - blImage
//                      - std::string
//
// NOTES:
//-------------------------------------------------------------------
template<typename blType>
inline void FillImageValuesFromString(blImage<blType>& DstImage,
                                      int& RowIndex,
                                      int& ColIndex,
                                      const char* Text,
                                      const int& TextLength,
                                      const int& PosInTextStringToStartReadingFrom,
                                      int& PosInTextStringToStartReadingFromNextTime,
                                      const char& RowDelimiter = '\n',
                                      const char& ColDelimiter = ',')
{
    // Let's get the size of
    // the destination image
    int Rows = DstImage.size1();
    int Cols = DstImage.size2();

    if(Rows <= 0 || Cols <= 0)
    {
        // Error -- The destination
        //          image is of size
        //          zero
        return;
    }

    // Check for the validity of
    // the starting row and column
    // We think of the image as a
    // circular buffer, meaning that
    // when we reach the end of the
    // image, we come back to the beginning
    if(RowIndex >= 0)
        RowIndex = RowIndex % Rows;
    else
        RowIndex = Rows + (RowIndex % Rows);

    if(ColIndex >= 0)
        ColIndex = ColIndex % Cols;
    else
        ColIndex = Cols + (ColIndex % Cols);


    // The delimiter that we
    // find in the text string
    char DelimiterFound = 0;

    // The current position
    // in the text string
    int CurrentPosInTextString = PosInTextStringToStartReadingFrom;

    // Find the next delimiter
    // and its position in the
    // text string
    int PosOfDelimiterFound = FindPositionOfNextRowOrCol(Text,
                                                         CurrentPosInTextString,
                                                         TextLength - CurrentPosInTextString,
                                                         DelimiterFound,
                                                         RowDelimiter,
                                                         ColDelimiter);

    // Variable used to know
    // how many characters were
    // used to determine the
    // number from the string
    int NumOfCharsUsed;

    while(CurrentPosInTextString < TextLength &&
          PosOfDelimiterFound < TextLength)
    {
        // We try to convert the text
        // to a number and save it into
        // destination image
        DstImage[RowIndex % Rows][ColIndex % Cols] = ConvertStringToNumber<blType>(Text,
                                                                                   NumOfCharsUsed,
                                                                                   CurrentPosInTextString,
                                                                                   PosOfDelimiterFound - CurrentPosInTextString,
                                                                                   '.');

        // If the next delimiter
        // is a row delimiter then
        // we increase the number
        // of rows, otherwise
        // if it's a col delimiters
        // we increase the number
        // of cols
        if(DelimiterFound == RowDelimiter)
        {
            ++RowIndex;
            ColIndex = 0;
        }
        else if(DelimiterFound == ColDelimiter)
            ++ColIndex;

        // Save the position of the
        // delimiter found as the
        // current position in the string
        CurrentPosInTextString = PosOfDelimiterFound + 1;

        // Find the next delimiter
        // and its position in the
        // text string
        PosOfDelimiterFound = FindPositionOfNextRowOrCol(Text,
                                                         CurrentPosInTextString,
                                                         TextLength - CurrentPosInTextString,
                                                         DelimiterFound,
                                                         RowDelimiter,
                                                         ColDelimiter);
    }

    // Store the current position
    // in the text string so that
    // the user knows where to start
    // reading from next time he calls
    // this function
    PosInTextStringToStartReadingFromNextTime = CurrentPosInTextString;
}
//-------------------------------------------------------------------
*/


#endif // BL_BUFFERALGORITHMS_HPP
