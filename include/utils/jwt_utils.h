
#pragma once
#include <string>
#include <ctime>
#include <cstdint>

namespace jwt_utils {

inline string base64url_encode(const string& in)
{
    static const char tbl[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    string out;
    out.reserve(((in.size() + 2) / 3) * 4);
    uint32_t val = 0;
    int valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out += tbl[(val >> valb) & 0x3F];
            valb -= 6;
        }
    }
    if (valb > -6) out += tbl[((val << 8) >> (valb + 8)) & 0x3F];
    return out; 
}

inline string make_jwt(
    const string& account_id,
    const string& product_user_id = "",
    int64_t            iat_offset      = 0,
    int64_t            exp_offset      = 3600)
{
    
    const string header_json = R"({"alg":"none","typ":"JWT"})";

    std::time_t now = std::time(nullptr);
    int64_t iat = static_cast<int64_t>(now) + iat_offset;
    int64_t exp = iat + exp_offset;

    string payload_json = "{";
    payload_json += "\"sub\":\"" + account_id + "\",";
    payload_json += "\"iss\":\"epic\",";
    payload_json += "\"iat\":" + std::to_string(iat) + ",";
    payload_json += "\"exp\":" + std::to_string(exp);
    if (!product_user_id.empty())
        payload_json += ",\"puid\":\"" + product_user_id + "\"";
    payload_json += "}";

    return base64url_encode(header_json) + "."
         + base64url_encode(payload_json) + ".";
    
}

} 
