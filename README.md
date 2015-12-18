# What?

serialusb is a cheap (~5$) USB proxy intended to be used with input devices.

Input devices generally use interrupt IN and OUT endpoints, and operate at low or full speed modes.  
The maximum throughput is 64 kB/s = 512kbps in each direction (1000Hz, 64-byte packets).  
A USB proxy requires a system with both device and host interfaces.  
Many ARM boards fulfill this requirement, but those aren't as cheap (yet).  

serialusb is the combination of:  
* a PC software operating the host side of the proxy
* an atmega32u4 firmware operating the device side of the proxy  

The atmega32u4 and the PC are connected using a USB to UART adapter, running at a baudrate of 500kbps.  

target device &harr; PC &harr; USB to UART adapter &harr; atmega32u4 &harr; target host  

# Why?

I'm the developper of the [GIMX](https://github.com/matlo/GIMX/) project, which aims at allowing people to use any input device with any gaming console. It is quite frequent that people contact me asking if GIMX could emulate a specific device. As I can't afford buying every input device, the only solution is that people provide me a USB capture of the device protocol. Commercial USB capture tools are costly, and many people can't afford buying one. serialusb aims to be a solution to this problem.  

serialusb attempts to meet the following goals:

1. low cost (< 10$)
2. low latency and low CPU footprint
3. low disturbance on transfered data
4. reusability:
    * allow code parts to be used into the GIMX project
    * make it easy to later add support for cheap ARM-based devices such as CHIP

These goals led to the following decisions:

* use the same hardware as the [GIMX DIY USB adapter](http://gimx.fr/wiki/index.php?title=DIY_USB_adapter) &rarr; 1.
* interrupt-driven event processing in a single-threaded process &rarr; 2.
* once the proxy is started, only use [libusb's asynchronous API](http://libusb.sourceforge.net/api-1.0/group__asyncio.html) &rarr; 2.
* use raw descriptors (don't reconstruct descriptors) &rarr; 3.
* abstract USB device and serial port handling &rarr; 4.

# Software requirements

* GNU/Linux
* libusb >= 1.0.16

# Hardware requirements

* a computer with 2 USB host ports
* an atmega32u4 board running at 5V
* a CP2102- or FT232RL-based USB to UART adapter with 5V tolerance
   * the FT232RL is better as its max baudrate is 3Mbps (vs 921600bps)

# Notable components

* The atmega32u4 firmware is based on [LUFA](https://github.com/abcminiuser/lufa) which is a great USB stack for AVRs.

# Limitations

* Only control and interrupt endpoints are currently supported.
* The size of any control transfer (setup + data) should not exceed 254 bytes.<br />
Standard descriptors such as device, configuration, string and HID report descriptors are loaded on the atmega32u4 into a 1kB RAM buffer.<br />
The 254-byte limitation does not apply to all standard descriptors that fit into the 1kB RAM buffer.
* This is a software proxy, not a hardware one: it's usefull for reverse-engineering protocols, not for investigating hardware issues.
* Because the USB interface of the atmega32u4 has some constraints, such as a limited number of endpoints, serialusb does a few changes to the USB descriptors used at the enumeration step.
* For now the UART speed is 500kbps, which means the theorical max throughput is 50kB/s. This is not enough to reach 64kB/s.
* When using a Raspberry Pi as the proxy host, expect issues with devices using interrupt OUT endpoints.

# Licence

[![GPLv3](http://www.gnu.org/graphics/gplv3-127x51.png)](http://www.gnu.org/licenses/gpl-3.0.html)

# Related projects

* [GIMX](https://github.com/matlo/GIMX/)
* [USBSniffer](https://github.com/matlo/bb_usb_sniffer): kernel module, runs on the beagleboard-xm (~150$)
* [USBProxy](https://github.com/dominicgs/USBProxy): userland software, runs on the beaglebone black (~50$)
