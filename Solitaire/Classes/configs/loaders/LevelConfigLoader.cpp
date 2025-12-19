#include "LevelConfigLoader.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

bool LevelConfigLoader::loadLevelConfig(int levelId, LevelConfig& outConfig)
{
    // 根据关卡ID构建文件路径
    std::string filePath = "configs/level" + std::to_string(levelId) + ".json";
    return loadFromFile(filePath, outConfig);
}

bool LevelConfigLoader::loadFromFile(const std::string& filePath, LevelConfig& outConfig)
{
    // 读取JSON文件内容
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(filePath);
    std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(fullPath);
    
    if (content.empty())
    {
        CCLOG("Failed to load level config file: %s", filePath.c_str());
        return false;
    }
    
    // 解析JSON
    rapidjson::Document doc;
    doc.Parse<0>(content.c_str());
    
    if (doc.HasParseError())
    {
        CCLOG("JSON parse error: %d", doc.GetParseError());
        return false;
    }
    
    return parseJsonDocument(doc, outConfig);
}

bool LevelConfigLoader::parseJsonDocument(const rapidjson::Document& doc, LevelConfig& outConfig)
{
    outConfig.clear();
    
    // 解析Playfield数组
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray())
    {
        const rapidjson::Value& playfieldArray = doc["Playfield"];
        for (rapidjson::SizeType i = 0; i < playfieldArray.Size(); i++)
        {
            CardConfigData cardData;
            if (parseCardData(playfieldArray[i], cardData))
            {
                outConfig.addPlayfieldCard(cardData);
            }
        }
    }
    
    // 解析Stack数组
    if (doc.HasMember("Stack") && doc["Stack"].IsArray())
    {
        const rapidjson::Value& stackArray = doc["Stack"];
        for (rapidjson::SizeType i = 0; i < stackArray.Size(); i++)
        {
            CardConfigData cardData;
            if (parseCardData(stackArray[i], cardData))
            {
                outConfig.addStackCard(cardData);
            }
        }
    }
    
    return true;
}

bool LevelConfigLoader::parseCardData(const rapidjson::Value& cardObj, CardConfigData& outCardData)
{
    if (!cardObj.IsObject())
    {
        return false;
    }
    
    // 解析CardFace
    if (cardObj.HasMember("CardFace") && cardObj["CardFace"].IsInt())
    {
        int faceValue = cardObj["CardFace"].GetInt();
        outCardData.face = static_cast<CardFace>(faceValue);
    }
    else
    {
        return false;
    }
    
    // 解析CardSuit
    if (cardObj.HasMember("CardSuit") && cardObj["CardSuit"].IsInt())
    {
        int suitValue = cardObj["CardSuit"].GetInt();
        outCardData.suit = static_cast<CardSuit>(suitValue);
    }
    else
    {
        return false;
    }
    
    // 解析Position
    if (cardObj.HasMember("Position") && cardObj["Position"].IsObject())
    {
        const rapidjson::Value& posObj = cardObj["Position"];
        
        float x = 0.0f;
        float y = 0.0f;
        
        if (posObj.HasMember("x") && (posObj["x"].IsInt() || posObj["x"].IsDouble()))
        {
            x = posObj["x"].IsInt() ? posObj["x"].GetInt() : posObj["x"].GetDouble();
        }
        
        if (posObj.HasMember("y") && (posObj["y"].IsInt() || posObj["y"].IsDouble()))
        {
            y = posObj["y"].IsInt() ? posObj["y"].GetInt() : posObj["y"].GetDouble();
        }
        
        outCardData.position = cocos2d::Vec2(x, y);
    }
    
    return true;
}
