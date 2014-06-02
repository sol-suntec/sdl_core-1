/**
 * \file bluetooth_device.h
 * \brief BluetoothDevice class header file.
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_H_

// Todd: BT Support
//#include <bluetooth/bluetooth.h>
//#include <bluetooth/hci.h>
//#include <bluetooth/hci_lib.h>
//#include <bluetooth/sdp.h>
//#include <bluetooth/sdp_lib.h>
//#include <bluetooth/rfcomm.h>

#include <vector>
#include <applink_types.h>
#include <mqueue.h>
#include <pthread.h>

#include "transport_manager/common.h"
#include "transport_manager/transport_adapter/device.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief Type definition for vector that holds uint8_t variables.
 */
typedef std::vector<uint8_t> RfcommChannelVector;

/**
 * @brief Information about device that use bluetooth transport.
 */
class BluetoothDevice : public Device {
 public:

  /**
   * @brief Return device unique identifier.
   *
   * @return string with device unique identifier.
   */

// Todd: BT Support
//  static std::string GetUniqueDeviceId(const bdaddr_t& device_address);

  /**
   * @brief Constructor.
   *
   * @param address Bluetooth address.
   * @param name Human-readable device name.
   * @param rfcomm_channels List of RFCOMM channels where SmartDeviceLink service has been discovered.
   **/

// Todd: BT Support
//  BluetoothDevice(const bdaddr_t& address, const char* name,
//                  const RfcommChannelVector& rfcomm_channels);


// Todd: BT support

  struct SCOMMChannel
  {
      SCOMMChannel();
      SCOMMChannel(const char *sppQue);

//      bool empty() {return (mSendQueueName.empty() && mRcvQueueName.empty());}

      bool operator==(const SCOMMChannel& other) const {
    	  return mSppQueueName == other.mSppQueueName;
      }

      std::string mSppQueueName;

  };

  BluetoothDevice(const char* Name, const UI_8 (&mac)[6]);
  virtual ~BluetoothDevice();

  /**
   * @brief Compare devices.
   *
   * This method checks whether two SBluetoothDevice structures
   * refer to the same device.
   *
   * @param other Device to compare with.
   *
   * @return true if devices are equal, false otherwise.
   **/
  virtual bool IsSameAs(const Device* other) const;

// Todd: BT Support - multiple SPP support will be in the future.
//  bool GetRfcommChannel(const ApplicationHandle app_handle,
//                        uint8_t* channel_out);
   std::string GetSppQName(ApplicationHandle app_handle) const;

   void AddChannel(const SCOMMChannel& channel);
   bool RemoveChannel(const SCOMMChannel& channel, ApplicationHandle* app_handle);
   const UI_8* mac() const {return mac_;}

  /**
   * @brief Update list of applications available on device.
   *
   * @return Container with list of applications.
   */
  virtual ApplicationList GetApplicationList() const;

  /**
   * @brief Return device bluetooth address.
   *
   * @return Device bluetooth address.
   */

// Todd: BT Support
//  const bdaddr_t& address() const {
//    return address_;
//  }

  bool GetApplicationHandle(const SCOMMChannel& scomm_channel, ApplicationHandle* app_handle) const;

 private:
  /**
   * @brief Device bluetooth address.
   **/
// Todd: BT Support
//  bdaddr_t address_;

  /**
   * @brief List of RFCOMM channels where SmartDeviceLink service has been discovered.
   **/
// Todd: BT Support
//  RfcommChannelVector rfcomm_channels_;

  // Todd: BT Support
  typedef std::vector<std::pair<ApplicationHandle, SCOMMChannel> > Applications;
  Applications applications_;
  mutable pthread_mutex_t applications_mutex_;
  ApplicationHandle last_app_handle_;
  UI_8 mac_[6];
};

std::string MacToString(const UI_8 (&mac)[6]);

}  // namespace transport_adapter
}  // namespace transport_manager

#endif // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_DEVICE_H_
