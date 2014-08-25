#ifndef PLAYERATTRIBUTES_HPP
#define PLAYERATTRIBUTES_HPP

#include <chrono>

typedef std::chrono::system_clock::time_point time_point;


/**
 * @brief The PlayerAttributes class Properties of the players e.g. health and endurance.
 */
class PlayerAttributes
{
public:

    /**
     * @brief PlayerAttributes Constructor
     * @param hearts Number of heart used in intilaization and will be set as maximum number.
     *        Default is 10.
     */
    PlayerAttributes(int hearts=10);

    /**
     * @brief getHearts Get current up-to-date status(number) of hearts
     * @return number of hearts
     */
    int getHearts() ;

    /**
     * @brief maxHearts Get maximum number  of hearts from intially setting
     * @return Maximum number of the heart that is initially set.
     */
    inline int maxHearts() const;

    /**
     * @brief setHearts Force reset status/number of the hearts
     * @param hearts number of hearts to be reset. Typically it is between zero and maximum number of hearts
     *        when we first initialized the class.
     */
    inline void setHearts(int hearts);

    /**
     * @brief updateHeartsBy Update the current heart by relative number
     * @param hearts increamental or decreamental number as positive or negative number.
     */
    inline void updateHeartsBy(int hearts);

private:

    const int MAX_HEARTS;
    int m_hearts;
    time_point m_timer;

};



int PlayerAttributes::maxHearts() const
{
    return MAX_HEARTS;
}

void PlayerAttributes::setHearts(int hearts)
{
    m_hearts = (hearts>MAX_HEARTS)?MAX_HEARTS:(hearts<0)? 0:hearts;
}


void PlayerAttributes::updateHeartsBy(int hearts)
{
    m_hearts += hearts;
    m_hearts = (m_hearts>MAX_HEARTS)?MAX_HEARTS:(m_hearts<0)? 0:m_hearts;
}



#endif // PLAYERATTRIBUTES_HPP
///////////////////////////////////////END OF FILE////////////////////////////////////
