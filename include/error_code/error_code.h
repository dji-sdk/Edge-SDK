/**
 ********************************************************************
 *
 * @copyright (c) 2023 DJI. All rights reserved.
 *
 * All information contained herein is, and remains, the property of DJI.
 * The intellectual and technical concepts contained herein are proprietary
 * to DJI and may be covered by U.S. and foreign patents, patents in process,
 * and protected by trade secret or copyright law.  Dissemination of this
 * information, including but not limited to data and other proprietary
 * material(s) incorporated within the information, in any form, is strictly
 * prohibited without the express written consent of DJI.
 *
 * If you receive this source code without DJI’s authorization, you may not
 * further disseminate the information, and you must immediately remove the
 * source code and notify DJI of its removal. DJI reserves the right to pursue
 * legal actions against you for any loss(es) or damage(s) caused by your
 * failure to do so.
 *
 *********************************************************************
 */
#ifndef __ERROR_CODE_H__
#define __ERROR_CODE_H__

namespace edge_sdk {

/* Exported types ------------------------------------------------------------*/
enum ErrorCode {
    kOk = 0,

    /*! an invalid or incorrect argument was passed to a function or method */
    kErrorInvalidArgument = 1,

    /*!
     * an operation has failed due to a problem with
     * the system itself, such as a software issue. System errors
     */
    kErrorSystemError = 2,

    /*! an invalid or incorrect operation has
     * occurred during the execution of a certain operation.
     */
    kErrorInvalidOperation = 3,

    /*! a repeated operation has been attempted.
     * It suggests that a function or process has been called repeatedly,
     * causing an error or unexpected behavior.
     */
    kErrorRepeatOperation = 4,

    /*! an operation has failed because a null pointer was
     * used where a valid pointer was expected
     */
    kErrorNullPointer = 5,

    /*! a parameter has exceeded the expected range. It
     * suggests that an argument passed to a function or method is outside the
     * valid range, causing an error or unexpected behavior.
     */
    kErrorParamOutOfRange = 6,

    /*! a parameter retrieval failure. It suggests that an
     * attempt to retrieve or access a parameter has failed, resulting in an
     * error or unexpected behavior.
     * */
    kErrorParamGetFailure = 7,

    /*! a failure when attempting to set or modify a parameter,
     * resulting in an error or unexpected behavior.
     */
    kErrorParamSetFailure = 8,

    /*! a failure when attempting to send a data packet, resulting
     * in an operation failure.
     */
    kErrorSendPackFailure = 9,

    /*! a request has timed out, meaning that the expected
     * response from the server did not arrive within the allocated time limit,
     * resulting in a failed request.
     */
    kErrorRequestTimeout = 10,

    /*! a failure in verifying the authorization information */
    kErrorAuthVerifyFailure = 11,

    /*! a failure during an attempt to encrypt data. */
    kErrorEncryptFailure = 12,

    /*! a failure during an attempt to decrypt data. */
    kErrorDecryptFailure = 13,

    /*! the received response is invalid or does not match the
     *  expected format.
     **/
    kErrorInvalidRespond = 14,

    /*! a failure on the remote server or remote process.
     */
    kErrorRemoteFailure = 15,

    /*! Failed to get a valid video ID while starting a live stream: At this
     * point, you should check if the flight has already been paired, and you
     * can confirm by checking if the cloud can open a live stream. */
    kErrorNoVideoID = 16,

    /*! a failure in establishing a connection */
    kErrorConnectFailure = 17,
};

}  // namespace edge_sdk

#endif
