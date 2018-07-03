# Ping Protocol

## Table of Contents
* [Format](#format)
* [ping1D Messages](#ping1D-messages)
  * [undefined](#0-undefined)
  * [ack](#1-ack)
  * [nack](#2-nack)
  * [ascii_text](#3-ascii_text)
  * [set_device_id](#1000-set_device_id)
  * [set_range](#1001-set_range)
  * [set_speed_of_sound](#1002-set_speed_of_sound)
  * [set_auto_manual](#1003-set_auto_manual)
  * [set_ping_rate_msec](#1004-set_ping_rate_msec)
  * [set_gain_index](#1005-set_gain_index)
  * [set_ping_enable](#1006-set_ping_enable)
  * [goto_bootloader](#1100-goto_bootloader)
  * [fw_version](#1200-fw_version)
  * [device_id](#1201-device_id)
  * [voltage_5](#1202-voltage_5)
  * [speed_of_sound](#1203-speed_of_sound)
  * [range](#1204-range)
  * [mode](#1205-mode)
  * [ping_rate_msec](#1206-ping_rate_msec)
  * [gain_index](#1207-gain_index)
  * [pulse_usec](#1208-pulse_usec)
  * [background_data](#1209-background_data)
  * [general_info](#1210-general_info)
  * [distance_simple](#1211-distance_simple)
  * [distance](#1212-distance)
  * [processor_temperature](#1213-processor_temperature)
  * [pcb_temperature](#1214-pcb_temperature)
  * [profile](#1300-profile)
  * [full_profile](#1301-full_profile)
  * [raw_data](#1302-raw_data)
  * [continuous_start](#1400-continuous_start)
  * [continuous_stop](#1401-continuous_stop)

## Format

| Byte | Type | Name             | Description                                                      |
|------|------|------------------|------------------------------------------------------------------|
| 0 | u8 | start1 | 'B' |
| 1 | u8 | start2 | 'R' |
| 2-3 | u16 | length | Number of bytes in payload. |
| 4-5 | u16 | message_id | The message id. |
| 6 | u8 | src_device_id | The device ID of the device sending the message. |
| 7 | u8 | dst_device_id | The device ID of the intended recipient of the message. |
| 8-n | u8[] | payload | The message payload. |
| (n+1)-(n+2) | u16 | checksum | The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message. |

## ping1D Messages


### 0 undefined
No payload

### 1 ack

Acknowledged.

No payload

### 2 nack

Not acknowledged.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id_to_nack | The message ID being NACKed. |  |
| u8[var] | err_msg | ASCII text message. |  |

### 3 ascii_text

A message for transmitting text data.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8[var] | msg | ASCII text message. |  |

### 1000 set_device_id

Set the device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | Device ID (0-254). 255 is reserved for broadcast messages. |  |

### 1001 set_range

Set the scan range for acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | start_mm |  | mm |
| u32 | length_mm | The length of the scan range. | mm |

### 1002 set_speed_of_sound

Set the speed of sound used for distance calculations.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | speed | The speed of sound in the measurement medium. ~1,500,000 mm/s for water. | mm/s |

### 1003 set_auto_manual

Set automatic or manual mode. Manual mode allows for manual selection of the gain and scan range.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | mode | 0: auto mode. 1: manual mode. |  |

### 1004 set_ping_rate_msec

The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | rate_msec | The interval between acoustic measurements. | ms |

### 1005 set_gain_index

Set the current gain selection.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | index | 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

### 1006 set_ping_enable

Enable or disable acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | enable | 0: Disable, 1: Enable. |  |

### 1100 goto_bootloader

Send the device into the booloader. This is useful for firmware updates.

No payload

### 1200 fw_version

Device information

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_type | Device type. 0: 1D Echosounder |  |
| u8 | device_model | Device model. 0: Ping1D |  |
| u16 | fw_version_major | Firmware version major number. |  |
| u16 | fw_version_minor | Firmware version minor number. |  |

### 1201 device_id

The device ID.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | device_id | The device ID (0-254). 255 is reserved for broadcast messages. |  |

### 1202 voltage_5

The 5V rail voltage.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | mvolts | The 5V rail voltage. | mV |

### 1203 speed_of_sound

The speed of sound used for distance calculations.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | speed_mmps | The speed of sound. | mm/s |

### 1204 range

The scan range for acoustic measurements. Measurements returned by the device will lie in the range (start_mm, start_mm + length_mm).

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | start_mm | The beginning of the scan range in mm from the transducer. | mm |
| u32 | length_mm | The length of the scan range. | mm |

### 1205 mode

The current operating mode of the device. Manual mode allows for manual selection of the gain and scan range.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u8 | auto_manual | 0: auto mode, 1: manual mode |  |

### 1206 ping_rate_msec

The interval between acoustic measurements.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | msec_per_ping | The interval between acoustic measurements. | ms |

### 1207 gain_index

The current gain setting.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | gain_index | 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

### 1208 pulse_usec

The duration of the acoustic activation/transmission.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | pulse_usec | Acoustic pulse duration. | microseconds |

### 1209 background_data

Background Data. **This message has incomplete documentation.**

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | depth_mm | The current distance to target. | mm |
| u16 | milli_confidence |  |  |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u32 | range_mm |  |  |
| u32 | rms_goertzel_noise |  |  |

### 1210 general_info

General information.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | vers_major | Firmware major version. |  |
| u16 | vers_minor | Firmware minor version. |  |
| u16 | mvolts | Device supply voltage. | mV |
| u16 | msec_per_ping | The interval between acoustic measurements. | ms |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u8 | is_auto | The current operating mode of the device. 0: manual mode, 1: auto mode |  |

### 1211 distance_simple

The distance to target with confidence estimate.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | Distance to the target. | mm |
| u8 | confidence | Confidence in the distance measurement. | % |

### 1212 distance
| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | pulse_usec | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | start_mm | The beginning of the scan region in mm from the transducer. | mm |
| u32 | length_mm | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |

### 1213 processor_temperature

Temperature of the device cpu.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | temp | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

### 1214 pcb_temperature

Temperature of the on-board thermistor.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | temp | The temperature in centi-degrees Centigrade (100 * degrees C). | cC |

### 1300 profile

A profile produced from a single acoustic measurement. The data returned is an array of response strength at even intervals across the scan region. The scan region defined the region between <start_mm> and <start_mm + length_mm> millimeters away from the transducer. A distance measurement to the target is also provided.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | distance | The current return distance determined for the most recent acoustic measurement. | mm |
| u16 | confidence | Confidence in the most recent range measurement. | % |
| u16 | pulse_usec | The acoustic pulse length during acoustic transmission/activation. | us |
| u32 | ping_number | The pulse/measurement count since boot. |  |
| u32 | start_mm | The beginning of the scan region in mm from the transducer. | mm |
| u32 | length_mm | The length of the scan region. | mm |
| u32 | gain_index | The current gain setting. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u16 | num_points | The number of data points for the profile. (The length of the proceeding array) |  |
| u8[200] | data | An array of return strength measurements taken at regular intervals across the scan region. |  |

### 1301 full_profile

Full Profile. **The documentation for this message is incomplete.**

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| i32 | this_ping_depth_mm | The current return distance determined for the most recent acoustic measurement. | mm |
| i32 | smoothed_depth_mm | The filtered return distance. | mm |
| i8 | smoothed_depth_confidence_percent | Filtered confidence in range measurement. |  |
| i8 | this_ping_confidence_percent | Confidence in the most recent range measurement. |  |
| i16 | ping_duration_usec | The acoustic pulse length during transmission/activation. | us |
| i32 | ping_number | The pulse/measurement count since boot. |  |
| u16 | supply_millivolts | Device supply voltage. | mV |
| u16 | degC | Device temperature. | C |
| i32 | start_mm | The beginning of the scan region in mm from the transducer. | mm |
| i32 | length_mm | The length of the scan region. | mm |
| i32 | y0_mm |  |  |
| i32 | yn_mm |  |  |
| i32 | gain_index | The current gain index. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| u32 | outlier_bits |  |  |
| i16 | index_of_bottom_result |  |  |
| i16 | num_results |  |  |
| u8[200] | results |  |  |

### 1302 raw_data

Raw data message. **The documentation for this message is incomplete.**

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u32 | v_major | Device firmware major version. |  |
| u32 | v_minor | Device firmware minor version. |  |
| u16 | supply_millivolts | Device supply voltage. | mV |
| u16 | degC | Device temperature. | C |
| u32 | gain_index | The current gain index. 0: 0.6dB, 1: 1.8dB, 2: 5.5dB, 3: 12.9dB, 4: 30.2dB, 5: 66.1dB, 6: 144dB |  |
| i32 | start_mm | The beginning of the scan region in mm from the transducer. | mm |
| i32 | length_mm | The length of the scan region. | mm |
| u32 | num_samples |  |  |
| u32 | ping_usec | The acoustic pulse length during transmission/activation. | us |
| u32 | ping_hz | The frequency of acoustic measurements. | Hz |
| u32 | adc_sample_hz | The ADC sample frequency. | Hz |
| u32 | ping_num | Ping count since boot. |  |
| u32 | rms_goertzel_noise | RMS Goertzel noise. |  |

### 1400 continuous_start

Command to initiate continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stream. 0: profile, 1: full_profile, 2: raw_data |  |

### 1401 continuous_stop

Command to stop the continuous data stream of profile messages.

| Type | Name             | Description                                                      | Units |
|------|------------------|------------------------------------------------------------------|-------|
| u16 | id | The message id to stream. 0: profile, 1: full_profile, 2: raw_data |  |
