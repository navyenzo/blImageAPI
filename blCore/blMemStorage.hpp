#ifndef BL_MEMSTORAGE_HPP
#define BL_MEMSTORAGE_HPP


//-------------------------------------------------------------------
// FILE:            blMemStorage.hpp
// CLASS:           blMemStorage
// BASE CLASS:      None
//
// PURPOSE:         A base class used to wrap OpenCV's CvMemStorage
//                  with std::shared_ptr
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blColor3 -- To represent color
//                  CvMemStorage -- The memory storage
//                  std::shared_ptr -- To allow for automatic
//                                       garbage collection of
//                                       memory storage
//
//                  cvcreate -- To create the memory storage
//
// NOTES:
//
// DATE CREATED:    Nov/05/2010
// DATE UPDATED:
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Includes and libs needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
// Enums needed for this file
//-------------------------------------------------------------------
//-------------------------------------------------------------------


//-------------------------------------------------------------------
class blMemStorage
{
public: // Constructors and destructors

    // Default constructor
    blMemStorage(const int& BlockSize = 0);

    // Copy constructor
    blMemStorage(const blMemStorage& MemStorage);

    // Constructor from CvMemStorage
    blMemStorage(CvMemStorage* MemStorage);

    // Destructor
    ~blMemStorage()
    {
    }

public: // Public functions

    // Function used to get
    // the memory storage

    const std::shared_ptr<CvMemStorage>&        getMemStorage()const;

    // Function used to create
    // the memory storage

    void                                        create(const int& blockSize = 0);

    // Function used to get the
    // size of the memory storage

    int                                         size()const;

    // Function used to get the free
    // space for this memory storage

    int                                         getFreeSpace()const;

    // Function used to convert
    // this const object to
    // a pure OpenCV CvCvMemStorage
    // storage

    operator CvMemStorage*()
    {
        if(m_memStorage.use_count() > 0)
            return m_memStorage.get();
        else
            return NULL;
    }

    // Function used to convert
    // this const object to
    // a pure OpenCV CvCvMemStorage
    // storage

    operator const CvMemStorage*()const
    {
        if(m_memStorage.use_count() > 0)
            return m_memStorage.get();
        else
            return NULL;
    }

protected: // Protected variables

    // Memory storage

    std::shared_ptr<CvMemStorage>               m_memStorage;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(const int& BlockSize)
{
    // Create the memory storage
    create(BlockSize);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(const blMemStorage& memStorage)
{
    // In this case we simply
    // link to the memory storage

    m_memStorage = memStorage.getMemStorage();
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline blMemStorage::blMemStorage(CvMemStorage* memStorage)
{
    // If the mem storage is not NULL
    // then store it inside this object

    if(memStorage != NULL)
        m_memStorage = std::shared_ptr<CvMemStorage>(memStorage,releaseMemStorage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline const std::shared_ptr<CvMemStorage>& blMemStorage::getMemStorage()const
{
    return m_memStorage;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline void blMemStorage::create(const int& blockSize)
{
    m_memStorage = std::shared_ptr<CvMemStorage>(cvCreateMemStorage(blockSize),releaseMemStorage());
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blMemStorage::size()const
{
    return m_memStorage->block_size;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
inline int blMemStorage::getFreeSpace()const
{
    return m_memStorage->free_space;
}
//-------------------------------------------------------------------


#endif // BL_MEMSTORAGE_HPP
