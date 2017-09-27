/*
 *
 *    Copyright (c) 2016-2017 Nest Labs, Inc.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file implements a process to effect a functional test for
 *      the Weave error string support library interfaces.
 *
 */

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include <stdint.h>
#include <string.h>
#include <Weave/Support/ASN1Error.h>
#include <InetLayer/InetError.h>
#include <BleLayer/BleError.h>
#include <Weave/Core/WeaveError.h>
#include <Weave/Support/ErrorStr.h>

#include <nltest.h>

#include "ToolCommon.h"


// Test input data.


static int32_t sContext[] = {
      0,
      WEAVE_SYSTEM_ERROR_NOT_IMPLEMENTED,
      WEAVE_SYSTEM_ERROR_NOT_SUPPORTED,
      WEAVE_SYSTEM_ERROR_BAD_ARGS,
      WEAVE_SYSTEM_ERROR_UNEXPECTED_STATE,
      WEAVE_SYSTEM_ERROR_UNEXPECTED_EVENT,
      WEAVE_SYSTEM_ERROR_NO_MEMORY,

      INET_ERROR_WRONG_ADDRESS_TYPE,
      INET_ERROR_CONNECTION_ABORTED,
      INET_ERROR_PEER_DISCONNECTED,
      INET_ERROR_INCORRECT_STATE,
      INET_ERROR_MESSAGE_TOO_LONG,
      INET_ERROR_NO_CONNECTION_HANDLER,
      INET_ERROR_NO_MEMORY,
      INET_ERROR_OUTBOUND_MESSAGE_TRUNCATED,
      INET_ERROR_INBOUND_MESSAGE_TOO_BIG,
      INET_ERROR_HOST_NOT_FOUND,
      INET_ERROR_DNS_TRY_AGAIN,
      INET_ERROR_DNS_NO_RECOVERY,
      INET_ERROR_BAD_ARGS,
      INET_ERROR_WRONG_PROTOCOL_TYPE,
      INET_ERROR_UNKNOWN_INTERFACE,
      INET_ERROR_NOT_IMPLEMENTED,
      INET_ERROR_ADDRESS_NOT_FOUND,
      INET_ERROR_HOST_NAME_TOO_LONG,
      INET_ERROR_INVALID_HOST_NAME,
      INET_ERROR_NOT_SUPPORTED,
      INET_ERROR_NO_ENDPOINTS,
      INET_ERROR_IDLE_TIMEOUT,
      INET_ERROR_UNEXPECTED_EVENT,
      INET_ERROR_INVALID_IPV6_PKT,
      INET_ERROR_INTERFACE_INIT_FAILURE,
      INET_ERROR_TCP_USER_TIMEOUT,

      BLE_ERROR_BAD_ARGS,
      BLE_ERROR_INCORRECT_STATE,
      BLE_ERROR_NO_ENDPOINTS,
      BLE_ERROR_NO_CONNECTION_RECEIVED_CALLBACK,
      BLE_ERROR_CENTRAL_UNSUBSCRIBED,
      BLE_ERROR_GATT_SUBSCRIBE_FAILED,
      BLE_ERROR_GATT_UNSUBSCRIBE_FAILED,
      BLE_ERROR_GATT_WRITE_FAILED,
      BLE_ERROR_GATT_INDICATE_FAILED,
      BLE_ERROR_NOT_IMPLEMENTED,
      BLE_ERROR_INVALID_ROLE,
      BLE_ERROR_WOBLE_PROTOCOL_ABORT,
      BLE_ERROR_REMOTE_DEVICE_DISCONNECTED,
      BLE_ERROR_APP_CLOSED_CONNECTION,
      BLE_ERROR_OUTBOUND_MESSAGE_TOO_BIG,
      BLE_ERROR_NOT_WEAVE_DEVICE,
      BLE_ERROR_INCOMPATIBLE_PROTOCOL_VERSIONS,
      BLE_ERROR_NO_MEMORY,
      BLE_ERROR_MESSAGE_INCOMPLETE,
      BLE_ERROR_INVALID_FRAGMENT_SIZE,
      BLE_ERROR_START_TIMER_FAILED,
      BLE_ERROR_CONNECT_TIMED_OUT,
      BLE_ERROR_RECEIVE_TIMED_OUT,
      BLE_ERROR_INVALID_MESSAGE,
      BLE_ERROR_FRAGMENT_ACK_TIMED_OUT,
      BLE_ERROR_KEEP_ALIVE_TIMED_OUT,
      BLE_ERRROR_NO_CONNECT_COMPLETE_CALLBACK,
      BLE_ERROR_INVALID_ACK,
      BLE_ERROR_REASSEMBLER_MISSING_DATA,
      BLE_ERROR_INVALID_BTP_HEADER_FLAGS,
      BLE_ERROR_INVALID_BTP_SEQUENCE_NUMBER,
      BLE_ERROR_REASSEMBLER_INCORRECT_STATE,
      BLE_ERROR_RECEIVED_MESSAGE_TOO_BIG,

      WEAVE_ERROR_TOO_MANY_CONNECTIONS,
      WEAVE_ERROR_SENDING_BLOCKED,
      WEAVE_ERROR_CONNECTION_ABORTED,
      WEAVE_ERROR_INCORRECT_STATE,
      WEAVE_ERROR_MESSAGE_TOO_LONG,
      WEAVE_ERROR_UNSUPPORTED_EXCHANGE_VERSION,
      WEAVE_ERROR_TOO_MANY_UNSOLICITED_MESSAGE_HANDLERS,
      WEAVE_ERROR_NO_UNSOLICITED_MESSAGE_HANDLER,
      WEAVE_ERROR_NO_CONNECTION_HANDLER,
      WEAVE_ERROR_TOO_MANY_PEER_NODES,
      WEAVE_ERROR_NO_MEMORY,
      WEAVE_ERROR_NO_MESSAGE_HANDLER,
      WEAVE_ERROR_MESSAGE_INCOMPLETE,
      WEAVE_ERROR_DATA_NOT_ALIGNED,
      WEAVE_ERROR_UNKNOWN_KEY_TYPE,
      WEAVE_ERROR_KEY_NOT_FOUND,
      WEAVE_ERROR_WRONG_ENCRYPTION_TYPE,
      WEAVE_ERROR_TOO_MANY_KEYS,
      WEAVE_ERROR_INTEGRITY_CHECK_FAILED,
      WEAVE_ERROR_INVALID_SIGNATURE,
      WEAVE_ERROR_UNSUPPORTED_MESSAGE_VERSION,
      WEAVE_ERROR_UNSUPPORTED_ENCRYPTION_TYPE,
      WEAVE_ERROR_UNSUPPORTED_SIGNATURE_TYPE,
      WEAVE_ERROR_INVALID_MESSAGE_LENGTH,
      WEAVE_ERROR_BUFFER_TOO_SMALL,
      WEAVE_ERROR_DUPLICATE_KEY_ID,
      WEAVE_ERROR_WRONG_KEY_TYPE,
      WEAVE_ERROR_WELL_UNINITIALIZED,
      WEAVE_ERROR_WELL_EMPTY,
      WEAVE_ERROR_INVALID_STRING_LENGTH,
      WEAVE_ERROR_INVALID_LIST_LENGTH,
      WEAVE_ERROR_INVALID_INTEGRITY_TYPE,
      WEAVE_END_OF_TLV,
      WEAVE_ERROR_TLV_UNDERRUN,
      WEAVE_ERROR_INVALID_TLV_ELEMENT,
      WEAVE_ERROR_INVALID_TLV_TAG,
      WEAVE_ERROR_UNKNOWN_IMPLICIT_TLV_TAG,
      WEAVE_ERROR_WRONG_TLV_TYPE,
      WEAVE_ERROR_TLV_CONTAINER_OPEN,
      WEAVE_ERROR_INVALID_TRANSFER_MODE,
      WEAVE_ERROR_INVALID_PROFILE_ID,
      WEAVE_ERROR_INVALID_MESSAGE_TYPE,
      WEAVE_ERROR_UNEXPECTED_TLV_ELEMENT,
      WEAVE_ERROR_STATUS_REPORT_RECEIVED,
      WEAVE_ERROR_NOT_IMPLEMENTED,
      WEAVE_ERROR_INVALID_ADDRESS,
      WEAVE_ERROR_INVALID_ARGUMENT,
      WEAVE_ERROR_TLV_TAG_NOT_FOUND,
      WEAVE_ERROR_INVALID_PATH_LIST,
      WEAVE_ERROR_INVALID_DATA_LIST,
      WEAVE_ERROR_TRANSACTION_CANCELED,
      WEAVE_ERROR_LISTENER_ALREADY_STARTED,
      WEAVE_ERROR_LISTENER_ALREADY_STOPPED,
      WEAVE_ERROR_UNKNOWN_TOPIC,

      WEAVE_ERROR_TIMEOUT,
      WEAVE_ERROR_INVALID_DEVICE_DESCRIPTOR,
      WEAVE_ERROR_UNSUPPORTED_DEVICE_DESCRIPTOR_VERSION,
      WEAVE_END_OF_INPUT,
      WEAVE_ERROR_RATE_LIMIT_EXCEEDED,
      WEAVE_ERROR_SECURITY_MANAGER_BUSY,
      WEAVE_ERROR_INVALID_PASE_PARAMETER,
      WEAVE_ERROR_PASE_SUPPORTS_ONLY_CONFIG1,
      WEAVE_ERROR_NO_COMMON_PASE_CONFIGURATIONS,
      WEAVE_ERROR_INVALID_PASE_CONFIGURATION,
      WEAVE_ERROR_KEY_CONFIRMATION_FAILED,
      WEAVE_ERROR_INVALID_USE_OF_SESSION_KEY,
      WEAVE_ERROR_CONNECTION_CLOSED_UNEXPECTEDLY,
      WEAVE_ERROR_MISSING_TLV_ELEMENT,
      WEAVE_ERROR_RANDOM_DATA_UNAVAILABLE,
      WEAVE_ERROR_UNSUPPORTED_HOST_PORT_ELEMENT,
      WEAVE_ERROR_INVALID_HOST_SUFFIX_INDEX,
      WEAVE_ERROR_HOST_PORT_LIST_EMPTY,
      WEAVE_ERROR_UNSUPPORTED_AUTH_MODE,

      WEAVE_ERROR_INVALID_SERVICE_EP,
      WEAVE_ERROR_INVALID_DIRECTORY_ENTRY_TYPE,
      WEAVE_ERROR_FORCED_RESET,
      WEAVE_ERROR_NO_ENDPOINT,
      WEAVE_ERROR_INVALID_DESTINATION_NODE_ID,
      WEAVE_ERROR_NOT_CONNECTED,
      WEAVE_ERROR_NO_SW_UPDATE_AVAILABLE,
      WEAVE_ERROR_CA_CERT_NOT_FOUND,
      WEAVE_ERROR_CERT_PATH_LEN_CONSTRAINT_EXCEEDED,
      WEAVE_ERROR_CERT_PATH_TOO_LONG,
      WEAVE_ERROR_CERT_USAGE_NOT_ALLOWED,
      WEAVE_ERROR_CERT_EXPIRED,
      WEAVE_ERROR_CERT_NOT_VALID_YET,
      WEAVE_ERROR_UNSUPPORTED_CERT_FORMAT,
      WEAVE_ERROR_UNSUPPORTED_ELLIPTIC_CURVE,
      WEAVE_CERT_NOT_USED,
      WEAVE_ERROR_CERT_NOT_FOUND,
      WEAVE_ERROR_INVALID_CASE_PARAMETER,
      WEAVE_ERROR_UNSUPPORTED_CASE_CONFIGURATION,
      WEAVE_ERROR_CERT_LOAD_FAIL,
      WEAVE_ERROR_CERT_NOT_TRUSTED,
      WEAVE_ERROR_INVALID_ACCESS_TOKEN,
      WEAVE_ERROR_WRONG_CERT_SUBJECT,
      WEAVE_ERROR_WRONG_NODE_ID,
      WEAVE_ERROR_CONN_ACCEPTED_ON_WRONG_PORT,
      WEAVE_ERROR_CALLBACK_REPLACED,
      WEAVE_ERROR_NO_CASE_AUTH_DELEGATE,
      WEAVE_ERROR_DEVICE_LOCATE_TIMEOUT,
      WEAVE_ERROR_DEVICE_CONNECT_TIMEOUT,
      WEAVE_ERROR_DEVICE_AUTH_TIMEOUT,
      WEAVE_ERROR_MESSAGE_NOT_ACKNOWLEDGED,
      WEAVE_ERROR_RETRANS_TABLE_FULL,
      WEAVE_ERROR_INVALID_ACK_ID,
      WEAVE_ERROR_SEND_THROTTLED,
      WEAVE_ERROR_WRONG_MSG_VERSION_FOR_EXCHANGE,
      WEAVE_ERROR_UNSUPPORTED_WEAVE_FEATURE,
      WEAVE_ERROR_UNSOLICITED_MSG_NO_ORIGINATOR,
      WEAVE_ERROR_UNSUPPORTED_TUNNEL_VERSION,
      WEAVE_ERROR_INVALID_FABRIC_ID,
      WEAVE_ERROR_TUNNEL_NEXTHOP_TABLE_FULL,
      WEAVE_ERROR_TUNNEL_SERVICE_QUEUE_FULL,
      WEAVE_ERROR_TUNNEL_PEER_ENTRY_NOT_FOUND,
      WEAVE_ERROR_TUNNEL_FORCE_ABORT,
      WEAVE_ERROR_DRBG_ENTROPY_SOURCE_FAILED,
      WEAVE_ERROR_NO_TAKE_AUTH_DELEGATE,
      WEAVE_ERROR_TAKE_RECONFIGURE_REQUIRED,
      WEAVE_ERROR_TAKE_REAUTH_POSSIBLE,
      WEAVE_ERROR_INVALID_TAKE_PARAMETER,
      WEAVE_ERROR_UNSUPPORTED_TAKE_CONFIGURATION,
      WEAVE_ERROR_TAKE_TOKEN_IDENTIFICATION_FAILED,

      WEAVE_ERROR_INVALID_TOKENPAIRINGBUNDLE,
      WEAVE_ERROR_UNSUPPORTED_TOKENPAIRINGBUNDLE_VERSION,

      WEAVE_ERROR_KEY_NOT_FOUND_FROM_PEER,
      WEAVE_ERROR_WRONG_ENCRYPTION_TYPE_FROM_PEER,
      WEAVE_ERROR_UNKNOWN_KEY_TYPE_FROM_PEER,
      WEAVE_ERROR_INVALID_USE_OF_SESSION_KEY_FROM_PEER,
      WEAVE_ERROR_UNSUPPORTED_ENCRYPTION_TYPE_FROM_PEER,
      WEAVE_ERROR_INTERNAL_KEY_ERROR_FROM_PEER,
      WEAVE_ERROR_INVALID_KEY_ID,
      WEAVE_ERROR_INVALID_TIME,
      WEAVE_ERROR_LOCKING_FAILURE,
      WEAVE_ERROR_UNSUPPORTED_PASSCODE_CONFIG,
      WEAVE_ERROR_PASSCODE_AUTHENTICATION_FAILED,
      WEAVE_ERROR_PASSCODE_FINGERPRINT_FAILED,
      WEAVE_ERROR_INVALID_INTEGER_VALUE,
      WEAVE_ERROR_TOO_MANY_CASE_RECONFIGURATIONS,
      WEAVE_ERROR_INVALID_MESSAGE_FLAG,

      WEAVE_ERROR_SERIALIZATION_ELEMENT_NULL,
      WEAVE_ERROR_KEY_EXPORT_RECONFIGURE_REQUIRED,
      WEAVE_ERROR_INVALID_KEY_EXPORT_CONFIGURATION,
      WEAVE_ERROR_NO_COMMON_KEY_EXPORT_CONFIGURATIONS,
      WEAVE_ERROR_NO_KEY_EXPORT_DELEGATE,
      WEAVE_ERROR_UNAUTHORIZED_KEY_EXPORT_REQUEST,
      WEAVE_ERROR_UNAUTHORIZED_KEY_EXPORT_RESPONSE,
      WEAVE_ERROR_EXPORTED_KEY_AUTHENTICATION_FAILED,
      WEAVE_ERROR_TOO_MANY_SHARED_SESSION_END_NODES,
      WEAVE_ERROR_WDM_MALFORMED_DATA_ELEMENT,
      WEAVE_ERROR_WRONG_CERT_TYPE,
      WEAVE_ERROR_DEFAULT_EVENT_HANDLER_NOT_CALLED,
      WEAVE_ERROR_PERSISTED_STORAGE_FAIL,
      WEAVE_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND,
      WEAVE_ERROR_TUNNEL_RESET_RECONNECT_ALREADY_ARMED,

      WEAVE_ERROR_TIME_NOT_SYNCED_YET,
      WEAVE_ERROR_UNSUPPORTED_CLOCK,
      WEAVE_ERROR_TUNNEL_ROUTING_RESTRICTED,

      ASN1_END,
      ASN1_ERROR_UNDERRUN,
      ASN1_ERROR_OVERFLOW,
      ASN1_ERROR_INVALID_STATE,
      ASN1_ERROR_MAX_DEPTH_EXCEEDED,
      ASN1_ERROR_INVALID_ENCODING,
      ASN1_ERROR_UNSUPPORTED_ENCODING,
      ASN1_ERROR_TAG_OVERFLOW,
      ASN1_ERROR_LENGTH_OVERFLOW,
      ASN1_ERROR_VALUE_OVERFLOW,
      ASN1_ERROR_UNKNOWN_OBJECT_ID,

      // SubsystemFormatError
      INET_ERROR_MIN + 150,
      WEAVE_ERROR_MIN + 150,
      ASN1_ERROR_MIN + 150,
      BLE_ERROR_MIN + 150
};

