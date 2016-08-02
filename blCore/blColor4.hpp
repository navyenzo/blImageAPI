#ifndef BL_COLOR4_HPP
#define BL_COLOR4_HPP


//-------------------------------------------------------------------
// FILE:            blColor4.hpp
// CLASS:           blColor4
// BASE CLASS:      blColor3
//
// PURPOSE:         Based on blColor3, adds an alpha transparency value
//
// AUTHOR:          Vincenzo Barbato
//                  http://www.barbatolabs.com
//                  navyenzo@gmail.com
//
// LISENSE:         MIT-LICENCE
//                  http://www.opensource.org/licenses/mit-license.php
//
// DEPENDENCIES:    blColor4
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
class blColor4 : public blColor3<blDataType>
{
public: // Constructors and destructors

    // default constructor

    blColor4(const blDataType& defaultValue = 0);

    // Constructor from four values

    blColor4(const blDataType& blue,
             const blDataType& green,
             const blDataType& red,
             const blDataType& alpha);

    // Copy constructor

    template<typename blDataType2>
    blColor4(const blColor4<blDataType2>& color);

    // Construction from a CvScalar

    blColor4(const CvScalar& opencvColor);

    // Construction from an sfml color
    // from the sfml library

    #ifdef SFML_COLOR_HPP

    blColor4(const sf::Color& color) : blColor3<blDataType>(color)
    {
        this->m_alpha = color.a;
    }

    #endif

    // Destructor

    ~blColor4()
    {
    }

public: // Assignment operators

    #ifdef SFML_COLOR_HPP

    blColor4&                       operator=(const sf::Color& color)
    {
        this->m_blue = color.b;
        this->m_green = color.g;
        this->m_red = color.r;
        this->m_alpha = color.a;

        return *this;
    }

    #endif

public: // Overloaded operators

    bool                                operator==(const blColor4<blDataType>& color)const;
    bool                                operator!=(const blColor4<blDataType>& color)const;

    const blColor4<blDataType>          operator-()const;
    blColor4<blDataType>&               operator+=(const blColor4<blDataType>& color);
    blColor4<blDataType>&               operator-=(const blColor4<blDataType>& color);
    blColor4<blDataType>&               operator*=(const blColor4<blDataType>& color);
    blColor4<blDataType>&               operator/=(const blColor4<blDataType>& color);
    blColor4<blDataType>&               operator%=(const blColor4<blDataType>& color);
    blColor4<blDataType>&               operator*=(const blDataType& Scalar);
    blColor4<blDataType>&               operator/=(const blDataType& Scalar);
    blColor4<blDataType>&               operator%=(const blDataType& Scalar);

    const blColor4<blDataType>          operator+(const blColor4<blDataType>& color)const;
    const blColor4<blDataType>          operator-(const blColor4<blDataType>& color)const;
    const blColor4<blDataType>          operator*(const blDataType& Scalar)const;
    const blColor4<blDataType>          operator/(const blDataType& Scalar)const;
    const blColor4<blDataType>          operator%(const blDataType& Scalar)const;

    const blColor4<blDataType>          operator*(const blColor4<blDataType>& color)const;
    const blColor4<blDataType>          operator/(const blColor4<blDataType>& color)const;
    const blColor4<blDataType>          operator%(const blColor4<blDataType>& color)const;

public: // Public functions

    // Functions used to
    // convert this color
    // to a CvScalar

    operator CvScalar()
    {
        CvScalar color;
        color.val[0] = this->m_blue;
        color.val[1] = this->m_green;
        color.val[2] = this->m_red;
        color.val[3] = m_alpha;
        return color;
    }
    operator const CvScalar()const
    {
        CvScalar color;
        color.val[0] = this->m_blue;
        color.val[1] = this->m_green;
        color.val[2] = this->m_red;
        color.val[3] = m_alpha;
        return color;
    }

    #ifdef SFML_COLOR_HPP

    // Functions used to convert this
    // color to an sfml color (from
    // the sfml library)

    operator sf::Color()
    {
        return sf::Color(this->m_red,this->m_green,this->m_blue,m_alpha);
    }
    operator sf::Color()const
    {
        return sf::Color(this->m_red,this->m_green,this->m_blue,m_alpha);
    }

    #endif

public: // Public variables

    // The alpha component

