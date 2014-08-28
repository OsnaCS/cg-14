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
     * @brief PlayerInventory Constructor.
     * @param maxDisplayItems maximum number of displayed items to be drawn.
     */
    PlayerInventory(int maxDisplayItems=20);

    /**
     * @brief addItem Add @sa BlockType item to internal container
     * @param item Any item except @sa BlockType::Air or @sa BlockType::Sand
     * @return true if this item is now stored in the container, false otherwise.
     */
    bool addItem(BlockType item);

    /**
     * @brief removeItem Remove @sa BlockType item from the internal container
     * @param item Item to be removed from the container
     * @return true if the given item is removed from the container, false if no given item found
     *              in the container.
     */
    bool removeItem(BlockType item);

    /**
     * @brief maxDisplayItems Get maximum "type" of items that allowed to be stored in the container.
     * @return number of the maximum item
     */
    inline int maxDisplayItems() const;

    /**
     * @brief getItems Get container
     * @return container of inventory
     */
    inline std::map<BlockType, int>& getItems();

    /**
     * @brief getType
     * @param idx
     * @return
     */
    BlockType getType(int idx) const;

    void setInventory(std::map<BlockType, int> newInv);

    void reset()
    {
        m_items.clear();
    }


private:
    int MAX_DISPLAY_ITEMS;
    std::map<BlockType, int> m_items;

};


//////////////////////////INLINE

int PlayerInventory::maxDisplayItems() const
{
    return MAX_DISPLAY_ITEMS;
}


std::map<BlockType, int>& PlayerInventory::getItems()
{
    return m_items;
}



#endif // PLAYERINVENTORY_HPP
//////////////////////////////////////END OF FILE///////////////////////////////////////////