static const size_t kTestElements = sizeof(sContext) / sizeof(sContext[0]);


static void CheckErrorStrStart(nlTestSuite *inSuite, void *inContext)
{
    char errStr[20];

    // init errStr with FormatError(err), which is for the error that does not have a well defined description string
    // then compare the actural ErrorStr(err) with errStr to ensure that each err has its description sting
    for (size_t ith = 0; ith < kTestElements; ith++)
    {
#if WEAVE_CONFIG_SHORT_ERROR_STR
        snprintf(errStr, sizeof(errStr), "Error 0x%" PRIx32, sContext[ith]);
#else
        snprintf(errStr, sizeof(errStr), "Error %" PRId32, sContext[ith]);
#endif
        NL_TEST_ASSERT(inSuite, (strcmp(ErrorStr(sContext[ith]), errStr) != 0));
    }
}


/**
 *   Test Suite. It lists all the test functions.
 */
static const nlTest sTests[] = {
    NL_TEST_DEF("ErrorStr", CheckErrorStrStart),

    NL_TEST_SENTINEL()
};

/**
 *  Set up the test suite.
 *  This is a work-around to initiate InetBuffer protected class instance's
 *  data and set it to a known state, before an instance is created.
 */
static int TestSetup(void *inContext)
{
    return (SUCCESS);
}

/**
 *  Tear down the test suite.
 *  Free memory reserved at TestSetup.
 */
static int TestTeardown(void *inContext)
{

    return (SUCCESS);
}

int main(void)
{
    nlTestSuite theSuite = {
        "Error-Strings",
        &sTests[0],
        TestSetup,
        TestTeardown
    };

    // Generate machine-readable, comma-separated value (CSV) output.
    nl_test_set_output_style(OUTPUT_CSV);

    // Run test suit againt one context.
    nlTestRunner(&theSuite, &sContext);

    return nlTestRunnerStats(&theSuite);
}
