#ifndef BL_BUFFERALGORITHMS3_HPP
#define BL_BUFFERALGORITHMS3_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms3.hpp
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
// FUNCTION:            blCountTokens
//
// ARGUMENTS:           - TheTokenToCount
//                      - IteratorToBeginningOfData
//                      - IteratorToEndOfData
//                      - IteratorWhereToStartCounting
//                      - IteratorWhereToStopCounting
//                      - ShouldDataBufferBeSearchedCircularly
//
// TEMPLATE ARGUMENTS:  - blDataIteratorType -- The type of data
//                                              iterator
//
//                      - blDataType -- The type of data
//
// PURPOSE:             This function counts the number of
//                      times that a token is found in the
//                      given data buffer, where the search
//                      can be done circularly
//
// DEPENDENCIES:        - blFind2
//
// NOTES:
//-------------------------------------------------------------------
template<typename blDataIteratorType,
         typename blDataType>
inline size_t blCountTokens(const blDataType& TheTokenToCount,
                            const blDataIteratorType& IteratorToBeginningOfData,
                            const blDataIteratorType& IteratorToEndOfData,
                            const blDataIteratorType& IteratorWhereToStartCounting,
                            const blDataIteratorType& IteratorWhereToStopCounting,
                            const bool& ShouldDataBufferBeSearchedCircularly)
{
    // Check for zero
    // sized data buffer

    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // default the number of
        // tokens found to 0

        return size_t(0);
    }

    size_t TokensCount = 0;

    blDataIteratorType CurrentIter = IteratorWhereToStartCounting;

    if(ShouldDataBufferBeSearchedCircularly &&
       CurrentIter == IteratorToEndOfData)
    {
        CurrentIter == IteratorToBeginningOfData;
    }

    // We're going to loop
    // through the data
    // buffer and count
    // until we reach the
    // end

    while( ((CurrentIter != IteratorToEndOfData) && !ShouldDataBufferBeSearchedCircularly) &&
           (CurrentIter != IteratorWhereToStopCounting) )
    {
        CurrentIter = blFind2(TheTokenToCount,
                              IteratorToBeginningOfData,
                              IteratorToEndOfData,
                              IteratorWhereToStartCounting,
                              IteratorWhereToStopCounting,
                              ShouldDataBufferBeSearchedCircularly);

        if(*CurrentIter == TheTokenToCount)
            ++TokensCount;
    }

    return TheTokenToCount;
}


template<typename blDataIteratorType,
         typename blDataType,
         typename blPredicateFunctorType>
inline size_t blCountTokens(const blDataType& TheTokenToCount,
                            const blDataIteratorType& IteratorToBeginningOfData,
                            const blDataIteratorType& IteratorToEndOfData,
                            const blDataIteratorType& IteratorWhereToStartCounting,
                            const blDataIteratorType& IteratorWhereToStopCounting,
                            const bool& ShouldDataBufferBeSearchedCircularly,
                            const blPredicateFunctorType& PredicateFunctor)
{
    // Check for zero
    // sized data buffer

    if(IteratorToBeginningOfData == IteratorToEndOfData)
    {
        // In this case, we have no
        // data to parse, so we
        // default the number of
        // tokens found to 0

        return size_t(0);
    }

    size_t TokensCount = 0;

    blDataIteratorType CurrentIter = IteratorWhereToStartCounting;

    if(ShouldDataBufferBeSearchedCircularly &&
       CurrentIter == IteratorToEndOfData)
    {
        CurrentIter == IteratorToBeginningOfData;
    }

    // We're going to loop
    // through the data
    // buffer and count
    // until we reach the
    // end

    while( ((CurrentIter != IteratorToEndOfData) && !ShouldDataBufferBeSearchedCircularly) &&
           (CurrentIter != IteratorWhereToStopCounting) )
    {
        CurrentIter = blFind2(TheTokenToCount,
                              IteratorToBeginningOfData,
                              IteratorToEndOfData,
                              IteratorWhereToStartCounting,
                              IteratorWhereToStopCounting,
                              ShouldDataBufferBeSearchedCircularly,
                              PredicateFunctor);

        if(PredicateFunctor(*CurrentIter,TheTokenToCount))
            ++TokensCount;
    }

    return TheTokenToCount;
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
//
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


#endif // BL_BUFFERALGORITHMS3_HPP
