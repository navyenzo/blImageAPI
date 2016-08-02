#ifndef BL_COLOR3_HPP
#define BL_COLOR3_HPP


//-------------------------------------------------------------------
// FILE:            blColor3.hpp
// CLASS:           blColor3
// BASE CLASS:      None
//
// PURPOSE:         A simple and efficient color structure of three
//                  components saved in a blue,green,red sequence
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    CvScalar -- From opencv library
//
// NOTES:
//
// DATE CREATED:    May/31/2010
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
template<typename blDataType>
class blColor3
{
public: // Constructors and destructors

    // default constructor

    blColor3(const blDataType& defaultValue = 0);

    // Constructor from three values

    blColor3(const blDataType& blue,
             const blDataType& green,
             const blDataType& red);

    // Copy constructor

    template<typename blDataType2>
    blColor3(const blColor3<blDataType2>& color);

    // Construction from a CvScalar

    blColor3(const CvScalar& opencvColor);

    // Construction from an sfml color
    // from the sfml library

    #ifdef SFML_COLOR_HPP

    blColor3(const sf::Color& color)
    {
        m_blue = color.b;
        m_green = color.g;
        m_red = color.r;
    }

    #endif

    // Destructor

    ~blColor3()
    {
    }

public: // Assignment operators

    #ifdef SFML_COLOR_HPP

    blColor3&                       operator=(const sf::Color& color)
    {
        m_blue = color.b;
        m_green = color.g;
        m_red = color.r;

        return *this;
    }

    #endif

public: // Overloaded operators

    bool                            operator==(const blColor3<blDataType>& color)const;
    bool                            operator!=(const blColor3<blDataType>& color)const;

    const blColor3<blDataType>      operator-()const;
    blColor3<blDataType>&           operator+=(const blColor3<blDataType>& color);
    blColor3<blDataType>&           operator-=(const blColor3<blDataType>& color);
    blColor3<blDataType>&           operator*=(const blColor3<blDataType>& color);
    blColor3<blDataType>&           operator/=(const blColor3<blDataType>& color);
    blColor3<blDataType>&           operator%=(const blColor3<blDataType>& color);
    blColor3<blDataType>&           operator*=(const blDataType& scalar);
    blColor3<blDataType>&           operator/=(const blDataType& scalar);
    blColor3<blDataType>&           operator%=(const blDataType& scalar);

    const blColor3<blDataType>      operator+(const blColor3<blDataType>& color)const;
    const blColor3<blDataType>      operator-(const blColor3<blDataType>& color)const;
    const blColor3<blDataType>      operator*(const blDataType& scalar)const;
    const blColor3<blDataType>      operator/(const blDataType& scalar)const;
    const blColor3<blDataType>      operator%(const blDataType& scalar)const;

    const blColor3<blDataType>      operator*(const blColor3<blDataType>& color)const;
    const blColor3<blDataType>      operator/(const blColor3<blDataType>& color)const;
    const blColor3<blDataType>      operator%(const blColor3<blDataType>& color)const;

public: // Public functions

    // Functions used to convert
    // this color to a CvScalar

    operator CvScalar()
    {
        return CV_RGB(m_red,m_green,m_blue);
    }
    operator const CvScalar()const
    {
        return CV_RGB(m_red,m_green,m_blue);
    }

    #ifdef SFML_COLOR_HPP

    // Functions used to convert this
    // color to an sfml color (from
    // the sfml library)

    operator sf::Color()
    {
        return sf::Color(m_red,m_green,m_blue,255);
    }
    operator sf::Color()const
    {
        return sf::Color(m_red,m_green,m_blue,255);
    }

    #endif

    // Functions used to convert
    // this color to a single value
    // NOTE:  The function uses the
    //        formula (0.30*R + 0.59*G + 0.11*B)

    operator blDataType()
    {
        return blDataType(0.11*m_blue + 0.59*m_green + 0.30*m_red);
    }
    operator const blDataType()const
    {
        return blDataType(0.11*m_blue + 0.59*m_green + 0.30*m_red);
    }

public: // Public variables

    // The color components

