/*
 * Waiter.h
 *
 *  Created on: 20/10/2012
 *      Author: SkullKill
 */

#ifndef WAITER_H
#define WAITER_H

typedef uint32_t clock_t;
const clock_t INFINITE = ( (clock_t)(-1) );
const uint16_t SEC = 1000;
const uint16_t MIN = 60000;
const clock_t HOUR = 3600000;

/*
* waiter: returns true and resets if more than time has passed
*/
class Waiter {
private:
    clock_t last;
    bool _reset;
public:

    Waiter() : last(millis()), _reset(0) {}

    /*
     * reset timer to start now. can only be done once, then need to wait for wait function to be true once.
     * if force reset required, use function Waiter.resetreset()
     * this was done so that the reset function can be used in the loop of the program and will only be done once.
     */
    void reset()
    {
    	if (!_reset)
    	{
    		_reset = 1;
    		last = millis();
            //Serial.print("reset last= ");
            //Serial.println(last);
    	}
    };

    /*
    * wait for time, then reset, also reset reset flag. can be used without reset flag
    * if infinite toggle is required.
    */
    bool wait( const clock_t time ) {
        clock_t now = millis();
/*
        Serial.print("now= ");
        Serial.println(now);
        Serial.print("last= ");
        Serial.println(last);
        Serial.print("time= ");
        Serial.println(time);
        Serial.print("diff =");
        Serial.println((now - last));
        Serial.println(millis());
*/
        if( now - last >= time ) {
            last = now;
            _reset = 0;
            //Serial.println("true");
            return true;
        }
        //Serial.println("false");

        return false;
    };

    /*
     * force reset
     * if reset is required before the the wait has finish it's cycle.
     */
    void resetreset()
    {
    	_reset = 0;
    }
};


#endif /* WAITER_H */
