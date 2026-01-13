# RnR_NewGameMod (22.11.2025)
Version 0.1

A plugin that allows you to change the starting car and the old location when starting a new game.

### Usage ###
The bin\RnR_NewGameMod.ini file specifies the following parameters:

vehicle_ID - string identifier of the initial vehicle
vehicle_color - color variant index

Vehicle data is presented in the file list_vehicles. The numbering of colors starts from scratch.

use_new_position - if 1, then the starting position specified in the file is used, if 0, it is used by default.

startPosX, startPosY and startPosZ define the starting playing position. Unlike "Truckers - 2", the car will be installed exactly at the specified coordinates, and not on the nearest road.
Data about starting positions is presented in the file list_startpositions.

After setting the parameters, you should start a new game as usual. When playing with the plot enabled, you should disable overwriting the initial game position by setting the use_new_position = 0 parameter.
In the "one order" mode, only the car specified in the modification is taken into account, and the position will be selected according to the game settings.