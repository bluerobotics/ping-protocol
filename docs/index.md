# The protocol

The protocol is designed for synchronous communications in a master->slave network. The slave will only send data when requested by the master. This allows the protocol to be used on a half-duplex bus like RS485. Some devices may also provide facilities to allow asynchronous communication.

## Message Format

Each message consists of a header, optional payload, and checksum. The binary format is specified as follows:

| Byte | Type | Name             | Description                                                      |
|------|------|------------------|------------------------------------------------------------------|
| 0 | u8 | start1 | Start frame identifier, ASCII 'B' |
| 1 | u8 | start2 | Start frame identifier, ASCII 'R' |
| 2-3 | u16 | payload_length | Number of bytes in payload. |
| 4-5 | u16 | message_id | The message id. |
| 6 | u8 | src_device_id | The device ID of the device sending the message. |
| 7 | u8 | dst_device_id | The device ID of the intended recipient of the message. |
| 8-n | u8[] | payload | The message payload. |
| (n+1)-(n+2) | u16 | checksum | The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message. |

## Scheme

Messages are divided into 4 categories:

- general: messages that are used for general purpose signalling and communication.
- read/get: messages that are sent from the device in response to a [`general_request`](pingmessage-common#6-general_request) from the host. These messages are designed to read data from the device.
- write/set: messages that are sent from the host to configure some parameters on the device. These messages are designed to write data to the device.
- control: messages that are sent from the host to command the device to perform some action. These messages are designed to perform more complex device interactions than atomic read/write.

There are a some messages that are implemented by all devices, referred to as the 'common' message set. Message ids # 0~999 are reserved for the common messages. The _request_ message is a special message in the common set that is used to request the device to respond with a message from the _get_ category. Each device must also define it's own message set specific to the operation of the particular device.

### Message Definitions
- [common](pingmessage-common)
- [ping1d](pingmessage-ping1d)
- [ping360](pingmessage-ping360)

### Device Discovery

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

Establishing communication with a sensor using the [ping-protocol](https://github.com/bluerobotics/ping-protocol) should begin with [negotiating the protocol version](#device-discovery). This negotiation process consists of two steps:
  - the host application requests a [`protocol_version`](pingmessage-common#5-protocol_version) message from the device
  - the device responds with a [`protocol_version`](pingmessage-common#5-protocol_version) message

> Before reading these examples, you should be familiar with the [message format specification](#message-format).

### Request protocol version

In order to receive a [`protocol_version`](pingmessage-common#5-protocol_version) message from the device, we will first send a [`general_request`](pingmessage-common#6-general_request) message to the device to ask for the message. The [`general_request`](pingmessage-common#6-general_request) message has a single payload field, `requested_id`. We populate this field with a value of 5 to indicate that we want the sensor to respond with a `protocol_version` message:

| Byte | Value (hex) | Value (decimal) | Type | Name | Description |
|------|-------------|-----------------|------|------|-------------|
| 0    | 0x42 ('B')  | 66 | u8 | start1 | This is a message start identifier, an ascii letter 'B' |
| 1    | 0x52 ('R')  | 82 | u8 | start2 | This is a message start identifier, an ascii letter 'R' |
| 2-3  | 0x0002      | 2 | u16 | payload_length | The number of bytes in the [`general_request`](pingmessage-common#6-general_request) payload |
| 4-5  | 0x0006      | 6 | u16 | message_id | This message is a [`general_request`](pingmessage-common#6-general_request) (id pingmessage-common#6) message |
| 6    | 0x00        | 0 | u8  | src_device_id | The device ID of the device sending the message. This field is not currently implemented and should be populated with a value of zero |
| 7    | 0x00        | 0 | u8  | dst_device_id | The device ID of the intended recipient of the message. This portion is not currently implemented and should be populated with a value of zero |
| 8-9  | 0x0005      | 5 | u16 | requested_id | This is the message id that we would like the device to transmit to us. Valid ids are those in the [`get` category](pingmessage-common#get) of messages |
| 10-11 | 0x00a1     | 161 | u16 | checksum | The message checksum. The checksum is calculated as the sum of all the non-checksum bytes in the message: `66 + 82 + 2 + 6 + 0 + 0 + 5 = 161` |

The bytes should be transmitted in this order:
`0x42, 0x52, 0x02, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05, 0x00, 0xa1, 0x00`

> Note that the messages are transmitted in **little-endian** format (observe the byte-order of the 16 bit fields)

### Receive protocol version

If everything is right, the sensor will respond to our request with a [`protocol_version`](pingmessage-common#5-protocol_version) message. The [`protocol_version`](pingmessage-common#5-protocol_version) message has 4 bytes in the payload: `version_major`, `version_minor`, `version_patch`, and `reserved`.

In this example, the device is using (a hypothetical) protocol version 1.2.3:
  - version_major = 1
  - version_minor = 2
  - version_patch = 3

| Byte | Value (hex) | Value (decimal) | Type | Name | Description |
|------|-------------|-----------------|------|------|-------------|
| 0    | 0x42 ('B')  | 66 | u8 | start1 | This is a message start identifier, an ascii letter 'B' |
| 1    | 0x52 ('R')  | 82 | u8 | start2 | This is a message start identifier, an ascii letter 'R' |
| 2-3  | 0x0004      | 4 | u16 | payload_length | The number of bytes in the [`protocol_version`](pingmessage-common#5-protocol_version) payload |
| 4-5  | 0x0005      | 5 | u16 | message_id | This message is a [`protocol_version`](pingmessage-common#5-protocol_version) (id #5) message |
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
