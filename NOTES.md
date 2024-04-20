## Spread spectrum modulation

Originally created to provide secure communication over a large frequency band, including preventing jamming and others listening in. You use more bandwidth than the original signal requires, thus 'spreading' the signal accross a frequency spectrum. [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

The maximum amount of information which can be transmitted in the presence of noise is given by Shannon Hartleys theorem. [3](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R0000001OJa/2BF2MTeiqIwkmxkcjjDZzalPUGlJ76lLdqiv.30prH8)

$C = B * log_2(1+S/N)$, where C is data rate in bits/second, B is bandwidth.

For very high noise levels, this can be rewritten as $N/S ≈ B/C$, which shows that you only need to increase bandwidth to send more error free information (huh?). [3](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R0000001OJa/2BF2MTeiqIwkmxkcjjDZzalPUGlJ76lLdqiv.30prH8)

### Direct sequence spread spectrum modulation (DSSS)

A type of spread spectrum modulation.

"Direct sequence spread spectrum (DSSS) introduces rapid phase transition to the data making it larger in bandwidth." [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

"... the power density amplitude of the spread spectrum output signal is similar to the noise floor. The signal is “hidden” under the noise." [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

'chip' is simply a radio term for bit, which is used to distinguish the code sequence (sequence of bits used to spread data over a large spectrum) from the actual data being transmitted. By increasing the number of pulses / chips, you also increase the frequency band used in signal. DSSS requires a very accurate reference clock. [2](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/lora-and-lorawan/)

Image below shows the original signal (blue) and the signal after it has gone through the coding sequence (red).
![image](https://lora-developers.semtech.com/uploads/documents/images/DSSS_carrier.png)


## Chirp spread spectrum modulation

A type of spread spectrum modulation.

A chirp pulse is a signal going either up or down in frequency (up chirp and down chirp). Chirp spread spectrum utilizes chirp pulses for spread spectrum purposes. [1](https://www.researchgate.net/publication/311980840_Chirp_spread_spectrum_as_a_modulation_technique_for_long_range_communication)

CSS symbols have the properties: Spreading factor, minimum frequency, maximum frequency, and value (bits). The pulse starts at a frequency between the minimum and maximum frequency. The amount of bits that can be represented in a symbol is defined by $log_2(S)$, where S is the spreading factor.
The chirp starts at $f_0$, goes up to $f_{max}$, and then goes from $f_{min}$ to $f_0$.

The length of a symbol is given by $t_s = S / (f_{max} - f_{min})$.

And the bitrate is given by $R_b(S) = log_2(S) / t_s$ [1](https://www.researchgate.net/publication/311980840_Chirp_spread_spectrum_as_a_modulation_technique_for_long_range_communication)


## Frequency shift keying

## LoRa modulation

LoRa is a technology used for low power, long range communication. Used for IoT purposes. [1](https://www.semtech.com/lora/what-is-lora)

The LoRa modulation is based on chirp spread spectrum modulation. It is an implementation only on the physical layer. It does not need a reference clock as accurate as DSSS does. It also has better processing gain than FSK. [2](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/lora-and-lorawan/).

LoRa bitrate is given by:

$R_b = S * 1/(2^S/B)$,, where S denotes spreading factor, and B denotes bandwidth. 

LoRa symbol period is:

$T_s = 2^S/B$

Symbol rate is given by:

$R_s = 1/T_s$

Chip rate is given by:

$R_c = R_s * 2^S = B$, i.e. the chip rate corresponds to the bandwidth. "one chip is sent per second per hertz of bandwidth."

Nominal bit rate, including coding rate, is:

$R_b = S * CR/(2^S/B)$, where CR is the coding rate (4/5, 4/6, ...)

[3](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R0000001OJa/2BF2MTeiqIwkmxkcjjDZzalPUGlJ76lLdqiv.30prH8)

### Spreading factor
"Spreading factor (SF) is the speed at which the signal frequency changes across the bandwidth of a channel."[4](https://blog.ttulka.com/lora-spreading-factor-explained/)
With every 1 increase in SF, the amount of time spent sending 1 symbol doubles.

However, the amount of symbols is based on spreading factor:
S = 2^SF - 1 [5](https://ieeexplore.ieee.org/document/8067462)
This means that with a higher spreading factor, there is more information in each symbol, but it takes a lot longer to send each symbol.

For LoRa, the spreading factors are orthogonal [6](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/lora-and-lorawan/), meaning that 2 signals on the same frequency with different 2 spreading factors are distinguishable by LoRa. Signals using other spreading factors apprear as noise.

#### Spreading factor orthogonality

Maybe unnecessary, what makes the LoRa spreading factors orthogonal? math

### Bandwidth
Bandwidth is the range of frequencies that each symbol is spread across. The higher the bandwidth, the quicker each symbol will get sent, but each symbol will be sent over a wider range of frequencies. This means that an increase in bandwidth is an increase in data rate.

The SX1280 datasheet has the following possible bandwidths:
- 203.125 kHz
- 406.25 kHz
- 812.5 kHz
- 1625 kHz

### Coding rate
Coding rate is the ratio of useful bits to error-correction bits.

In LoRa, the coding rate can be:
- 4/5
- 4/6
- 5/7
- 4/8

Below is an image showing the coding rate 5/7:

![image](https://github.com/RadioDudes/BachelorProject/assets/126694958/4214f44e-62a7-42b2-9dd3-7fe676e5a599)[7](https://www.thethingsnetwork.org/docs/lorawan/fec-and-code-rate/)

### LoRa PHY packet
The LoRa PHY packet consists of:
- Preamble, used to sync the devices and to alert the receiver that a frame is coming (12.25 symbols)
- Header, used to specify payload length
- Header CRC, used for error correction of the header
- Payload, the actual message (max 255 bytes?)
- Payload CRC, used for error correction of the payload

Header, header CRC and payload CRC are optional.

[6](https://www.thethingsnetwork.org/docs/lorawan/lora-phy-format/)

### LoRa modules

### LoRa range

### Reference clock

### Transmission power

## LPWAN

"Long range communication or low power wide area net-works (LPWANs) are networks where high data rate require-ments are traded for range and energy improvements. In thesenetworks, data rates are very low, leaving hardly a few bytesper seconds, but enabling ranges of a few kilometers." [1](https://www.researchgate.net/publication/311980840_Chirp_spread_spectrum_as_a_modulation_technique_for_long_range_communication)

## LoRaWAN

### Link layer

Uses a star of stars topology. End nodes communicate with gateways, who in turn communicate with the application server. Most traffic is expected to be uplink (from end-nodes to application server). A gateway communicates with multiple end-nodes at different spreading factors and frequency channels, possible using an ADR scheme.
[1](https://lora-alliance.org/resource_hub/lorawan-104-specification-package/)

### ADR scheme

The ADR scheme adapts the transmission power and spreading factor. The spreading factor can be adjusted by the end-node sending packets with the ADR-flag enabled. The gateway uses these received packets to calculate an optimal data rate. The gateway then sends a downlink packet, with a command to adjust the spreading factor.
The end-node adjusts its own spreading factor in the other direction to (from lower to higher), in case that the gateway does not listen on the spreading factor that the end-node is sending on. It sends some packets and after a certain amount asks for an acknowledgement. If it receives no acknowledgement, it then increases the spreading factor...
[1](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/understanding-adr/)

## Doppler effect

## Doppler rate

## SPPI / signal strength

## SNR

## Noise floor

Absolute noise floor (in dBm) is expressed by $N = 10*log_{10}(K*T*B*1000)$, where k is Boltzmanns constant, T is the temperature in kelvin, and B is channel bandwidth. [1](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/2R0000001OJa/2BF2MTeiqIwkmxkcjjDZzalPUGlJ76lLdqiv.30prH8)

Look up "thermal noise"

## Path loss

## Fading

### Rayleigh

### Rician

## Fresnel zones

## Power spectral density / power density amplitude

## Cyclical redundancy check

## Antenna

### Radiation patterns

Isotropic radiation, equal amounts in every direction.

Omnidirectional radiation, isotropic along a single plane.

### Isotropic antenna

A 'perfect' antenna, that radiates in equal amounts in every direction. Doesn't exist in reality.
Antenna gain is measured in relation to an isotropic antenna. The unit is 'dBi', 'i' for isotropic.

### Antenna gain

'how much power is transmitted in the direction of peak radiation'.
In comparison to isotropic antenna.
[1](https://www.antenna-theory.com/basics/gain.php)

### Our antenna

We use a 4cm antenna, which has a gain of 2dBi. It is not meant for 2.4GHz, since the wavelength of 2.4 is 12.5cm, meaning that the antenna is roughly a third of the wavelength. This might affect gain.
[1](https://www.graylogix.in/product/4cms-male-stub-antennastraight)

## Link budget

|          | 2 T3S3's with stub antennas, 4km distance, transmitter with PA |
| -------- | --------------------------- |
| $P_{tx}$ |       20dBm              |
| $G_{tx}$ |       2dBi         |
| $L_{tx}$ |       0db?         |
| $L_{fs}$ |       -112.1dB          |
| $L_{m}$  |        0dB?         |
| $G_{rx}$ |        2dBi          |
| $L_{rx}$ |        0dB?          |
| $P_{rx}$ |        -88.1dBm          |

www.ve2dbe.com/rmonline_s.asp
https://www.heywhatsthat.com/
https://www.scadacore.com/tools/rf-path/rf-line-of-sight/
https://ispdesign.ui.com/#

Antenner brugt i Copenhill - Amager Strandpark test
Feeder antenna, circular polarization, 2.4
Patch antenna, 2.4
Cantenna, perhaps 8 - 12 dBi gain

Antenner i Lund - Danmark test
Cantenna
indkapslet yagi antenna, yagi wimo, 12dBi, https://www.wimo.com/en/ya2400-directional-wifi-antenna

### Receiver sensitivity

SX1280 receiver sensitivity goes down to -132dBm
[1](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/3n000000l9OZ/Kw7ZeYZuAZW3Q4A3R_IUjhYCQEJxkuLrUgl_GNNhuUo)

### Free space path loss

$FSPL = (\frac{4πd}{λ})^2$

or expressed in db, where the frequency is given as GHz, the distance in km:

$FSPL = 20 log_{10}(d) + 20 log_{10}(f) + 92.45$

### Friis transmission equation

Used to calculate power received at antenna. Takes gain of antennas and free space loss into consideration.
[1](https://www.antenna-theory.com/basics/friis.php)

## Half duplex

Half duplex transmission mode allows both parties to communicate with each other (as opposed to simplex), while not allowing communication to occur simultaneously (as opposed to full-duplex).
LoRa transceivers are half-duplex.
[1](https://semtech.my.salesforce.com/sfc/p/#E0000000JelG/a/3n000000l9OZ/Kw7ZeYZuAZW3Q4A3R_IUjhYCQEJxkuLrUgl_GNNhuUo)

## Theoretical data rate

Coding rate of 4/6

|         Parameters       | Data Rate | Data Rate, including coding rate | PHYPayload data rate | File data rate |
| ------------------------ | --------- | -------------------------------- | ----------------- | ----------------- |
| 203 kHz, SF12, 20 bytes |  |
| 203 kHz, SF12, 50 bytes |       2dBi         |
| 406 kHz, SF10, 50 bytes |       0db?         |
| 406 kHz, SF10, 100 bytes |       -112.1dB          |
| 406 kHz, SF8, 50 bytes  |        0dB?         |
| 406 kHz, SF8, 100 bytes  |        0dB?         |
| 812 kHz, SF7, 100 bytes  |        0dB?         |
| 812 kHz, SF7, 200 bytes  |        0dB?         |
| 1625 kHz, SF5, 100 bytes |        0dB?          |
| 1625 kHz, SF5, 250 bytes |        -88.1dBm          |

The amount of bits per symbol is represented by the spreading factor.

LoRa preamble is 12.25 symbols [1](https://www.thethingsnetwork.org/docs/lorawan/lora-phy-format/)

Data rate:
$R_b = SF * \frac{1}{\frac{2^SF}{BW}}$

Data rate, including code rate:
$R_b = SF * \frac{CR}{\frac{2^SF}{BW}}$


The data rate of LoRa is multiplied with the percentage of the data frame, that is actually file data. This way, the amount of the data rate which is used for file data is calculated. 

PHYPayload data rate:
$R_b =  SF * \frac{1}{\frac{2^SF}{BW}} * \frac{S_{PHYPayloadBits}}{S_{Preamble} + S_{Header} + S_{PHYPayloadBits}/CR + S_{CRCBits}/CR}$, where $S_{Preamble} = SF * 12.25$ and $S_{Header} = SF * 8$

File data rate:
$R_b =  SF * \frac{1}{\frac{2^SF}{BW}} * \frac{S_{FileBits}}{(S_{ContentFrameBits} + S_{ContentACKFrameBits}) * A_{Packet} + S_{MetaDataFrameBits} + S_{MetaDataAckFrameBits} + S_{FinFrameBits} + S_{FinACKFrameBits}}$, where $A_{Packet} = \frac{S_{FileBits} + S_{PayloadBits} - 1}{S_{PayloadBits}}$

The LoRa datasheet provides formulas for calculating the size of LoRa packets in terms of symbols. This depends on wether or not "long interleaving" is turned on, which for our purposes it isn't.

Where CR = 1, 2, 3, 4
For SF < 7:
$N_{Symbol} = N_{SymbolPreamble} + 6.25 + 8 + ceil(\frac{max(8 \cdot N_{BytePayload} + N_{BitCRC} - 4 \cdot SF + N_{SymbolHeader}, 0)}{4 \cdot SF}) \cdot (CR + 4)$

For 7 <= SF <= 10:
$N_{Symbol} = N_{SymbolPreamble} + 4.25 + 8 + ceil(\frac{max(8 \cdot N_{BytePayload} + N_{BitCRC} - 4 \cdot SF + 8 + N_{SymbolHeader}, 0)}{4 \cdot SF}) \cdot (CR + 4)$

For SF > 10:
$N_{Symbol} = N_{SymbolPreamble} + 4.25 + 8 + ceil(\frac{max(8 \cdot N_{BytePayload} + N_{BitCRC} - 4 \cdot SF + 8 + N_{SymbolHeader}, 0)}{4 \cdot (SF - 2)}) \cdot (CR + 4)$

|    | Payload Size in Bytes |
| -- | ------------ |
| Metadata packet | 3 + FilenameBytes |
| Content Packet | 3 + ContentBytes |
| EOF packet | 1 |
| All ACK packets | 1 |




This theoretical data rate of the file is optimistic, since it assumes that there is always one device transmitting, and that all frames are received, none are corrupted.

Example:
SF 5
BW 1625
CR 4/6
payload size 30B
file size = 3kB
filename size 5B

## Interleaving (short vs long)
