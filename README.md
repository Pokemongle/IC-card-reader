# IC-card-reader
based on MCS-51 and PN532, can read student M1 card infomation

# Summary

The system is composed of MCU core module, Power Module, PN532 card reading module, key module, Bluetooth module, LCD display module.
![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141539279.png)

IC card is divided into 16 sectors, each sector corresponding to 4 blocks (block 0-block 3) , a total of 64 blocks, numbered 0-63. Block 0 of Sector 0 is used to store the vendor code, which has been solidified and can not be changed. Block 0 -2 of the remaining sector is used to store the data, and Block 3 is the control block, which is used to store the password. The first segment of the first sector of the card (0 sector block 0) stores a globally unique UID.

# Module design example

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141541122.png)

# demo

Power on, initial interface

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141542999.png)

Key functions

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141543775.png)

Push key1 to wake up PN532

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141544649.png)

Successfully wake up the chip

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141544522.png)

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141545986.png)

Push key2 to read infomation from the IC card

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141545656.png)

Successfully find the card , display UID and ID (my campus card)

![image.png](https://pokemongle-images-1319763739.cos.ap-nanjing.myqcloud.com/sandox/img/202309141546092.png)
