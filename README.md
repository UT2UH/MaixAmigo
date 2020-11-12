# MaixAmigo Library

This library is a *mimic* of the original [M5Stack](https://github.com/m5stack/M5Stack/) and next generation [M5Core2](https://github.com/ropg/M5Core2) libraries and supports the Maix Amigo and Maix Go (and maybe other) development devices under the Sipeed Maixduino Arduino core.

GFX implementation is based on famous [Bodmer/TFT_eSPI](https://github.com/Bodmer/TFT_eSPI) library and courtesy of @fukuenbiz who shared his K210 driver code for the library [fukuen/TFT-eSPI](https://github.com/fukuen/TFT_eSPI).

# Usage

Choose between managed install:

  - Get it from the Arduino Library Manager

or manual install:

  - Download the [latest release](https://github.com/UT2UH/MaixAmigo/releases) and unzip it in the `~/Arduino/libraries` folder.
  - You're done!

Automatic board selection is based on the boards.txt definition, so changing the board from the Arduino IDE Tools menu is enough to trigger the detection.

# Maix Amigo

<img src="https://maixpy.sipeed.com/assets/hardware/maix_amigo/maix_amigo_0.png" alt="Maix Amigo" width="708" height="390">


<img src="https://maixpy.sipeed.com/assets/hardware/maix_amigo/sipeed_maix_amigo_vi.png" alt="Maix Amigo layout" width="602" height="898">

  - [Amigo](https://dl.sipeed.com/MAIX/HDK/Sipeed-Amigo)
  - [Go](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maix-GO)

This is still beta quality, most examples will fail due to the minor syntax changes. 
Support coming soon:

  - [Dock](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maix-Dock)
  - [M5StickV](https://www.hardkernel.com/shop/odroid-go/)
  - [Cube](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maix-Cube)
  - [Maixduino](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maixduino)
  - [BiT](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maix-Bit)
  - [Nano](https://dl.sipeed.com/MAIX/HDK/Sipeed-Maix-Nano)
  - [M5UnitV](https://www.tindie.com/products/lspoplove/dstike-d-duino-32-xs/)


**Onboard I2C devices**

<table>
 <tr><td>IC</td><td>I2C address</td><td>DeviceID</td><td>Irq</td></tr>
 <tr><td>AXP173</td><td>0x34</td><td>0x68</td><td>?</td></tr>
 <tr><td>ES8374</td><td>0x10</td><td>0x08</td><td>---</td></tr>
 <tr><td>FT6336</td><td>0x38</td><td>0x36</td><td>33</td></tr>
 <tr><td>MSA301</td><td>0x26</td><td>0x13</td><td>---</td></tr>
</table>

**Onboard Grove ports**

<table>
 <tr><td>PIN</td><td>GROVE1</td><td>GROVE2</td><td>GROVE3</td></tr>
 <tr><td>1</td><td>GND</td><td>GND</td><td>GND</td></tr>
 <tr><td>2</td><td>3V3</td><td>3V3</td><td>3V3</td></tr>
 <tr><td>3</td><td>IO5</td><td>IO3</td><td>IO1</td></tr>
 <tr><td>4</td><td>IO4</td><td>IO2</td><td>IO0</td></tr>
</table>

**Onboard SPMOD ports**

<table>
 <tr><td>PIN</td><td>SPMOD1</td><td>SPMOD2</td><td>SPMOD3</td></tr>
 <tr><td>1</td><td>GND</td><td>GND</td><td>GND</td></tr>
 <tr><td>2</td><td>IO12</td><td>IO29</td><td>---</td></tr>
 <tr><td>3</td><td>IO6</td><td>IO28</td><td>---</td></tr>
 <tr><td>4</td><td>IO24</td><td>IO22</td><td>IO24</td></tr>
 <tr><td>5</td><td>IO27</td><td>IO25</td><td>IO27</td></tr>
 <tr><td>6</td><td>IO10</td><td>IO30</td><td>---</td></tr>
 <tr><td>7</td><td>IO11</td><td>IO8</td><td>---</td></tr>
 <tr><td>8</td><td>3V3</td><td>3V3</td><td>3V3</td></tr>
</table>

# <img src="https://maixpy.sipeed.com/assets/hardware/module_spmod/spmod_interface_2.png" alt="SPMOD Pinout" width="664" height="243">

# Credits & Thanks

  - Special thanks to [ropg](https://github.com/ropg/M5Core2) for his M5Core2 improvements shamelessly copied to this port.
  - Kudos to [M5Stack](https://github.com/m5stack) for being the creators of the original M5Stack and its legacy library.

