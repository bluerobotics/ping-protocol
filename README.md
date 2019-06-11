# Ping Protocol

[![Travis Build Status](https://travis-ci.org/bluerobotics/ping-protocol.svg?branch=master)](https://travis-ci.org/bluerobotics/ping-protocol) [![Build status](https://ci.appveyor.com/api/projects/status/9wodlqle7wqvhiyn/branch/master?svg=true)](https://ci.appveyor.com/project/blueroboticsappveyor/ping-protocol/branch/master) [![Last Releases](https://img.shields.io/github/release/bluerobotics/ping-protocol.svg)](https://github.com/bluerobotics/ping-protocol/releases) [![Discuss](https://img.shields.io/badge/discuss-online-green.svg)](https://discuss.bluerobotics.com/)

## Table of Contents
* [Format](#format)
* [Common Messages](#Common-messages)
  * [general](##Common-general)
    * [ack](#1-ack)
    * [nack](#2-nack)
    * [ascii_text](#3-ascii_text)
    * [general_request](#6-general_request)
  * [get](##Common-get)
    * [device_information](#4-device_information)
    * [protocol_version](#5-protocol_version)
* [Ping1D Messages](#Ping1D-messages)
  * [set](##Ping1D-set)
    * [set_device_id](#1000-set_device_id)
    * [set_range](#1001-set_range)
    * [set_speed_of_sound](#1002-set_speed_of_sound)
    * [set_mode_auto](#1003-set_mode_auto)
    * [set_ping_interval](#1004-set_ping_interval)
    * [set_gain_setting](#1005-set_gain_setting)
    * [set_ping_enable](#1006-set_ping_enable)
  * [get](##Ping1D-get)
    * [firmware_version](#1200-firmware_version)
    * [device_id](#1201-device_id)
    * [voltage_5](#1202-voltage_5)
    * [speed_of_sound](#1203-speed_of_sound)
    * [range](#1204-range)
    * [mode_auto](#1205-mode_auto)
    * [ping_interval](#1206-ping_interval)
    * [gain_setting](#1207-gain_setting)
    * [transmit_duration](#1208-transmit_duration)
    * [general_info](#1210-general_info)
    * [distance_simple](#1211-distance_simple)
    * [distance](#1212-distance)
    * [processor_temperature](#1213-processor_temperature)
    * [pcb_temperature](#1214-pcb_temperature)
    * [ping_enable](#1215-ping_enable)
    * [profile](#1300-profile)
  * [control](##Ping1D-control)
    * [goto_bootloader](#1100-goto_bootloader)
    * [continuous_start](#1400-continuous_start)
    * [continuous_stop](#1401-continuous_stop)
* [Ping360 Messages](#Ping360-messages)
  * [set](##Ping360-set)
    * [device_id](#2000-device_id)
  * [get](##Ping360-get)
    * [device_data](#2300-device_data)
  * [control](##Ping360-control)
    * [reset](#2600-reset)
    * [transducer](#2601-transducer)

## Format

| Byte | Type | Name             | Description                                                      |
|------|------|------------------|------------------------------------------------------------------|
| 0 | u8 | start1 | 'B' |
| 1 | u8 | start2 | 'R' |
| 2-3 | u16 | payload_length | Number of bytes in payload. |
| 4-5 | u16 | message_id | The message id. |
| 6 | u8 | src_device_id | The device ID of the device sending the message. |
| 7 | u8 | dst_device_id | The device ID of the intended recipient of the message. |
| 8-n | u8[] | payload | The message payload. |
| (n+1)-(n+2) | u16 | checksum | The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message. |

## Scheme

Messages are divided into 4 categories:

general: messages that are used for general purpose signalling and communication.
read/get: messages that are sent from the device in response to a [request message] from the host. These messages are designed to read data from the device.
write/set: messages that are sent from the host to configure some parameters on the device. These messages are designed to write data to the device.
control: messages that are sent from the host to command the device to perform some action. These messages are designed to perform more complex device interactions than atomic read/write.

There are a some messages that are implemented by all devices, referred to as the 'common' message set. Message ids # 0~999 are reserved for the common messages. The _request_ message is a special message in the common set that is used to request the device to respond with a message from the _get_ category. Each device must also define it's own message set specific to the operation of the particular device.

### Device discovery

If necessary, Ping Protocol enabled devices may be discovered and identified by the host as follows:

1. The host first requests the protocol version from the device
1. After receiving a protocol version reply, the host switches to the matching protocol version and requests the device type and firmware version
1. The host then loads the appropriate message set matching the device
1. Communication may then continue using the device-specific messages

## Negotiation example

Here we demonstrate a byte-by-byte breakdown of some messages sent between the host application (master) and the device (slave). This example illustrates a few points:
  - how to pack and unpack some message data (the byte-order of a message)
  - how the request/response mechanism of the protcol works with the `general_request` message
  - how to identify the protocol version that the device is using

Establishing communication with a sensor using the [ping-protocol](github.com/bluerobotics/ping-protocol) should begin with [negotiating the protocol version](#device-discovery). This negotiation process consists of two steps:
  - the host application requests a [`protocol_version`](#5-protocol_version) message from the device
  - the device responds with a [`protocol_version`](#5-protocol_version) message

> Before reading these examples, you should be familiar with the [message format specification](#format).

### Request protocol version

In order to receive a [`protocol_version`](#5-protocol_version) message from the device, we will first send a [`general_request`](#6-general_request) message to the device to ask for the message. The [`general_request`](#6-general_request) message has a single payload field, `requested_id`. We populate this field with a value of 5 to indicate that we want the sensor to respond with a `protocol_version` message:

| Byte | Value (hex) | Value (decimal) | Type | Name | Description |
|------|-------------|-----------------|------|------|-------------|
| 0    | 0x42 ('B')  | 66 | u8 | start1 | This is a message start identifier, an ascii letter 'B' |
| 1    | 0x52 ('R')  | 82 | u8 | start2 | This is a message start identifier, an ascii letter 'R' |
| 2-3  | 0x0002      | 2 | u16 | payload_length | The number of bytes in the [`general_request`](#6-general_request) payload |
| 4-5  | 0x0006      | 6 | u16 | message_id | This message is a [`general_request`](#6-general_request) (id #6) message |
| 6    | 0x00        | 0 | u8  | src_device_id | The device ID of the device sending the message. This field is not currently implemented and should be populated with a value of zero |
| 7    | 0x00        | 0 | u8  | dst_device_id | The device ID of the intended recipient of the message. This portion is not currently implemented and should be populated with a value of zero |
| 8-9  | 0x0005      | 5 | u16 | requested_id | This is the message id that we would like the device to transmit to us. Valid ids are those in the [`get` category](#get) of messages |
| 10-11 | 0x00a1     | 161 | u16 | checksum | The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message: `66 + 82 + 2 + 6 + 0 + 0 + 5 = 161` |

The bytes should be transmitted in this order:
`0x42, 0x52, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0xa1, 0x00`

> Note that the messages are transmitted in **little-endian** format (observe the byte-order of the 16 bit fields)

### Receive protocol version

If everything is right, the sensor will respond to our request with a [`protocol_version`](#5-protocol_version) message. The [`protocol_version`](#5-protocol_version) message has 4 bytes in the payload: `version_major`, `version_minor`, `version_patch`, and `reserved`. 

In this example, the device is using (a hypothetical) protocol version 1.2.3:
  - version_major = 1
  - version_minor = 2
  - version_patch = 3

| Byte | Value (hex) | Value (decimal) | Type | Name | Description |
|------|-------------|-----------------|------|------|-------------|
| 0    | 0x42 ('B')  | 66 | u8 | start1 | This is a message start identifier, an ascii letter 'B' |
| 1    | 0x52 ('R')  | 82 | u8 | start2 | This is a message start identifier, an ascii letter 'R' |
| 2-3  | 0x0004      | 4 | u16 | payload_length | The number of bytes in the [`protocol_version`](#5-protocol_version) payload |
| 4-5  | 0x0005      | 5 | u16 | message_id | This message is a [`protocol_version`](#5-protocol_version) (id #5) message |
| 6    | 0x00        | 0 | u8 | src_device_id | The device ID of the device sending the message. This field is not currently implemented and should be populated with a value of zero |
| 7    | 0x00        | 0 | u8 | dst_device_id | The device ID of the intended recipient of the message. This field is not currently implemented and should be populated with a value of zero |
| 8    | 0x01        | 1 | u8 | version_major | This is the protocol _major_ version, the first digit in our example: v**1**.2.3 |
| 9    | 0x02        | 2 | u8 | version_minor | This is the protocol _minor_ version, the second digit in our example: v1.**2**.3 |
| 10   | 0x03        | 3 | u8 | version_patch | This is the protocol _patch_ version, the third digit in our example: v1.2.**3** |
| 11   | 0x00        | 0 | u8 | reserved | This byte is unused and will normally be zero (but it might be any value) |
| 12-13 | 0x00a3     | 163 |u16 | checksum |  The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message: `66 + 82 + 4 + 5 + 0 + 0 + 1 + 2 + 3 + 0 = 163` |

The bytes will be received in this order:
`0x42, 0x52, 0x04, 0x00, 0x05, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x00, 0xa3, 0x00`

> Note that the messages are transmitted in **little-endian** format (observe the byte-order of the 16 bit fields)

## Common Messages

### general

#### 1 ack
Acknowledged.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | acked_id | The message ID that is ACKnowledged. |  |

#### 2 nack
Not acknowledged.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | nacked_id | The message ID that is Not ACKnowledged. |  |
| char[] | nack_message | ASCII text message indicating NACK condition. (not necessarily NULL terminated) Length is derived from payload_length in the header. |  |

#### 3 ascii_text
A message for transmitting text data.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| char[] | ascii_message | ASCII text message. (not necessarily NULL terminated) Length is derived from payload_length in the header. |  |

#### 6 general_request
Requests a specific message to be sent from the sonar to the host. Command timeout should be set to 50 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | requested_id | Message ID to be requested. |  |


### get

#### 4 device_information
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device type. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u8 | device_revision | device-specific hardware revision |  |
| u8 | firmware_version_major | Firmware version major number. |  |
| u8 | firmware_version_minor | Firmware version minor number. |  |
| u8 | firmware_version_patch | Firmware version patch number. |  |
| u8 | reserved | reserved |  |

#### 5 protocol_version
The protocol version

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | version_major | Protocol version major number. |  |
| u8 | version_minor | Protocol version minor number. |  |
| u8 | version_patch | Protocol version patch number. |  |
| u8 | reserved | reserved |  |

## Ping1D Messages

### set

#### 1000 set_device_id
Set the device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | Device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 1001 set_range
Set the scan range for acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | scan_start |  | mm |
| u32 | scan_length | The length of the scan range. | mm |

#### 1002 set_speed_of_sound
Set the speed of sound used for distance calculations.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | speed_of_sound | The speed of sound in the measurement medium. ~1,500,000 mm/s for water. | mm/s |

#### 1003 set_mode_auto
Set automatic or manual mode. Manual mode allows for manual selection of the gain and scan range.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode_auto | 0: manual mode. 1: auto mode. |  |

#### 1004 set_ping_interval
The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | ping_interval | The interval between acoustic measurements. | ms |

#### 1005 set_gain_setting
Set the current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | gain_setting | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1006 set_ping_enable
Enable or disable acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | ping_enabled | 0: Disable, 1: Enable. |  |


### get

#### 1200 firmware_version
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device type. 0: Unknown; 1: Echosounder |  |
| u8 | device_model | Device model. 0: Unknown; 1: Ping1D |  |
| u16 | firmware_version_major | Firmware version major number. |  |
| u16 | firmware_version_minor | Firmware version minor number. |  |

#### 1201 device_id
The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 1202 voltage_5
The 5V rail voltage.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | voltage_5 | The 5V rail voltage. | mV |

#### 1203 speed_of_sound
The speed of sound used for distance calculations.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | speed_of_sound | The speed of sound in the measurement medium. ~1,500,000 mm/s for water. | mm/s |

#### 1204 range
The scan range for acoustic measurements. Measurements returned by the device will lie in the range (scan_start, scan_start + scan_length).

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | scan_start | The beginning of the scan range in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan range. | mm |

#### 1205 mode_auto
The current operating mode of the device. Manual mode allows for manual selection of the gain and scan range.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode_auto | 0: manual mode, 1: auto mode |  |

#### 1206 ping_interval
The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | ping_interval | The minimum interval between acoustic measurements. The actual interval may be longer. | ms |

#### 1207 gain_setting
The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_setting | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1208 transmit_duration
The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | transmit_duration | Acoustic pulse duration. | microseconds |

#### 1210 general_info
General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | firmware_version_major | Firmware major version. |  |
| u16 | firmware_version_minor | Firmware minor version. |  |
| u16 | voltage_5 | Device supply voltage. | mV |
| u16 | ping_interval | The interval between acoustic measurements. | ms |
| u8 | gain_setting | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
| u8 | mode_auto | The current operating mode of the device. 0: manual mode, 1: auto mode |  |

#### 1211 distance_simple
The distance to target with confidence estimate.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | Distance to the target. | mm |
| u8 | confidence | Confidence in the distance measurement. | % |

#### 1212 distance
The distance to target with confidence estimate. Relevant device parameters during the measurement are also provided.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | transmit_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_setting | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1213 processor_temperature
Temperature of the device cpu.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | processor_temperature | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

#### 1214 pcb_temperature
Temperature of the on-board thermistor.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pcb_temperature | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

#### 1215 ping_enable
Acoustic output enabled state.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | ping_enabled | The state of the acoustic output. 0: disabled, 1:enabled |  |

#### 1300 profile
A profile produced from a single acoustic measurement. The data returned is an array of response strength at even intervals across the scan region. The scan region is defined as the region between <scan_start> and <scan_start + scan_length> millimeters away from the transducer. A distance measurement to the target is also provided.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | transmit_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_setting | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
| u16 | profile_data_length | The length of the proceeding vector field | |
| u8[] | profile_data | An array of return strength measurements taken at regular intervals across the scan region. |  |


### control

#### 1100 goto_bootloader
Send the device into the bootloader. This is useful for firmware updates.

No payload.

#### 1400 continuous_start
Command to initiate continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stream. 1300: profile |  |

#### 1401 continuous_stop
Command to stop the continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stop streaming. 1300: profile |  |

## Ping360 Messages

### set

#### 2000 device_id
Change the device id

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | id | Device ID (1-254). 0 and 255 are reserved. |  |
| u8 | reserved | reserved |  |


### get

#### 2300 device_data
This message is used to communicate the current sonar state. If the data field is populated, the other fields indicate the sonar state when the data was captured. The time taken before the response to the command is sent depends on the difference between the last angle scanned and the new angle in the parameters as well as the number of samples and sample interval (range). To allow for the worst case reponse time the command timeout should be set to 4000 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for Ping360) |  |
| u8 | gain_setting | Analog gain setting (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle | gradian |
| u16 | transmit_duration | Acoustic transmission duration (1~1000 microseconds) | microsecond |
| u16 | sample_period | Time interval between individual signal intensity samples in 25nsec increments (80 to 40000 == 2 microseconds to 1000 microseconds) |  |
| u16 | transmit_frequency | Acoustic operating frequency. Frequency range is 500kHz to 1000kHz, however it is only practical to use say 650kHz to 850kHz due to the narrow bandwidth of the acoustic receiver. | kHz |
| u16 | number_of_samples | Number of samples per reflected signal |  |
| u16 | data_length | The length of the proceeding vector field | |
| u8[] | data | 8 bit binary data array representing sonar echo strength |  |


### control

#### 2600 reset
Reset the sonar. The bootloader may run depending on the selection according to the `bootloader` payload field. When the bootloader runs, the external LED flashes at 5Hz. If the bootloader is not contacted within 5 seconds, it will run the current program. If there is no program, then the bootloader will wait forever for a connection. Note that if you issue a reset then you will have to close all your open comm ports and go back to issuing either a discovery message for UDP or go through the break sequence for serial comms before you can talk to the sonar again.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | bootloader | 0 = skip bootloader; 1 = run bootloader |  |
| u8 | reserved | reserved |  |

#### 2601 transducer
The transducer will apply the commanded settings. The sonar will reply with a `ping360_data` message. If the `transmit` field is 0, the sonar will not transmit after locating the transducer, and the `data` field in the `ping360_data` message reply will be empty. If the `transmit` field is 1, the sonar will make an acoustic transmission after locating the transducer, and the resulting data will be uploaded in the `data` field of the `ping360_data` message reply. To allow for the worst case reponse time the command timeout should be set to 4000 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for Ping360) |  |
| u8 | gain_setting | Analog gain setting (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle | gradian |
| u16 | transmit_duration | Acoustic transmission duration (1~1000 microseconds) | microsecond |
| u16 | sample_period | Time interval between individual signal intensity samples in 25nsec increments (80 to 40000 == 2 microseconds to 1000 microseconds) |  |
| u16 | transmit_frequency | Acoustic operating frequency. Frequency range is 500kHz to 1000kHz, however it is only practical to use say 650kHz to 850kHz due to the narrow bandwidth of the acoustic receiver. | kHz |
| u16 | number_of_samples | Number of samples per reflected signal |  |
| u8 | transmit | 0 = do not transmit; 1 = transmit after the transducer has reached the specified angle |  |
| u8 | reserved | reserved |  |

