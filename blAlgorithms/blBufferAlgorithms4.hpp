#ifndef BL_BUFFERALGORITHMS4_HPP
#define BL_BUFFERALGORITHMS4_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms4.hpp
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
// FUNCTION:            blFindLengthOfLongestDataRow
//
// ARGUMENTS:           - BeginIter
//                      - EndIter
//                      - RowToken
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenIteratorType
//                      - blPredicateFunctorType
//
// PURPOSE:             This function finds data rows in a
//                      buffer by searching for the specified
//                      Row Token and then counts the length of
//                      each row between two row tokens to find
//                      the longest one
//
// DEPENDENCIES:        - blFind
//                      - std::distance
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blFindLengthOfLongestDataRow(const blDataIteratorType& BeginIter,
                                           const blDataIteratorType& EndIter,
                                           const blTokenType& RowToken)
{
    // Check the inputs
    if(BeginIter == EndIter)
    {
        // Error -- Passed a zero
        //          length buffer

        return size_t(0);
    }

    // The iterators to
    // the tokens found
    // in the data buffer

    blDataIteratorType FirstTokenIterator = BeginIter;
    blDataIteratorType SecondTokenIterator = EndIter;

    // The length of the
    // longest data row

    size_t LongestLength = 0;

    // The length of the
    // current data row

    size_t LengthOfCurrentRow = 0;

    while(FirstTokenIterator != EndIter)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blFind(FirstTokenIterator,
                                     EndIter,
                                     RowToken,
                                     0);

        // Calculate the
        // length of the
        // current data row

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

        if(FirstTokenIterator != EndIter)
            ++FirstTokenIterator;

        // In case of circular
        // iterators we might
        // be back to the
        // beginning, so we
        // quit in that case

        if(FirstTokenIterator == BeginIter)
            break;
    }

    // We're done
    // counting

    return LongestLength;
}


template<typename blDataIteratorType,
         typename blTokenType,
         typename blPredicateFunctorType>
inline size_t blFindLengthOfLongestDataRow(const blDataIteratorType& BeginIter,
                                           const blDataIteratorType& EndIter,
                                           const blTokenType& RowToken,
                                           const blPredicateFunctorType& PredicateFunctor)
{
    // Check the inputs
    if(BeginIter == EndIter)
    {
        // Error -- Passed a zero
        //          length buffer

        return size_t(0);
    }

    // The iterators to
    // the tokens found
    // in the data buffer

    blDataIteratorType FirstTokenIterator = BeginIter;
    blDataIteratorType SecondTokenIterator = EndIter;

    // The length of the
    // longest data row

    size_t LongestLength = 0;

    // The length of the
    // current data row

    size_t LengthOfCurrentRow = 0;

    while(FirstTokenIterator != EndIter)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blFind(FirstTokenIterator,
                                     EndIter,
                                     RowToken,
                                     0,
                                     PredicateFunctor);

        // Calculate the
        // length of the
        // current data row

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

        if(FirstTokenIterator != EndIter)
            ++FirstTokenIterator;

        // In case of circular
        // iterators we might
        // be back to the
        // beginning, so we
        // quit in that case

        if(FirstTokenIterator == BeginIter)
            break;
    }

    // We're done
    // counting

    return LongestLength;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blCountDataRowsLookingForRowToken
