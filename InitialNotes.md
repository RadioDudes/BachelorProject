## To clarify
- What would our process be?
	- Would the hardware be at DASYA on ITU?
	- Would we be able to test our code on the hardware as we are developing
	- When do we meet?
	- 
- Which board to use?
	- PyCubed
	- Others...
- What would would we need to do on the board?
	- Installing relevant firmware / driver for the LoRa module
	- CSP
- What is our involvment in the TinyG groundstation, is it already in place?
- Do we use CSP
- Assumptions and limitations
	- Bandwidth
	- Bitrate (how does frequency and bandwidth affect bitrate)
	- Chirp spreading
	- Link budget
	- Doppler effect
	- How will LoRa 2.4 clash with wifi. When in space? When testing it on ground?
	- How much information is lost? Is there a specific pattern to the lost information?
	- One way or two way communication?
	- Picture...
		- Picture size / resolution
		- What would they be used for on ground?
		- Example pictures
		- Lossy vs lossless
		- Confidentiality
	- Limitations regarding the board (low power)

## Preliminary problem statement
https://itustudent.itu.dk/Study-Administration/Project-Work/the-project-page
Needed at latest feb. 2
## Tasks
- Decide on project title (for when we register project, can be changed later)
- Preliminary problem statement
- Prepare board
	- Install firmware
	- Own software - code for image transfer
- Find strategies for image transfer
- Implement them...
- Formulate plan for testing these strategies
- Execute tests...

## Image transfer ideas

Partial transfers
Level of Detail (progressive image transmission)
Compression
Regions of interest
Joint source and channel coding (?)
Encoding

## Relevant Papers / Articles

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
## Other links

https://github.com/pycubed/software
https://github.com/pycubed/hardware
https://github.com/ITU-DASYALab/LoRa2.4/tree/main
[https://wndw.net/book.html#readBook](https://wndw.net/book.html#readBook)
