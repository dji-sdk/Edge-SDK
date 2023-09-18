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

#ifndef __KEY_STORE_DEFAULT_H__
#define __KEY_STORE_DEFAULT_H__

#include "error_code.h"
#include "init.h"

namespace edge_sdk {

class KeyStoreDefault : public KeyStore {
   public:
    KeyStoreDefault();

    virtual ~KeyStoreDefault() override {}

    ErrorCode RSA2048_GetDERPrivateKey(std::string& private_key) const override;

    ErrorCode RSA2048_GetDERPublicKey(std::string& public_key) const override;

   private:
    bool ReadKeys();
    bool GenerateKeys();
};

}  // namespace edge_sdk

#endif
