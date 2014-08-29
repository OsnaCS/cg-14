#include "PlayerInventory.hpp"

using namespace std;



PlayerInventory::PlayerInventory(int maxDisplayItems)
    : MAX_DISPLAY_ITEMS(maxDisplayItems)
{
    for(int i=0; i <99; i++){
        addItem(BlockType::Torch);
    }
}


bool PlayerInventory::addItem(BlockType item)
{
    if ( item == BlockType::Air )
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


BlockType PlayerInventory::getType(int idx) const
{
    if ( idx >= m_items.size() )
        return BlockType::Air;

    auto it = m_items.begin();
    for(  int i=0; i< m_items.size(); ++it, ++i ) {
        if( i==idx ) {
            return it->first;
        }
    }
    return BlockType::Air;
}

void PlayerInventory::setInventory(std::map<BlockType, int> newInv)
{

    m_items = newInv;

}
///////////////////////////////////END OF FILE/////////////////////////////////////