    blDataType                          m_alpha;
};
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>::blColor4(const blDataType& defaultValue)
                                      : blColor3<blDataType>(defaultValue)
{
    m_alpha = 1;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>::blColor4(const blDataType& blue,
                                      const blDataType& green,
                                      const blDataType& red,
                                      const blDataType& alpha)
                                      : blColor3<blDataType>(blue,green,red)
{
    m_alpha = alpha;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
template<typename blDataType2>
inline blColor4<blDataType>::blColor4(const blColor4<blDataType2>& color)
                                      : blColor3<blDataType>(color)
{
    m_alpha = color.m_alpha;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>::blColor4(const CvScalar& opencvColor)
                                      : blColor3<blDataType>(opencvColor)
{
    m_alpha = opencvColor.val[3];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blColor4<blDataType>::operator==(const blColor4<blDataType>& color)const
{
    if(color.m_blue == this->m_blue &&
       color.m_green == this->m_green &&
       color.m_red == this->m_red &&
       color.m_alpha == m_alpha)
        return true;
    else
        return false;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline bool blColor4<blDataType>::operator!=(const blColor4<blDataType>& color)const
{
    if(color.m_blue == this->m_blue &&
       color.m_green == this->m_green &&
       color.m_red == this->m_red &&
       color.m_alpha == m_alpha)
        return false;
    else
        return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator-()const
{
    return blColor4<blDataType>(-this->m_blue,
                            -this->m_green,
                            -this->m_red,
                            -this->m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator+=(const blColor4<blDataType>& color)
{
    this->m_blue += color.m_blue;
    this->m_green += color.m_green;
    this->m_red += color.m_red;
    m_alpha += color.m_alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator-=(const blColor4<blDataType>& color)
{
    this->m_blue -= color.m_blue;
    this->m_green -= color.m_green;
    this->m_red -= color.m_red;
    m_alpha -= color.m_alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator*=(const blColor4<blDataType>& color)
{
    this->m_blue *= color.m_blue;
    this->m_green *= color.m_green;
    this->m_red *= color.m_red;
    m_alpha *= color.m_alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator/=(const blColor4<blDataType>& color)
{
    this->m_blue /= color.m_blue;
    this->m_green /= color.m_green;
    this->m_red /= color.m_red;
    m_alpha /= color.m_alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator%=(const blColor4<blDataType>& color)
{
    this->m_blue %= color.m_blue;
    this->m_green %= color.m_green;
    this->m_red %= color.m_red;
    m_alpha %= color.m_alpha;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator*=(const blDataType& Scalar)
{
    this->m_blue *= Scalar;
    this->m_green *= Scalar;
    this->m_red *= Scalar;
    m_alpha *= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator/=(const blDataType& Scalar)
{
    this->m_blue /= Scalar;
    this->m_green /= Scalar;
    this->m_red /= Scalar;
    m_alpha /= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType>& blColor4<blDataType>::operator%=(const blDataType& Scalar)
{
    this->m_blue %= Scalar;
    this->m_green %= Scalar;
    this->m_red %= Scalar;
    m_alpha %= Scalar;

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator+(const blColor4<blDataType>& color)const
{
    return blColor4<blDataType>(this->m_blue + color.m_blue,
                                this->m_green + color.m_green,
                                this->m_red + color.m_red,
                                m_alpha + color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator-(const blColor4<blDataType>& color)const
{
    return blColor4<blDataType>(this->m_blue - color.m_blue,
                                this->m_green - color.m_green,
                                this->m_red - color.m_red,
                                m_alpha - color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator*(const blColor4<blDataType>& color)const
{
    return blColor4<blDataType>(this->m_blue * color.m_blue,
                                this->m_green * color.m_green,
                                this->m_red * color.m_red,
                                m_alpha * color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator/(const blColor4<blDataType>& color)const
{
    return blColor4<blDataType>(this->m_blue / color.m_blue,
                                this->m_green / color.m_green,
                                this->m_red / color.m_red,
                                m_alpha / color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator%(const blColor4<blDataType>& color)const
{
    return blColor4<blDataType>(this->m_blue % color.m_blue,
                                this->m_green % color.m_green,
                                this->m_red % color.m_red,
                                m_alpha % color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator*(const blDataType& Scalar)const
{
    return blColor4<blDataType>(this->m_blue * Scalar,
                                this->m_green * Scalar,
                                this->m_red * Scalar,
                                m_alpha * Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator/(const blDataType& Scalar)const
{
    return blColor4<blDataType>(this->m_blue / Scalar,
                                this->m_green / Scalar,
                                this->m_red / Scalar,
                                m_alpha / Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline const blColor4<blDataType> blColor4<blDataType>::operator%(const blDataType& Scalar)const
{
    return blColor4<blDataType>(this->m_blue % Scalar,
                                this->m_green % Scalar,
                                this->m_red % Scalar,
                                m_alpha % Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType> operator*(const blDataType& Scalar,
                                  const blColor4<blDataType>& color)
{
    return blColor4<blDataType>(color.m_blue * Scalar,
                                color.m_green * Scalar,
                                color.m_red * Scalar,
                                color.m_alpha * Scalar);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType> operator/(const blDataType& Scalar,
                                      const blColor4<blDataType>& color)
{
    return blColor4<blDataType>(Scalar / color.m_blue,
                                Scalar / color.m_green,
                                Scalar / color.m_red,
                                Scalar / color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline blColor4<blDataType> operator%(const blDataType& Scalar,
                                      const blColor4<blDataType>& color)
{
    return blColor4<blDataType>(Scalar % color.m_blue,
                                Scalar % color.m_green,
                                Scalar % color.m_red,
                                Scalar % color.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::ostream& operator<<(std::ostream& os,const blColor4<blDataType>& color)
{
    os << color.m_blue << " ";
    os << color.m_green << " ";
    os << color.m_red << " ";
    os << color.m_alpha;
    return os;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline std::istream& operator>>(std::istream& is,blColor4<blDataType>& color)
{
    is >> color.m_blue;
    is >> color.m_green;
    is >> color.m_red;
    is >> color.m_alpha;

    return is;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void min(const blColor4< blDataType >& value1,
                const blColor4< blDataType >& value2,
                blColor4< blDataType >& minValue)
{
    minValue.m_blue = std::min(value1.m_blue,value2.m_blue);
    minValue.m_green = std::min(value1.m_green,value2.m_green);
    minValue.m_red = std::min(value1.m_red,value2.m_red);
    minValue.m_alpha = std::min(value1.m_alpha,value2.m_alpha);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType>
inline void max(const blColor4< blDataType >& value1,
                const blColor4< blDataType >& value2,
                blColor4< blDataType >& maxValue)
{
    maxValue.m_blue = std::max(value1.m_blue,value2.m_blue);
    maxValue.m_green = std::max(value1.m_green,value2.m_green);
    maxValue.m_red = std::max(value1.m_red,value2.m_red);
    maxValue.m_alpha = std::max(value1.m_alpha,value2.m_alpha);
}
//-------------------------------------------------------------------


#endif // BL_COLOR4_HPP
