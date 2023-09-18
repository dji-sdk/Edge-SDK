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
#ifndef __CLOUD_API_H__
#define __CLOUD_API_H__

#include <cstdint>
#include <functional>

#include "error_code.h"

namespace edge_sdk {

/* Exported types ------------------------------------------------------------*/
/**
 * @brief function prototype for receiving custom data from the cloud
 * @param data: pointer to the "value" field in the custom protocol sent from
 * the cloud.
 * @param len: indicates the length of the "value" field, Note: message length
 * sent from cloud should not exceed 256 bytes
 */
using CloudAPICustomServicesMessageHandler =
    std::function<void(const uint8_t* data, uint32_t len)>;

/* Exported functions --------------------------------------------------------*/
/**
 *
 * @brief To register a handler function for receiving messages from the cloud ,
 * format of the message sent from the cloud should be as follows:
 * Topic: thing/product/${gateway_sn}/services
 * {
 *      "method": "custom_data_transmission_to_esdk",
 *      "timestamp": xxxxxxxxxx,
 *      "bid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
 *      "tid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
 *      "data": {
 *          "value": "xxxx"
 *       }
 * }
 * It is important to note that handler should not perform long-running tasks
 * as this can block the processing of messages and impact the responsiveness of
 * the system. If you have tasks that require significant processing or time to
 * complete, it is recommended to move them to separate processes or use task
 * queuing mechanisms to ensure that the handler function does not block other
 * messages from being processed.
 * @param handler: pointer to the handler for receiving message,
 * @return Execution result.
 */
ErrorCode CloudAPI_RegisterCustomServicesMessageHandler(
    CloudAPICustomServicesMessageHandler handler);

/*
 * @brief The cloud interaction API defines two custom data channels. The events
 * message is sent from Edge to the server, caller  only needs to send the
 * content of the "value" field, it will be formatted by the API that can be
 * received by the cloud like: 
 * Topic: thing/product/${gateway_sn}/events
 * {
 *      "method": "custom_data_transmission_from_esdk",
 *      "timestamp": xxxxxxxxxx,
 *      "bid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
 *      "tid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
 *      "data": {
 *          "value": "xxxx"
 *      },
 *      "gateway":"xxxxxxxxxx"
 * }
 * @param data: pointer to the "value" field in the custom protocol
 * @param len: indicates the length of the "value", can not exceed 256 bytes
 * @return Execution result.
 */
ErrorCode CloudAPI_SendCustomEventsMessage(const uint8_t* data, uint32_t len);

}  // namespace edge_sdk

#endif

