#ifndef THEW_H
#define THEW_H

#include <irrXml.h>

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class TheWeather : public sf::Drawable, public sf::Transformable
{
public:
    TheWeather(sf::Font& font, const std::string& woid = "2344116");
    ~TheWeather();
    void refresh();
    std::string setWoid(const std::string& woid);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    std::string loadHttp(const std::string& strHttp, const std::string& strReq);
    bool parseXml();
    bool loadXml();
    void loadImage(const std::string& localFileName);
    void layout();

    sf::RenderTexture   m_texture;
    sf::Sprite          m_sprite;

    sf::Text            m_TexTitle,
                        m_TexCity,
                        m_TexText,
                        m_TexTemp,
                        m_TexDate,
                        m_aTexDay[5],
                        m_aTexDate[5],
                        m_aTexLow[5],
                        m_aTexHigh[5],
                        m_aTexText[5];

    std::string         m_woid;

    sf::Texture         m_Texture, m_aTexture[5];
    sf::Sprite          m_Image, m_aImage[5];

    irr::io::EXML_NODE prevNodeType;
    std::string currNodeName;
    std::map<std::string, std::string> map;

};

#endif // THEW_H
