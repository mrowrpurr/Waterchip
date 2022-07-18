#pragma once

#define debug(text) debug_msg("[" + DEBUG_NAME + "] " + text)
#define debug_array(arr) debug(debug_array_str(arr))

#define debug1(text, a1)                     debug(sprintf(text +       " (%s)\n", a1))
#define debug2(text, a1, a2)                 debug(sprintf_array(text + " (%s) (%s)\n", [a1, a2]))
#define debug3(text, a1, a2, a3)             debug(sprintf_array(text + " (%s) (%s) (%s)\n", [a1, a2, a3]))
#define debug4(text, a1, a2, a3, a4)         debug(sprintf_array(text + " (%s) (%s) (%s) (%s)\n", [a1, a2, a3, a4]))
#define debug5(text, a1, a2, a3, a4, a5)     debug(sprintf_array(text + " (%s) (%s) (%s) (%s) (%s)\n", [a1, a2, a3, a4, a5]))
#define debug6(text, a1, a2, a3, a4, a5, a6) debug(sprintf_array(text + " (%s) (%s) (%s) (%s) (%s) (%s)\n", [a1, a2, a3, a4, a5, a6]))

#define debug1f(text, a1)                     debug(sprintf(text + "\n", a1))
#define debug2f(text, a1, a2)                 debug(sprintf_array(text + "\n", [a1, a2]))
#define debug3f(text, a1, a2, a3)             debug(sprintf_array(text + "\n", [a1, a2, a3]))
#define debug4f(text, a1, a2, a3, a4)         debug(sprintf_array(text + "\n", [a1, a2, a3, a4]))
#define debug5f(text, a1, a2, a3, a4, a5)     debug(sprintf_array(text + "\n", [a1, a2, a3, a4, a5]))
#define debug6f(text, a1, a2, a3, a4, a5, a6) debug(sprintf_array(text + "\n", [a1, a2, a3, a4, a5, a6]))
