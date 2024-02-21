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

#### What to measure

We will measure time from the first delivered packet to the last delivered packet. We take into account the fact that this number will be misleading, since some data might not be delivered.

We will measure the packet loss, i.e. the amount of delivered packets divided by the amount of sent packets.

#### Parameters

##### Payload size

We test following payload sizes:

##### Spreading factor

We test following spreading factors: 

##### Bandwidth

We test following bandwidths:
 - 203.125 kHz
 - 

##### Frequencies?

We test at following frequencies.

#### Locations

#### Expected result

The time on air for LoRa is calculated by $ToA = 2^{SF}/BW * N_{symbol}$.
We thus expect the time to transmit to be longer for lower bandwidths, higher spreading factors, and larger payload sizes. 
We expect the packet loss to increase with bandwidth, (spreading factor?). At certain frequencies, we expect a higher packet loss.
We also expect a higher packet loss depending on the location where we are transmitting. In cases where we are seperated by thick walls, the packet loss will be higher.
From this



