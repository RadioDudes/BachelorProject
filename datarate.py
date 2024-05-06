import math
import argparse


def dataRate(sf, bw):
    return sf/((2**sf)/bw)

def packetAmount(filesize, payload):
    return math.floor((filesize * 1000 + payload - 1)/payload)

def frameBits(sf, cr, payload, preamble, bitcrc, header):
    if sf < 7:
        return sf * (preamble + 6.25 + 8 + math.ceil(max(8*payload+bitcrc-4*sf+header, 0)/(4*sf)) * (cr + 4))
    elif 7 <= sf <= 10:
        return sf * (preamble + 4.25 + 8 + math.ceil(max(8*payload+bitcrc-4*sf+8+header, 0)/(4*sf)) * (cr + 4))
    else:
        return sf * (preamble + 4.25 + 8 + math.ceil(max(8*payload+bitcrc-4*sf+8+header, 0)/(4*(sf-2))) * (cr + 4))
    
def fileDataRate(sf, bw, cr, payload, filesize, filename, preamble, bitcrc, header):
    _packetAmount = packetAmount(filesize, payload)
    contentFrameBits = frameBits(sf, cr, 3+payload, preamble, bitcrc, header)
    contentACKFrameBits = frameBits(sf, cr, 3, preamble, bitcrc, header)
    metaDataFrameBits = frameBits(sf, cr, 3+filename, preamble, bitcrc, header)
    metaDataACKFrameBits = EOFFrameBits = EOFACKFrameBits = frameBits(sf, cr, 1, preamble, bitcrc, header)
    
    fileSizeBits = filesize * 1000 * 8
    _dataRate = dataRate(sf, bw)
    dataTransferred = (contentFrameBits + contentACKFrameBits) * _packetAmount + metaDataFrameBits + metaDataACKFrameBits + EOFFrameBits + EOFACKFrameBits
    
    return  _dataRate * (fileSizeBits/dataTransferred)

if __name__ == "__main__":
    sf = 7
    bw = 203.125
    cr = 2
    payload = 30
    filesize = 3
    filename = 8
    preamble = 12
    bitcrc = 16
    header = 20

    parser = argparse.ArgumentParser()
    parser.add_argument("-sf", "--spreadingfactor", help = "Set spreading factor, default is 7")
    parser.add_argument("-bw", "--bandwidth", help = "Set bandwidth, default is 203.125")
    parser.add_argument("-cr", "--codingrate", help = "Set coding rate, default is 2 (4/6)")
    parser.add_argument("-ps", "--payloadsize", help = "Set size of payload in bytes, default is 30B")
    parser.add_argument("-fs", "--filesize", help = "Set size of file in kB, default is 3kB")
    parser.add_argument("-fn", "--filename", help = "Set size of filename in bytes, default is 8B")
    parser.add_argument("-pre", "--preamble", help = "Set size of preamble in symbols, default is 12")
    parser.add_argument("-crc", "--bitcrc", help = "Set size of crc in bits, default is 16")
    parser.add_argument("-head", "--header", help = "Set size of header, default is 20")

    args = parser.parse_args()

    if args.spreadingfactor:
        sf = int(args.spreadingfactor)
    if args.bandwidth:
        bw = float(args.bandwidth)
    if args.codingrate:
        cr = int(args.codingrate)
    if args.payloadsize:
        payload = int(args.payloadsize)
    if args.filesize:
        filesize = float(args.filesize)
    if args.filename:
        filename = int(args.filename)
    if args.preamble:
        preamble = float(args.preamble)
    if args.bitcrc:
        bitcrc = int(args.bitcrc)
    if args.header:
        header = float(args.header)

    print(fileDataRate(sf, bw, cr, payload, filesize, filename, preamble, bitcrc, header))