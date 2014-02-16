#include <thew.h>

TheWeather::TheWeather(sf::Font& font, const std::string& woid)
{
    m_texture.create(800, 600);
    m_sprite.setTexture(m_texture.getTexture());
    m_sprite.setOrigin(0, 600);
    m_sprite.setScale(1, -1);


    m_TexTitle.setFont(font);
    m_TexTitle.setCharacterSize(16);
    m_TexTitle.setColor(sf::Color::Blue);
    m_TexTitle.setStyle(sf::Text::Underlined);

    m_TexCity.setFont(font);
    m_TexCity.setCharacterSize(16);
    m_TexCity.setColor(sf::Color::Black);

    m_TexText.setFont(font);
    m_TexText.setCharacterSize(16);
    m_TexText.setColor(sf::Color::Black);

    m_TexTemp.setFont(font);
    m_TexTemp.setCharacterSize(16);
    m_TexTemp.setColor(sf::Color::Black);

    m_TexDate.setFont(font);
    m_TexDate.setCharacterSize(16);
    m_TexDate.setColor(sf::Color::Black);

    for(int i = 0; i < 5; ++i)
    {
        m_aTexDay[i].setFont(font);
        m_aTexDay[i].setCharacterSize(16);
        m_aTexDay[i].setColor(sf::Color::Black);
        m_aTexDay[i].setStyle(sf::Text::Bold);

        m_aTexDate[i].setFont(font);
        m_aTexDate[i].setCharacterSize(12);
        m_aTexDate[i].setColor(sf::Color::Black);

        m_aTexLow[i].setFont(font);
        m_aTexLow[i].setCharacterSize(12);
        m_aTexLow[i].setColor(sf::Color::Black);

        m_aTexHigh[i].setFont(font);
        m_aTexHigh[i].setCharacterSize(12);
        m_aTexHigh[i].setColor(sf::Color::Black);

        m_aTexText[i].setFont(font);
        m_aTexText[i].setCharacterSize(12);
        m_aTexText[i].setColor(sf::Color::Black);
    }

    m_woid = woid;
}

TheWeather::~TheWeather()
{
    map.clear();
}

void TheWeather::refresh()
{
    parseXml();
    loadImage(map["code"]);
    loadImage(map["code1"]);
    loadImage(map["code2"]);
    loadImage(map["code3"]);
    loadImage(map["code4"]);
    loadImage(map["code5"]);

    layout();
}

std::string TheWeather::loadHttp(const std::string& strHttp, const std::string& strReq)
{
    sf::Http http(strHttp);
    sf::Http::Request req(strReq);
    sf::Http::Response res = http.sendRequest(req);

    const std::string& retVal = res.getBody();
    return retVal;
}

bool TheWeather::loadXml()
{
    const std::string& bodyXml = loadHttp("http://weather.yahooapis.com", "/forecastrss?w=" + m_woid + "&u=c");

    std::ofstream file(m_woid + ".xml", std::ios::binary);
    file.write(bodyXml.c_str(), bodyXml.size());
    file.flush();
    file.close();

    return true;
}

bool TheWeather::parseXml()
{
    bool title = false;
    loadXml();
    const std::string& xmlFileName =  m_woid + ".xml";
    unsigned int forecast = 0;

    irr::io::IrrXMLReader* xml = irr::io::createIrrXMLReader(xmlFileName.c_str());
    while(xml && xml->read()){
        switch(xml->getNodeType()){
        case irr::io::EXN_ELEMENT:
            currNodeName = xml->getNodeName();

            if (currNodeName.compare("yweather:location") == 0) {
                map["city"] = xml->getAttributeValue("city");
                map["country"] = xml->getAttributeValue("country");
            }
            if (currNodeName.compare("yweather:condition") == 0) {
                map["text"] = xml->getAttributeValue("text");
                map["code"] = xml->getAttributeValue("code");
                map["temp"] = xml->getAttributeValue("temp");
                map["date"] = xml->getAttributeValue("date");
            }
            if (currNodeName.compare("yweather:forecast") == 0) {
                ++forecast;
                map["day"   + std::to_string(forecast)] = xml->getAttributeValue("day");
                map["date"  + std::to_string(forecast)] = xml->getAttributeValue("date");
                map["low"   + std::to_string(forecast)] = xml->getAttributeValue("low");
                map["high"  + std::to_string(forecast)] = xml->getAttributeValue("high");
                map["text"  + std::to_string(forecast)] = xml->getAttributeValue("text");
                map["code"  + std::to_string(forecast)] = xml->getAttributeValue("code");
            }
            break;
        case irr::io::EXN_TEXT:
            if (currNodeName.compare("lastBuildDate") == 0)
                map["lastBuildDate"] = xml->getNodeData();
            if (title && (currNodeName.compare("title") == 0))
                map["title"] = xml->getNodeData();
            break;
        case irr::io::EXN_COMMENT:
            break;
        case irr::io::EXN_CDATA:
            break;
        case irr::io::EXN_ELEMENT_END:
            currNodeName = xml->getNodeName();
            if (currNodeName.compare("image") == 0)
                title = true;

            currNodeName = "";
            break;
        case irr::io::EXN_UNKNOWN:
            break;
        case irr::io::EXN_NONE:
            break;
        }
        prevNodeType = xml->getNodeType();
    }
    delete xml;

    return true;
}

