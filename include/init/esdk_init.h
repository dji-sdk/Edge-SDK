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
 * legal actions against you for any loss(es) or damage(s) caused by your *
 *failure to do so.
 *
 *********************************************************************
 */

#ifndef __ESDK_INIT_H__
#define __ESDK_INIT_H__

#include <memory>

#include "error_code.h"
#include "options.h"

namespace edge_sdk {

/* Exported functions --------------------------------------------------------*/

/**
 * @brief ESDKInit initialize SDK environment, preparing for SDK function
 * components, caller should ensure that it is initialized correctly, otherwise
 * the function may be abnormal
 */
class ESDKInit final {
   public:
    ~ESDKInit();

    ESDKInit(const ESDKInit&) = delete;
    ESDKInit& operator=(const ESDKInit&) = delete;

    /**
     * @brief get the singleton of ESDKInit.
     * @return instance.
     */
    static ESDKInit* Instance();

    /**
     * @brief Initialize SDK environment
     * @param options: options for initializing the environment
     * @return Execution result.
     */
    ErrorCode Init(const Options& options);

    /**
     * @brief Returns the firmware version
     * @return Firmware version
     */
    FirmwareVersion GetFirmwareVersion() const;

    /**
     * @brief Returns the product name
     * @return Product name
     */
    ProductName GetProductName() const;

    /**
     * @brief Returns the vendor name
     * @return Vendor name
     */
    VendorName GetVendorName() const;

    /**
     * @brief Returns the serial number
     * @return Serial number
     */
    SerialNumber GetSerialNumber() const;

    /**
     * @brief Returns the App license info
     * @return App info
     */
    AppInfo GetAppInfo() const;

    /**
     * @brief DeInitialize SDK environment
     * @return Execution result.
     */
    ErrorCode DeInit();

   private:
    ESDKInit();
    class Impl;

    std::unique_ptr<Impl> impl_;
};

}  // namespace edge_sdk

#endif
