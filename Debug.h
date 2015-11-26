/**
 * Debug
 * Klasse fuers elegantere Debuggen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @created  21.1.2013
 * @updated  8.2.2015
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 * V 1.1:  - Zwei Argumente zugelassen.
 */
#ifdef DEBUG
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINT2(x, y) Serial.print(x, y)
    #define DEBUG_PRINTLN(x) Serial.println(x)
    #define DEBUG_PRINTLN2(x, y) Serial.println(x, y)
    #define DEBUG_FLUSH() Serial.flush()
#else
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINT2(x, y)
    #define DEBUG_PRINTLN(x)
    #define DEBUG_PRINTLN2(x, y)
    #define DEBUG_FLUSH()
#endif
