# Ping Protocol

[![Travis Build Status](https://travis-ci.org/bluerobotics/ping-protocol-cpp.svg?branch=dev)](https://travis-ci.org/bluerobotics/ping-protocol-cpp) [![Build status](https://ci.appveyor.com/api/projects/status/3bvhlq9wn3ioj6o7?svg=true)](https://ci.appveyor.com/project/blueroboticsappveyor/ping-protocol-cpp) [![Last Releases](https://img.shields.io/github/release/bluerobotics/ping-protocol-cpp.svg)](https://github.com/bluerobotics/ping-protocol-cpp/releases) [![Discuss](https://img.shields.io/badge/discuss-online-green.svg)](https://discuss.bluerobotics.com/)

## Table of Contents
* [Format](#format)
* [ping1D Messages](#ping1D-messages)
  * [general](#general)
    * [undefined](#0-undefined)
    * [ack](#1-ack)
    * [nack](#2-nack)
    * [ascii_text](#3-ascii_text)
  * [set](#set)
    * [set_device_id](#1000-set_device_id)
    * [set_range](#1001-set_range)
    * [set_speed_of_sound](#1002-set_speed_of_sound)
    * [set_mode_auto](#1003-set_mode_auto)
    * [set_ping_rate](#1004-set_ping_rate)
    * [set_gain_index](#1005-set_gain_index)
    * [set_ping_enable](#1006-set_ping_enable)
  * [get](#get)
    * [fw_version](#1200-fw_version)
    * [device_id](#1201-device_id)
    * [voltage_5](#1202-voltage_5)
    * [speed_of_sound](#1203-speed_of_sound)
    * [range](#1204-range)
    * [mode_auto](#1205-mode_auto)
    * [ping_rate](#1206-ping_rate)
    * [gain_index](#1207-gain_index)
    * [pulse_usec](#1208-pulse_usec)
    * [general_info](#1210-general_info)
    * [distance_simple](#1211-distance_simple)
    * [distance](#1212-distance)
    * [processor_temperature](#1213-processor_temperature)
    * [pcb_temperature](#1214-pcb_temperature)
    * [profile](#1300-profile)
    * [protocol_version](#5-protocol_version)
  * [control](#control)
    * [goto_bootloader](#1100-goto_bootloader)
    * [continuous_start](#1400-continuous_start)
    * [continuous_stop](#1401-continuous_stop)

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

## ping1D Messages

### general

#### 0 undefined

No payload.

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
| char[var] | nack_msg | ASCII text message indicating NACK condition. |  |

#### 3 ascii_text
A message for transmitting text data.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| char[var] | msg |  NULL terminated ASCII text message. |  |


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

#### 1004 set_ping_rate
The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | ping_rate | The interval between acoustic measurements. | ms |

#### 1005 set_gain_index
Set the current gain selection.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | gain_index | 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

#### 1006 set_ping_enable
Enable or disable acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | enable | 0: Disable, 1: Enable. |  |


### get

#### 1200 fw_version
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device type. 0: 1D Echosounder |  |
| u8 | device_model | Device model. 0: Ping1D |  |
| u16 | fw_version_major | Firmware version major number. |  |
| u16 | fw_version_minor | Firmware version minor number. |  |

#### 1201 device_id
The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 1202 voltage_5
The 5V rail voltage.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | mvolts | The 5V rail voltage. | mV |

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

#### 1206 ping_rate
The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | ping_rate | The interval between acoustic measurements. | ms |

#### 1207 gain_index
The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_index | 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

#### 1208 pulse_usec
The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pulse_usec | Acoustic pulse duration. | microseconds |

#### 1210 general_info
General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | fw_version_major | Firmware major version. |  |
| u16 | fw_version_minor | Firmware minor version. |  |
| u16 | mvolts | Device supply voltage. | mV |
| u16 | ping_rate | The interval between acoustic measurements. | ms |
| u8 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u8 | mode_auto | The current operating mode of the device. 0: manual mode, 1: auto mode |  |

#### 1211 distance_simple
The distance to target with confidence estimate.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | Distance to the target. | mm |
| u8 | confidence | Confidence in the distance measurement. | % |

#### 1212 distance

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | pulse_usec | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

#### 1213 processor_temperature
Temperature of the device cpu.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | temp | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

#### 1214 pcb_temperature
Temperature of the on-board thermistor.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | temp | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

#### 1300 profile
A profile produced from a single acoustic measurement. The data returned is an array of response strength at even intervals across the scan region. The scan region defined the region between <scan_start> and <scan_start + scan_length> millimeters away from the transducer. A distance measurement to the target is also provided.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | pulse_usec | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u16 | num_points | The number of data points for the profile. (The length of the proceeding array) |  |
| u8[200] | data | An array of return strength measurements taken at regular intervals across the scan region. |  |

#### 5 protocol_version
The protocol version

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | protocol_version |  |  |


### control

#### 1100 goto_bootloader
Send the device into the booloader. This is useful for firmware updates.

No payload.

#### 1400 continuous_start
Command to initiate continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stream. 0: profile, 1: full_profile, 2: raw_data |  |

#### 1401 continuous_stop
Command to stop the continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stream. 0: profile, 1: full_profile, 2: raw_data |  |

