# Bill of Materials

| Part                   | Description                                | Image                                                                                 | Qty      | Notes                                                                                 |
| ---------------------- | ------------------------------------------ | ------------------------------------------------------------------------------------- | -------- | ------------------------------------------------------------------------------------- |
| ESP32 DevKitC          | Main embedded controller                   |  <img src="/docs/08-assets/components/esp%2032.png" width="100" height="100">             | 1        | ESP32-WROOM-32D was used in this project, primarily because of innate I2S support     |
| INMP441*                | Digital I2S MEMS microphone                | <img src="/docs/08-assets/components/inmp441.png" width="100" height="100">            | 1        | Preferably get the version with pre-soldered header pins. To get these, I recommend you order it from [Microscale](https://www.microscale.net/products/inmp441-omnidirectional-microphone-module?_pos=1&_sid=2cc91557d&_ss=r) for builders in Nigeria.                                                 |
| MicroSD Module*         | Local audio storage interface              | <img src="/docs/08-assets/components/sd card module.webp" width="100" height="100">    | 1        | FAT32 recommended                                                                     |
| MicroSD Card           | Audio recording storage                    | <img src="/docs/08-assets/components/sd card.png" width="100" height="100">            | 1        | ≥ 4GB recommended; adapter version helps with debugging and file extraction           |                                                            |
| 18650 Cells            | Battery cells used in battery assembly     | <img src="/docs/08-assets/components/1S2P1 battery.webp" width="100" height="100">             | 2        | Used in parallel battery, 3.7V configuration                                                |
| TP4056 Module*          | Battery charging module                    | <img src="/docs/08-assets/components/TP4056.jpeg" width="100" height="100">            | 1        | USB charging with protection circuitry                                                |
| MT3608 Boost Converter | Voltage step-up converter                  | <img src="/docs/08-assets/components/mt3608.png" width="100" height="100">            | 1        | Used for regulated voltage supply                                                     |
| Push Button            | Recording control                          | <img src="/docs/08-assets/components/button.png" width="100" height="100">             | 1        | Used for recording start/stop control                                                 |
| Power Switch           | Main power control switch                  | <img src="/docs/08-assets/components/power switch.webp" width="100" height="100">      | 1        | Used for isolating battery power                                                      |
| Vero Board             | Prototype circuit mounting board           | <img src="/docs/08-assets/components/veroboard.jpg" width="100" height="100">         | 1        | Small dotted Vero board used for prototyping                                          |
| Female Pin Headers*     | Modular pin connections                    | <img src="/docs/08-assets/components/female headers.jpg" width="100" height="100">       | 2 19-pin headers | Additional bent headers cam also be utilized                                                       |
| Jumper Wires           | Interconnections between modules           | <img src="/docs/08-assets/components/jumper wires.webp" width="100" height="100">      | Multiple | Includes male-female, male-male, and female-female jumper wires                                   |
| Hook-up Wires          | Power and signal wiring                    | <img src="/docs/08-assets/components/wires.jpg" width="100" height="100">             | Multiple | Recycled phone and PC wires were also utilized                                        |
| Spring Connectors      | Wire joining and modular connection        | <img src="/docs/08-assets/components/battery contacts.webp" width="100" height="100"> | Multiple | Useful for temporary secure connections                                               |
| 470uF Capacitor*      | Voltage stabilizer, transient buffer        | <img src="/docs/08-assets/components/470uF capacitor.webp" width="100" height="100"> | 1 |                                               |
| Heat Shrink Tubing     | Electrical insulation and cable management | <img src="/docs/08-assets/components/heat shrinks.jpg" width="100" height="100">        | Multiple small sizes (optional) | Used for battery and wire insulation                                                  |
| Velcro Strap           | Portable mounting system                   | <img src="/docs/08-assets/components/velcro strap.jpg" width="100" height="100">            | 1        | Used for wearable/portable deployment                                                 |
| Zip Ties               | Cable management and internal fastening    | <img src="/docs/08-assets/components/zip ties.jpg" width="100" height="100">           | Multiple | Used for internal enclosure organization                                              |
| Enclosure              | Device housing                             | <img src="/docs/08-assets/components/pattress.webp" width="100" height="100">          | 1        | Double socket pattress used in prototype stage; 3D printable enclosure also available |
| 3D Printed Enclosure   | Custom enclosure system                    | <img src="/docs/08-assets/components/" width="100" height="100">      | 1        | Optional final enclosure design                                                       |
| Bag Material   | Portable carrying/mounting accessory               | <img src="/docs/08-assets/components/bag handle material.jpg" width="100" height="100">      | 1        | Used for wearable deployment concept                                                  |
| Buckle   | Portable carrying/mounting accessory                     | <img src="/docs/08-assets/components/buckle.jpg" width="100" height="100">      | 1        | Used for wearable deployment concept                                                  |
| Soldering Lead         | Soldering material                         | <img src="/docs/08-assets/components/soldering lead.jpg" width="100" height="100">            | Multiple | Used for permanent electrical connections                                             |
| Glue Sticks            | Hot glue application material              | <img src="/docs/08-assets/components/glue sticks.jpg" width="100" height="100">       | Multiple | Used for internal fastening and insulation                                            |

- Apart from materials like Jumper wires, soldering lead, which usually need to be bought in multiples, I recommend buying (at least) 2-3 of the materials that have been starrred (*). I found these particlar components to be particularly tricky. More in the hardware documentation.
  
- I also recommend buying extra components, that are usually cheaper and more prone to error than the microcontrollers, as good prcatice for any hardware/embedded systems project.

# Other Equipment

These items were utilized during development, prototyping, testing, and assembly but are not part of the final deployed device.

| Equipment                | Purpose                                     | Notes                                         |
| ------------------------ | ------------------------------------------- | --------------------------------------------- |
| Glue Gun                 | Component mounting and enclosure fastening  | Used extensively during prototyping           |
| Hand Drill               | Enclosure modification and mounting holes   | Used for pattress and enclosure customization |
| Cutter with Extra Blades | Wire stripping and enclosure cutting        | Useful for precision modifications            |
| Soldering Iron           | Electronic assembly and soldering           | Required for permanent hardware assembly      |
| USB Cable                | ESP32 programming and debugging             | USB data cable recommended                    |
| Computer/Laptop          | Firmware development and backend deployment | Used for Arduino IDE and cloud setup          |
| SD Card Adapter/Dongle   | SD card debugging and file transfer         | Useful for extracting recordings directly     |
| Multimeter               | Voltage and continuity testing              | Important during power system debugging       |
| USB Cord                 | Firmware development and system debugging   | Important during system debugging; Either Type B or Type C depending on your ESP 32 dev board connector pin      |
| Breadboard               | Initial prototyping and testing             | Optional but highly recommended               |


# Software Purchases & Cloud Services

These services and software-related resources were utilized as part of the Audionote backend infrastructure and deployment workflow.

| Service / Software                   | Purpose                                                         | Notes                                                |
| ------------------------------------ | --------------------------------------------------------------- | ---------------------------------------------------- |
| Google Workspace Subscription        | Shared Drive infrastructure and institutional file organization | Required for structured archival workflow            |
| Domain Name                          | Custom domain configuration for backend/cloud services          | Used for platform branding and service configuration |
| Google Cloud Platform (GCP) (Free credits available)         | Cloud backend hosting and infrastructure                        | Includes Cloud Run, Storage, IAM, APIs, etc. You can get this for free on a new account. See Backend documentation for more details.               |
| Arduino IDE (Free)                          | ESP32 firmware development                                      | Main embedded firmware environment                   |
| SolidWorks/ CAD Software (Optional)  | 3D enclosure modeling                                           | Used for enclosure development workflow              |

