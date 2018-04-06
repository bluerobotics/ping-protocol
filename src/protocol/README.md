# Protocol
Each sensor can have a parser and a protocol, a json/template structure was created to help the development.

#### Json file example:
```json
{
  "class_info":{
      "_comment":"Handle the main class protocol, the class name will be the file prefix in lower",
      "file":"pingmessage.h.in (template file in templates folder)",
      "name":"PingMessage (main class name, files will be class_info.name.lower() as default prefix)"
  },
  "subclass_info":{
      "_comment":"Create a class for each message type",
      "file":"pingmessage_.h.in (template file, can be any name, all files will be created from default prefix)",
      "name":"ping_msg_ (class name prefix, subclass_info.name + messages[:] + _ + messages[:][:], e.g: ping_msg_gen_goto_bootloader)"
  },
  "sensor_info":{
  	  "_comment": "Not working, for sensor creation",
      "file":"ping.h.in",
      "name":"Ping"
  },
  "_comment": "All messages",
  "messages":{
  	  "_comment": "Message type",
      "gen":{
      	  "_comment": "Message name",
          "goto_bootloader":{
              "_comment": "Message identifier and payload",
              "id":"0x109",
              "payload":[

              ]
          },
          "version":{
              "id":"101",
              "_comment": "The payload is a C struct, the declaration order is important :)",
              "payload":[
                  {
                      "name":"device_type",
                      "type":"u8"
                  },
                  {
                      "name":"device_model",
                      "type":"u8"
                  },
                  {
                      "name":"fw_version_major",
                      "type":"u16"
                  },
                  {
                      "name":"fw_version_minor",
                      "type":"u16"
                  }
              ]
          },
      "es":{
          "distance_simple":{
              "id":"1100",
              "payload":[
                  {
                      "name":"distance",
                      "type":"u32"
                  },
                  {
                      "name":"confidence",
                      "type":"u8"
                  }
              ]
          }
      }
   }
 }
```

From the json file, a simple structure will be created.
```
├ protocol
├── generator.pri (Created, will include all headers for compiler and linker)
├── generator.py (File generator)
├── parsers (All parsers)
│   ├── detector.h
│   ├── parser.h
│   ├── parser_json.h
│   └── parser_ping.h
├── pingmessage (Created: for each *json.class_info.name.lower()*)
│   ├── pingmessage_all.h (Created: will include all header for sensor)
│   ├── pingmessage_es.h (Created: from messages[:])
│   ├── pingmessage_gen.h (Created: from messages[:])
│   └── pingmessage.h (Created: Main class)
├── protocol.pri (This file will call generator.py and include generator.pri when done)
└── templates (Folder with all templates, files with _file are used by generator.py)
    ├── _all.h.in (Will generate class_info.name.lower()_all.h header)
    ├── _generator.pri.in (Will generate generator.pri)
    ├── pingmessage_.h.in (File included by json file)
    ├── pingmessage.h.in (File included by json file)
    └── ping_protocol.json (Json file)

```