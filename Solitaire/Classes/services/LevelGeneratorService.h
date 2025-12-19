#ifndef __LEVEL_GENERATOR_SERVICE_H__
#define __LEVEL_GENERATOR_SERVICE_H__

#include "../configs/models/LevelConfig.h"
#include "../models/GameModel.h"

/**
 * @class LevelGeneratorService
 * @brief 关卡生成服务类
 * @details 将静态配置（LevelConfig）转换为动态运行时数据（GameModel）
 *          不持有数据，所有方法都是静态的
 */
class LevelGeneratorService
{
public:
    /**
     * @brief 从关卡配置生成游戏数据模型
     * @param config 关卡配置
     * @param outGameModel 输出的游戏数据模型
     * @return 是否生成成功
     */
    static bool generateGameModel(const LevelConfig& config, GameModel& outGameModel);
    
private:
    /**
     * @brief 为每张卡牌生成唯一ID
     */
    static int _nextCardId;
    
    /**
     * @brief 从配置数据创建卡牌模型
     * @param configData 配置数据
     * @return 创建的卡牌模型
     */
    static CardModel createCardFromConfig(const CardConfigData& configData);
};

#endif // __LEVEL_GENERATOR_SERVICE_H__
