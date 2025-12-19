#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "../models/LevelConfig.h"
#include "cocos2d.h"
#include "json/document.h"

/**
 * @class LevelConfigLoader
 * @brief 关卡配置加载器
 * @details 负责从JSON文件加载关卡配置数据
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 从JSON文件加载关卡配置
     * @param levelId 关卡ID
     * @param outConfig 输出的配置对象
     * @return 是否加载成功
     */
    static bool loadLevelConfig(int levelId, LevelConfig& outConfig);
    
    /**
     * @brief 从JSON文件路径加载关卡配置
     * @param filePath JSON文件路径
     * @param outConfig 输出的配置对象
     * @return 是否加载成功
     */
    static bool loadFromFile(const std::string& filePath, LevelConfig& outConfig);
    
private:
    /**
     * @brief 解析JSON文档
     * @param doc JSON文档对象
     * @param outConfig 输出的配置对象
     * @return 是否解析成功
     */
    static bool parseJsonDocument(const rapidjson::Document& doc, LevelConfig& outConfig);
    
    /**
     * @brief 解析单张卡牌数据
     * @param cardObj JSON卡牌对象
     * @param outCardData 输出的卡牌数据
     * @return 是否解析成功
     */
    static bool parseCardData(const rapidjson::Value& cardObj, CardConfigData& outCardData);
};

#endif // __LEVEL_CONFIG_LOADER_H__
