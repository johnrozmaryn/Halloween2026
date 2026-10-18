# Halloween2026
Attempting to make the CandyCannon digital, with external wireless siren

_SimpleWebServer_
First Attempt, ESP32 creates wifi access point and hosts a page.
Pro: simple, done, wireless
Con: need to have a dedicated browser, sleeping may be an issue
Possible future: wire a clicker to a cellphone so I don't need to navigate for button clicking

_ESP-NOW Tunnel_
A grander plan. The Siren ESP32 will be just a receiver connected to power and the siren. There weill be s second Control ESP32 that will have the input switches/buttons, and two types of outputs: a digital output to trigger a MOSFET that engagles the cannon valve, and an ESP-NOW signal sent to the Siren ESP32 tht triggers the Siren on and off.
