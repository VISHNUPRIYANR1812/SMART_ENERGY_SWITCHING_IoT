# SMART_ENERGY_SWITCHING_IoT

# Smart Power Distribution Unit

This project implements a Smart Power Distribution Unit (PDU) using an ESP32 microcontroller, Blynk IoT platform, and Telegram bot integration. It allows monitoring and control of power sources (solar and backup battery) based on voltage readings, with real-time notifications via Telegram.

## Features

- Monitors solar panel voltage and backup battery voltage.
- Automatically switches between solar and backup modes based on voltage thresholds.
- Sends mode and voltage information to a Telegram bot for remote monitoring.

## Hardware Requirements

- ESP32 development board
- Voltage sensors (for solar panel and backup battery)
- Relays for switching power sources
- Necessary resistors and connectors

## Software Requirements

- Arduino IDE with ESP32 support
- Blynk IoT platform account
- Telegram bot API token

## Installation and Setup

1. Clone or download this repository to your local machine.
2. Open the Arduino IDE and install the necessary libraries:
   - Blynk library for ESP32
   - UniversalTelegramBot library
3. Configure the `auth`, `ssid`, `pass`, `BOTtoken`, and `CHAT_ID` variables in the `main.ino` file with your Blynk and Telegram credentials.
4. Connect the hardware components according to the pin configurations in the code.
5. Upload the code to your ESP32 board.
6. Install the Blynk app on your smartphone, create a new project, and scan the QR code in the Blynk app to configure the widgets.
7. Start the Blynk app and connect it to your device.
8. Interact with the PDU via the Blynk app and monitor notifications from the Telegram bot.

## Usage

1. Power on the Smart PDU.
2. Monitor voltage readings and mode (solar or backup) on the Blynk app dashboard.
3. Receive notifications on your Telegram bot when the mode switches or voltage levels change.

## Contributing

Contributions are welcome! Fork the repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- Thanks to the developers of Blynk and UniversalTelegramBot libraries for their contributions.
