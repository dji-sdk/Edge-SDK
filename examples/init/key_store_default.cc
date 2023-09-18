/*
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
#include "key_store_default.h"
#include <unistd.h>

#include <memory>

#include "init.h"
#include "openssl/rsa.h"

namespace edge_sdk {

namespace {
uint8_t buffer[1500];
std::shared_ptr<std::string> rsa2048_public_key_;
std::shared_ptr<std::string> rsa2048_private_key_;
const char* kPathPublickKey = "/tmp/pub_key";
const char* kPathPrivateKey = "/tmp/private_key";
}  // namespace

KeyStoreDefault::KeyStoreDefault() {
    if (ReadKeys()) {
        return;
    }
    auto ret = GenerateKeys();
    ReadKeys();
    if (!ret) printf("ERROR: can not get valid keys\n");
}

ErrorCode KeyStoreDefault::RSA2048_GetDERPrivateKey(
    std::string& private_key) const {
    if (rsa2048_public_key_->empty()) {
        return kErrorParamGetFailure;
    }
    private_key = *rsa2048_private_key_;
    return kOk;
}
ErrorCode KeyStoreDefault::RSA2048_GetDERPublicKey(
    std::string& public_key) const {
    if (rsa2048_private_key_->empty()) {
        return kErrorParamGetFailure;
    }
    public_key = *rsa2048_public_key_;
    return kOk;
}

bool KeyStoreDefault::ReadKeys() {
    uint32_t len = 0;
    FILE* file = fopen(kPathPublickKey, "rb");
    if (file) {
        auto ret = fread(buffer, 1, sizeof(buffer), file);
        if (ret > 0) {
            len = ret;
        }
        printf("public key read len: %ld\n", ret);
        fclose(file);
    } else {
        return false;
    }
    rsa2048_public_key_ = std::make_shared<std::string>((char*)buffer, len);

    file = fopen(kPathPrivateKey, "rb");
    if (file) {
        auto ret = fread(buffer, 1, sizeof(buffer), file);
        if (ret > 0) {
            len = ret;
        }
        printf("private key len read len: %d\n", len);
        fclose(file);
    } else {
        return false;
    }
    rsa2048_private_key_ = std::make_shared<std::string>((char*)buffer, len);
    return true;
}

bool KeyStoreDefault::GenerateKeys() {
    RSA* rsa2048 = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    uint8_t* PKey = buffer;
    auto key_len = i2d_RSAPublicKey(rsa2048, &PKey);

    if (!rsa2048) {
        return false;
    }
    rsa2048_public_key_ = std::make_shared<std::string>((char*)buffer, key_len);

    FILE* pkey = fopen(kPathPublickKey, "wb");
    if (pkey) {
        fwrite(buffer, 1, key_len, pkey);
        fclose(pkey);
    }

    uint8_t* PrivKey = buffer;
    key_len = i2d_RSAPrivateKey(rsa2048, &PrivKey);
    printf("Private len=%d\n", key_len);
    rsa2048_private_key_ =
        std::make_shared<std::string>((char*)buffer, key_len);

    FILE* privkey = fopen(kPathPrivateKey, "wb");
    if (privkey) {
        fwrite(buffer, 1, key_len, privkey);
        fclose(privkey);
    }
    RSA_free(rsa2048);
    return true;
}

}  // namespace edge_sdk

