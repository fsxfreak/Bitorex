Bitorex
=======

Bitorex is a small weekend pet project to satisfy my renewed interest in Bitcoin and a newfound curiosity for trading currency and stocks in general. It simulates a Bitcoin exchange, where you can make buy (long) or sell (short) trades.

Bitorex runs in the Windows command line. Trade requests are done by pressing a button (W, S, E, F) and typing a number. No fancy command prompt graphics library was used, so everything is displayed and inputted using stdout and stdin. 

## Exploring

The source for Bitorex is found under src/.
* ```account.hpp``` - Declares the Account struct, where the trader's money and trade history is stored.
* ```account.cpp``` - Defines all the boilerplate for Account and some printing functions.
* ```exchange.hpp``` - Declares the Exchange struct, where the ticker is updated and orders are placed. Here the `FEE` constant is defined.
* ```exchange.cpp``` - Here's where the main bulk of the code is. Defines self-contained functions for pulling from HTTPS webpages and parsing JSON. Also defines functions that update the ticker, place orders, prints information, and gets user input.
* ```main.cpp``` - Contains ```main()```. Handles read/write of the trader's account, as well as starting the exchange. 

## Dependencies
##### Platform agnostic
* **[json-cpp](https://github.com/mrtazz/json-cpp)** - Used for parsing the JSON formatted ticker data from [BTC-e](https://btc-e.com/api/3/ticker/btc_usd).
* **[libcurl](http://curl.haxx.se/libcurl/)** - I used libcurl with SSL and zlib to make HTTPS requests to pull data.
* **C++11 features** - If your compiler does not support ```<thread> && <mutex> && <chrono>```, you will not be able to compile Bitorex.

##### Platform dependent
* ```_getch()``` - Windows only function from <conio.h> which receives input without needing ```\n``` character.


## Usage
#### Downloading
To download, simply

	$ git clone https://github.com/fsxfreak/Bitorex.git

#### Building
**Windows**

Get a fixed version of MinGW from here: [x32-4.8.1-release-posix-sjlj-rev-5.7.z](http://sourceforge.net/projects/mingwbuilds/files/host-windows/releases/4.8.1/32-bit/threads-posix/sjlj/)

Then,

    $ cd Bitorex/src
    $ g++ -std=c++11 -Wall main.cpp account.cpp exchange.cpp -o ../bin/bitorex.exe -L../lib/ -lcurldll -L../lib/mingw -ljson_mingw_libmt -I../include/

**Linux**

**OSX**

Sorry, I don't have any specific step-by-step instructions. Download a precompiled version of [curl-7.3.4](http://curl.haxx.se/download.html) with SSL and zlib for your OS. Then download json-cpp and build. You must use a compiler that supports the C++11 features mentioned above. Place the relevant files in the include and lib directories, and compile and link.

#### Running

    $ cd Bitorex/bin
    $ bitorex account_name.txt

#### Usage

**Controls**
```
Quit       - Q
Buy        - W
Close buy  - S
Sell       - E
Close sell - D
Save	   - A
```
**Defining an account**

To give yourself fiat (an arbitrary currency) and some crypto-coin, do

    $ cd Bitorex/bin
    $ touch account_name.txt
    $ vim account_name.txt
    
The first line of the file contains the amount of fiat you want, and the second line the coin you want. Example:
```
5000
0.25
```
This gives you 5000 in fiat and 0.25 in coins.


## Author's comments
This project is my first foray into threading, which I had thought was harder than it actually is. It seems that threading is simple: start a thread, and lock some variables when accessing. However, it seems that I had to make many of my variables global in order to share them between threads. Perhaps someone could fork this repo and help me out with that.

This is also my first time pulling and parsing code from webpages, which I had thought was also extremely difficult. Fortunately, libraries to the rescue.

It seems that the actual difficult part of this project was the building and linking. The ordinary build of MinGW has no support for threads, so I had to use the external one. Also, curl and OpenSSL were very difficult to build. Lastly, it was a pain to link everything together.

## Future plans

In the future, I want to make an actual GUI with graph displays, ask/bid tickers, trade history.

## License

The MIT License (MIT)

Copyright (c) 2013 Leon Cheung

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
