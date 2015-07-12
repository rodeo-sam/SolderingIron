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

It is done in three different stages, we would like to go over.
- First Stage - Amplification: Since the thermocouples voltage is too low to work with we used an OPA336U to amplify the voltage by 511. This makes it easy for the MCU to measure it.
- Second Stage - Inversion: We used an LM324 to implement this and all further stages. This second stage is a inverting amplifier for the already amplified thermocouple voltage. It is connected to an LL4148 diode and capacitor with a pull down. This arrangement does basically this: Anytime the thermocouple voltage drops (this means the tip is not heated) the inverting amplifier will charge the capacitor. The diode only prevents fast discharging over the amplifier. So if the thermocouples voltage stays high for an extended period of time (meaning you keep heating the soldering tip) the capacitor will discharge fully. This will also happen if you let the tip get too hot. Because then the amplifier will not load the capacitor since the voltage it sees is too high.
- Third Stage - Schmitt Trigger: The last part consists of another amplifier used as a inverting Schmitt trigger. As input it uses the stored charge of the mentioned capacitor (meaning its voltage) and the output is connected to a FET, which can cut the power going into the soldering tip. It also features a LED so you can check if you screwed things up.

As you can see you have almost no way of killing your soldering tip by writing bad software.
This comes of course with some drawbacks.
For example you cannot exceed temperatures of about 390&deg;C with your tip although you might want to and your tip could take it.
Since every feature you cannot turn off should be considered a bug we added a pin header for a jumper so you can have the option to turn this safety circuit off.

In the picture below you see the individual stages.

![View on Safety Circuit]({{ site.imageurl }}/layout_safety_circuit.png)

[back to Hardware](Hardware)
