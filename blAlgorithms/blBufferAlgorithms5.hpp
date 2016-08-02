#ifndef BL_BUFFERALGORITHMS5_HPP
#define BL_BUFFERALGORITHMS5_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms5.hpp
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
// FUNCTION:            blFindStartingAndEndingPositionsOfNthDataRowLookingForToken
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenType,
         typename blIntegerType>
inline blIntegerType blFindStartingAndEndingPositionsOfNthDataRowLookingForToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                 const blDataIteratorType& IteratorToEndOfData,
                                                                                 const blTokenType& RowToken,
                                                                                 const bool& ShouldZeroLengthRowsBeCounted,
                                                                                 const blIntegerType& WhichRowToFind,
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
        return blIntegerType(0);
    }

    // The iterator to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    IteratorToStartingPositionOfNthRow = FirstTokenIterator;
    IteratorToEndingPositionOfNthRow = FirstTokenIterator;

    // The current data row
    blIntegerType CurrentRow = blIntegerType(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow < WhichRowToFind)
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenIteratorType,
         typename blIntegerType>
inline blIntegerType blFindStartingAndEndingPositionsOfNthDataRowLookingForSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                            const blDataIteratorType& IteratorToEndOfData,
                                                                                            const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                            const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                            const bool& ShouldZeroLengthRowsBeCounted,
                                                                                            const blIntegerType& WhichRowToFind,
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
        return blIntegerType(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we won't
        // find any tokens
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return blIntegerType(0);
    }

    // The iterator to the
    // tokens found in the data
    blDataIteratorType FirstTokenIterator = IteratorToBeginningOfData;
    blDataIteratorType SecondTokenIterator = IteratorToBeginningOfData;
    IteratorToStartingPositionOfNthRow = FirstTokenIterator;
    IteratorToEndingPositionOfNthRow = FirstTokenIterator;

    // The current data row
    blIntegerType CurrentRow = blIntegerType(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow < WhichRowToFind)
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenIteratorType,
         typename blIntegerType>
inline blIntegerType blFindStartingAndEndingPositionsOfNthDataRowLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                                         const blDataIteratorType& IteratorToEndOfData,
                                                                                         const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                                         const blTokenIteratorType& IteratorToEndOfTokens,
                                                                                         const bool& ShouldZeroLengthRowsBeCounted,
                                                                                         const blIntegerType& WhichRowToFind,
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
        return blIntegerType(0);
    }
    else if(IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case we won't
        // find any tokens
        IteratorToStartingPositionOfNthRow = IteratorToBeginningOfData;
        IteratorToEndingPositionOfNthRow = IteratorToEndOfData;
        return blIntegerType(0);
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
    size_t CurrentRow = blIntegerType(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          CurrentRow < WhichRowToFind)
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
// FUNCTION:            blFindPositionOfNthDataRowAndMthDataColumnLookingForToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - RowToken
//                      - ShouldZeroLengthRowsBeCounted
//                      - WhichRowToFind
//                      - WhichColToFind
//                      - IteratorToPositionOfNthRowAndMthColumn
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowLookingForToken
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenType,
         typename blIntegerType>
inline void blFindPositionOfNthDataRowAndMthDataColumnLookingForToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                                      const blDataIteratorType& IteratorToEndOfData,
                                                                      const blTokenType& RowToken,
                                                                      const bool& ShouldZeroLengthRowsBeCounted,
                                                                      const blIntegerType& WhichRowToFind,
                                                                      const blIntegerType& WhichColToFind,
                                                                      blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                      blIntegerType& ActualRowFound,
                                                                      blIntegerType& ActualColFound)
{
    // We first try to
    // find the starting
    // and ending positions
    // of the specified
    // Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    ActualRowFound = blFindStartingAndEndingPositionsOfNthDataRowLookingForToken(IteratorToBeginningOfData,
                                                                                 IteratorToEndOfData,
                                                                                 RowToken,
                                                                                 ShouldZeroLengthRowsBeCounted,
                                                                                 WhichRowToFind,
                                                                                 IteratorToPositionOfNthRowAndMthColumn,
                                                                                 IteratorToEndingPositionOfNthRow);

    // We calculate the
    // actual column
    // that the iterator
    // will be pointing to

    ActualColFound = std::min( WhichColToFind,
                               blIntegerType(std::distance(IteratorToPositionOfNthRowAndMthColumn,
                                                           IteratorToEndingPositionOfNthRow)) );

    // Advance the iterator
    // to get it to the Mth
    // column making sure
    // it does not pass the
    // end of the row

    std::advance(IteratorToPositionOfNthRowAndMthColumn,ActualColFound);
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType,
         typename blIntegerType>