void TheWeather::loadImage(const std::string& localFileName)
{
    const std::string& remoteFileName = "/a/i/us/we/52/" + localFileName + ".gif";

    if (!m_Texture.loadFromFile(localFileName + ".gif")) {
        const std::string& bodyImage = loadHttp("http://l.yimg.com", remoteFileName);

        std::ofstream file(localFileName + ".gif", std::ios::binary);
        file.write(bodyImage.c_str(), bodyImage.size());
        file.flush();
        file.close();
    }
}

void TheWeather::layout()
{
    m_texture.clear(sf::Color(255, 255, 255));

    m_TexTitle.setString(map["title"]);
    m_TexCity.setString(map["city"] + "/" + map["country"]);
    m_TexText.setString(map["text"]);

    m_Texture.loadFromFile(map["code"] + ".gif");
    m_Image.setTexture(m_Texture);

    m_TexTemp.setString(map["temp"]);
    m_TexDate.setString(map["date"]);

    m_TexTitle.setPosition(0, 0);
    m_TexCity.setPosition(0,
                          m_TexTitle.getGlobalBounds().top +
                          m_TexTitle.getGlobalBounds().height + 4);
    m_Image.setPosition(0,
                        m_TexCity.getGlobalBounds().top +
                        m_TexCity.getGlobalBounds().height + 4);
    m_TexText.setPosition(0,
                      m_Image.getGlobalBounds().top +
                      m_Image.getGlobalBounds().height + 4);
    m_TexTemp.setPosition(0,
                      m_TexText.getGlobalBounds().top +
                      m_TexText.getGlobalBounds().height + 4);

    m_TexDate.setPosition(0,
                      m_TexTemp.getGlobalBounds().top +
                      m_TexTemp.getGlobalBounds().height + 4);

    m_texture.draw(m_TexTitle);
    m_texture.draw(m_TexCity);
    m_texture.draw(m_TexText);
    m_texture.draw(m_Image);
    m_texture.draw(m_TexTemp);
    m_texture.draw(m_TexDate);

    unsigned int y = m_TexDate.getGlobalBounds().top +
                        m_TexDate.getGlobalBounds().height + 4;

    for(int i = 0; i < 5; ++i)
    {
        std::cout << map["code" + std::to_string(i+1)] + ".gif" << std::endl;

        m_aTexture[i].loadFromFile(map["code" + std::to_string(i+1)] + ".gif");
        m_aImage[i].setTexture(m_aTexture[i]);

        m_aTexDay[i].setString (map["day"  + std::to_string(i+1)]);
        m_aTexDate[i].setString(map["date" + std::to_string(i+1)]);
        m_aTexLow[i].setString (map["low"  + std::to_string(i+1)]);
        m_aTexHigh[i].setString(map["high" + std::to_string(i+1)]);
        m_aTexText[i].setString(map["text" + std::to_string(i+1)]);

        m_aTexDay[i].setPosition(i * 96, y + 16);
        m_aImage[i].setPosition(i * 96,
                            m_aTexDay[i].getGlobalBounds().top +
                            m_aTexDay[i].getGlobalBounds().height + 4);

        m_aTexDate[i].setPosition(i * 96,
                          m_aImage[i].getGlobalBounds().top +
                          m_aImage[i].getGlobalBounds().height + 4);
        m_aTexLow[i].setPosition(i * 96,
                          m_aTexDate[i].getGlobalBounds().top +
                          m_aTexDate[i].getGlobalBounds().height + 4);
        m_aTexHigh[i].setPosition(i * 96,
                          m_aTexLow[i].getGlobalBounds().top +
                          m_aTexLow[i].getGlobalBounds().height + 4);
        m_aTexText[i].setPosition(i * 96,
                          m_aTexHigh[i].getGlobalBounds().top +
                          m_aTexHigh[i].getGlobalBounds().height + 4);

        m_texture.draw(m_aTexDay[i]);
        m_texture.draw(m_aImage[i]);
        m_texture.draw(m_aTexDate[i]);
        m_texture.draw(m_aTexLow[i]);
        m_texture.draw(m_aTexHigh[i]);
        m_texture.draw(m_aTexText[i]);

    }
}

void TheWeather::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;

    target.draw(m_sprite, states);
}

std::string TheWeather::setWoid(const std::string& woid)
{
    std::string oldWoid = m_woid;

    m_woid = woid;
    return oldWoid;
}
