/**
 * @file CFormatterJsonALRPCv1.hpp
 * @brief CFormatterJsonALRPCv1 header file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __CFORMATTERJSONALRPCV1_HPP__
#define __CFORMATTERJSONALRPCV1_HPP__

#include "SmartObjects/CSmartObject.hpp"
#include "json/json.h"

#include "CFormatterJsonBase.hpp"

#include "JSONHandler/CSmartFactory.hpp"
#include "SmartObjects/TEnumSchemaItem.hpp"

namespace NsSmartDeviceLink { namespace NsJSONHandler { namespace Formatters {

/**
 * @brief Class is used to convert SmartObjects to JSON string and vice versa.
 *
 * JSON strings is in ALPCv1 format.
 */
class CFormatterJsonALRPCv1: public CFormatterJsonBase {
private:
  /**
   * @brief Hidden constructor.
   *
   * The class contains only static methods. Should not be instantiated.
   */
  CFormatterJsonALRPCv1();

  /**
   * @brief Hidden copy constructor.
   *
   * The class contains only static methods. Should not be instantiated.
   */
  CFormatterJsonALRPCv1(const CFormatterJsonALRPCv1&);

  /**
   * @brief Extracts a message type from the SmartObject
   */
  static const std::string getMessageType(
      const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj);

  /**
   * @brief Extracts a message type from the root JSON object.
   *
   * @return Type or empty string if there's no type in the JSON object.
   */
  static const std::string getMessageType(const Json::Value& root);

  // ALRPCv1 string consts

  /**
   * @brief String constant for REQUEST.
   */
  static const std::string S_REQUEST;

  /**
   * @brief String constant for RESPONSE.
   */
  static const std::string S_RESPONSE;

  /**
   * @brief String constant for NOTIFICATION.
   */
  static const std::string S_NOTIFICATION;

  /**
   * @brief String constant for PARAMETERS.
   */
  static const std::string S_PARAMETERS;

  /**
   * @brief String constant for NAME.
   */
  static const std::string S_NAME;

  /**
   * @brief String constant for CORRELATION_ID.
   */
  static const std::string S_CORRELATION_ID;

public:

  static const int kSuccess;
  static const int kParsingError;
  static const int kFunctionIdNotFound;
  static const int kMessageTypeNotFound;
  static const int kCorrelationIdNotFound;

  /**
   * @brief Creates a JSON string from a SmartObject.
   *
   * @param obj input SmartObject
   * @param outStr resulting JSON string
   * @return true if success, false otherwise
   */
  static bool toString(
      const NsSmartDeviceLink::NsSmartObjects::CSmartObject &obj,
      std::string& outStr);

  /**
   * @brief Creates a SmartObject from a JSON string.
   *
   * @param str input JSON string
   * @param out The resulting SmartObject
   * @return true if success, otherwise - false
   */
  template<typename FunctionId, typename MessageType>
  static int fromString(const std::string &str,
      NsSmartDeviceLink::NsSmartObjects::CSmartObject &out);

};

// ----------------------------------------------------------------------------

template<typename FunctionId, typename MessageType>
int Formatters::CFormatterJsonALRPCv1::fromString(const std::string& str,
    NsSmartDeviceLink::NsSmartObjects::CSmartObject& out) {
  Json::Value root;
  Json::Reader reader;
  std::string type;
  int result = kSuccess;

  if (false == reader.parse(str, root)) {
    result = kParsingError | kMessageTypeNotFound |
              kFunctionIdNotFound | kCorrelationIdNotFound;
  }

  if (kSuccess == result) {
    type = getMessageType(root);
    if (true == type.empty())
      result = kMessageTypeNotFound | kFunctionIdNotFound | kCorrelationIdNotFound;
  }

  FunctionId functionId;
  MessageType messageType;

  if (kSuccess == result) {
    typedef NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<MessageType> MessageTypeEnum;
    if (false == MessageTypeEnum::stringToEnum(type, messageType)) {
      // If MessageType is not found than FunctionId and CorrelationId can not be found either
      result = kMessageTypeNotFound | kFunctionIdNotFound | kCorrelationIdNotFound;
    }
  }

  if (kSuccess == result) {
    typedef NsSmartDeviceLink::NsSmartObjects::TEnumSchemaItem<FunctionId> FunctionIdEnum;
    if (false == FunctionIdEnum::stringToEnum(root[type][S_NAME].asString(), functionId))
      result = kFunctionIdNotFound;
  }

  namespace S = NsSmartDeviceLink::NsJSONHandler::strings;

  if (!(result & kMessageTypeNotFound)) {
    jsonValueToObj(root[type][S_PARAMETERS], out[S::S_MSG_PARAMS]);

    out[S::S_PARAMS][S::S_MESSAGE_TYPE] = messageType;
    out[S::S_PARAMS][S::S_FUNCTION_ID] = functionId;
    out[S::S_PARAMS][S::S_CORRELATION_ID] =
        root[type][S_CORRELATION_ID].asInt();
    out[S::S_PARAMS][S::S_PROTOCOL_TYPE] = 0;
    out[S::S_PARAMS][S::S_PROTOCOL_VERSION] = 1;
  }

  return result;
}

} } } // namespace NsSmartDeviceLink::NsJSONHandler::Formatters

#endif // __CFORMATTERJSONALRPCV1_HPP__
