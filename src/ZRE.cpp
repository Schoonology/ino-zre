/* zRE library by Michael Schoonmaker
 */

#include "zRE.h"

/**
 * Constructor.
 */
ZRE::ZRE()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void ZRE::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void ZRE::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void ZRE::doit()
{
    Serial.println("called doit");
}
