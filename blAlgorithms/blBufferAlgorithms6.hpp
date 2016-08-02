#ifndef BL_BUFFERALGORITHMS6_HPP
#define BL_BUFFERALGORITHMS6_HPP


//-------------------------------------------------------------------
// FILE:            blBufferAlgorithms6.hpp
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
        return;
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
        return;
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


#endif // BL_BUFFERALGORITHMS6_HPP
