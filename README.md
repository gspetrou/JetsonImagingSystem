
# Imaging System

Captures some images, saves them somewhere, and sends them with telemetry data to a web address. Telemetry data isn't finished yet but adding your own telemetry source should not be hard at all.

I originally made this for [Penn State UAS](https://uas.engr.psu.edu).

## License/Usage

If you intend on using/modifying this software you make it open source/available for anyone to download.

## Getting Started

The program is designed to make changing the multimedia backend framework pretty easy but for now only GStreamer is supported so we have to install that.

### Prerequisites

**Linux (Ubuntu/Debian):** These commands are available on GStreamer's website but here you go anyway. It definitely installs packages we don't need but whatever.
```
apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
```
**Mac:** Make sure you have [homebrew](https://brew.sh) installed, then run the following
```
brew install gstreamer gst-plugins-base gst-plugins-good gst-plugins-bad pkg-config
```
**Windows:** Although I've never tried on Windows myself, the easiest way seems to be using [Chocolatey](https://chocolatey.org) and running the following, although a few more gstreamer plugins might be needed.
```
choco install gstreamer
```

### Compiling

Simply running the `make` command on Linux will build the program. If you're building on MacOS then I recommend installing `g++-9` with homebrew, then using the command `make mac` to build the program. This is only to keep the compiler consistent with Linux as the version of g++ that ships with MacOS is just a wrapper for llvm and you might get some unknown warnings. I've never compiled from Windows, good luck and god speed.

Note that any file that needs to include Gstreamer (<gst/gst.h>) must be compiled with the format below below:
```
$(CXX) `pkg-config gstreamer-1.0 --cflags` $(CXXFLAGS) -o output input.cpp `pkg-config gstreamer-1.0 --libs`
```

## Running the Program

Below is the documentation for the command line arguments. Make sure when you're running on anything other than the Jetson that testmode is enabled. You are also always required to specify at least one sensorid.

**Command Line Arguments:**
Every argument has a short and long form, to pass an argument in the short form its like so: `./img_sys -s 0` and in long form: `./img_sys --sensorid=0`

- `-d/--destination` - Web address of where to send files to
	- Default value: "localhost:8000/"
- `-f/--fps` - Sets the frame rate of capture. Note that as of now the program can't really keep up with anything greater than 20fps, will try to fix this later.
	- Valid range: [1:30]
	- Default value: 1
- `-h/--height` - Sets the image capture height
	- Valid range: [1:3046]
	- Default value: 3046
- `-l/--location` - File save location
	- Takes in a string, make sure the save path exists first
	- Default value: "/media/nvidia/sd/"
	- The image file names saved in this location are formatted like so: "camN_######.jpeg" where N is the sensor-id
- `-q/--quality` - Jpeg capture quality
	- Valid range: [1:100]
	- Default value: 100
- `-s/--sensorid` - Which camera sensor ids to enable, at least one must be enabled
	- Example, enables the camera on sensor 2: `--sensorid=2`
	- Example, enables the cameras on sensor 0, 1, and 2: `-s 0,1,2`
- `-t/--testmode` - Enables testmode, allowing you to run the program without the need of the Jetson
	- The images will just be completely white
 - `-w/--width` - Sets the image capture width
	 - Valid range: [1:4104]
	 - Default value: 4104

**Tools:**

 - test_server.py is a simple http server that will print the incoming POST requests. You might want to mess with the logging line in do_POST function incase you don't want the jpeg images to be printed and fill up the log. Usage: `test_server.py <port>`

## Authors

* **George Petrou** - *Initial Author* - george@gpetrou.com
