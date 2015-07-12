---
layout: default
title:  "Safety Circuit"
date:   2015-07-11 12:00:00
---

## SAFETY CIRCUIT

If one of those two features triggers the power to the tip will be cut.
The 5V logic will stay on and the AVR can detect the safety features triggering when R32 is mounted (we recommend a 1 kOhm resistor).
The circuit was designed using LTSpice and tested with the real hardware after simulation showed promising results.

Here is some more explanation on how it works.
First you have to know the background on how the soldering tip is designed inside.
It is connected via three wires: one for heating, one for temperature measurement and one for ground.
Since the heating element and the temperature sensor share the same ground wire, we cannot heat the tip while measuring its temperature.
That is in fact a bit of a hassle.
Sensing the temperature is done with a thermocouple.
Thermocouples consist of two different metals and produce a voltage between its two connectors.
In the tip one of the wires is connected to ground.
That means in order to measure the tips temperature you have to measure the voltage between ground and temperature measurement wire.
Here comes the tricky part.
If you heat the tip you will run a current of up to 5 amps through the heating element.
This current will also run through the ground wire.
The voltage you will be measuring at the thermocouple ranges from 2mV to 5mV.
That means if your wire has more than 0 Ohm resistance (which is very likely the case) you will see a different (higher) voltage between the temperature measurement wire and ground.
So the only way to avoid this is to heat and to measure interleaving.
Since you can write your own code and flash it to the MCU you could try to kill the soldering tip by overheating it.
Usually if your MCU crashes you have between 4 to 6 seconds to realize that and pull the plug before your soldering tip glows orange and is most likely permanently damaged.

We wanted to not let this happen to anyone (since a tip died during testing in our lab).
So we used some analog magic and created a circuit, that protects you from overheating your soldering tip.
This also includes no software necessary to operate the circuit or in other words: You cannot (should not) turn it off.

It is done in four different stages, we would like to go over.
- Amplification: Since the thermocouples voltage is too low to work with we used an OPA336U to amplify the voltage by 511. This makes it easy for the MCU to measure it.
-

![View on Safety Circuit]({{ site.imageurl }}/layout_safety_circuit.png)

[back to Hardware](Hardware)
