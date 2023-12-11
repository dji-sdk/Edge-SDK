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

#ifndef __KEY_STROE_H__
#define __KEY_STROE_H__

#include <string>

#include "error_code.h"

namespace edge_sdk {

/* Exported functions --------------------------------------------------------*/

/* @brief The KeyStore interface class requires developers to implement ,
 * and pass in the implementation instance when initializing through
 * ESDKInit::Init. Through this interface, the EdgeSDK obtains the RSA2048 key
 * for establishing secure communication. The implementer should securely
 * generate and persistently store the key. Once bound to pilot, keys
 * should not change again, otherwise it is necessary to re-use pilot for
 * binding.
 */
class KeyStore {
   public:
    KeyStore() = default;
    virtual ~KeyStore() {}

    KeyStore(const KeyStore&) = delete;
    KeyStore& operator=(const KeyStore&) = delete;

    /* @brief Returns RSA2048 DER private key.
     * @return Execution result.
     */
    virtual ErrorCode RSA2048_GetDERPrivateKey(
        std::string& private_key) const = 0;

    /* @brief Returns RSA2048 DER public key.
     * @return Execution result.
     */
    virtual ErrorCode RSA2048_GetDERPublicKey(
        std::string& public_key) const = 0;
};

}  // namespace edge_sdk

#endif
