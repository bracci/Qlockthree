/**
 * Events.h
 * Instanzierung jährlicher Events
 *
 * @mc       Arduino/UNO
 * @autor    Manuel Bracher / manuel.bracher@gmail.com
 * @version  1.0
 * @created  02.01.15
 *
 * Versionshistorie:
 * V 1.0:  - Erstellt.
 */
#ifndef EVENTS_H
#define EVENTS_H

#include "Event.h"
#include "Colors.h"

#define DURATION_BM   15
#define TICKER_SPEED  6

#define DURATION_ANI_BM  (DURATION_BM/5)

class Events{
public:
Events();

static Event events[];
static byte nbrOfEvts;

static void show(byte eventID);

};

#endif // EVENTS_H
