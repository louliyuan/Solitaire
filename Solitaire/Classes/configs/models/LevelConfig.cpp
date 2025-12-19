#include "LevelConfig.h"

LevelConfig::LevelConfig()
    : _levelId(0)
{
}

LevelConfig::~LevelConfig()
{
}

void LevelConfig::addPlayfieldCard(const CardConfigData& card)
{
    _playfieldCards.push_back(card);
}

void LevelConfig::addStackCard(const CardConfigData& card)
{
    _stackCards.push_back(card);
}

void LevelConfig::clear()
{
    _levelId = 0;
    _playfieldCards.clear();
    _stackCards.clear();
}