//
// ARGUMENTS:           - RowToken
//                      - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorWhereToStartCounting
//                      - IteratorWhereToStopCounting
//                      - ShouldZeroLengthRowsBeCounted
//                      - ShouldCountingBeDoneCircularly
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenType
//
// PURPOSE:             This function counts the number of
//                      data rows from a data buffer by
//                      looking for a user specified "row token".
//                      This function allows the user to specify
//                      whether or not to count rows of zero length,
//                      and whether or not the counting should
//                      be done circularly.
//
// DEPENDENCIES:        - blFind2
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blCountDataRowsLookingForRowToken(const blTokenType& RowToken,
                                                const blDataIteratorType& IteratorToBeginningOfData,
                                                const blDataIteratorType& IteratorToEndOfData,
                                                const blDataIteratorType& IteratorWhereToStartCounting,
                                                const blDataIteratorType& IteratorWhereToStopCounting,
                                                const bool& ShouldZeroLengthRowsBeCounted,
                                                const bool& ShouldCountingBeDoneCircularly)
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

    blDataIteratorType FirstTokenIterator = IteratorWhereToStartCounting;
    blDataIteratorType SecondTokenIterator = IteratorWhereToStartCounting;

    if(FirstTokenIterator == IteratorToEndOfData &&
       ShouldCountingBeDoneCircularly)
    {
        FirstTokenIterator = IteratorToBeginningOfData;
    }

    // The total number
    // of rows found by
    // this function

    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorWhereToStopCounting &&
          FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blFind2(RowToken,
                                      IteratorToBeginningOfData,
                                      IteratorToEndOfData,
                                      FirstTokenIterator,
                                      IteratorWhereToStopCounting,
                                      ShouldCountingBeDoneCircularly);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
        else
        {
            // Increse the total
            // number of rows

            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
    }

    // We're done counting

    return TotalNumberOfRows;
}


template<typename blDataIteratorType,
         typename blTokenType,
         typename blPredicateFunctorType>
inline size_t blCountDataRowsLookingForRowToken(const blTokenType& RowToken,
                                                const blDataIteratorType& IteratorToBeginningOfData,
                                                const blDataIteratorType& IteratorToEndOfData,
                                                const blDataIteratorType& IteratorWhereToStartCounting,
                                                const blDataIteratorType& IteratorWhereToStopCounting,
                                                const bool& ShouldZeroLengthRowsBeCounted,
                                                const bool& ShouldCountingBeDoneCircularly,
                                                const blPredicateFunctorType& PredicateFunctor)
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

    blDataIteratorType FirstTokenIterator = IteratorWhereToStartCounting;
    blDataIteratorType SecondTokenIterator = IteratorWhereToStartCounting;

    if(FirstTokenIterator == IteratorToEndOfData &&
       ShouldCountingBeDoneCircularly)
    {
        FirstTokenIterator = IteratorToBeginningOfData;
    }

    // The total number
    // of rows found by
    // this function

    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorWhereToStopCounting &&
          FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blFind2(RowToken,
                                      IteratorToBeginningOfData,
                                      IteratorToEndOfData,
                                      FirstTokenIterator,
                                      IteratorWhereToStopCounting,
                                      ShouldCountingBeDoneCircularly,
                                      PredicateFunctor);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
        else
        {
            // Increse the total
            // number of rows

            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
    }

    // We're done counting

    return TotalNumberOfRows;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blReverseCountDataRowsLookingForRowToken
//
// ARGUMENTS:           - RowToken
//                      - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorWhereToStartCounting
//                      - IteratorWhereToStopCounting
//                      - ShouldZeroLengthRowsBeCounted
//                      - ShouldCountingBeDoneCircularly
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType
//                      - blTokenType
//
// PURPOSE:             This function counts the number of
//                      data rows from a data buffer by
//                      looking for a user specified "row token".
//                      This function allows the user to specify
//                      whether or not to count rows of zero length,
//                      and whether or not the counting should
//                      be done circularly.
//
// DEPENDENCIES:        - blFind2
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType>
inline size_t blReverseCountDataRowsLookingForRowToken(const blTokenType& RowToken,
                                                       const blDataIteratorType& IteratorToBeginningOfData,
                                                       const blDataIteratorType& IteratorToEndOfData,
                                                       const blDataIteratorType& IteratorWhereToStartCounting,
                                                       const blDataIteratorType& IteratorWhereToStopCounting,
                                                       const bool& ShouldZeroLengthRowsBeCounted,
                                                       const bool& ShouldCountingBeDoneCircularly)
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

    blDataIteratorType FirstTokenIterator = IteratorWhereToStartCounting;
    blDataIteratorType SecondTokenIterator = IteratorWhereToStartCounting;

    if(FirstTokenIterator == IteratorToEndOfData &&
       ShouldCountingBeDoneCircularly)
    {
        FirstTokenIterator = IteratorToBeginningOfData;
    }

    // The total number
    // of rows found by
    // this function

    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorWhereToStopCounting &&
          FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blReverseFind2(RowToken,
                                             IteratorToBeginningOfData,
                                             IteratorToEndOfData,
                                             FirstTokenIterator,
                                             IteratorWhereToStopCounting,
                                             ShouldCountingBeDoneCircularly);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
        else
        {
            // Increse the total
            // number of rows

            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
    }

    // We're done counting

    return TotalNumberOfRows;
}


