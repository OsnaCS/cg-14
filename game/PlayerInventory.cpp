#include "PlayerInventory.hpp"

using namespace std;



PlayerInventory::PlayerInventory(int maxDisplayItems)
    : MAX_DISPLAY_ITEMS(maxDisplayItems)
{


}


bool PlayerInventory::addItem(BlockType item)
{
    if ( item == BlockType::Air || item == BlockType::Water )
        return false;

    auto it = m_items.find(item);
    if ( it != m_items.end() ) {
        it->second +=1;
    }else {
        m_items.insert( InventoryPair(item,1) );
    }
    return true;
}


bool PlayerInventory::removeItem(BlockType item)
{
    auto it = m_items.find(item);
    if ( it != m_items.end() ) {
        it->second -=1;
        if ( it->second <= 0 ) {
            m_items.erase( it );
        }
        return true;
    }
    return false;

}

///////////////////////////////////END OF FILE/////////////////////////////////////
