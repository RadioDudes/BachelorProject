## Spread spectrum modulation

Originally created to provide secure communication over a large frequency band, including preventing jamming and others listening in. You use more bandwidth than the original signal requires, thus 'spreading' the signal accross a frequency spectrum. [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

### Direct sequence spread spectrum modulation (DSSS)

"Direct sequence spread spectrum (DSSS) introduces rapid phase transition to the data making it larger in bandwidth." [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

"... the power density amplitude of the spread spectrum output signal is similar to the noise floor. The signal is “hidden” under the noise." [1](https://www.ni.com/en/solutions/aerospace-defense/communications-navigation/understanding-spread-spectrum-for-communications.html)

By increasing the number of pulses / chips, you also increase the frequency band used in signal. See diagram in [2](https://lora-developers.semtech.com/documentation/tech-papers-and-guides/lora-and-lorawan/)

## Chirp spread spectrum modulation

A chirp pulse is a signal going either up or down in frequency (up chirp and down chirp). Chirp spread spectrum utilizes chirp pulses for spread spectrum purposes. [1](https://www.researchgate.net/publication/311980840_Chirp_spread_spectrum_as_a_modulation_technique_for_long_range_communication)

CSS symbols have the properties: Spreading factor, minimum frequency, maximum frequency, and value (bits). The pulse starts at a frequency between the minimum and maximum frequency. The amount of bits that can be represented in a symbol is defined by $log_2(S)$, where S is the spreading factor.
The chirp starts at $f_0$, goes up to $f_{max}$, and then goes from $f_{min}$ to $f_0$.

The length of a symbol is given by $t_s = S / (f_{max} - f_{min})$.

And the bitrate is given by $R_b(S) = log_2(S) / t_s$


## Frequency shift keying

## LoRa modulation

### Spreading factor
"Spreading factor (SF) is the speed at which the signal frequency changes across the bandwidth of a channel."[4](https://blog.ttulka.com/lora-spreading-factor-explained/)
With every 1 increase in SF, the amount of time spent sending 1 symbol doubles.

However, the amount of symbols is based on spreading factor:
S = 2^SF - 1 [5](https://ieeexplore.ieee.org/document/8067462)
This means that with a higher spreading factor, there is more information in each symbol, but it takes a lot longer to send each symbol.

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

## Doppler effect

## Doppler rate

## SPPI / signal strength

## SNR

## Noise floor

Look up "thermal noise"

## Path loss

## Fading

### Rayleigh

### Rician

## Fresnel zones

## Power spectral density / power density amplitude

## Cyclical redundancy check


