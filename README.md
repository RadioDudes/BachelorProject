## Report

https://www.overleaf.com/project/65ae51a0a8d8acb858f070be

## Preliminary problem statement
https://itustudent.itu.dk/Study-Administration/Project-Work/the-project-page

LoRa is a proprietary radio communication technology, that excels over long distances, on low power devices, and high noise environments. Other advantages include the fact that it can work in ISM-bands, thus avoiding the need of having a license. Furthermore, on the 2.4GHz ISM band, there is no duty cycle. It is most commonly used in conjunction with LoRaWAN, a low power wide area network, for IoT purposes.
Use cases can include monitoring the contents of silos (https://www.loriot.io/use-cases/agritech-silo-monitoring.html) and monitoring air quality in schools (https://www.alliot.co.uk/lorawan-use-cases/) and in general collecting data from sensors. 

LoRa operates at low data rates. This means that while LoRa is very suitable for transmitting small amounts of data generated by sensors to a central server, it is not suitable for the transfer of files. 
We believe that this is a challenge that warrents further investigation, since the transfer of files through LoRa is relevant in cases where image or audio data is gathered.
Possible uses could be the transfer of various forms of data from satellites to groundstations, or video and audio monitoring on ground.

We will implement software solutions that address the challenge of file transfer. These solutions are then tested in a real setting. We will then use data from testing to evaluate our solutions.



### Previous problem statement

Setup communication between a transmitting device and receiving device over LoRa 2.4GHz, and perform file transfer.
Investigate the challenges and limits of file transfer through LoRa 2.4GHz.
Evaluate effective strategies for file transfer over LoRa 2.4GHz.

Needed at latest feb. 2

## Questions and other notes

Why is it that the bitrate decreases with higher spreading factors? The length of a chirp halves, but symbols per chirp double...

What is the impact of doppler effect and doppler rate on LoRa when communicating with a sattelite in LEO? 
**[See this article](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/3n000000l9JA/FoIjLVxAx4xpf3Wh5xvvX3B9PWTw.S7R.YPZHazMzBo)** and **[this one](https://www.researchgate.net/publication/356713129_The_influence_of_LEO_satellite_Doppler_effect_on_LoRa_modulation_and_its_solutio)**


What is the background noise power in different situations (e.g. in the case of sattelite communication)? This is necessary to calculate signal to noise ratio, which affects symbol error rate... 

Related to the two above, which combination of spreading factor and bandwidth will result in the most transmitted information?

How long does it take to switch from send to receive on LoRa transceivers?

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
Idea related to video: visualize LoRa chirps with code in various stages... before modulation, during modulation, transmission, after demodulation...

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

**[Chirp spread spectrum](https://www.researchgate.net/publication/311980840_Chirp_spread_spectrum_as_a_modulation_technique_for_long_range_communication)**

**[Paper on LoRa doppler LEO](https://www.researchgate.net/publication/356713129_The_influence_of_LEO_satellite_Doppler_effect_on_LoRa_modulation_and_its_solutio)**

**[LoRa point to point path loss](https://www.researchgate.net/publication/337301203_Characterization_of_LoRa_Point-to-Point_Path-Loss_Measurement_Campaigns_and_Modeling_Considering_Censored_Data)**

Fyr der skriver om fil-transfer med LoRa:
https://stuartsprojects.github.io/2021/02/26/Large-Data-Transfers-with-LoRa-Part1.html
https://stuartsprojects.github.io/2021/03/03/Large-Data-Transfers-with-LoRa-Part2.html

**[LoRaWAN firmware file transfer](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/firmware-updates-over-the-air/)**

**[Stats on LoRaWAN connections](https://www.statista.com/statistics/880822/lpwan-ic-market-share-by-technology/)**

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