template<typename blDataIteratorType,
         typename blTokenType,
         typename blPredicateFunctorType>
inline size_t blReverseCountDataRowsLookingForRowToken(const blTokenType& RowToken,
                                                       const blDataIteratorType& IteratorToBeginningOfData,
                                                       const blDataIteratorType& IteratorToEndOfData,
                                                       const blDataIteratorType& IteratorWhereToStartCounting,
                                                       const blDataIteratorType& IteratorWhereToStopCounting,
                                                       const bool& ShouldZeroLengthRowsBeCounted,
                                                       const bool& ShouldCountingBeDoneCircularly,
                                                       const blPredicateFunctorType& PredicateFunctor)
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

    blDataIteratorType FirstTokenIterator = IteratorWhereToStartCounting;
    blDataIteratorType SecondTokenIterator = IteratorWhereToStartCounting;

    if(FirstTokenIterator == IteratorToEndOfData &&
       ShouldCountingBeDoneCircularly)
    {
        FirstTokenIterator = IteratorToBeginningOfData;
    }

    // The total number
    // of rows found by
    // this function

    size_t TotalNumberOfRows = size_t(0);

    while(FirstTokenIterator != IteratorWhereToStopCounting &&
          FirstTokenIterator != IteratorToEndOfData)
    {
        // Find the next
        // token in the
        // data

        SecondTokenIterator = blReverseFind2(RowToken,
                                             IteratorToBeginningOfData,
                                             IteratorToEndOfData,
                                             FirstTokenIterator,
                                             IteratorWhereToStopCounting,
                                             ShouldCountingBeDoneCircularly,
                                             PredicateFunctor);

        if(SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case, we do not
            // want to count this as
            // a data row

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
        else
        {
            // Increse the total
            // number of rows

            ++TotalNumberOfRows;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the iterator
            // if we've not reached
            // the end yet

            if(FirstTokenIterator == IteratorToEndOfData &&
               ShouldCountingBeDoneCircularly)
            {
                FirstTokenIterator = IteratorToBeginningOfData;
            }
            else if(FirstTokenIterator != IteratorWhereToStopCounting)
            {
                ++FirstTokenIterator;
            }
        }
    }

    // We're done counting

    return TotalNumberOfRows;
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
         typename blTokenType,
         typename blIntegerType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForRowToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                               const blDataIteratorType& IteratorToEndOfData,
                                                                               const blTokenType& RowToken,
                                                                               const bool& ShouldZeroLengthRowsBeCounted,
                                                                               blIntegerType& TotalNumberOfRows,
                                                                               blIntegerType& LengthOfLongestRow)
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
    blIntegerType LengthOfCurrentRow = 0;

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
         typename blTokenIteratorType,
         typename blIntegerType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForAnyoneOfSeveralRowTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                               const blDataIteratorType& IteratorToEndOfData,
                                                                                               const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                               const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                               const bool& ShouldZeroLengthRowsBeCounted,
                                                                                               blIntegerType& TotalNumberOfRows,
                                                                                               blIntegerType& LengthOfLongestRow)
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
    blIntegerType LengthOfCurrentRow = 0;

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
         typename blTokenIteratorType,
         typename blIntegerType>
inline blDataIteratorType blCountDataRowsAndLongestRowLengthLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                    const blDataIteratorType& IteratorToEndOfData,
                                                                                    const blTokenIteratorType& IteratorToBeginningOfToken,
                                                                                    const blTokenIteratorType& IteratorToEndOfToken,
                                                                                    const bool& ShouldZeroLengthRowsBeCounted,
                                                                                    blIntegerType& TotalNumberOfRows,
                                                                                    blIntegerType& LengthOfLongestRow)
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
    blIntegerType TokenLength = std::distance(IteratorToBeginningOfToken,IteratorToEndOfToken);

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
    blIntegerType LengthOfCurrentRow = 0;

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


#endif // BL_BUFFERALGORITHMS4_HPP
