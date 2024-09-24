# KRSIP - A Lightweight and Portable SIP Library
KRSIP is a compact, feature-rich SIP (Session Initiation Protocol) library written in C. It provides a powerful signaling protocol for real-time multimedia communications, supporting audio, video, presence, and instant messaging. The library is designed to be portable and runs on a wide variety of systems, including desktops, embedded systems, and mobile devices. KRSIP includes advanced NAT traversal features with ICE, STUN, and TURN, and integrates media handling through WebRTC. It is secure, offering support for SIP authentication and OAuth 2.0.

## Initial structure
    krsip/
    ├── include/               # Main library interface (.h)
    │   ├── krsip.h            # Interface principal da biblioteca
    │   ├── sip.h              # Definitions for SIP
    │   ├── sdp.h              # Definitions for SDP
    │   ├── rtp.h              # Definitions for RTP
    │   └── stun_turn_ice.h    # Definitions for STUN, TURN, ICE
    ├── src/                   # Library source code (.c)
    │   ├── krsip.c            # Main implementation
    │   ├── sip.c              # SIP implementation
    │   ├── sdp.c              # SDP implementation
    │   ├── rtp.c              # RTP implementation
    │   └── stun_turn_ice.c    # STUN, TURN, ICE implementation
    ├── platform/              # Support for different platforms
    │   ├── windows/           # Windows-specific implementation
    │   ├── linux/             # Linux-specific implementation
    │   └── mobile/            # iOS/Android implementation
    └── Makefile               # Makefile build file


## Features
    •	Platforms supported:
            o	Desktop: Windows (32/64-bit), macOS (Intel and M1), Linux
            o	Mobile: Android, iOS
            o	Embedded Systems

    •	Protocol support:
            o	SIP (RFC 3261): INVITE, CANCEL, BYE, REGISTER, OPTIONS, INFO
            o	SDP (RFC 4566): Session Description Protocol for media negotiation
            o	RTP (RFC 3550): Real-Time Transport Protocol for media streaming
            o	STUN, TURN, and ICE (RFCs 5245, 5766, 5389): NAT traversal
            o	TLS for secure communications
            o	OAuth 2.0 for authentication
    •	Multimedia support:
            o	Audio and video streaming via WebRTC
            o	Presence and instant messaging (SUBSCRIBE, NOTIFY, MESSAGE)
            o	NAT traversal using ICE, STUN, and TURN
    •	Security:
            o	Digest authentication (RFC 2617)
            o	Bearer authentication using OAuth 2.0 (RFC 8898)
            o	Transport security using TLS/SSL (OpenSSL)
    •	Network features:
            o	IPv4 and IPv6 support
            o	DNS resolution (RFC 3263)
            o	Quality of Service (QoS) support: DSCP, WMM
            o	NAT traversal: ICE, STUN, TURN
## Installation

    Prerequisites
        Before building KRSIP, make sure you have the following installed:
            •	GCC or Clang (for Linux/macOS)
            •	Visual Studio Build Tools or MinGW (for Windows)
            •	OpenSSL development libraries
            •	WebRTC development libraries
            •	CMake (optional, but needed for setting up WebRTC)
            •	Make or Ninja build system
        Compiling
            The project is built using a Makefile, which provides a simple and modern build process. Follow the steps below to compile KRSIP on your system.
                1.	Clone the repository:
                    bash
                    Copy code
                    git clone https://github.com/rabbittrix/krsip.git
                    cd krsip
                2.	Open the Makefile and ensure the INCLUDES and LIBS paths point to your WebRTC and OpenSSL installation directories.
                3.	Run make to build the library:
                    bash
                    Copy code
                    make
                4.	After successful compilation, the static library libkrsip.a will be generated in the project root directory.
                    Cleaning Up
                    To remove compiled files, run:
                    bash
                    Copy code
                    make clean
                    To force a rebuild of all files, use:
                    bash
                    Copy code
                    make rebuild
                    Usage
                    KRSIP is a library designed to be linked with your applications that require SIP signaling, NAT traversal, and multimedia capabilities.
                    Example: Registering a SIP User
                    Here’s a small example of how to use KRSIP to register a SIP user to a server.
                    c
                    Copy code
                        #include "krsip.h"

                        int main() {
                            // Initialize the KRSIP library
                            krsip_init();

                            // Register the user
                            const char *username = "user";
                            const char *domain = "sip.example.com";
                            const char *password = "secret";
                            krsip_register(username, domain, password);

                            // Clean up resources
                            krsip_cleanup();

                            return 0;
                        }
                        Example: Making an Audio Call with WebRTC
                        c
                        Copy code
                        #include "krsip.h"

                        int main() {
                            // Initialize KRSIP
                            krsip_init();

                            // Make a SIP INVITE call
                            krsip_invite("sip:callee@sip.example.com");

                            // Start WebRTC audio stream
                            krsip_start_audio_stream("192.168.1.100", 5004);

                            // Hang up and stop audio
                            krsip_bye();
                            krsip_stop_audio_stream();

                            // Clean up
                            krsip_cleanup();

                            return 0;
                        }
                        Example: OAuth 2.0 Authentication
                        c
                        Copy code
                        #include "krsip.h"

                        int main() {
                            // Initialize KRSIP
                            krsip_init();

                            // Authenticate using OAuth 2.0
                            const char *token = "your_oauth2_token";
                            krsip_authenticate_oauth(token);

                            // Continue with SIP operations...

                            // Clean up
                            krsip_cleanup();

                            return 0;
                        }
                        
## NAT Traversal
    KRSIP includes built-in support for NAT traversal using STUN, TURN, and ICE. To enable NAT traversal, ensure that you initialize ICE with the following steps:
    c
    Copy code
    krsip_ice_init();
    krsip_stun_turn_init();
    You can integrate with external STUN/TURN servers to ensure seamless NAT traversal during media streaming sessions.
    Testing and Debugging
    To test KRSIP functionality, we recommend using tools like Wireshark to capture SIP and RTP traffic. You can monitor SIP signaling, RTP media streams, and validate the correct usage of NAT traversal techniques like STUN and ICE.
    Additionally, you can set up a local SIP server (e.g., Asterisk or FreeSWITCH) for end-to-end testing of SIP registration and call flows.
    Contributing
    We welcome contributions to KRSIP! If you have any suggestions or would like to submit bug fixes or features, please open an issue or submit a pull request on GitHub.
    Running Tests
    Ensure all tests pass before submitting a pull request. You can run unit tests using:
    bash
    Copy code
    make test

## License
    This project is licensed under the MIT License. See the LICENSE file for more details.
________________________________________
KRSIP is designed to be lightweight, powerful, and easy to integrate into any real-time communication system. Whether you're building a VoIP client, an embedded SIP device, or a multimedia application, KRSIP provides the tools and flexibility you need.
For more detailed documentation, visit the KRSIP Documentation Site https://www.kr-so.com .

