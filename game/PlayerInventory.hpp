#ifndef PLAYERINVENTORY_HPP
#define PLAYERINVENTORY_HPP

#include "BlockType.hpp"
#include <map>

typedef std::pair<BlockType, int> InventoryPair;

/**
 * @brief The PlayerInventory class is the player's inventory. @sa Player
 */
class PlayerInventory
{
public:

    /**
     * @brief PlayerInventory
     * @param maxDisplayItems
     */
    PlayerInventory(int maxDisplayItems=20);

    /**
     * @brief addItem
     * @param item
     * @return
     */
    bool addItem(BlockType item);

    /**
     * @brief removeItem
     * @param item
     * @return
     */
    bool removeItem(BlockType item);

    /**
     * @brief maxDisplayItems
     * @return
     */
    inline int maxDisplayItems() const;

    /**
     * @brief getItems
     * @return
     */
    inline const std::map<BlockType, int>& getItems() const;


private:
    const int MAX_DISPLAY_ITEMS;
    std::map<BlockType, int> m_items;

};


//////////////////////////INLINE

int PlayerInventory::maxDisplayItems() const
{
    return MAX_DISPLAY_ITEMS;
}


const std::map<BlockType, int>& PlayerInventory::getItems() const
{
    return m_items;
}


#endif // PLAYERINVENTORY_HPP
//////////////////////////////////////END OF FILE///////////////////////////////////////////
