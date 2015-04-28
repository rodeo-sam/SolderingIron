SolderingIron
=============

Idea for an OpenSource Soldering Iron

This project is just another avr based soldering iron. Since professional equipment is quite pricy and not very complicated this projects takes a good, changeable soldering iron tip like the Weller RT1 and provides all the electronics behind that.

![View on PCB Layout](https://github.com/debaal/SolderingIron/raw/master/pcb_draft.png)
## Features
* open hardware (schematic and layout made in Eagle 7.2 freeware license)
* open firmware written in C (not ready to use right now)
* fast heatup
* saving and restoring temperature to and from EEPROM
* high-precision op-amp for temperature measurement
* safety measures against overheating and controller crashes
* automatic temperature control (pid controller)
* different temperature scales available (recompiling necessary)
* solder tip rest detection via hall sensor (optional)
* almost all parts are available from the [Seeedstudio Open Parts Library](http://www.seeedstudio.com/wiki/Open_parts_library)

## How does it work
* 5V for logic
* 12V for soldering iron
* ATmega328p controlled

### Temperature measure
The thermo-voltage signal is amplified by an op-amp with a gain
	`g = 1 + R1/R2 = 1 + 51k/100 = 511`
this signal is converted by ADC0 using Vcc=5V as voltage reference

### Solder tip detection
Solved by a hall sensor connected to ADC2. This sensor is powered by 5V


## References
### Links:
* [arduino uno shield](http://www.martin-kumm.de/wiki/doku.php?id=Projects:SMD_Solderstation)
  * Forum discussion + group order coordination [Thema: DIY SMD - Lötstation Ersatz für (WELLER)](http://fpv-community.de/showthread.php?41335-DIY-SMD-Lötstation-Ersatz-für-(WELLER))
* [original Weller cable](http://www.pewa.de/cgi-bin/cosmoshop/lshop.cgi?action=showdetail&wkid=1415179514-14110&ls=d&nc=1415179711-29852&rubnum=&artnum=C-51042699&file=&gesamt_zeilen=0Tsuche--silikon)
* unfinished project [Custom Weller WSM 1 style controller for RT series tips](http://dangerousprototypes.com/forum/viewtopic.php?f=19&t=3583)
* [Thermoelement im Weller WMRP.](http://www.mikrocontroller.net/topic/175412#1686071)

### Datasheets:
* power MOSFET [IRF7424](http://www.irf.com/product-info/datasheets/data/irf7424.pdf)
* Voltage regulator [REG1117](http://www.ti.com/lit/pdf/SBVS001)
* uC [ATmega328P](http://www.atmel.com/devices/atmega328p.aspx)
* Quad OP-Amp [LM324D](http://pdf1.alldatasheet.com/datasheet-pdf/view/22759/STMICROELECTRONICS/LM324D.html)
* Single OP-Amp [OPA336u](http://www.ti.com/lit/ds/symlink/opa336.pdf)
* Solder Tip [Weller RT1](http://datasheet.octopart.com/RT1-Cooper-Tools-datasheet-13539183.pdf)
* Solder Tip detector [Hallsensor unipolar](http://www.reichelt.de/index.html?&ACTION=7&LA=3&OPEN=0&INDEX=0&FILENAME=B400%252FTLE4905L_TLE4935L_TLE4945L_TLE4935-2L%2523SIE.pdf)

### Current State:
* hardware version 1.2 is almost ready to be produced
* optimized bill of materials by reducing unnecessary components
* PCB measures 5cm x 5cm
* individual 7-segment displays in use (broader availability)
* I2C is now unused and can be used for additional hardware