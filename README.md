# DJI Edge SDK (ESDK)

![](https://img.shields.io/badge/version-V1.0.0-yellow.svg)
![](https://img.shields.io/badge/platform-linux_.svg)
![](https://img.shields.io/badge/license-MIT-blue.svg)

## What is the DJI Edge SDK?

Edge-SDK is an edge computing development kit for DJI Airport, providing the following functionalities:

* Stream subscription for DJI Airport
* Media file retrieval from DJI Airport
* Small data upload channel to the cloud
* Secure communication functionality

## Functionalities
#### Stream Subscription for DJI Airport
This feature allows you to subscribe to the video stream of DJI Airport and receive real-time video data.

#### Media File Retrieval from DJI Airport
This feature allows you to download and retrieve media files from DJI Airport, including photos and videos.

#### Small Data Upload Channel to the Cloud
Edge-SDK provides a small data upload channel that allows you to effectively upload small amounts of data to cloud storage or processing.

#### Secure Communication Functionality
To ensure the security of communication, Edge-SDK provides encryption and authentication mechanisms to ensure secure communication with DJI Airport.

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

## Contributing

Please read the following guidelines before starting work on a pull request.

#### Summary of the guidelines:

* One pull request per issue;
* Choose the right base branch;
* Clean up "oops" commits before submitting;
* Follow the coding style by [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)

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
