[
    {
        "id": "39b51334.8a069c",
        "type": "tab",
        "label": "LEDs",
        "disabled": false,
        "info": ""
    },
    {
        "id": "92cd9b3.8a3a268",
        "type": "mqtt in",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/commands1",
        "qos": "2",
        "broker": "3cc69636.e3e24a",
        "x": 174,
        "y": 630.0000114440918,
        "wires": [
            [
                "1a9d2918.88a557",
                "6fc87764.5098f8"
            ]
        ]
    },
    {
        "id": "b6c7f2ee.02ad2",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Green",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.g",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 550,
        "y": 540,
        "wires": [
            [
                "441612a2.5004ec"
            ]
        ]
    },
    {
        "id": "1a9d2918.88a557",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/state1",
        "qos": "",
        "retain": "true",
        "broker": "3cc69636.e3e24a",
        "x": 580,
        "y": 660,
        "wires": []
    },
    {
        "id": "6fc87764.5098f8",
        "type": "json",
        "z": "39b51334.8a069c",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 368.4000015258789,
        "y": 574.000011920929,
        "wires": [
            [
                "b6c7f2ee.02ad2",
                "4efb4ff.39f08b",
                "f2db22b9.55e56"
            ]
        ]
    },
    {
        "id": "f2db22b9.55e56",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Blue",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.b",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 550,
        "y": 580,
        "wires": [
            [
                "d92a50d4.3f4d1"
            ]
        ]
    },
    {
        "id": "4efb4ff.39f08b",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Red",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.r",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 540,
        "y": 500,
        "wires": [
            [
                "ebfbf935.e19e68"
            ]
        ]
    },
    {
        "id": "f0831c45.bb40a",
        "type": "mqtt in",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/commands2",
        "qos": "2",
        "broker": "3cc69636.e3e24a",
        "x": 178.39993286132812,
        "y": 934.9999852180481,
        "wires": [
            [
                "a9b75a27.445418",
                "d20ffd7.efb26"
            ]
        ]
    },
    {
        "id": "8886ed47.3e3a4",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Green",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.g",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 550,
        "y": 840,
        "wires": [
            [
                "fda48a62.0b6118"
            ]
        ]
    },
    {
        "id": "a9b75a27.445418",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/state2",
        "qos": "",
        "retain": "true",
        "broker": "3cc69636.e3e24a",
        "x": 580,
        "y": 980,
        "wires": []
    },
    {
        "id": "d20ffd7.efb26",
        "type": "json",
        "z": "39b51334.8a069c",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 372.79993438720703,
        "y": 878.9999856948853,
        "wires": [
            [
                "8886ed47.3e3a4",
                "6d20d26f.c3d8fc",
                "934289e0.5d09b8"
            ]
        ]
    },
    {
        "id": "934289e0.5d09b8",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Blue",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.b",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 550,
        "y": 880,
        "wires": [
            [
                "e52fba50.180a28"
            ]
        ]
    },
    {
        "id": "6d20d26f.c3d8fc",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Red",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.r",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 540,
        "y": 800,
        "wires": [
            [
                "ebc7f6dc.a6f578"
            ]
        ]
    },
    {
        "id": "74a38d59.42f4d4",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/red1",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 910,
        "y": 500,
        "wires": []
    },
    {
        "id": "8ace5f09.c061d",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/green1",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 920,
        "y": 540,
        "wires": []
    },
    {
        "id": "dfdaf9a7.5e63f8",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/blue1",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 910,
        "y": 580,
        "wires": []
    },
    {
        "id": "45660429.d78ecc",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/red2",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 950,
        "y": 800,
        "wires": []
    },
    {
        "id": "fd5dedb0.6f9d9",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/green2",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 956.999921798706,
        "y": 841.9999904632568,
        "wires": []
    },
    {
        "id": "7230e27f.d2dddc",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/blue2",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 948.0000228881836,
        "y": 881.0000138282776,
        "wires": []
    },
    {
        "id": "ebfbf935.e19e68",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 500,
        "wires": [
            [
                "74a38d59.42f4d4"
            ]
        ]
    },
    {
        "id": "441612a2.5004ec",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 540,
        "wires": [
            [
                "8ace5f09.c061d"
            ]
        ]
    },
    {
        "id": "d92a50d4.3f4d1",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 580,
        "wires": [
            [
                "dfdaf9a7.5e63f8"
            ]
        ]
    },
    {
        "id": "ebc7f6dc.a6f578",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 800,
        "wires": [
            [
                "45660429.d78ecc"
            ]
        ]
    },
    {
        "id": "fda48a62.0b6118",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 840,
        "wires": [
            [
                "fd5dedb0.6f9d9"
            ]
        ]
    },
    {
        "id": "e52fba50.180a28",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 710,
        "y": 880,
        "wires": [
            [
                "7230e27f.d2dddc"
            ]
        ]
    },
    {
        "id": "ea142181.e7b15",
        "type": "mqtt in",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/commands3",
        "qos": "2",
        "broker": "3cc69636.e3e24a",
        "x": 192.4000244140625,
        "y": 1300.0000171661377,
        "wires": [
            [
                "f5005e7.ee46fa",
                "a783cb0d.303278"
            ]
        ]
    },
    {
        "id": "f5005e7.ee46fa",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/state3",
        "qos": "",
        "retain": "true",
        "broker": "3cc69636.e3e24a",
        "x": 596.9999961853027,
        "y": 1332.0000367164612,
        "wires": []
    },
    {
        "id": "a783cb0d.303278",
        "type": "json",
        "z": "39b51334.8a069c",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 386.8000259399414,
        "y": 1244.0000176429749,
        "wires": [
            [
                "13e15907.8424a7",
                "5bbb7429.7791ec",
                "f15811be.4b01f"
            ]
        ]
    },
    {
        "id": "13e15907.8424a7",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Green",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.g",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 566.9999961853027,
        "y": 1192.0000367164612,
        "wires": [
            [
                "702ffa48.a75654"
            ]
        ]
    },
    {
        "id": "5bbb7429.7791ec",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Red",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.r",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 556.9999961853027,
        "y": 1152.0000367164612,
        "wires": [
            [
                "abd73568.3de4d8"
            ]
        ]
    },
    {
        "id": "f15811be.4b01f",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Blue",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.color.b",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 566.9999961853027,
        "y": 1232.0000367164612,
        "wires": [
            [
                "7b455081.94881"
            ]
        ]
    },
    {
        "id": "702ffa48.a75654",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 726.9999961853027,
        "y": 1192.0000367164612,
        "wires": [
            [
                "ec7bb3bb.7e1ed"
            ]
        ]
    },
    {
        "id": "abd73568.3de4d8",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 706.9999961853027,
        "y": 1152.0000367164612,
        "wires": [
            [
                "d80453f.77572b"
            ]
        ]
    },
    {
        "id": "7b455081.94881",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 726.9999961853027,
        "y": 1232.0000367164612,
        "wires": [
            [
                "e40d5e6a.ce163"
            ]
        ]
    },
    {
        "id": "ec7bb3bb.7e1ed",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/green3",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 936.9999961853027,
        "y": 1192.0000367164612,
        "wires": []
    },
    {
        "id": "d80453f.77572b",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/red3",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 926.9999961853027,
        "y": 1152.0000367164612,
        "wires": []
    },
    {
        "id": "e40d5e6a.ce163",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/blue3",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 926.9999961853027,
        "y": 1232.0000367164612,
        "wires": []
    },
    {
        "id": "f756562e.59d598",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/state",
        "qos": "",
        "retain": "true",
        "broker": "3cc69636.e3e24a",
        "x": 513.0002593994141,
        "y": 255.99999809265137,
        "wires": []
    },
    {
        "id": "c24f27ac.93d208",
        "type": "json",
        "z": "39b51334.8a069c",
        "name": "",
        "property": "payload",
        "action": "",
        "pretty": false,
        "x": 418.4000015258789,
        "y": 129.00000047683716,
        "wires": [
            [
                "9ca6cc4a.5ad0a",
                "51b96107.9e7a2",
                "423a3100.797d7"
            ]
        ]
    },
    {
        "id": "9ca6cc4a.5ad0a",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Effect",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.effect",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 578.4000549316406,
        "y": 128.0000228881836,
        "wires": [
            [
                "24618d0.add0874"
            ]
        ]
    },
    {
        "id": "51b96107.9e7a2",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send State",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.state",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 579.4000244140625,
        "y": 75,
        "wires": [
            [
                "3d0ce70d.933de8"
            ]
        ]
    },
    {
        "id": "3d0ce70d.933de8",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/power",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 994.4999313354492,
        "y": 75.00002241134644,
        "wires": []
    },
    {
        "id": "24618d0.add0874",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/effect",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 985.3999309539795,
        "y": 128.00002336502075,
        "wires": []
    },
    {
        "id": "c8f3e9e5.cb3858",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "hoildayLights/brightness",
        "qos": "",
        "retain": "",
        "broker": "3cc69636.e3e24a",
        "x": 1007.8000469207764,
        "y": 187.00000047683716,
        "wires": []
    },
    {
        "id": "423a3100.797d7",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "Send Brightness",
        "rules": [
            {
                "t": "set",
                "p": "payload",
                "pt": "msg",
                "to": "payload.brightness",
                "tot": "msg"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 597.3002262115479,
        "y": 186.9999942779541,
        "wires": [
            [
                "299ec7ed.9349d8"
            ]
        ]
    },
    {
        "id": "299ec7ed.9349d8",
        "type": "switch",
        "z": "39b51334.8a069c",
        "name": "0-255",
        "property": "payload",
        "propertyType": "msg",
        "rules": [
            {
                "t": "btwn",
                "v": "0",
                "vt": "num",
                "v2": "255",
                "v2t": "num"
            }
        ],
        "checkall": "true",
        "repair": false,
        "outputs": 1,
        "x": 787.3999328613281,
        "y": 188.0000238418579,
        "wires": [
            [
                "c8f3e9e5.cb3858"
            ]
        ]
    },
    {
        "id": "25d8f71.4f6df08",
        "type": "server-state-changed",
        "z": "39b51334.8a069c",
        "name": "LED Locator",
        "server": "d64dbedb.b22df",
        "entityidfilter": "input_number.led_identification",
        "entityidfiltertype": "substring",
        "haltifstate": "",
        "x": 129,
        "y": 372.00000762939453,
        "wires": [
            [
                "a5bd79ee.101c88"
            ]
        ]
    },
    {
        "id": "a5bd79ee.101c88",
        "type": "change",
        "z": "39b51334.8a069c",
        "name": "remove decimal",
        "rules": [
            {
                "t": "change",
                "p": "payload",
                "pt": "msg",
                "from": ".0",
                "fromt": "str",
                "to": "",
                "tot": "str"
            }
        ],
        "action": "",
        "property": "",
        "from": "",
        "to": "",
        "reg": false,
        "x": 369.00000381469727,
        "y": 371.9999656677246,
        "wires": [
            [
                "cfce6727.d5d298"
            ]
        ]
    },
    {
        "id": "cfce6727.d5d298",
        "type": "mqtt out",
        "z": "39b51334.8a069c",
        "name": "Configure LED",
        "topic": "holidayLights/configure",
        "qos": "0",
        "retain": "false",
        "broker": "3cc69636.e3e24a",
        "x": 648,
        "y": 371.999960899353,
        "wires": []
    },
    {
        "id": "6e500e57.6fa3f",
        "type": "mqtt in",
        "z": "39b51334.8a069c",
        "name": "",
        "topic": "holidayLights/commands",
        "qos": "2",
        "broker": "3cc69636.e3e24a",
        "x": 166,
        "y": 148.00002479553223,
        "wires": [
            [
                "f756562e.59d598",
                "c24f27ac.93d208"
            ]
        ]
    },
    {
        "id": "3cc69636.e3e24a",
        "type": "mqtt-broker",
        "z": "",
        "name": "",
        "broker": "192.168.86.168",
        "port": "1883",
        "clientid": "",
        "usetls": false,
        "compatmode": true,
        "keepalive": "60",
        "cleansession": true,
        "birthTopic": "",
        "birthQos": "0",
        "birthPayload": "",
        "closeTopic": "",
        "closePayload": "",
        "willTopic": "",
        "willQos": "0",
        "willPayload": ""
    },
    {
        "id": "d64dbedb.b22df",
        "type": "server",
        "z": "",
        "name": "Home Assistant",
        "url": "http://hassio/homeassistant",
        "pass": "apikey"
    }
]