inline void blFindPositionOfNthDataRowAndMthDataColumnUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                            const blDataIteratorType& IteratorToEndOfData,
                                                                            const blTokenIteratorType& IteratorToBeginningOfSingleDataTokens,
                                                                            const blTokenIteratorType& IteratorToEndOfSingleDataTokens,
                                                                            const bool& ShouldZeroSizedVectorsBeAllowed,
                                                                            const blIntegerType& WhichRowToFind,
                                                                            const blIntegerType& WhichColToFind,
                                                                            blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                            blIntegerType& ActualRowFound,
                                                                            blIntegerType& ActualColFound)
{
    // We first try to
    // find the starting
    // and ending positions
    // of the specified
    // Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    // We calculate the
    // actual column
    // that the iterator
    // will be pointing to

    ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowUsingSingleDataTokens(IteratorToBeginningOfData,
                                                                                     IteratorToEndOfData,
                                                                                     IteratorToBeginningOfSingleDataTokens,
                                                                                     IteratorToEndOfSingleDataTokens,
                                                                                     ShouldZeroSizedVectorsBeAllowed,
                                                                                     WhichRowToFind,
                                                                                     IteratorToPositionOfNthRowAndMthColumn,
                                                                                     IteratorToEndingPositionOfNthRow);

    ActualColFound = std::min( WhichColToFind,
                               std::distance(IteratorToPositionOfNthRowAndMthColumn,
                                             IteratorToEndingPositionOfNthRow) );

    // Advance the iterator
    // to get it to the Mth
    // column making sure
    // it does not pass the
    // end of the row

    std::advance(IteratorToPositionOfNthRowAndMthColumn,ActualColFound);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blFindPositionOfNthDataRowAndMthDataColumnLookingForTokenSequence
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
//
// PURPOSE:             This function gets an iterator to a data
//                      point in the data array corresponding to
//                      the specified Nth data row and Mth data
//                      column.
//
// DEPENDENCIES:        - FindStartingAndEndingPositionsOfNthDataRowLookingForTokenSequence
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blTokenIteratorType,
         typename blIntegerType>
inline void blFindPositionOfNthDataRowAndMthDataColumnLookingForTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                              const blDataIteratorType& IteratorToEndOfData,
                                                                              const blTokenIteratorType& IteratorToBeginningOfSingleDataTokens,
                                                                              const blTokenIteratorType& IteratorToEndOfSingleDataTokens,
                                                                              const bool& ShouldZeroSizedVectorsBeAllowed,
                                                                              const blIntegerType& WhichRowToFind,
                                                                              const blIntegerType& WhichColToFind,
                                                                              blDataIteratorType& IteratorToPositionOfNthRowAndMthColumn,
                                                                              blIntegerType& ActualRowFound,
                                                                              blIntegerType& ActualColFound)
{
    // We first try to
    // find the starting
    // and ending positions
    // of the specified
    // Nth data row

    blDataIteratorType IteratorToEndingPositionOfNthRow;

    // We calculate the
    // actual column
    // that the iterator
    // will be pointing to

    ActualRowFound = FindStartingAndEndingPositionsOfNthDataRowLookingForTokenSequence(IteratorToBeginningOfData,
                                                                                       IteratorToEndOfData,
                                                                                       IteratorToBeginningOfSingleDataTokens,
                                                                                       IteratorToEndOfSingleDataTokens,
                                                                                       ShouldZeroSizedVectorsBeAllowed,
                                                                                       WhichRowToFind,
                                                                                       IteratorToPositionOfNthRowAndMthColumn,
                                                                                       IteratorToEndingPositionOfNthRow);

    ActualColFound = std::min( WhichColToFind,
                               std::distance(IteratorToPositionOfNthRowAndMthColumn,
                                             IteratorToEndingPositionOfNthRow) );

    // Advance the iterator
    // to get it to the Mth
    // column making sure
    // it does not pass the
    // end of the row

    std::advance(IteratorToPositionOfNthRowAndMthColumn,ActualColFound);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blGetRowAndColumnNumberOfDataPositionUsingToken
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - Token
//                      - ShouldZeroLengthRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - IteratorToDataPosition
//                      - RowNumber
//                      - ColNumber
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenType,
         typename blIntegerType>
inline void blGetRowAndColumnNumberOfDataPositionUsingToken(const blDataIteratorType& IteratorToBeginningOfData,
                                                            const blDataIteratorType& IteratorToEndOfData,
                                                            const blTokenType& Token,
                                                            const bool& ShouldZeroLengthRowsBeCounted,
                                                            const blDataIteratorType& IteratorToDataPosition,
                                                            blIntegerType& RowNumber,
                                                            blIntegerType& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroLengthRowsBeCounted)
        {
            RowNumber = blIntegerType(-1);
            ColNumber = blIntegerType(-1);
        }
        else
        {
            RowNumber = blIntegerType(0);
            ColNumber = blIntegerType(0);
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
    if(ShouldZeroLengthRowsBeCounted)
        RowNumber = blIntegerType(0);
    else
        RowNumber = blIntegerType(-1);

    while(FirstTokenIterator != IteratorToEndOfData &&
          std::distance(FirstTokenIterator,IteratorToDataPosition) > 0)
    {
        // Find the next
        // token in the
        // data buffer
        SecondTokenIterator = std::find(FirstTokenIterator,IteratorToEndOfData,Token);

        // Check to see if
        // the row we found
        // was a zero length
        // row
        if(SecondTokenIterator != IteratorToEndOfData &&
           SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case,
            // the row is of
            // zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else if(SecondTokenIterator != IteratorToEndOfData &&
                std::distance(SecondTokenIterator,IteratorToDataPosition) >= 0)
        {
            ++RowNumber;

            FirstTokenIterator = SecondTokenIterator;
            ++FirstTokenIterator;

            StartingPositionOfCurrentRow = FirstTokenIterator;
        }
        else
        {
            FirstTokenIterator = SecondTokenIterator;
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);
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
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenIteratorType,
         typename blIntegerType>
inline void blGetRowAndColumnNumberOfDataPositionUsingSingleDataTokens(const blDataIteratorType& IteratorToBeginningOfData,
                                                                       const blDataIteratorType& IteratorToEndOfData,
                                                                       const blTokenIteratorType& IteratorToBeginningOfTokens,
                                                                       const blTokenIteratorType& IteratorToEndOfTokens,
                                                                       const bool& ShouldZeroLengthRowsBeCounted,
                                                                       const blDataIteratorType& IteratorToDataPosition,
                                                                       blIntegerType& RowNumber,
                                                                       blIntegerType& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData ||
       IteratorToBeginningOfTokens == IteratorToEndOfTokens)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroLengthRowsBeCounted)
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
    if(ShouldZeroLengthRowsBeCounted)
        RowNumber = 0;
    else
        RowNumber = -1;

    while(FirstTokenIterator != IteratorToEndOfData &&
          FirstTokenIterator != IteratorToDataPosition)
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
        if(SecondTokenIterator != IteratorToEndOfData &&
           SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
        {
            // In this case,
            // the row is of
            // zero length and
            // we don't count it
            ++FirstTokenIterator;
        }
        else if(SecondTokenIterator != IteratorToEndOfData &&
                std::distance(SecondTokenIterator,IteratorToDataPosition) >= 0)
        {
            ++RowNumber;

            FirstTokenIterator = SecondTokenIterator;
            ++FirstTokenIterator;

            StartingPositionOfCurrentRow = FirstTokenIterator;
        }
        else
        {
            FirstTokenIterator = SecondTokenIterator;
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// FUNCTION:            blGetRowAndColumnNumberOfDataPositionUsingTokenSequence
//
// ARGUMENTS:           - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorToBeginningOfTokenSequence
//                      - IteratorToEndOfTokenSequence
//                      - ShouldZeroLengthRowsBeCounted
//                      - ShouldZeroSizedColsBeCounted
//                      - IteratorToDataPosition
//                      - RowNumber
//                      - ColNumber
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//                      - blTokenIteratorType -- The type of token
//                                               iterator
//                      - blIntegerType -- The type of integer to
//                                         indicate the row and col
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
         typename blTokenIteratorType,
         typename blIntegerType>
inline void blGetRowAndColumnNumberOfDataPositionUsingTokenSequence(const blDataIteratorType& IteratorToBeginningOfData,
                                                                    const blDataIteratorType& IteratorToEndOfData,
                                                                    const blTokenIteratorType& IteratorToBeginningOfTokenSequence,
                                                                    const blTokenIteratorType& IteratorToEndOfTokenSequence,
                                                                    const bool& ShouldZeroLengthRowsBeCounted,
                                                                    const blDataIteratorType& IteratorToDataPosition,
                                                                    blIntegerType& RowNumber,
                                                                    blIntegerType& ColNumber)
{
    // Check the inputs
    if(IteratorToBeginningOfData == IteratorToEndOfData ||
       IteratorToBeginningOfTokenSequence == IteratorToEndOfTokenSequence)
    {
        // In this case, we
        // have no data to
        // parse, or no
        // tokens to find
        if(!ShouldZeroLengthRowsBeCounted)
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
    blIntegerType TokenSequenceLength = std::distance(IteratorToBeginningOfTokenSequence,IteratorToEndOfTokenSequence);

    // We initialize the
    // row number differently
    // depending on whether
    // or not rows of zero size
    // are to be counted
    if(ShouldZeroLengthRowsBeCounted)
        RowNumber = 0;
    else
        RowNumber = -1;

    while(FirstTokenIterator != IteratorToEndOfData &&
          FirstTokenIterator != IteratorToDataPosition)
    {
        // Check to see if
        // the row we found
        // was a zero length
        // row
        if(SecondTokenIterator != IteratorToEndOfData &&
           SecondTokenIterator == FirstTokenIterator &&
           !ShouldZeroLengthRowsBeCounted)
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
        else if(SecondTokenIterator != IteratorToEndOfData &&
                std::distance(SecondTokenIterator,IteratorToDataPosition) >= 0)
        {
            ++RowNumber;

            FirstTokenIterator = SecondTokenIterator;

            // Advance the
            // iterator
            std::advance(FirstTokenIterator,
                         std::min(TokenSequenceLength,
                                  std::distance(FirstTokenIterator,
                                                IteratorToEndOfData)));

            StartingPositionOfCurrentRow = FirstTokenIterator;
        }
        else
        {
            FirstTokenIterator = SecondTokenIterator;
        }
    }

    // We calculated the
    // row, now we need
    // to calculate the
    // column

    ColNumber = std::distance(StartingPositionOfCurrentRow,IteratorToDataPosition);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// std::string specializations of
// the above functions
//-------------------------------------------------------------------
template<typename blIntegerType>
inline blIntegerType blFindStartingAndEndingPositionsOfNthRowInString(const std::string& TextString,
                                                                      const char& RowToken,
                                                                      const blIntegerType& WhichRowToFind,
                                                                      blIntegerType& StartingPositionOfNthRow,
                                                                      blIntegerType& EndingPositionOfNthRow)
{
    // Check the inputs
    if(TextString.length() <= 0)
    {
        // In this case
        // we have no
        // data to look
        // through
        StartingPositionOfNthRow = blIntegerType(0);
        EndingPositionOfNthRow = blIntegerType(0);
        return blIntegerType(0);
    }

    const char* IteratorToStartingPositionOfNthRow;
    const char* IteratorToEndingPositionOfNthRow;

    blIntegerType ActualRowFound = blFindStartingAndEndingPositionsOfNthDataRowLookingForToken(&TextString[0],
                                                                                               &TextString[0] + TextString.length(),
                                                                                               RowToken,
                                                                                               true,
                                                                                               WhichRowToFind,
                                                                                               IteratorToStartingPositionOfNthRow,
                                                                                               IteratorToEndingPositionOfNthRow);

    StartingPositionOfNthRow = blIntegerType(std::distance(&TextString[0],IteratorToStartingPositionOfNthRow));
    EndingPositionOfNthRow = blIntegerType(std::distance(&TextString[0],IteratorToEndingPositionOfNthRow));

    return ActualRowFound;
}


template<typename blIntegerType>
inline blIntegerType blFindPositionOfNthRowAndMthColumnInString(const std::string& TextString,
                                                                const char& RowToken,
                                                                const blIntegerType& WhichRowToFind,
                                                                const blIntegerType& WhichColToFind,
                                                                blIntegerType& ActualRowFound,
                                                                blIntegerType& ActualColFound)
{
    // First we find the
    // iterator to the
    // desired position

    const char* PosIter = NULL;

    blFindPositionOfNthDataRowAndMthDataColumnLookingForToken(&TextString[0],
                                                              &TextString[0] + TextString.length(),
                                                              RowToken,
                                                              true,
                                                              WhichRowToFind,
                                                              WhichColToFind,
                                                              PosIter,
                                                              ActualRowFound,
                                                              ActualColFound);

    // Return an integer
    // to the string
    // position corresponding
    // to the actual row
    // and column
    return ( std::distance(&TextString[0],PosIter) );
}


template<typename blIntegerType>
inline blIntegerType blFindPositionOfNthRowAndMthColumnInString(const std::string& TextString,
                                                                const char& RowToken,
                                                                const blIntegerType& WhichRowToFind,
                                                                const blIntegerType& WhichColToFind)
{
    // We first try to
    // find the starting
    // and ending positions
    // of the specified
    // Nth data row

    blIntegerType ActualRowFound;
    blIntegerType ActualColFound;

    return ( blFindPositionOfNthRowAndMthColumnInString(TextString,
                                                        RowToken,
                                                        WhichRowToFind,
                                                        WhichColToFind,
                                                        ActualRowFound,
                                                        ActualColFound) );
}


template<typename blIntegerType>
inline void blGetRowAndColumnOfStringPosition(const std::string& TextString,
                                              const char& RowToken,
                                              const blIntegerType& TextStringPosition,
                                              blIntegerType& RowNumber,
                                              blIntegerType& ColNumber)
{
    blGetRowAndColumnNumberOfDataPositionUsingToken(&TextString[0],
                                                    &TextString[0] + TextString.length(),
                                                    RowToken,
                                                    true,
                                                    &TextString[TextStringPosition],
                                                    RowNumber,
                                                    ColNumber);

    return;
}
//-------------------------------------------------------------------


#endif // BL_BUFFERALGORITHMS5_HPP
