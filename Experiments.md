# Experiments

## What is the purpose of the experiments?

To find out what is the 'fastest' way of transferring files over LoRa.

## What do we measure?

Metrics include:
 - time it takes to transfer file (s)
 - packet loss (% of packets delivered)
 
Depending on the protocol we are testing, packet loss may or may not affect time.

We use these metrics to find a function, for each protocol / strategy, that can be used to calculate time to transmit as a function of packet loss, and file size. $t(l, s) = ...$, or alternately, the bitrate as a function of packet loss $b_f(l) = ...$ (here bitrate is referring to the rate of transmission of the contents of the file).

The point is to, using this data, compare different file transmission strategies with each other. 

## How do we measure this?

We place the two transceivers in different locations, and measure time, packet loss, total packets delivered, and distance, for different protocols we have described. 

## Experiments

### 50 kB of data

#### Goal

The goal of this experiment is simply to send 1 kB of 'random/trivial' data. The point is to compare to the theoretical data rate, as well as set the expectations of packet loss for future experiments.

We also hope to find locations and frequencies where packet loss is higher, for future tests.

#### What to measure

We will measure time from the first delivered packet to the last delivered packet. We take into account the fact that this number will be misleading, since some data might not be delivered.

We will measure the packet loss, i.e. the amount of delivered packets divided by the amount of sent packets.

#### Parameters

##### Payload size

We test following payload sizes:
 - 16 bytes
 - 64 bytes
 - 192 bytes

##### Spreading factor

We test following spreading factors: 
 - 7
 - 9
 - 11

##### Bandwidth

We test following bandwidths:
 - 203.125 kHz
 - 406.25 kHz
 - 1625 kHz

##### Frequencies?

We test at following frequencies.
 - 2400 MHz
 - 2437 MHz (in the middle of a WiFi channel)

#### Locations

We test at following locations
 - Right next to each other.
 - Opposite ends of ITU.
 - From nth floor to basement. ()
 - ...

#### Expected result

The time on air for LoRa is calculated by $ToA = 2^{SF}/BW * N_{symbol}$.
We thus expect the time to transmit to be longer for lower bandwidths, higher spreading factors, and larger payload sizes. 
We expect the packet loss to increase with bandwidth, (spreading factor?). At certain frequencies, we expect a higher packet loss.
We also expect a higher packet loss depending on the location where we are transmitting. In cases where we are seperated by thick walls, the packet loss will be higher.
From this

#### Data table


| Location        | Bandwidth   | Frequency |  SF  | Payload size | Packet loss | Total transferred data | 
|:-------------:|:---------------------:|:----------:|:----------:|:----:|:------------:|:-------------:|
| Right next to each other | 203.125 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 203.125 kHz | 2437 MHz | 11 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 406.25 kHz | 2437 MHz | 11 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| Right next to each other | 1625 kHz | 2437 MHz | 11 |  192 bytes |              |              |

| From nth floor to basement | 203.125 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement| 203.125 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 203.125 kHz | 2437 MHz | 11 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 406.25 kHz | 2437 MHz | 11 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2400 MHz | 11 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 7 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 7 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 7 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 9 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 9 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 9 |  192 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 11 |  16 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 11 |  64 bytes |              |              |
| From nth floor to basement | 1625 kHz | 2437 MHz | 11 |  192 bytes |              |              |





