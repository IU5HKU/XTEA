# XTEA
 XTEA (eXtended TEA) a block cipher designed to correct weaknesses in TEA

Absolutely not my work, you can refer the wiki at this [LINK](https://en.wikipedia.org/wiki/XTEA) to retrieve better infos.
This is a simple example of use for encipher() and decipher() functions proposed by the authors, David Wheeler and Roger Needham of the Cambridge Computer Laboratory. XTEA was a simple method for crypting firmware for some goodies around, and to decipher them at runtime. The algo uses 128bit keys over 64bit chuncks of data, with suggested 64 rounds of xoring. An easy and fast algo to implement, but really weak.
