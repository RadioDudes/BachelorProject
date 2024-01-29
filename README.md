## Report

https://www.overleaf.com/project/65ae51a0a8d8acb858f070be

## Preliminary problem statement
https://itustudent.itu.dk/Study-Administration/Project-Work/the-project-page

Setup communication between 2 LoRa 2.4GHz transceivers, and perform file transfer.
Investigate the limits of file transfer through LoRa 2.4GHz in regards to bitrate by adjusting on parameters such as spreading factor, bandwidth, antenna choice, distance. 
Evaluate effective strategies for file transfer over LoRa 2.4GHz over long distances.

Needed at latest feb. 2

## Meeting times
- Monday 11-??
- Thursday 10-16

## Tasks
- [x] Project title
- [ ] Preliminary problem statement
- [ ] Linux, SPI, SX1280 (break these tasks down further)
	- [ ] Setup
        - [ ] Perform image transfer
- [ ] ESP32, via LoRa, using CSP, canbus

## Questions

Why is it that the bitrate decreases with higher spreading factors? The length of a chirp halves, but symbols per chirp double...

## Image transfer ideas

Partial transfers
Level of Detail (progressive image transmission)
Compression
Regions of interest
Joint source and channel coding (?)
Encoding

## Relevant Papers / Articles



### Lora related

How LoRa Modulation really works - https://www.youtube.com/watch?v=jHWepP1ZWTk

Understanding the relationship between LoRa chips, chirps, symbols and bits - https://electronics.stackexchange.com/questions/278192/understanding-the-relationship-between-lora-chips-chirps-symbols-and-bits

**[What is LoRa?](https://www.semtech.com/lora/what-is-lora)**

**[Paper on the LoRa modulation](https://ieeexplore.ieee.org/abstract/document/8067462)**

**[Range and Capacity of LoRa 2.4 GHz](https://link.springer.com/chapter/10.1007/978-3-031-34776-4_21)**

**[LoRa physical layer packet](https://www.thethingsnetwork.org/docs/lorawan/lora-phy-format/)**

**[LoRa modulation basics](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R0000001OJa/2BF2MTeiqIwkmxkcjjDZzalPUGlJ76lLdqiv.30prH8)**

**[LoRa modem doppler immunity](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/3n000000l9JA/FoIjLVxAx4xpf3Wh5xvvX3B9PWTw.S7R.YPZHazMzBo)**

**[Ranging with SX1280](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/44000000MDiH/OF02Lve2RzM6pUw9gNgSJXbDNaQJ_NtQ555rLzY3UvY)**
**[SX1280 design of ranging protocol and result processing](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R000000UypY/5mprGH6TIzeLnfosUgj1xK5ftoqDpoCnRk_dzY_jAx4)**
Protocol for finding distance between 2 LoRa transcievers

**[Selecting the optimal reference clock](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/3n000000qQ4C/H3aWcOcSgO0E35Pl87UiFNYvs7qo9LxDjdz2CyRUHW8)**

**[SX1280/SX1281 in low power applications](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R000000HSO9/vmLAj4haZE78QozA8W0mnLRc1WY.SDQ7RYewLFF3k8k)**

**[LoRa Calculator](https://www.semtech.com/design-support/lora-calculator)**
calculates stuff like 'time on air', amount of symbols, as well as link budget and other stuff.


### Image transmission related

Google Scholar -> "low bitrate image transmission"

**[Overcoming Limitations of LoRa Physical Layer in Image Transmission](https://www.mdpi.com/1424-8220/18/10/3257)**

**[Survey of progressive image transmission methods](https://onlinelibrary.wiley.com/doi/abs/10.1002/(SICI)1098-1098(1999)10:1%3C3::AID-IMA2%3E3.0.CO;2-E)**
**[Progressive Image Transmission: A Review And Comparison Of Techniques ](https://www.spiedigitallibrary.org/journals/optical-engineering/volume-26/issue-7/267581/Progressive-Image-Transmission-A-Review-And-Comparison-Of-Techniques/10.1117/12.7974121.short?SSO=1)**
There are many papers on progressive image transmission

**[Joint source channel coding](https://www.sciencedirect.com/topics/computer-science/joint-source-channel-coding)**

**[A low-rate encoder for image transmission using LoRa communication modules](https://link.springer.com/article/10.1007/s41870-022-01077-7)**

**[Image transmission by two-dimensional contour coding](https://ieeexplore.ieee.org/abstract/document/1447420)**

**[Low complexity energy efficient very low bit-rate image compression scheme for wireless sensor network](https://www.sciencedirect.com/science/article/pii/S0020019013001750)**

**[Study of Lora Module Ra-02 For Long Range, Low Power, Low Rate Picture Transfer Applications](https://iopscience.iop.org/article/10.1088/1742-6596/1845/1/012054/meta)**

**[Opportunities and Challenges of LoRa 2.4 GHz](https://ieeexplore.ieee.org/abstract/document/10049306)**
**[LoRa 2.4 GHz Communication Link and Range](https://www.mdpi.com/1424-8220/20/16/4366)**
**[Ranging Capabilities of LoRa 2.4 GHz](https://ieeexplore.ieee.org/abstract/document/9221049)**
**[First Flight-Testing of LoRa Modulation in Satellite Radio Communications in Low-Earth Orbit](https://ieeexplore.ieee.org/abstract/document/9895236)**
"In conclusion, we would like to note that we are planning to test our results for the 868 MHz and 2.4 GHz bands in the near future on the new NORBY-2 CubeSat."
Many more articles...
**[Thingsat :: LoRa and SatIoT benchmarks from cubesat](https://gricad-gitlab.univ-grenoble-alpes.fr/thingsat/public/-/tree/abbc4336b722b259d66ea14cfae539ba931b1ded/cubesat_mission_2)**

## Other links

https://github.com/pycubed/software
https://github.com/pycubed/hardware
https://github.com/ITU-DASYALab/LoRa2.4/tree/main
[https://wndw.net/book.html#readBook](https://wndw.net/book.html#readBook)