    blDataType                      m_blue;
    blDataType                      m_green;
    blDataType                      m_red;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>::blColor3(const blDataType& defaultValue)
{
    m_blue = defaultValue;
    m_green = defaultValue;
    m_red = defaultValue;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>::blColor3(const blDataType& blue,
                                      const blDataType& green,
                                      const blDataType& red)
{
    m_blue = blue;
    m_green = green;
    m_red = red;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline blColor3<blDataType>::blColor3(const blColor3<blDataType2>& color)
{
    m_blue = blDataType(color.m_blue);
    m_green = blDataType(color.m_green);
    m_red = blDataType(color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>::blColor3(const CvScalar& opencvColor)
{
    m_blue = opencvColor.val[0];
    m_green = opencvColor.val[1];
    m_red = opencvColor.val[2];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blColor3<blDataType>::operator==(const blColor3<blDataType>& color)const
{
    if(color.m_blue == m_blue &&
       color.m_green == m_green &&
       color.m_red == m_red)
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blColor3<blDataType>::operator!=(const blColor3<blDataType>& color)const
{
    if(color.m_blue == m_blue &&
       color.m_green == m_green &&
       color.m_red == m_red)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator-()const
{
    return blColor3<blDataType>(-m_blue,-m_green,-m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator+=(const blColor3<blDataType>& color)
{
    m_blue += color.m_blue;
    m_green += color.m_green;
    m_red += color.m_red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator-=(const blColor3<blDataType>& color)
{
    m_blue -= color.m_blue;
    m_green -= color.m_green;
    m_red -= color.m_red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator*=(const blColor3<blDataType>& color)
{
    m_blue *= color.m_blue;
    m_green *= color.m_green;
    m_red *= color.m_red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator/=(const blColor3<blDataType>& color)
{
    m_blue /= color.m_blue;
    m_green /= color.m_green;
    m_red /= color.m_red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator%=(const blColor3<blDataType>& color)
{
    m_blue %= color.m_blue;
    m_green %= color.m_green;
    m_red %= color.m_red;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator*=(const blDataType& scalar)
{
    m_blue *= scalar;
    m_green *= scalar;
    m_red *= scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator/=(const blDataType& scalar)
{
    m_blue /= scalar;
    m_green /= scalar;
    m_red /= scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType>& blColor3<blDataType>::operator%=(const blDataType& scalar)
{
    m_blue %= scalar;
    m_green %= scalar;
    m_red %= scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator+(const blColor3<blDataType>& color)const
{
    return blColor3<blDataType>(m_blue + color.m_blue,
                                m_green + color.m_green,
                                m_red + color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator-(const blColor3<blDataType>& color)const
{
    return blColor3<blDataType>(m_blue - color.m_blue,
                                m_green - color.m_green,
                                m_red - color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator*(const blColor3<blDataType>& color)const
{
    return blColor3<blDataType>(m_blue * color.m_blue,
                                m_green * color.m_green,
                                m_red * color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator/(const blColor3<blDataType>& color)const
{
    return blColor3<blDataType>(m_blue / color.m_blue,
                                m_green / color.m_green,
                                m_red / color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator%(const blColor3<blDataType>& color)const
{
    return blColor3<blDataType>(m_blue % color.m_blue,
                                m_green % color.m_green,
                                m_red % color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator*(const blDataType& scalar)const
{
    return blColor3<blDataType>(m_blue * scalar,
                                m_green * scalar,
                                m_red * scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator/(const blDataType& scalar)const
{
    return blColor3<blDataType>(m_blue / scalar,
                                m_green / scalar,
                                m_red / scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor3<blDataType> blColor3<blDataType>::operator%(const blDataType& scalar)const
{
    return blColor3<blDataType>(m_blue % scalar,
                                m_green % scalar,
                                m_red % scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType> operator*(const blDataType& scalar,
                                      const blColor3<blDataType>& color)
{
    return blColor3<blDataType>(color.m_blue * scalar,
                                color.m_green * scalar,
                                color.m_red * scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType> operator/(const blDataType& scalar,
                                      const blColor3<blDataType>& color)
{
    return blColor3<blDataType>(scalar / color.m_blue,
                                scalar / color.m_green,
                                scalar / color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor3<blDataType> operator%(const blDataType& scalar,
                                      const blColor3<blDataType>& color)
{
    return blColor3<blDataType>(scalar % color.m_blue,
                                scalar % color.m_green,
                                scalar % color.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::ostream& operator<<(std::ostream& os,const blColor3<blDataType>& color)
{
    os << color.m_blue << " ";
    os << color.m_green << " ";
    os << color.m_red;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::istream& operator>>(std::istream& is,blColor3<blDataType>& color)
{
    is >> color.m_blue;
    is >> color.m_green;
    is >> color.m_red;

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void min(const blColor3< blDataType >& value1,
                const blColor3< blDataType >& value2,
                blColor3< blDataType >& minValue)
{
    minValue.m_blue = std::min(value1.m_blue,value2.m_blue);
    minValue.m_green = std::min(value1.m_green,value2.m_green);
    minValue.m_red = std::min(value1.m_red,value2.m_red);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void max(const blColor3< blDataType >& value1,
                const blColor3< blDataType >& value2,
                blColor3< blDataType >& maxValue)
{
    maxValue.m_blue = std::max(value1.m_blue,value2.m_blue);
    maxValue.m_green = std::max(value1.m_green,value2.m_green);
    maxValue.m_red = std::max(value1.m_red,value2.m_red);
}
//-------------------------------------------------------------------


#endif // BL_COLOR3_HPP
