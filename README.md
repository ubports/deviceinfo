# Deviceinfo
Library to detect and configure devices


## Config files
Deviceinfo have 3 different types of config files

default.conf, this is the default config file where configs will comes from if a device config file is not found or does not contain the requested config key.

The sections can be divided into type and driver default, example for halium (driver) and phone (type). (sections are the ones in [])

example:
```
[default]
Name=generic

[halium]
Type=phone

[phone]
GridUnit=21
PrettyName=Generic Halium device

[tablet]
GridUnit=10

[desktop]
GridUnit=8
```

[device].conf, this is the file that will be loaded on each device, it will try to autodetect this to try to find this file based on model of the device, using android props for halium/android devices and dtb for native linux devices. *Note that this file needs to be all lowercase! any autodetected name will be converted to lowercase*

section name MUST be named `device` (one in [])

example:
```
[device]
Name=cooldevice
```

alias.conf, this is the file to lookup alternative names, example if a device have multiple names it can forward to the correct file. example Oneplus one has both A0001 and bacon as name. 

section name MUST be named `alias` (one in [])

example:
```
[alias]
A0001=bacon
```

## Built in config keys (with special logic and fallback logic)
 - Name Device name/codename
 - PrettyName Pretty name of the device
 - DeviceType Sets device type (desktop, tablet, phone)
 - GridUnit Sets scale units
 - SupportedOrientations= (Portrait,InvertedPortrait,Landscape,InvertedLandscape)
 - PrimaryOrientation
 - PortraitOrientation
 - InvertedPortraitOrientation
 - LandscapeOrientation
 - InvertedLandscapeOrientation

Any key can be added without needing to modify deviceinfo to use, the downside is that you wont get hardcoded fallback if default config files is not found.

## Detect logic

`Name` is autodetected by using the model of the device, using android props for halium/android devices and dtb for native linux devices. (this can be overwritten by config)

`DeviceType` on android/halium this is autodetected by trying to look at the android characteristics prop, if this include tablet we assume its a tablet, if not we assume it's an phpne. On linux we have no way of detecting this atm, it fallsback to desktop. (this can be overwritten by config)

`DriverType` is autodetected by trying to get any type of android prop, if it can find it we assume its an android device, if not we assume it's a native linux device. (this can `NOT` be overwritten by config)

## Tool usage

```
    - device-info get [prop]
```

## Override enviroment varables
 - DEVICEINFO_CONFIG_PATH overrides path to find config files
 - DEVICEINFO_DEFAULT_CONFIG overrides default config file
 - DEVICEINFO_DEVICE_CONFIG overrides device config file
 - DEVICEINFO_ALIAS_CONFIG overrides alias config file
 - DEVICEINFO_DEVICE_NAME overrides device detected name
 - DEVICEINFO_DEBUG overrides debug level (-1 = None, 0 = Info, 1 = Debug, 2 = Verbose)

## Building

```
mkdir build
cd build
cmake ..
```

## c++ api

The api is fairly simple, see the headers/deviceinfo.h for full api. At the moment there is no c wrapper, this is planned but not needed for us yet. 


Example: 
```
std::shared_ptr<DeviceInfo> info = std::make_shared<DeviceInfo>();
std::cout << info->name() << std::endl;
```
