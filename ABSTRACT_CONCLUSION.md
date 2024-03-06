# Abstract

## First draft

LoRa is a spread spectrum modulation that sacrifices data rate in exchange for range. It provides the physical layer for LoRaWAN, a wide area network used in IoT. 
Files transfer is very rarely, if ever, performed over a LoRa physical layer, due to the low data rate. 
We implement a protocol, to perform point-to-point transmission of files. We then perform file transfer, between 2 LoRa transceivers on the 2.4GHz ISM band at various distances. Experiments show a data rate of ___ bits per second over a range of ______ km.
This means that performing file transfer over LoRa is feasible, and also useful in cases where file sizes a small enough.

## Second draft

This paper evaluates the feasability of using LoRa on the 2.4gHz ISM band for file transfer. LoRa, a spread spectrum modulation, is most often used as the physical layer for LoRaWAN, in cases where small amounts of data are transferred from accross a wide area to a central server. It operates at a low data rate, which is exchanged for a long operating range. This low data rate is suitable for sensor data, but not file transfer.
We implement a simple protocol for file transfer between 2 LoRa transceivers. The protocol ensures that the complete file is transmitted, even in the face of lost frames. Using this protocol we successfully perform file transfer of a file 50kb large, over a distance of 4km over a period of 25 minutes. 
This shows that LoRa point-to-point file transfer, although slow, is possible over long distances. The solution provided lays the foundation for further development of file transfer over LoRa, and can also be fitted to work on LoRaWAN networks.


## Notes on abstract

announce topic
announce central question
what has previous research shown
what methods we use
main findings
significance of findings

https://writing.wisc.edu/handbook/assignments/writing-an-abstract-for-your-research-paper/


# Conclusion

In this paper we show that file transfer over the LoRa physical layer is feasible. 
To show this, a simple file transfer protocol was implemented specifically with LoRa in mind. The implementation makes sure to divide files into small chunks that can be transferred in LoRa frames, and keeps track of what data has and hasn't been transferred. It thus assures that the full file is transferred eventually.
Furthermore, the protocol adapts itself to maximize the data rate, by automatically adjusting on bandwidth, spreading factor and coding rate.
Using this simple file transfer protocol, point-to-point file transfer is performed at various distances and on files of various sizes. Most notably, file transfer is achieved over a distance of 4km with a file of 50kb. This takes a grand total of 25 minutes, and shows that file transfer is indeed feasible over long distances on the LoRa physical layer. 
Despite the success of file transfer, it is still a very slow process, and LoRa is thus not the first choice of a physical layer for file transfer. It does however make sense, when files are only transferred on rare occasions. 
The typical use case of LoRa is as the physical layer for LoRaWAN, a low power, high range, wide area network. Further research should look into how file transfer could be fitted to work with LoRaWAN, which is widely used for IoT purposes. 

## Notes on conclusion

Answer research question
Summarize research in paper
Future recommendations
Emphasize own contributions

https://www.scribbr.com/dissertation/write-conclusion/



