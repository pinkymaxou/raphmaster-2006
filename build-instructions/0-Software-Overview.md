# GUI

## The machine can be controlled remotely.

- Using a web interface 
- Http API to allow to communicate with another online service.

## Physical buttons:

- STOP button that cut the power to the moving parts of the machine.
- Cancel order button 
- Software reset

## Lightning

- Decorative lightning to indicate process progress
- Lights behind the alcohol pourer?

# Settings

It should allows to import/export settings easily, using a JSON file for example.

## Wi-Fi settings

Allows to change Wi-Fi settings for "Soft Access Point" and "Station" mode.

- Allows to set "soft access point" password
- Allows to change station mode SSID and password (or deactivate it)

## Lightning

Allows to change between different lightning pattern and animation

TO BE DEFINED.

## Calibration

Must allows to calibrate the sleigh to be sure it go on the right location

Required functions:

- Allow to do homing manually
- A mode to allow to freely move the sleigh manually
- Save the coordinate for each bottle stations
- Save the maximum allowed distance for the train
- Calibrate pourer
- Calibrate the scale
- Set minimum weight (for glass detection)

## Ingredients global settings

- Default volumic mass

See reference: [volumic-mass-data](design-data/volumic-mass-data.md)

## Bottle inventory and stations

There will be at least 14 available stations, we need to be able to assign bottle in each stations.
Each stations shall be configurable:

- Assign a bottle to a station
- Ingredients, matched to existing values (example: Tequila)
    - Volumic mass is matched to ingredient
- Quantity in ml

## Ingredient

It must contains a list of already existing ingredient but it should also allow to easily add new one

- Alcohol percentage
- Volumic mass (an helper can help calculate it)
    - It could also help to find it based on existing datas

# Prepare drink

## Cocktails

Cocktail shall be stored on the device or SD card.

- Should be able to add or browse new recipe easily.
- Should be able to automatically find what recipe can be done with loaded bottle
- Should take account quantities and display an error if the incredient isn't available
- Should be able to activate/deactivate recipe

## Expected functions

- Allows to make a cocktail based on available recipes.
    - If an incredient isn't available, warn the user before continuing but still allows to do it
- Allows to go in manual mode and choose ingredient manually.
- Allows to choose quantity (ml)
- Allows to queue drinks?

## Unit of measure

Accepted units of measures:

- oz
- ml
- unit
