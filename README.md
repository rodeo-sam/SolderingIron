SolderingIron
=============

Idea for an OpenSource Soldering Iron

This project is just another avr based soldering iron. Since professional equipment is quite pricy and not very complicated this projects takes a good, changeable soldering iron tip like the Weller RT1 and provides all the electronics behind that.

##References:
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
* Solder Tip [Weller RT1](http://datasheet.octopart.com/RT1-Cooper-Tools-datasheet-13539183.pdf)
