# TV Voice Control with AI and ESP32

This project enables voice control of a TV using Claude AI, Whisper speech recognition, and an ESP32 microcontroller with an IR LED for command broadcasting.

## Project Structure

The repository is organized into two main folders:

1. `ino-files`: Contains the Arduino project files for setting up the ESP32.
2. `webapp`: Contains the server-side application.

## Features

- Voice command recognition using Whisper
- Natural language processing with Claude AI
- IR command broadcasting via ESP32
- Web-based control interface

## Setup

### ESP32 Setup (ino-files)

1. Open the Arduino IDE.
2. Load the project files from the `ino-files` folder.
3. Install required libraries (list libraries here).
4. Configure your ESP32 board in the Arduino IDE.
5. Upload the code to your ESP32.

### Web Application Setup (webapp)

1. Navigate to the `webapp` folder.
2. Install dependencies:
   ```
   npm install
   ```
3. Configure environment variables (create a `.env` file based on `.env.example`).
4. Start the server:
   ```
   npm start
   ```

## Usage

1. Ensure your ESP32 is powered and connected to your local network.
2. Start the web application.
3. Access the web interface through your browser.
4. Speak voice commands on the web interface to control your TV.
