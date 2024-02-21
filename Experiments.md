# Experiments

## What is the purpose of the experiments?

To find out what is the 'fastest' way of transferring files over LoRa.

## What do we measure?

Metrics include:
 - time it takes to transfer file (s)
 - packet loss (% of packets delivered)
 - Total packets delivered.
 - Packet air time (s). Or distance (m).

Depending on the protocol we are testing, packet loss may or may not affect time.

We use these metrics to find a function, for each protocol / strategy, that can be used to calculate time to transmit as a function of packet loss, and file size. $t(l, s) = ...$, or alternately, the bitrate as a function of packet loss $b_f(l) = ...$ (here bitrate is referring to the rate of transmission of the contents of the file).

## How do we measure this?

We place the two transceivers in different locations, and measure time, packet loss, total packets delivered, and distance, for different protocols we have described. 

