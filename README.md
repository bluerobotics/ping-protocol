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
    * [protocol_version](#5-protocol_version)
* [Ping1D Messages](#Ping1D-messages)
  * [set](##Ping1D-set)
    * [set_device_id](#1000-set_device_id)
    * [set_range](#1001-set_range)
    * [set_speed_of_sound](#1002-set_speed_of_sound)
    * [set_mode_auto](#1003-set_mode_auto)
    * [set_ping_interval](#1004-set_ping_interval)
    * [set_gain_index](#1005-set_gain_index)
    * [set_ping_enable](#1006-set_ping_enable)
  * [get](##Ping1D-get)
    * [firmware_version](#1200-firmware_version)
    * [device_id](#1201-device_id)
    * [voltage_5](#1202-voltage_5)
    * [speed_of_sound](#1203-speed_of_sound)
    * [range](#1204-range)
    * [mode_auto](#1205-mode_auto)
    * [ping_interval](#1206-ping_interval)
    * [gain_index](#1207-gain_index)
    * [pulse_duration](#1208-pulse_duration)
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
    * [parameters](#2000-parameters)
    * [device_id](#2001-device_id)
  * [get](##Ping360-get)
    * [device_id](#2300-device_id)
    * [firmware_version](#2301-firmware_version)
    * [parameters](#2302-parameters)
    * [upload_data](#2303-upload_data)
    * [motor_position](#2304-motor_position)
  * [control](##Ping360-control)
    * [goto_bootloader](#2600-goto_bootloader)
    * [reset](#2601-reset)
    * [step_forward](#2602-step_forward)
    * [step_reverse](#2603-step_reverse)
    * [reset_position](#2604-reset_position)
    * [motor_off](#2605-motor_off)

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

#### 5 protocol_version
The protocol version

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | protocol_version | The protocol version |  |



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

#### 5 protocol_version
The protocol version

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | protocol_version | The protocol version |  |



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

#### 1005 set_gain_index
Set the current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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

#### 1207 gain_index
The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1208 pulse_duration
The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pulse_duration | Acoustic pulse duration. | microseconds |

#### 1210 general_info
General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | firmware_version_major | Firmware major version. |  |
| u16 | firmware_version_minor | Firmware minor version. |  |
| u16 | voltage_5 | Device supply voltage. | mV |
| u16 | ping_interval | The interval between acoustic measurements. | ms |
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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

#### 1005 set_gain_index
Set the current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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

#### 1207 gain_index
The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1208 pulse_duration
The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pulse_duration | Acoustic pulse duration. | microseconds |

#### 1210 general_info
General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | firmware_version_major | Firmware major version. |  |
| u16 | firmware_version_minor | Firmware minor version. |  |
| u16 | voltage_5 | Device supply voltage. | mV |
| u16 | ping_interval | The interval between acoustic measurements. | ms |
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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

#### 1005 set_gain_index
Set the current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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

#### 1207 gain_index
The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

#### 1208 pulse_duration
The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pulse_duration | Acoustic pulse duration. | microseconds |

#### 1210 general_info
General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | firmware_version_major | Firmware major version. |  |
| u16 | firmware_version_minor | Firmware minor version. |  |
| u16 | voltage_5 | Device supply voltage. | mV |
| u16 | ping_interval | The interval between acoustic measurements. | ms |
| u8 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |

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
| u16 | pulse_duration | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | scan_start | The beginning of the scan region in mm from the transducer. | mm |
| u32 | scan_length | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6, 1: 1.8, 2: 5.5, 3: 12.9, 4: 30.2, 5: 66.1, 6: 144 |  |
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



### set

#### 2000 parameters
This is the main parameter block.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 decimal == 1 microsecond to 1000 microseconds) |  |

#### 2001 device_id
Sets the device ID that will be reported by requesting a 'device_id' message from the sonar. Command timeout should be set to 50 msec. 

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | id | 1-254 are valid values (default value is 2 for Ping360) |  |


### get

#### 2300 device_id
The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 2301 firmware_version
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u8 | device_model | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u16 | firmware_version_major | Firmware version major number. |  |
| u16 | firmware_version_minor | Firmware version minor number. |  |

#### 2302 parameters
The Parameters message is sent from the sonar to the host in response to a request from the host.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |

#### 2303 upload_data
The Upload Data message is sent by the sonar to the host upon request. The sonar returns the parameter block sent as part of the command to verify how the raw data was captured. The time taken before the response to the command is sent depends on the difference between the last angle scanned and the new angle in the parameters as well as the number of samples and sample interval (range). To allow for the worst case reponse time the command timeout should be set to 4000 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |
| u16 | num_points | Number of data points in proceeding array |  |
| u16 | data_length | The length of the proceeding vector field | |
| u8[] | data | 8 bit binary data array representing sonar echo strength |  |

#### 2304 motor_position
The Motor Position message is sent from the sonar to the host. The sonar returns the current transducer head position in units of gradians, i.e., 400 steps in 360°.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | angle | Head angle in gradians (0 to 399 = 0 to 360 degrees) |  |


### control

#### 2600 goto_bootloader
The Goto Bootloader command is sent from the host to the sonar and puts the SS1 sonar in Bootloader mode (external LED flashes at 5Hz), effectively a `cold boot`. If the user does not connect the bootloader program within 3 seconds then the SS1 will attempt to resume executing it's current code. If there is no user code loaded in the sonar then the bootloader will wait indefinitely for a connection to the bootloader program. Note:- There is no response message from the sonar to this command 

No payload.

#### 2601 reset
The Reset command is sent from the host to the sonar and restarts the SS1 sonar without going through the bootloader sequence, effectively a `warm boot`. Note:- This command will be acknowledged with an error code of 0 before the sonar is reset. The command timeout should be set to 50 milliseconds.

No payload.

#### 2602 step_forward
The Step Forward command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a +ve direction (400 gradians in 360°). The internal sonar position count will increase by 1, if the current position is 399 the count will wrap around to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2603 step_reverse
The Step Reverse command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a -ve direction. The internal sonar position count will decrease by 1, if the current position is 0 the count will wrap around to 399. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2604 reset_position
The Reset Position command message is sent by the host to the sonar. The sonar first locates the position reference sensor and then moves the transducer head to the zero position. The internal sonar position count will be reset to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 4000 msec.

No payload.

#### 2605 motor_off
The Motor Off command message is sent by the host to the sonar. The sonar switches the current through the stepper motor windings off to save power. When it is required to move the transducer again a Reset Position command should be sent to ensure that the motor position is accurate. Typically when the sonar is paused the motor current would be switched off. When the user sends any command that involves moving the transducer then the motor current is automatically re-enabled. The sonar will send an <Error Code> message of 0 in response. The command timeout should be set to 50 msec.

No payload.



### set

#### 2000 parameters
This is the main parameter block.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 decimal == 1 microsecond to 1000 microseconds) |  |

#### 2001 device_id
Sets the device ID that will be reported by requesting a 'device_id' message from the sonar. Command timeout should be set to 50 msec. 

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | id | 1-254 are valid values (default value is 2 for Ping360) |  |


### get

#### 2300 device_id
The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 2301 firmware_version
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u8 | device_model | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u16 | firmware_version_major | Firmware version major number. |  |
| u16 | firmware_version_minor | Firmware version minor number. |  |

#### 2302 parameters
The Parameters message is sent from the sonar to the host in response to a request from the host.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |

#### 2303 upload_data
The Upload Data message is sent by the sonar to the host upon request. The sonar returns the parameter block sent as part of the command to verify how the raw data was captured. The time taken before the response to the command is sent depends on the difference between the last angle scanned and the new angle in the parameters as well as the number of samples and sample interval (range). To allow for the worst case reponse time the command timeout should be set to 4000 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |
| u16 | num_points | Number of data points in proceeding array |  |
| u16 | data_length | The length of the proceeding vector field | |
| u8[] | data | 8 bit binary data array representing sonar echo strength |  |

#### 2304 motor_position
The Motor Position message is sent from the sonar to the host. The sonar returns the current transducer head position in units of gradians, i.e., 400 steps in 360°.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | angle | Head angle in gradians (0 to 399 = 0 to 360 degrees) |  |


### control

#### 2600 goto_bootloader
The Goto Bootloader command is sent from the host to the sonar and puts the SS1 sonar in Bootloader mode (external LED flashes at 5Hz), effectively a `cold boot`. If the user does not connect the bootloader program within 3 seconds then the SS1 will attempt to resume executing it's current code. If there is no user code loaded in the sonar then the bootloader will wait indefinitely for a connection to the bootloader program. Note:- There is no response message from the sonar to this command 

No payload.

#### 2601 reset
The Reset command is sent from the host to the sonar and restarts the SS1 sonar without going through the bootloader sequence, effectively a `warm boot`. Note:- This command will be acknowledged with an error code of 0 before the sonar is reset. The command timeout should be set to 50 milliseconds.

No payload.

#### 2602 step_forward
The Step Forward command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a +ve direction (400 gradians in 360°). The internal sonar position count will increase by 1, if the current position is 399 the count will wrap around to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2603 step_reverse
The Step Reverse command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a -ve direction. The internal sonar position count will decrease by 1, if the current position is 0 the count will wrap around to 399. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2604 reset_position
The Reset Position command message is sent by the host to the sonar. The sonar first locates the position reference sensor and then moves the transducer head to the zero position. The internal sonar position count will be reset to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 4000 msec.

No payload.

#### 2605 motor_off
The Motor Off command message is sent by the host to the sonar. The sonar switches the current through the stepper motor windings off to save power. When it is required to move the transducer again a Reset Position command should be sent to ensure that the motor position is accurate. Typically when the sonar is paused the motor current would be switched off. When the user sends any command that involves moving the transducer then the motor current is automatically re-enabled. The sonar will send an <Error Code> message of 0 in response. The command timeout should be set to 50 msec.

No payload.



### set

#### 2000 parameters
This is the main parameter block.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 decimal == 1 microsecond to 1000 microseconds) |  |

#### 2001 device_id
Sets the device ID that will be reported by requesting a 'device_id' message from the sonar. Command timeout should be set to 50 msec. 

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | id | 1-254 are valid values (default value is 2 for Ping360) |  |


### get

#### 2300 device_id
The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

#### 2301 firmware_version
Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u8 | device_model | Device model. 0: Unknown; 1: Ping Echosounder; 2: Ping360 |  |
| u16 | firmware_version_major | Firmware version major number. |  |
| u16 | firmware_version_minor | Firmware version minor number. |  |

#### 2302 parameters
The Parameters message is sent from the sonar to the host in response to a request from the host.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |

#### 2303 upload_data
The Upload Data message is sent by the sonar to the host upon request. The sonar returns the parameter block sent as part of the command to verify how the raw data was captured. The time taken before the response to the command is sent depends on the difference between the last angle scanned and the new angle in the parameters as well as the number of samples and sample interval (range). To allow for the worst case reponse time the command timeout should be set to 4000 msec.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | Operating mode (1 for SS1) |  |
| u8 | gain_index | Analog gain index (0 = low, 1 = normal, 2 = high) |  |
| u16 | angle | Head angle in gradians (3 ASCII/Hex digits 0 to 399 decimal = 0 to 360 degrees) |  |
| u16 | pulse_usec | Tx pulse length in microseconds. (1 to 1000) |  |
| u16 | interval | Sample interval in 25nsec clock ticks (40 to 40000 == 1 microsecond to 1000 microseconds) |  |
| u16 | num_points | Number of data points in proceeding array |  |
| u16 | data_length | The length of the proceeding vector field | |
| u8[] | data | 8 bit binary data array representing sonar echo strength |  |

#### 2304 motor_position
The Motor Position message is sent from the sonar to the host. The sonar returns the current transducer head position in units of gradians, i.e., 400 steps in 360°.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | angle | Head angle in gradians (0 to 399 = 0 to 360 degrees) |  |


### control

#### 2600 goto_bootloader
The Goto Bootloader command is sent from the host to the sonar and puts the SS1 sonar in Bootloader mode (external LED flashes at 5Hz), effectively a `cold boot`. If the user does not connect the bootloader program within 3 seconds then the SS1 will attempt to resume executing it's current code. If there is no user code loaded in the sonar then the bootloader will wait indefinitely for a connection to the bootloader program. Note:- There is no response message from the sonar to this command 

No payload.

#### 2601 reset
The Reset command is sent from the host to the sonar and restarts the SS1 sonar without going through the bootloader sequence, effectively a `warm boot`. Note:- This command will be acknowledged with an error code of 0 before the sonar is reset. The command timeout should be set to 50 milliseconds.

No payload.

#### 2602 step_forward
The Step Forward command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a +ve direction (400 gradians in 360°). The internal sonar position count will increase by 1, if the current position is 399 the count will wrap around to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2603 step_reverse
The Step Reverse command message is sent by the host to the sonar. The sonar moves the transducer head one gradian in a -ve direction. The internal sonar position count will decrease by 1, if the current position is 0 the count will wrap around to 399. The sonar will send a `Motor Position` message in response. The command timeout should be set to 50 msec.

No payload.

#### 2604 reset_position
The Reset Position command message is sent by the host to the sonar. The sonar first locates the position reference sensor and then moves the transducer head to the zero position. The internal sonar position count will be reset to 0. The sonar will send a `Motor Position` message in response. The command timeout should be set to 4000 msec.

No payload.

#### 2605 motor_off
The Motor Off command message is sent by the host to the sonar. The sonar switches the current through the stepper motor windings off to save power. When it is required to move the transducer again a Reset Position command should be sent to ensure that the motor position is accurate. Typically when the sonar is paused the motor current would be switched off. When the user sends any command that involves moving the transducer then the motor current is automatically re-enabled. The sonar will send an <Error Code> message of 0 in response. The command timeout should be set to 50 msec.

No payload.

