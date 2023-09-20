# DJI Edge SDK (ESDK)

![](https://img.shields.io/badge/version-V1.0.0-yellow.svg)
![](https://img.shields.io/badge/platform-linux_|_x86_64-green.svg)
![](https://img.shields.io/badge/license-MIT-blue.svg)

## What is the DJI Edge SDK?

Edge-SDK is an edge computing development kit for DJI Airport, providing the following functionalities:

* Fetch Aircraft Media Files
* Subscribe to Aircraft Data Streams
* Secure Local Communication Links
* Cloud Data Intercommunication Channels

## Functionalities
#### Fetch Aircraft Media Files
Enables fetching and processing media files captured by aircraft to local system for refined recognition and data filtering. This reduces cloud traffic and latency and enhances data processing efficiency.

#### Subscribe to Aircraft Data Streams
Allowing real-time video recognition and other AI processes for aircraft-transmitted data streams on local system, which greatly enhances aircraft intelligence, and provides better support for aircraft mission execution.

#### Secure Local Communication Links
Edge computing offers a secure local communication link, ensuring communication safety and privacy protection between aircraft. This avoids risks of network attacks and data breaches during aircraft communication.

#### Cloud Data Intercommunication Channels
Edge computing can process and transmit data in unstable network environments, utilizing small data channels to enhance transmission efficiency. This ensures aircraft achieve efficient data processing and transmission across various environments.

## Dependencies
* libssh2 (version 1.10.x or hight)
* openssl (reference version 1.1.1f)
* opencv (version 3.4.16 or hight)
* ffmpeg (version 4.13 or hight)

## Documentation

For full documentation, please visit
the [DJI Developer Documentation](https://developer.dji.com/doc/edge-sdk-tutorial/en/). Documentation
regarding the code can be found in the [ESDK API Reference](https://developer.dji.com/doc/edge-sdk-api-reference/en/)
section of the developer's website. Please visit
the [Latest Version Information](https://developer.dji.com/doc/edge-sdk-tutorial/en/)
to get the latest version information.

## License

Edge SDK codebase is MIT-licensed. Please refer to the LICENSE file for detailed information.

## Support

You can get official support from DJI and the community with the following methods:

- Post questions on Developer Forum
    * [DJI SDK Developer Forum(Cn)](https://djisdksupport.zendesk.com/hc/zh-cn/community/topics)
    * [DJI SDK Developer Forum(En)](https://djisdksupport.zendesk.com/hc/en-us/community/topics)
- Submit a request describing your problem on Developer Support
    * [DJI SDK Developer Support(Cn)](https://djisdksupport.zendesk.com/hc/zh-cn/requests/new)
    * [DJI SDK Developer Support(En)](https://djisdksupport.zendesk.com/hc/en-us/requests/new)

You can also communicate with other developers by the following methods:

- Post questions on [**Stackoverflow**](http://stackoverflow.com) using [**
  dji-sdk**](http://stackoverflow.com/questions/tagged/dji-sdk) tag
