#if defined(RENDERSTACK_USE_AMD_GPU_PERF_API_AND_ADL)

#include "renderstack_toolkit/platform.hpp"
#include "renderstack_toolkit/amd_gpa_adl.hpp"
#include "renderstack_toolkit/window.hpp"

#include "GPUPerfAPIFunctionTypes.h"
#include "GPUPerfAPITypes.h"

#include "adl_defines.h"
#include "adl_sdk.h"
#include "adl_structures.h"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>

namespace renderstack { namespace toolkit {

using namespace std;

/* Main control */
typedef int (*PFN_ADL2_Main_ControlX2_Create                      )(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADL_CONTEXT_HANDLE *context, ADLThreadingModel threadingModel);
typedef int (*PFN_ADL_Main_ControlX2_Create                       )(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADLThreadingModel threadingModel);
typedef int (*PFN_ADL2_Main_Control_Create                        )(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters, ADL_CONTEXT_HANDLE *context);
typedef int (*PFN_ADL_Main_Control_Create                         )(ADL_MAIN_MALLOC_CALLBACK callback, int iEnumConnectedAdapters);
typedef int (*PFN_ADL2_Main_Control_Refresh                       )(ADL_CONTEXT_HANDLE context);
typedef int (*PFN_ADL_Main_Control_Refresh                        )();
typedef int (*PFN_ADL2_Main_Control_Destroy                       )(ADL_CONTEXT_HANDLE context);
typedef int (*PFN_ADL_Main_Control_Destroy                        )();
typedef int (*PFN_ADL2_Graphics_Platform_Get                      )(ADL_CONTEXT_HANDLE context, int *lpPlatForm);
typedef int (*PFN_ADL_Graphics_Platform_Get                       )(int *lpPlatForm);

/* General Adapter APIs */
typedef int (*PFN_ADL2_Adapter_Active_Get                         )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpStatus);
typedef int (*PFN_ADL_Adapter_Active_Get                          )(int iAdapterIndex, int *lpStatus);
typedef int (*PFN_ADL2_Adapter_NumberOfAdapters_Get               )(ADL_CONTEXT_HANDLE context, int *lpNumAdapters);
typedef int (*PFN_ADL_Adapter_NumberOfAdapters_Get                )(int *lpNumAdapters);
typedef int (*PFN_ADL2_Flush_Driver_Data                          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex);
typedef int (*PFN_ADL_Flush_Driver_Data                           )(int iAdapterIndex);
typedef int (*PFN_ADL2_Adapter_AdapterInfo_Get                    )(ADL_CONTEXT_HANDLE context, LPAdapterInfo lpInfo, int iInputSize);
typedef int (*PFN_ADL_Adapter_AdapterInfo_Get                     )(LPAdapterInfo lpInfo, int iInputSize);
typedef int (*PFN_ADL2_Adapter_AdapterInfoX2_Get                  )(ADL_CONTEXT_HANDLE context, AdapterInfo **lppAdapterInfo);
typedef int (*PFN_ADL_Adapter_AdapterInfoX2_Get                   )(AdapterInfo **lppAdapterInfo);
typedef int (*PFN_ADL2_Adapter_ASICFamilyType_Get                 )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpAsicTypes, int *lpValids);
typedef int (*PFN_ADL_Adapter_ASICFamilyType_Get                  )(int iAdapterIndex, int *lpAsicTypes, int *lpValids);
typedef int (*PFN_ADL2_Adapter_Speed_Caps                         )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCaps, int *lpValid);
typedef int (*PFN_ADL_Adapter_Speed_Caps                          )(int iAdapterIndex, int *lpCaps, int *lpValid);
typedef int (*PFN_ADL2_Adapter_Speed_Get                          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrent, int *lpDefault);
typedef int (*PFN_ADL_Adapter_Speed_Get                           )(int iAdapterIndex, int *lpCurrent, int *lpDefault);
typedef int (*PFN_ADL2_Adapter_Speed_Set                          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iSpeed);
typedef int (*PFN_ADL_Adapter_Speed_Set                           )(int iAdapterIndex, int iSpeed);
typedef int (*PFN_ADL2_Adapter_Accessibility_Get                  )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpAccessibility);
typedef int (*PFN_ADL_Adapter_Accessibility_Get                   )(int iAdapterIndex, int *lpAccessibility);
typedef int (*PFN_ADL2_Adapter_VideoBiosInfo_Get                  )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLBiosInfo *lpBiosInfo);
typedef int (*PFN_ADL_Adapter_VideoBiosInfo_Get                   )(int iAdapterIndex, ADLBiosInfo *lpBiosInfo);
typedef int (*PFN_ADL2_Adapter_ID_Get                             )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpAdapterID);
typedef int (*PFN_ADL_Adapter_ID_Get                              )(int iAdapterIndex, int *lpAdapterID);

/* Overdrive5 */
typedef int (*PFN_ADL2_Overdrive5_CurrentActivity_Get             )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLPMActivity *lpActivity);
typedef int (*PFN_ADL_Overdrive5_CurrentActivity_Get              )(int iAdapterIndex, ADLPMActivity *lpActivity);
typedef int (*PFN_ADL2_Overdrive5_ThermalDevices_Enum             )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo *lpThermalControllerInfo);
typedef int (*PFN_ADL_Overdrive5_ThermalDevices_Enum              )(int iAdapterIndex, int iThermalControllerIndex, ADLThermalControllerInfo *lpThermalControllerInfo);
typedef int (*PFN_ADL2_Overdrive5_Temperature_Get                 )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLTemperature *lpTemperature);
typedef int (*PFN_ADL_Overdrive5_Temperature_Get                  )(int iAdapterIndex, int iThermalControllerIndex, ADLTemperature *lpTemperature);
typedef int (*PFN_ADL2_Overdrive5_FanSpeedInfo_Get                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo);
typedef int (*PFN_ADL_Overdrive5_FanSpeedInfo_Get                 )(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedInfo *lpFanSpeedInfo);
typedef int (*PFN_ADL2_Overdrive5_FanSpeed_Get                    )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL_Overdrive5_FanSpeed_Get                     )(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL2_Overdrive5_FanSpeed_Set                    )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL_Overdrive5_FanSpeed_Set                     )(int iAdapterIndex, int iThermalControllerIndex, ADLFanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL2_Overdrive5_FanSpeedToDefault_Set           )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iThermalControllerIndex);
typedef int (*PFN_ADL_Overdrive5_FanSpeedToDefault_Set            )(int iAdapterIndex, int iThermalControllerIndex);
typedef int (*PFN_ADL2_Overdrive5_ODParameters_Get                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLODParameters *lpOdParameters);
typedef int (*PFN_ADL_Overdrive5_ODParameters_Get                 )(int iAdapterIndex, ADLODParameters *lpOdParameters);
typedef int (*PFN_ADL2_Overdrive5_ODPerformanceLevels_Get         )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iDefault, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int (*PFN_ADL_Overdrive5_ODPerformanceLevels_Get          )(int iAdapterIndex, int iDefault, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int (*PFN_ADL2_Overdrive5_ODPerformanceLevels_Set         )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int (*PFN_ADL_Overdrive5_ODPerformanceLevels_Set          )(int iAdapterIndex, ADLODPerformanceLevels *lpOdPerformanceLevels);
typedef int (*PFN_ADL2_Overdrive5_PowerControl_Caps               )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL_Overdrive5_PowerControl_Caps                )(int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL2_Overdrive5_PowerControlInfo_Get            )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo);
typedef int (*PFN_ADL_Overdrive5_PowerControlInfo_Get             )(int iAdapterIndex, ADLPowerControlInfo *lpPowerControlInfo);
typedef int (*PFN_ADL2_Overdrive5_PowerControl_Get                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL_Overdrive5_PowerControl_Get                 )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL2_Overdrive5_PowerControl_Set                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iValue);
typedef int (*PFN_ADL_Overdrive5_PowerControl_Set                 )(int iAdapterIndex, int iValue);
typedef int (*PFN_ADL2_Overdrive_Caps                             )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *iSupported, int *iEnabled, int *iVersion);
typedef int (*PFN_ADL_Overdrive_Caps                              )(int iAdapterIndex, int *iSupported, int *iEnabled, int *iVersion);
 	
/* Overdrive6 */
typedef int (*PFN_ADL2_Overdrive6_Capabilities_Get                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6Capabilities *lpODCapabilities);
typedef int (*PFN_ADL_Overdrive6_Capabilities_Get                 )(int iAdapterIndex, ADLOD6Capabilities* lpODCapabilities);
typedef int (*PFN_ADL2_Overdrive6_StateInfo_Get                   )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, ADLOD6StateInfo* lpStateInfo);
typedef int (*PFN_ADL_Overdrive6_StateInfo_Get                    )(int iAdapterIndex, int iStateType, ADLOD6StateInfo* lpStateInfo);
typedef int (*PFN_ADL2_Overdrive6_State_Set                       )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, ADLOD6StateInfo* lpStateInfo);
typedef int (*PFN_ADL_Overdrive6_State_Set                        )(int iAdapterIndex, int iStateType, ADLOD6StateInfo *lpStateInfo);
typedef int (*PFN_ADL2_Overdrive6_State_Reset                     )(ADL_CONTEXT_HANDLE context,int iAdapterIndex, int iStateType);
typedef int (*PFN_ADL_Overdrive6_State_Reset                      )(int iAdapterIndex, int iStateType);
typedef int (*PFN_ADL2_Overdrive6_CurrentStatus_Get               )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus);
typedef int (*PFN_ADL_Overdrive6_CurrentStatus_Get                )(int iAdapterIndex, ADLOD6CurrentStatus *lpCurrentStatus);
typedef int (*PFN_ADL2_Overdrive6_ThermalController_Caps          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int (*PFN_ADL_Overdrive6_ThermalController_Caps           )(int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int (*PFN_ADL2_Overdrive6_Temperature_Get                 )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpTemperature);
typedef int (*PFN_ADL_Overdrive6_Temperature_Get                  )(int iAdapterIndex, int *lpTemperature);
typedef int (*PFN_ADL2_Overdrive6_FanSpeed_Get                    )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo);
typedef int (*PFN_ADL_Overdrive6_FanSpeed_Get                     )(int iAdapterIndex, ADLOD6FanSpeedInfo *lpFanSpeedInfo);
typedef int (*PFN_ADL2_Overdrive6_FanSpeed_Set                    )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6FanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL_Overdrive6_FanSpeed_Set                     )(int iAdapterIndex, ADLOD6FanSpeedValue *lpFanSpeedValue);
typedef int (*PFN_ADL2_Overdrive6_FanSpeed_Reset                  )(ADL_CONTEXT_HANDLE context, int iAdapterIndex);
typedef int (*PFN_ADL_Overdrive6_FanSpeed_Reset                   )(int iAdapterIndex);
typedef int (*PFN_ADL2_Overdrive6_PowerControl_Caps               )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL_Overdrive6_PowerControl_Caps                )(int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL2_Overdrive6_PowerControlInfo_Get            )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6PowerControlInfo *lpPowerControlInfo);
typedef int (*PFN_ADL_Overdrive6_PowerControlInfo_Get             )(int iAdapterIndex, ADLOD6PowerControlInfo *lpPowerControlInfo);
typedef int (*PFN_ADL2_Overdrive6_PowerControl_Get                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL_Overdrive6_PowerControl_Get                 )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL2_Overdrive6_PowerControl_Set                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iValue);
typedef int (*PFN_ADL_Overdrive6_PowerControl_Set                 )(int iAdapterIndex, int iValue);
typedef int (*PFN_ADL2_Overdrive6_VoltageControlInfo_Get          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6VoltageControlInfo *lpVoltageControlInfo);
typedef int (*PFN_ADL_Overdrive6_VoltageControlInfo_Get           )(int iAdapterIndex, ADLOD6VoltageControlInfo *lpVoltageControlInfo);
typedef int (*PFN_ADL2_Overdrive6_VoltageControl_Get              )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL_Overdrive6_VoltageControl_Get               )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL2_Overdrive6_VoltageControl_Set              )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iValue);
typedef int (*PFN_ADL_Overdrive6_VoltageControl_Set               )(int iAdapterIndex, int iValue);
typedef int (*PFN_ADL2_Overdrive6_CapabilitiesEx_Get              )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6CapabilitiesEx *lpODCapabilities);
typedef int (*PFN_ADL_Overdrive6_CapabilitiesEx_Get               )(int iAdapterIndex, ADLOD6CapabilitiesEx *lpODCapabilities);
typedef int (*PFN_ADL2_Overdrive6_StateEx_Get                     )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, ADLOD6StateEx *lpODState);
typedef int (*PFN_ADL_Overdrive6_StateEx_Get                      )(int iAdapterIndex, int iStateType, ADLOD6StateEx *lpODState);
typedef int (*PFN_ADL2_Overdrive6_StateEx_Set                     )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, ADLOD6StateEx *lpODState);
typedef int (*PFN_ADL_Overdrive6_StateEx_Set                      )(int iAdapterIndex, int iStateType, ADLOD6StateEx *lpODState);
typedef int (*PFN_ADL_Overdrive6_ThermalLimitUnlock_Set           )(int iAdapterIndex, int iStateType, int iEnable);
typedef int (*PFN_ADL2_Overdrive6_ThermalLimitUnlock_Set          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, int iEnable);
typedef int (*PFN_ADL_Overdrive6_ThermalLimitUnlock_Get           )(int iAdapterIndex, int iStateType, int *pEnabled);
typedef int (*PFN_ADL2_Overdrive6_ThermalLimitUnlock_Get          )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iStateType, int *pEnabled);
typedef int (*PFN_ADL_Overdrive6_AdvancedFan_Caps                 )(int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL2_Overdrive6_AdvancedFan_Caps                )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpSupported);
typedef int (*PFN_ADL_Overdrive6_TargetTemperatureRangeInfo_Get   )(int iAdapterIndex, ADLOD6ParameterRange *lpTargetTemperatureInfo);
typedef int (*PFN_ADL2_Overdrive6_TargetTemperatureRangeInfo_Get  )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6ParameterRange *lpTargetTemperatureInfo);
typedef int (*PFN_ADL_Overdrive6_TargetTemperatureData_Get        )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL2_Overdrive6_TargetTemperatureData_Get       )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL_Overdrive6_TargetTemperatureData_Set        )(int iAdapterIndex, int iCurrentValue);
typedef int (*PFN_ADL2_Overdrive6_TargetTemperatureData_Set       )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iCurrentValue);
typedef int (*PFN_ADL_Overdrive6_FanPWMLimitRangeInfo_Get         )(int iAdapterIndex, ADLOD6ParameterRange *lpFanPWMLimitInfo);
typedef int (*PFN_ADL2_Overdrive6_FanPWMLimitRangeInfo_Get        )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, ADLOD6ParameterRange *lpFanPWMLimitInfo);
typedef int (*PFN_ADL_Overdrive6_FanPWMLimitData_Get              )(int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL2_Overdrive6_FanPWMLimitData_Get             )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int *lpCurrentValue, int *lpDefaultValue);
typedef int (*PFN_ADL_Overdrive6_FanPWMLimitData_Set              )(int iAdapterIndex, int iCurrentValue);
typedef int (*PFN_ADL2_Overdrive6_FanPWMLimitData_Set	            )(ADL_CONTEXT_HANDLE context, int iAdapterIndex, int iCurrentValue);

GPA_RegisterLoggingCallbackPtrType  GPA_RegisterLoggingCallback   = nullptr;
GPA_InitializePtrType               GPA_Initialize                = nullptr;
GPA_DestroyPtrType                  GPA_Destroy                   = nullptr;
GPA_OpenContextPtrType              GPA_OpenContext               = nullptr;
GPA_CloseContextPtrType             GPA_CloseContext              = nullptr;
GPA_SelectContextPtrType            GPA_SelectContext             = nullptr;
GPA_GetNumCountersPtrType           GPA_GetNumCounters            = nullptr;
GPA_GetCounterNamePtrType           GPA_GetCounterName            = nullptr;
GPA_GetCounterDescriptionPtrType    GPA_GetCounterDescription     = nullptr;
GPA_GetCounterDataTypePtrType       GPA_GetCounterDataType        = nullptr;
GPA_GetCounterUsageTypePtrType      GPA_GetCounterUsageType       = nullptr;
GPA_GetDataTypeAsStrPtrType         GPA_GetDataTypeAsStr          = nullptr;
GPA_GetUsageTypeAsStrPtrType        GPA_GetUsageTypeAsStr         = nullptr;
GPA_GetStatusAsStrPtrType           GPA_GetStatusAsStr            = nullptr;
GPA_EnableCounterPtrType            GPA_EnableCounter             = nullptr;
GPA_DisableCounterPtrType           GPA_DisableCounter            = nullptr;
GPA_GetEnabledCountPtrType          GPA_GetEnabledCount           = nullptr;
GPA_GetEnabledIndexPtrType          GPA_GetEnabledIndex           = nullptr;
GPA_IsCounterEnabledPtrType         GPA_IsCounterEnabled          = nullptr;
GPA_EnableCounterStrPtrType         GPA_EnableCounterStr          = nullptr;
GPA_DisableCounterStrPtrType        GPA_DisableCounterStr         = nullptr;
GPA_EnableAllCountersPtrType        GPA_EnableAllCounters         = nullptr;
GPA_DisableAllCountersPtrType       GPA_DisableAllCounters        = nullptr;
GPA_GetCounterIndexPtrType          GPA_GetCounterIndex           = nullptr;
GPA_GetPassCountPtrType             GPA_GetPassCount              = nullptr;
GPA_BeginSessionPtrType             GPA_BeginSession              = nullptr;
GPA_EndSessionPtrType               GPA_EndSession                = nullptr;
GPA_BeginPassPtrType                GPA_BeginPass                 = nullptr;
GPA_EndPassPtrType                  GPA_EndPass                   = nullptr;
GPA_BeginSamplePtrType              GPA_BeginSample               = nullptr;
GPA_EndSamplePtrType                GPA_EndSample                 = nullptr;
GPA_GetSampleCountPtrType           GPA_GetSampleCount            = nullptr;
GPA_IsSampleReadyPtrType            GPA_IsSampleReady             = nullptr;
GPA_IsSessionReadyPtrType           GPA_IsSessionReady            = nullptr;
GPA_GetSampleUInt64PtrType          GPA_GetSampleUInt64           = nullptr;
GPA_GetSampleUInt32PtrType          GPA_GetSampleUInt32           = nullptr;
GPA_GetSampleFloat32PtrType         GPA_GetSampleFloat32          = nullptr;
GPA_GetSampleFloat64PtrType         GPA_GetSampleFloat64          = nullptr;

PFN_ADL2_Main_ControlX2_Create                     ADL2_Main_ControlX2_Create                      = nullptr;
PFN_ADL_Main_ControlX2_Create                      ADL_Main_ControlX2_Create                       = nullptr;
PFN_ADL2_Main_Control_Create                       ADL2_Main_Control_Create                        = nullptr;
PFN_ADL_Main_Control_Create                        ADL_Main_Control_Create                         = nullptr;
PFN_ADL2_Main_Control_Refresh                      ADL2_Main_Control_Refresh                       = nullptr;
PFN_ADL_Main_Control_Refresh                       ADL_Main_Control_Refresh                        = nullptr;
PFN_ADL2_Main_Control_Destroy                      ADL2_Main_Control_Destroy                       = nullptr;
PFN_ADL_Main_Control_Destroy                       ADL_Main_Control_Destroy                        = nullptr;
PFN_ADL2_Graphics_Platform_Get                     ADL2_Graphics_Platform_Get                      = nullptr;
PFN_ADL_Graphics_Platform_Get                      ADL_Graphics_Platform_Get                       = nullptr;

PFN_ADL2_Adapter_Active_Get                        ADL2_Adapter_Active_Get                         = nullptr;
PFN_ADL_Adapter_Active_Get                         ADL_Adapter_Active_Get                          = nullptr;
PFN_ADL2_Adapter_NumberOfAdapters_Get              ADL2_Adapter_NumberOfAdapters_Get               = nullptr;
PFN_ADL_Adapter_NumberOfAdapters_Get               ADL_Adapter_NumberOfAdapters_Get                = nullptr;
PFN_ADL2_Flush_Driver_Data                         ADL2_Flush_Driver_Data                          = nullptr;
PFN_ADL_Flush_Driver_Data                          ADL_Flush_Driver_Data                           = nullptr;
PFN_ADL2_Adapter_AdapterInfo_Get                   ADL2_Adapter_AdapterInfo_Get                    = nullptr;
PFN_ADL_Adapter_AdapterInfo_Get                    ADL_Adapter_AdapterInfo_Get                     = nullptr;
PFN_ADL2_Adapter_AdapterInfoX2_Get                 ADL2_Adapter_AdapterInfoX2_Get                  = nullptr;
PFN_ADL_Adapter_AdapterInfoX2_Get                  ADL_Adapter_AdapterInfoX2_Get                   = nullptr;
PFN_ADL2_Adapter_ASICFamilyType_Get                ADL2_Adapter_ASICFamilyType_Get                 = nullptr;
PFN_ADL_Adapter_ASICFamilyType_Get                 ADL_Adapter_ASICFamilyType_Get                  = nullptr;
PFN_ADL2_Adapter_Speed_Caps                        ADL2_Adapter_Speed_Caps                         = nullptr;
PFN_ADL_Adapter_Speed_Caps                         ADL_Adapter_Speed_Caps                          = nullptr;
PFN_ADL2_Adapter_Speed_Get                         ADL2_Adapter_Speed_Get                          = nullptr;
PFN_ADL_Adapter_Speed_Get                          ADL_Adapter_Speed_Get                           = nullptr;
PFN_ADL2_Adapter_Speed_Set                         ADL2_Adapter_Speed_Set                          = nullptr;
PFN_ADL_Adapter_Speed_Set                          ADL_Adapter_Speed_Set                           = nullptr;
PFN_ADL2_Adapter_Accessibility_Get                 ADL2_Adapter_Accessibility_Get                  = nullptr;
PFN_ADL_Adapter_Accessibility_Get                  ADL_Adapter_Accessibility_Get                   = nullptr;
PFN_ADL2_Adapter_VideoBiosInfo_Get                 ADL2_Adapter_VideoBiosInfo_Get                  = nullptr;
PFN_ADL_Adapter_VideoBiosInfo_Get                  ADL_Adapter_VideoBiosInfo_Get                   = nullptr;
PFN_ADL2_Adapter_ID_Get                            ADL2_Adapter_ID_Get                             = nullptr;
PFN_ADL_Adapter_ID_Get                             ADL_Adapter_ID_Get                              = nullptr;

PFN_ADL2_Overdrive5_CurrentActivity_Get            ADL2_Overdrive5_CurrentActivity_Get             = nullptr;
PFN_ADL_Overdrive5_CurrentActivity_Get             ADL_Overdrive5_CurrentActivity_Get              = nullptr;
PFN_ADL2_Overdrive5_ThermalDevices_Enum            ADL2_Overdrive5_ThermalDevices_Enum             = nullptr;
PFN_ADL_Overdrive5_ThermalDevices_Enum             ADL_Overdrive5_ThermalDevices_Enum              = nullptr;
PFN_ADL2_Overdrive5_Temperature_Get                ADL2_Overdrive5_Temperature_Get                 = nullptr;
PFN_ADL_Overdrive5_Temperature_Get                 ADL_Overdrive5_Temperature_Get                  = nullptr;
PFN_ADL2_Overdrive5_FanSpeedInfo_Get               ADL2_Overdrive5_FanSpeedInfo_Get                = nullptr;
PFN_ADL_Overdrive5_FanSpeedInfo_Get                ADL_Overdrive5_FanSpeedInfo_Get                 = nullptr;
PFN_ADL2_Overdrive5_FanSpeed_Get                   ADL2_Overdrive5_FanSpeed_Get                    = nullptr;
PFN_ADL_Overdrive5_FanSpeed_Get                    ADL_Overdrive5_FanSpeed_Get                     = nullptr;
PFN_ADL2_Overdrive5_FanSpeed_Set                   ADL2_Overdrive5_FanSpeed_Set                    = nullptr;
PFN_ADL_Overdrive5_FanSpeed_Set                    ADL_Overdrive5_FanSpeed_Set                     = nullptr;
PFN_ADL2_Overdrive5_FanSpeedToDefault_Set          ADL2_Overdrive5_FanSpeedToDefault_Set           = nullptr;
PFN_ADL_Overdrive5_FanSpeedToDefault_Set           ADL_Overdrive5_FanSpeedToDefault_Set            = nullptr;
PFN_ADL2_Overdrive5_ODParameters_Get               ADL2_Overdrive5_ODParameters_Get                = nullptr;
PFN_ADL_Overdrive5_ODParameters_Get                ADL_Overdrive5_ODParameters_Get                 = nullptr;
PFN_ADL2_Overdrive5_ODPerformanceLevels_Get        ADL2_Overdrive5_ODPerformanceLevels_Get         = nullptr;
PFN_ADL_Overdrive5_ODPerformanceLevels_Get         ADL_Overdrive5_ODPerformanceLevels_Get          = nullptr;
PFN_ADL2_Overdrive5_ODPerformanceLevels_Set        ADL2_Overdrive5_ODPerformanceLevels_Set         = nullptr;
PFN_ADL_Overdrive5_ODPerformanceLevels_Set         ADL_Overdrive5_ODPerformanceLevels_Set          = nullptr;
PFN_ADL2_Overdrive5_PowerControl_Caps              ADL2_Overdrive5_PowerControl_Caps               = nullptr;
PFN_ADL_Overdrive5_PowerControl_Caps               ADL_Overdrive5_PowerControl_Caps                = nullptr;
PFN_ADL2_Overdrive5_PowerControlInfo_Get           ADL2_Overdrive5_PowerControlInfo_Get            = nullptr;
PFN_ADL_Overdrive5_PowerControlInfo_Get            ADL_Overdrive5_PowerControlInfo_Get             = nullptr;
PFN_ADL2_Overdrive5_PowerControl_Get               ADL2_Overdrive5_PowerControl_Get                = nullptr;
PFN_ADL_Overdrive5_PowerControl_Get                ADL_Overdrive5_PowerControl_Get                 = nullptr;
PFN_ADL2_Overdrive5_PowerControl_Set               ADL2_Overdrive5_PowerControl_Set                = nullptr;
PFN_ADL_Overdrive5_PowerControl_Set                ADL_Overdrive5_PowerControl_Set                 = nullptr;
PFN_ADL2_Overdrive_Caps                            ADL2_Overdrive_Caps                             = nullptr;
PFN_ADL_Overdrive_Caps                             ADL_Overdrive_Caps                              = nullptr;

PFN_ADL2_Overdrive6_Capabilities_Get               ADL2_Overdrive6_Capabilities_Get                = nullptr;
PFN_ADL_Overdrive6_Capabilities_Get                ADL_Overdrive6_Capabilities_Get                 = nullptr;
PFN_ADL2_Overdrive6_StateInfo_Get                  ADL2_Overdrive6_StateInfo_Get                   = nullptr;
PFN_ADL_Overdrive6_StateInfo_Get                   ADL_Overdrive6_StateInfo_Get                    = nullptr;
PFN_ADL2_Overdrive6_State_Set                      ADL2_Overdrive6_State_Set                       = nullptr;
PFN_ADL_Overdrive6_State_Set                       ADL_Overdrive6_State_Set                        = nullptr;
PFN_ADL2_Overdrive6_State_Reset                    ADL2_Overdrive6_State_Reset                     = nullptr;
PFN_ADL_Overdrive6_State_Reset                     ADL_Overdrive6_State_Reset                      = nullptr;
PFN_ADL2_Overdrive6_CurrentStatus_Get              ADL2_Overdrive6_CurrentStatus_Get               = nullptr;
PFN_ADL_Overdrive6_CurrentStatus_Get               ADL_Overdrive6_CurrentStatus_Get                = nullptr;
PFN_ADL2_Overdrive6_ThermalController_Caps         ADL2_Overdrive6_ThermalController_Caps          = nullptr;
PFN_ADL_Overdrive6_ThermalController_Caps          ADL_Overdrive6_ThermalController_Caps           = nullptr;
PFN_ADL2_Overdrive6_Temperature_Get                ADL2_Overdrive6_Temperature_Get                 = nullptr;
PFN_ADL_Overdrive6_Temperature_Get                 ADL_Overdrive6_Temperature_Get                  = nullptr;
PFN_ADL2_Overdrive6_FanSpeed_Get                   ADL2_Overdrive6_FanSpeed_Get                    = nullptr;
PFN_ADL_Overdrive6_FanSpeed_Get                    ADL_Overdrive6_FanSpeed_Get                     = nullptr;
PFN_ADL2_Overdrive6_FanSpeed_Set                   ADL2_Overdrive6_FanSpeed_Set                    = nullptr;
PFN_ADL_Overdrive6_FanSpeed_Set                    ADL_Overdrive6_FanSpeed_Set                     = nullptr;
PFN_ADL2_Overdrive6_FanSpeed_Reset                 ADL2_Overdrive6_FanSpeed_Reset                  = nullptr;
PFN_ADL_Overdrive6_FanSpeed_Reset                  ADL_Overdrive6_FanSpeed_Reset                   = nullptr;
PFN_ADL2_Overdrive6_PowerControl_Caps              ADL2_Overdrive6_PowerControl_Caps               = nullptr;
PFN_ADL_Overdrive6_PowerControl_Caps               ADL_Overdrive6_PowerControl_Caps                = nullptr;
PFN_ADL2_Overdrive6_PowerControlInfo_Get           ADL2_Overdrive6_PowerControlInfo_Get            = nullptr;
PFN_ADL_Overdrive6_PowerControlInfo_Get            ADL_Overdrive6_PowerControlInfo_Get             = nullptr;
PFN_ADL2_Overdrive6_PowerControl_Get               ADL2_Overdrive6_PowerControl_Get                = nullptr;
PFN_ADL_Overdrive6_PowerControl_Get                ADL_Overdrive6_PowerControl_Get	               = nullptr;
PFN_ADL2_Overdrive6_PowerControl_Set               ADL2_Overdrive6_PowerControl_Set                = nullptr;
PFN_ADL_Overdrive6_PowerControl_Set                ADL_Overdrive6_PowerControl_Set                 = nullptr;
PFN_ADL2_Overdrive6_VoltageControlInfo_Get         ADL2_Overdrive6_VoltageControlInfo_Get          = nullptr;
PFN_ADL_Overdrive6_VoltageControlInfo_Get          ADL_Overdrive6_VoltageControlInfo_Get           = nullptr;
PFN_ADL2_Overdrive6_VoltageControl_Get             ADL2_Overdrive6_VoltageControl_Get              = nullptr;
PFN_ADL_Overdrive6_VoltageControl_Get              ADL_Overdrive6_VoltageControl_Get               = nullptr;
PFN_ADL2_Overdrive6_VoltageControl_Set             ADL2_Overdrive6_VoltageControl_Set              = nullptr;
PFN_ADL_Overdrive6_VoltageControl_Set              ADL_Overdrive6_VoltageControl_Set               = nullptr;
PFN_ADL2_Overdrive6_CapabilitiesEx_Get             ADL2_Overdrive6_CapabilitiesEx_Get              = nullptr;
PFN_ADL_Overdrive6_CapabilitiesEx_Get              ADL_Overdrive6_CapabilitiesEx_Get               = nullptr;
PFN_ADL2_Overdrive6_StateEx_Get                    ADL2_Overdrive6_StateEx_Get                     = nullptr;
PFN_ADL_Overdrive6_StateEx_Get                     ADL_Overdrive6_StateEx_Get                      = nullptr;
PFN_ADL2_Overdrive6_StateEx_Set                    ADL2_Overdrive6_StateEx_Set                     = nullptr;
PFN_ADL_Overdrive6_StateEx_Set                     ADL_Overdrive6_StateEx_Set                      = nullptr;
PFN_ADL_Overdrive6_ThermalLimitUnlock_Set          ADL_Overdrive6_ThermalLimitUnlock_Set           = nullptr;
PFN_ADL2_Overdrive6_ThermalLimitUnlock_Set         ADL2_Overdrive6_ThermalLimitUnlock_Set          = nullptr;
PFN_ADL_Overdrive6_ThermalLimitUnlock_Get          ADL_Overdrive6_ThermalLimitUnlock_Get           = nullptr;
PFN_ADL2_Overdrive6_ThermalLimitUnlock_Get         ADL2_Overdrive6_ThermalLimitUnlock_Get          = nullptr;
PFN_ADL_Overdrive6_AdvancedFan_Caps                ADL_Overdrive6_AdvancedFan_Caps                 = nullptr;
PFN_ADL2_Overdrive6_AdvancedFan_Caps               ADL2_Overdrive6_AdvancedFan_Caps                = nullptr;
PFN_ADL_Overdrive6_TargetTemperatureRangeInfo_Get  ADL_Overdrive6_TargetTemperatureRangeInfo_Get   = nullptr;
PFN_ADL2_Overdrive6_TargetTemperatureRangeInfo_Get ADL2_Overdrive6_TargetTemperatureRangeInfo_Get  = nullptr;
PFN_ADL_Overdrive6_TargetTemperatureData_Get       ADL_Overdrive6_TargetTemperatureData_Get        = nullptr;
PFN_ADL2_Overdrive6_TargetTemperatureData_Get      ADL2_Overdrive6_TargetTemperatureData_Get       = nullptr;
PFN_ADL_Overdrive6_TargetTemperatureData_Set       ADL_Overdrive6_TargetTemperatureData_Set        = nullptr;
PFN_ADL2_Overdrive6_TargetTemperatureData_Set      ADL2_Overdrive6_TargetTemperatureData_Set       = nullptr;
PFN_ADL_Overdrive6_FanPWMLimitRangeInfo_Get        ADL_Overdrive6_FanPWMLimitRangeInfo_Get         = nullptr;
PFN_ADL2_Overdrive6_FanPWMLimitRangeInfo_Get       ADL2_Overdrive6_FanPWMLimitRangeInfo_Get        = nullptr;
PFN_ADL_Overdrive6_FanPWMLimitData_Get             ADL_Overdrive6_FanPWMLimitData_Get              = nullptr;
PFN_ADL2_Overdrive6_FanPWMLimitData_Get            ADL2_Overdrive6_FanPWMLimitData_Get             = nullptr;
PFN_ADL_Overdrive6_FanPWMLimitData_Set             ADL_Overdrive6_FanPWMLimitData_Set              = nullptr;
PFN_ADL2_Overdrive6_FanPWMLimitData_Set            ADL2_Overdrive6_FanPWMLimitData_Set             = nullptr;


#if defined(_WIN32)
void amd_performance::load_gpa()
{
   HMODULE gpuperfapigl_dll = LoadLibraryA("GPUPerfAPIGL.dll");
   if (gpuperfapigl_dll)
   {
      GPA_RegisterLoggingCallback   = reinterpret_cast<GPA_RegisterLoggingCallbackPtrType >(GetProcAddress(gpuperfapigl_dll, "GPA_RegisterLoggingCallback"));
      GPA_Initialize                = reinterpret_cast<GPA_InitializePtrType              >(GetProcAddress(gpuperfapigl_dll, "GPA_Initialize"));
      GPA_Destroy                   = reinterpret_cast<GPA_DestroyPtrType                 >(GetProcAddress(gpuperfapigl_dll, "GPA_Destroy"));
      GPA_OpenContext               = reinterpret_cast<GPA_OpenContextPtrType             >(GetProcAddress(gpuperfapigl_dll, "GPA_OpenContext"));
      GPA_CloseContext              = reinterpret_cast<GPA_CloseContextPtrType            >(GetProcAddress(gpuperfapigl_dll, "GPA_CloseContext"));
      GPA_SelectContext             = reinterpret_cast<GPA_SelectContextPtrType           >(GetProcAddress(gpuperfapigl_dll, "GPA_SelectContext"));
      GPA_GetNumCounters            = reinterpret_cast<GPA_GetNumCountersPtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_GetNumCounters"));
      GPA_GetCounterName            = reinterpret_cast<GPA_GetCounterNamePtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_GetCounterName"));
      GPA_GetCounterDescription     = reinterpret_cast<GPA_GetCounterDescriptionPtrType   >(GetProcAddress(gpuperfapigl_dll, "GPA_GetCounterDescription"));
      GPA_GetCounterDataType        = reinterpret_cast<GPA_GetCounterDataTypePtrType      >(GetProcAddress(gpuperfapigl_dll, "GPA_GetCounterDataType"));
      GPA_GetCounterUsageType       = reinterpret_cast<GPA_GetCounterUsageTypePtrType     >(GetProcAddress(gpuperfapigl_dll, "GPA_GetCounterUsageType"));
      GPA_GetDataTypeAsStr          = reinterpret_cast<GPA_GetDataTypeAsStrPtrType        >(GetProcAddress(gpuperfapigl_dll, "GPA_GetDataTypeAsStr"));
      GPA_GetUsageTypeAsStr         = reinterpret_cast<GPA_GetUsageTypeAsStrPtrType       >(GetProcAddress(gpuperfapigl_dll, "GPA_GetUsageTypeAsStr"));
      GPA_GetStatusAsStr            = reinterpret_cast<GPA_GetStatusAsStrPtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_GetStatusAsStr"));
      GPA_EnableCounter             = reinterpret_cast<GPA_EnableCounterPtrType           >(GetProcAddress(gpuperfapigl_dll, "GPA_EnableCounter"));
      GPA_DisableCounter            = reinterpret_cast<GPA_DisableCounterPtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_DisableCounter"));
      GPA_GetEnabledCount           = reinterpret_cast<GPA_GetEnabledCountPtrType         >(GetProcAddress(gpuperfapigl_dll, "GPA_GetEnabledCount"));
      GPA_GetEnabledIndex           = reinterpret_cast<GPA_GetEnabledIndexPtrType         >(GetProcAddress(gpuperfapigl_dll, "GPA_GetEnabledIndex"));
      GPA_IsCounterEnabled          = reinterpret_cast<GPA_IsCounterEnabledPtrType        >(GetProcAddress(gpuperfapigl_dll, "GPA_IsCounterEnabled"));
      GPA_EnableCounterStr          = reinterpret_cast<GPA_EnableCounterStrPtrType        >(GetProcAddress(gpuperfapigl_dll, "GPA_EnableCounterStr"));
      GPA_DisableCounterStr         = reinterpret_cast<GPA_DisableCounterStrPtrType       >(GetProcAddress(gpuperfapigl_dll, "GPA_DisableCounterStr"));
      GPA_EnableAllCounters         = reinterpret_cast<GPA_EnableAllCountersPtrType       >(GetProcAddress(gpuperfapigl_dll, "GPA_EnableAllCounters"));
      GPA_DisableAllCounters        = reinterpret_cast<GPA_DisableAllCountersPtrType      >(GetProcAddress(gpuperfapigl_dll, "GPA_DisableAllCounters"));
      GPA_GetCounterIndex           = reinterpret_cast<GPA_GetCounterIndexPtrType         >(GetProcAddress(gpuperfapigl_dll, "GPA_GetCounterIndex"));
      GPA_GetPassCount              = reinterpret_cast<GPA_GetPassCountPtrType            >(GetProcAddress(gpuperfapigl_dll, "GPA_GetPassCount"));
      GPA_BeginSession              = reinterpret_cast<GPA_BeginSessionPtrType            >(GetProcAddress(gpuperfapigl_dll, "GPA_BeginSession"));
      GPA_EndSession                = reinterpret_cast<GPA_EndSessionPtrType              >(GetProcAddress(gpuperfapigl_dll, "GPA_EndSession"));
      GPA_BeginPass                 = reinterpret_cast<GPA_BeginPassPtrType               >(GetProcAddress(gpuperfapigl_dll, "GPA_BeginPass"));
      GPA_EndPass                   = reinterpret_cast<GPA_EndPassPtrType                 >(GetProcAddress(gpuperfapigl_dll, "GPA_EndPass"));
      GPA_BeginSample               = reinterpret_cast<GPA_BeginSamplePtrType             >(GetProcAddress(gpuperfapigl_dll, "GPA_BeginSample"));
      GPA_EndSample                 = reinterpret_cast<GPA_EndSamplePtrType               >(GetProcAddress(gpuperfapigl_dll, "GPA_EndSample"));
      GPA_GetSampleCount            = reinterpret_cast<GPA_GetSampleCountPtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_GetSampleCount"));
      GPA_IsSampleReady             = reinterpret_cast<GPA_IsSampleReadyPtrType           >(GetProcAddress(gpuperfapigl_dll, "GPA_IsSampleReady"));
      GPA_IsSessionReady            = reinterpret_cast<GPA_IsSessionReadyPtrType          >(GetProcAddress(gpuperfapigl_dll, "GPA_IsSessionReady"));
      GPA_GetSampleUInt64           = reinterpret_cast<GPA_GetSampleUInt64PtrType         >(GetProcAddress(gpuperfapigl_dll, "GPA_GetSampleUInt64"));
      GPA_GetSampleUInt32           = reinterpret_cast<GPA_GetSampleUInt32PtrType         >(GetProcAddress(gpuperfapigl_dll, "GPA_GetSampleUInt32"));
      GPA_GetSampleFloat32          = reinterpret_cast<GPA_GetSampleFloat32PtrType        >(GetProcAddress(gpuperfapigl_dll, "GPA_GetSampleFloat32"));
      GPA_GetSampleFloat64          = reinterpret_cast<GPA_GetSampleFloat64PtrType        >(GetProcAddress(gpuperfapigl_dll, "GPA_GetSampleFloat64"));
      m_gpuperfapigl_dll = gpuperfapigl_dll;
      m_use_gpa = true;
   }
}

void amd_performance::load_adl()
{
   HMODULE atiadl_dll = LoadLibrary("atiadlxx.dll");
   if (!atiadl_dll)
      atiadl_dll = LoadLibrary("atiadlxy.dll");

   if (atiadl_dll)
   {
      // These can be missing in older drivers
      ADL2_Main_ControlX2_Create                      = reinterpret_cast<PFN_ADL2_Main_ControlX2_Create                     >(GetProcAddress(atiadl_dll, "ADL2_Main_ControlX2_Create"));
      ADL_Main_ControlX2_Create                       = reinterpret_cast<PFN_ADL_Main_ControlX2_Create                      >(GetProcAddress(atiadl_dll, "ADL_Main_ControlX2_Create"));

      // ADL2 versions below can also be missing in older drivers
      ADL2_Main_Control_Create                        = reinterpret_cast<PFN_ADL2_Main_Control_Create                       >(GetProcAddress(atiadl_dll, "ADL2_Main_Control_Create"));
      ADL_Main_Control_Create                         = reinterpret_cast<PFN_ADL_Main_Control_Create                        >(GetProcAddress(atiadl_dll, "ADL_Main_Control_Create"));
      ADL2_Main_Control_Refresh                       = reinterpret_cast<PFN_ADL2_Main_Control_Refresh                      >(GetProcAddress(atiadl_dll, "ADL2_Main_Control_Refresh"));
      ADL_Main_Control_Refresh                        = reinterpret_cast<PFN_ADL_Main_Control_Refresh                       >(GetProcAddress(atiadl_dll, "ADL_Main_Control_Refresh"));
      ADL2_Main_Control_Destroy                       = reinterpret_cast<PFN_ADL2_Main_Control_Destroy                      >(GetProcAddress(atiadl_dll, "ADL2_Main_Control_Destroy"));
      ADL_Main_Control_Destroy                        = reinterpret_cast<PFN_ADL_Main_Control_Destroy                       >(GetProcAddress(atiadl_dll, "ADL_Main_Control_Destroy"));
      ADL2_Graphics_Platform_Get                      = reinterpret_cast<PFN_ADL2_Graphics_Platform_Get                     >(GetProcAddress(atiadl_dll, "ADL2_Graphics_Platform_Get"));
      ADL_Graphics_Platform_Get                       = reinterpret_cast<PFN_ADL_Graphics_Platform_Get                      >(GetProcAddress(atiadl_dll, "ADL_Graphics_Platform_Get"));
      ADL2_Adapter_Active_Get                         = reinterpret_cast<PFN_ADL2_Adapter_Active_Get                        >(GetProcAddress(atiadl_dll, "ADL2_Adapter_Active_Get"));
      ADL_Adapter_Active_Get                          = reinterpret_cast<PFN_ADL_Adapter_Active_Get                         >(GetProcAddress(atiadl_dll, "ADL_Adapter_Active_Get"));
      ADL2_Adapter_NumberOfAdapters_Get               = reinterpret_cast<PFN_ADL2_Adapter_NumberOfAdapters_Get              >(GetProcAddress(atiadl_dll, "ADL2_Adapter_NumberOfAdapters_Get"));
      ADL_Adapter_NumberOfAdapters_Get                = reinterpret_cast<PFN_ADL_Adapter_NumberOfAdapters_Get               >(GetProcAddress(atiadl_dll, "ADL_Adapter_NumberOfAdapters_Get"));
      ADL2_Flush_Driver_Data                          = reinterpret_cast<PFN_ADL2_Flush_Driver_Data                         >(GetProcAddress(atiadl_dll, "ADL2_Flush_Driver_Data"));
      ADL_Flush_Driver_Data                           = reinterpret_cast<PFN_ADL_Flush_Driver_Data                          >(GetProcAddress(atiadl_dll, "ADL_Flush_Driver_Data"));
      ADL2_Adapter_AdapterInfo_Get                    = reinterpret_cast<PFN_ADL2_Adapter_AdapterInfo_Get                   >(GetProcAddress(atiadl_dll, "ADL2_Adapter_AdapterInfo_Get"));
      ADL_Adapter_AdapterInfo_Get                     = reinterpret_cast<PFN_ADL_Adapter_AdapterInfo_Get                    >(GetProcAddress(atiadl_dll, "ADL_Adapter_AdapterInfo_Get"));
      ADL2_Adapter_AdapterInfoX2_Get                  = reinterpret_cast<PFN_ADL2_Adapter_AdapterInfoX2_Get                 >(GetProcAddress(atiadl_dll, "ADL2_Adapter_AdapterInfoX2_Get"));
      ADL_Adapter_AdapterInfoX2_Get                   = reinterpret_cast<PFN_ADL_Adapter_AdapterInfoX2_Get                  >(GetProcAddress(atiadl_dll, "ADL_Adapter_AdapterInfoX2_Get"));
      ADL2_Adapter_ASICFamilyType_Get                 = reinterpret_cast<PFN_ADL2_Adapter_ASICFamilyType_Get                >(GetProcAddress(atiadl_dll, "ADL2_Adapter_ASICFamilyType_Get"));
      ADL_Adapter_ASICFamilyType_Get                  = reinterpret_cast<PFN_ADL_Adapter_ASICFamilyType_Get                 >(GetProcAddress(atiadl_dll, "ADL_Adapter_ASICFamilyType_Get"));
      ADL2_Adapter_Speed_Caps                         = reinterpret_cast<PFN_ADL2_Adapter_Speed_Caps                        >(GetProcAddress(atiadl_dll, "ADL2_Adapter_Speed_Caps"));
      ADL_Adapter_Speed_Caps                          = reinterpret_cast<PFN_ADL_Adapter_Speed_Caps                         >(GetProcAddress(atiadl_dll, "ADL_Adapter_Speed_Caps"));
      ADL2_Adapter_Speed_Get                          = reinterpret_cast<PFN_ADL2_Adapter_Speed_Get                         >(GetProcAddress(atiadl_dll, "ADL2_Adapter_Speed_Get"));
      ADL_Adapter_Speed_Get                           = reinterpret_cast<PFN_ADL_Adapter_Speed_Get                          >(GetProcAddress(atiadl_dll, "ADL_Adapter_Speed_Get"));
      ADL2_Adapter_Speed_Set                          = reinterpret_cast<PFN_ADL2_Adapter_Speed_Set                         >(GetProcAddress(atiadl_dll, "ADL2_Adapter_Speed_Set"));
      ADL_Adapter_Speed_Set                           = reinterpret_cast<PFN_ADL_Adapter_Speed_Set                          >(GetProcAddress(atiadl_dll, "ADL_Adapter_Speed_Set"));
      ADL2_Adapter_Accessibility_Get                  = reinterpret_cast<PFN_ADL2_Adapter_Accessibility_Get                 >(GetProcAddress(atiadl_dll, "ADL2_Adapter_Accessibility_Get"));
      ADL_Adapter_Accessibility_Get                   = reinterpret_cast<PFN_ADL_Adapter_Accessibility_Get                  >(GetProcAddress(atiadl_dll, "ADL_Adapter_Accessibility_Get"));
      ADL2_Adapter_VideoBiosInfo_Get                  = reinterpret_cast<PFN_ADL2_Adapter_VideoBiosInfo_Get                 >(GetProcAddress(atiadl_dll, "ADL2_Adapter_VideoBiosInfo_Get"));
      ADL_Adapter_VideoBiosInfo_Get                   = reinterpret_cast<PFN_ADL_Adapter_VideoBiosInfo_Get                  >(GetProcAddress(atiadl_dll, "ADL_Adapter_VideoBiosInfo_Get"));
      ADL2_Adapter_ID_Get                             = reinterpret_cast<PFN_ADL2_Adapter_ID_Get                            >(GetProcAddress(atiadl_dll, "ADL2_Adapter_ID_Get"));
      ADL_Adapter_ID_Get                              = reinterpret_cast<PFN_ADL_Adapter_ID_Get                             >(GetProcAddress(atiadl_dll, "ADL_Adapter_ID_Get"));

      ADL2_Overdrive5_CurrentActivity_Get             = reinterpret_cast<PFN_ADL2_Overdrive5_CurrentActivity_Get            >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_CurrentActivity_Get"));
      ADL_Overdrive5_CurrentActivity_Get              = reinterpret_cast<PFN_ADL_Overdrive5_CurrentActivity_Get             >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_CurrentActivity_Get"));
      ADL2_Overdrive5_ThermalDevices_Enum             = reinterpret_cast<PFN_ADL2_Overdrive5_ThermalDevices_Enum            >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_ThermalDevices_Enum"));
      ADL_Overdrive5_ThermalDevices_Enum              = reinterpret_cast<PFN_ADL_Overdrive5_ThermalDevices_Enum             >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_ThermalDevices_Enum"));
      ADL2_Overdrive5_Temperature_Get                 = reinterpret_cast<PFN_ADL2_Overdrive5_Temperature_Get                >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_Temperature_Get"));
      ADL_Overdrive5_Temperature_Get                  = reinterpret_cast<PFN_ADL_Overdrive5_Temperature_Get                 >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_Temperature_Get"));
      ADL2_Overdrive5_FanSpeedInfo_Get                = reinterpret_cast<PFN_ADL2_Overdrive5_FanSpeedInfo_Get               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_FanSpeedInfo_Get"));
      ADL_Overdrive5_FanSpeedInfo_Get                 = reinterpret_cast<PFN_ADL_Overdrive5_FanSpeedInfo_Get                >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_FanSpeedInfo_Get"));
      ADL2_Overdrive5_FanSpeed_Get                    = reinterpret_cast<PFN_ADL2_Overdrive5_FanSpeed_Get                   >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_FanSpeed_Get"));
      ADL_Overdrive5_FanSpeed_Get                     = reinterpret_cast<PFN_ADL_Overdrive5_FanSpeed_Get                    >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_FanSpeed_Get"));
      ADL2_Overdrive5_FanSpeed_Set                    = reinterpret_cast<PFN_ADL2_Overdrive5_FanSpeed_Set                   >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_FanSpeed_Set"));
      ADL_Overdrive5_FanSpeed_Set                     = reinterpret_cast<PFN_ADL_Overdrive5_FanSpeed_Set                    >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_FanSpeed_Set"));
      ADL2_Overdrive5_FanSpeedToDefault_Set           = reinterpret_cast<PFN_ADL2_Overdrive5_FanSpeedToDefault_Set          >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_FanSpeedToDefault_Set"));
      ADL_Overdrive5_FanSpeedToDefault_Set            = reinterpret_cast<PFN_ADL_Overdrive5_FanSpeedToDefault_Set           >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_FanSpeedToDefault_Set"));
      ADL2_Overdrive5_ODParameters_Get                = reinterpret_cast<PFN_ADL2_Overdrive5_ODParameters_Get               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_ODParameters_Get"));
      ADL_Overdrive5_ODParameters_Get                 = reinterpret_cast<PFN_ADL_Overdrive5_ODParameters_Get                >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_ODParameters_Get"));
      ADL2_Overdrive5_ODPerformanceLevels_Get         = reinterpret_cast<PFN_ADL2_Overdrive5_ODPerformanceLevels_Get        >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_ODPerformanceLevels_Get"));
      ADL_Overdrive5_ODPerformanceLevels_Get          = reinterpret_cast<PFN_ADL_Overdrive5_ODPerformanceLevels_Get         >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_ODPerformanceLevels_Get"));
      ADL2_Overdrive5_ODPerformanceLevels_Set         = reinterpret_cast<PFN_ADL2_Overdrive5_ODPerformanceLevels_Set        >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_ODPerformanceLevels_Set"));
      ADL_Overdrive5_ODPerformanceLevels_Set          = reinterpret_cast<PFN_ADL_Overdrive5_ODPerformanceLevels_Set         >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_ODPerformanceLevels_Set"));
      ADL2_Overdrive5_PowerControl_Caps               = reinterpret_cast<PFN_ADL2_Overdrive5_PowerControl_Caps              >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_PowerControl_Caps"));
      ADL_Overdrive5_PowerControl_Caps                = reinterpret_cast<PFN_ADL_Overdrive5_PowerControl_Caps               >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_PowerControl_Caps"));
      ADL2_Overdrive5_PowerControlInfo_Get            = reinterpret_cast<PFN_ADL2_Overdrive5_PowerControlInfo_Get           >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_PowerControlInfo_Get"));
      ADL_Overdrive5_PowerControlInfo_Get             = reinterpret_cast<PFN_ADL_Overdrive5_PowerControlInfo_Get            >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_PowerControlInfo_Get"));
      ADL2_Overdrive5_PowerControl_Get                = reinterpret_cast<PFN_ADL2_Overdrive5_PowerControl_Get               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_PowerControl_Get"));
      ADL_Overdrive5_PowerControl_Get                 = reinterpret_cast<PFN_ADL_Overdrive5_PowerControl_Get                >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_PowerControl_Get"));
      ADL2_Overdrive5_PowerControl_Set                = reinterpret_cast<PFN_ADL2_Overdrive5_PowerControl_Set               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive5_PowerControl_Set"));
      ADL_Overdrive5_PowerControl_Set                 = reinterpret_cast<PFN_ADL_Overdrive5_PowerControl_Set                >(GetProcAddress(atiadl_dll, "ADL_Overdrive5_PowerControl_Set"));
      ADL2_Overdrive_Caps                             = reinterpret_cast<PFN_ADL2_Overdrive_Caps                            >(GetProcAddress(atiadl_dll, "ADL2_Overdrive_Caps"));
      ADL_Overdrive_Caps                              = reinterpret_cast<PFN_ADL_Overdrive_Caps                             >(GetProcAddress(atiadl_dll, "ADL_Overdrive_Caps"));

      ADL2_Overdrive6_Capabilities_Get                = reinterpret_cast<PFN_ADL2_Overdrive6_Capabilities_Get               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_Capabilities_Get"));
      ADL_Overdrive6_Capabilities_Get                 = reinterpret_cast<PFN_ADL_Overdrive6_Capabilities_Get                >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_Capabilities_Get"));
      ADL2_Overdrive6_StateInfo_Get                   = reinterpret_cast<PFN_ADL2_Overdrive6_StateInfo_Get                  >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_StateInfo_Get"));
      ADL_Overdrive6_StateInfo_Get                    = reinterpret_cast<PFN_ADL_Overdrive6_StateInfo_Get                   >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_StateInfo_Get"));
      ADL2_Overdrive6_State_Set                       = reinterpret_cast<PFN_ADL2_Overdrive6_State_Set                      >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_State_Set"));
      ADL_Overdrive6_State_Set                        = reinterpret_cast<PFN_ADL_Overdrive6_State_Set                       >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_State_Set"));
      ADL2_Overdrive6_State_Reset                     = reinterpret_cast<PFN_ADL2_Overdrive6_State_Reset                    >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_State_Reset"));
      ADL_Overdrive6_State_Reset                      = reinterpret_cast<PFN_ADL_Overdrive6_State_Reset                     >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_State_Reset"));
      ADL2_Overdrive6_CurrentStatus_Get               = reinterpret_cast<PFN_ADL2_Overdrive6_CurrentStatus_Get              >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_CurrentStatus_Get"));
      ADL_Overdrive6_CurrentStatus_Get                = reinterpret_cast<PFN_ADL_Overdrive6_CurrentStatus_Get               >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_CurrentStatus_Get"));
      ADL2_Overdrive6_ThermalController_Caps          = reinterpret_cast<PFN_ADL2_Overdrive6_ThermalController_Caps         >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_ThermalController_Caps"));
      ADL_Overdrive6_ThermalController_Caps           = reinterpret_cast<PFN_ADL_Overdrive6_ThermalController_Caps          >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_ThermalController_Caps"));
      ADL2_Overdrive6_Temperature_Get                 = reinterpret_cast<PFN_ADL2_Overdrive6_Temperature_Get                >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_Temperature_Get"));
      ADL_Overdrive6_Temperature_Get                  = reinterpret_cast<PFN_ADL_Overdrive6_Temperature_Get                 >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_Temperature_Get"));
      ADL2_Overdrive6_FanSpeed_Get                    = reinterpret_cast<PFN_ADL2_Overdrive6_FanSpeed_Get                   >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanSpeed_Get"));
      ADL_Overdrive6_FanSpeed_Get                     = reinterpret_cast<PFN_ADL_Overdrive6_FanSpeed_Get                    >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanSpeed_Get"));
      ADL2_Overdrive6_FanSpeed_Set                    = reinterpret_cast<PFN_ADL2_Overdrive6_FanSpeed_Set                   >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanSpeed_Set"));
      ADL_Overdrive6_FanSpeed_Set                     = reinterpret_cast<PFN_ADL_Overdrive6_FanSpeed_Set                    >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanSpeed_Set"));
      ADL2_Overdrive6_FanSpeed_Reset                  = reinterpret_cast<PFN_ADL2_Overdrive6_FanSpeed_Reset                 >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanSpeed_Reset"));
      ADL_Overdrive6_FanSpeed_Reset                   = reinterpret_cast<PFN_ADL_Overdrive6_FanSpeed_Reset                  >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanSpeed_Reset"));
      ADL2_Overdrive6_PowerControl_Caps               = reinterpret_cast<PFN_ADL2_Overdrive6_PowerControl_Caps              >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_PowerControl_Caps"));
      ADL_Overdrive6_PowerControl_Caps                = reinterpret_cast<PFN_ADL_Overdrive6_PowerControl_Caps               >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_PowerControl_Caps"));
      ADL2_Overdrive6_PowerControlInfo_Get            = reinterpret_cast<PFN_ADL2_Overdrive6_PowerControlInfo_Get           >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_PowerControlInfo_Get"));
      ADL_Overdrive6_PowerControlInfo_Get             = reinterpret_cast<PFN_ADL_Overdrive6_PowerControlInfo_Get            >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_PowerControlInfo_Get"));
      ADL2_Overdrive6_PowerControl_Get                = reinterpret_cast<PFN_ADL2_Overdrive6_PowerControl_Get               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_PowerControl_Get"));
      ADL_Overdrive6_PowerControl_Get	               = reinterpret_cast<PFN_ADL_Overdrive6_PowerControl_Get                >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_PowerControl_Get	"));
      ADL2_Overdrive6_PowerControl_Set                = reinterpret_cast<PFN_ADL2_Overdrive6_PowerControl_Set               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_PowerControl_Set"));
      ADL_Overdrive6_PowerControl_Set                 = reinterpret_cast<PFN_ADL_Overdrive6_PowerControl_Set                >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_PowerControl_Set"));
      ADL2_Overdrive6_VoltageControlInfo_Get          = reinterpret_cast<PFN_ADL2_Overdrive6_VoltageControlInfo_Get         >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_VoltageControlInfo_Get"));
      ADL_Overdrive6_VoltageControlInfo_Get           = reinterpret_cast<PFN_ADL_Overdrive6_VoltageControlInfo_Get          >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_VoltageControlInfo_Get"));
      ADL2_Overdrive6_VoltageControl_Get              = reinterpret_cast<PFN_ADL2_Overdrive6_VoltageControl_Get             >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_VoltageControl_Get"));
      ADL_Overdrive6_VoltageControl_Get               = reinterpret_cast<PFN_ADL_Overdrive6_VoltageControl_Get              >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_VoltageControl_Get"));
      ADL2_Overdrive6_VoltageControl_Set              = reinterpret_cast<PFN_ADL2_Overdrive6_VoltageControl_Set             >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_VoltageControl_Set"));
      ADL_Overdrive6_VoltageControl_Set               = reinterpret_cast<PFN_ADL_Overdrive6_VoltageControl_Set              >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_VoltageControl_Set"));
      ADL2_Overdrive6_CapabilitiesEx_Get              = reinterpret_cast<PFN_ADL2_Overdrive6_CapabilitiesEx_Get             >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_CapabilitiesEx_Get"));
      ADL_Overdrive6_CapabilitiesEx_Get               = reinterpret_cast<PFN_ADL_Overdrive6_CapabilitiesEx_Get              >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_CapabilitiesEx_Get"));
      ADL2_Overdrive6_StateEx_Get                     = reinterpret_cast<PFN_ADL2_Overdrive6_StateEx_Get                    >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_StateEx_Get"));
      ADL_Overdrive6_StateEx_Get                      = reinterpret_cast<PFN_ADL_Overdrive6_StateEx_Get                     >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_StateEx_Get"));
      ADL2_Overdrive6_StateEx_Set                     = reinterpret_cast<PFN_ADL2_Overdrive6_StateEx_Set                    >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_StateEx_Set"));
      ADL_Overdrive6_StateEx_Set                      = reinterpret_cast<PFN_ADL_Overdrive6_StateEx_Set                     >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_StateEx_Set"));
      ADL_Overdrive6_ThermalLimitUnlock_Set           = reinterpret_cast<PFN_ADL_Overdrive6_ThermalLimitUnlock_Set          >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_ThermalLimitUnlock_Set"));
      ADL2_Overdrive6_ThermalLimitUnlock_Set          = reinterpret_cast<PFN_ADL2_Overdrive6_ThermalLimitUnlock_Set         >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_ThermalLimitUnlock_Set"));
      ADL_Overdrive6_ThermalLimitUnlock_Get           = reinterpret_cast<PFN_ADL_Overdrive6_ThermalLimitUnlock_Get          >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_ThermalLimitUnlock_Get"));
      ADL2_Overdrive6_ThermalLimitUnlock_Get          = reinterpret_cast<PFN_ADL2_Overdrive6_ThermalLimitUnlock_Get         >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_ThermalLimitUnlock_Get"));
      ADL_Overdrive6_AdvancedFan_Caps                 = reinterpret_cast<PFN_ADL_Overdrive6_AdvancedFan_Caps                >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_AdvancedFan_Caps"));
      ADL2_Overdrive6_AdvancedFan_Caps                = reinterpret_cast<PFN_ADL2_Overdrive6_AdvancedFan_Caps               >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_AdvancedFan_Caps"));
      ADL_Overdrive6_TargetTemperatureRangeInfo_Get   = reinterpret_cast<PFN_ADL_Overdrive6_TargetTemperatureRangeInfo_Get  >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_TargetTemperatureRangeInfo_Get"));
      ADL2_Overdrive6_TargetTemperatureRangeInfo_Get  = reinterpret_cast<PFN_ADL2_Overdrive6_TargetTemperatureRangeInfo_Get >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_TargetTemperatureRangeInfo_Get"));
      ADL_Overdrive6_TargetTemperatureData_Get        = reinterpret_cast<PFN_ADL_Overdrive6_TargetTemperatureData_Get       >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_TargetTemperatureData_Get"));
      ADL2_Overdrive6_TargetTemperatureData_Get       = reinterpret_cast<PFN_ADL2_Overdrive6_TargetTemperatureData_Get      >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_TargetTemperatureData_Get"));
      ADL_Overdrive6_TargetTemperatureData_Set        = reinterpret_cast<PFN_ADL_Overdrive6_TargetTemperatureData_Set       >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_TargetTemperatureData_Set"));
      ADL2_Overdrive6_TargetTemperatureData_Set       = reinterpret_cast<PFN_ADL2_Overdrive6_TargetTemperatureData_Set      >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_TargetTemperatureData_Set"));
      ADL_Overdrive6_FanPWMLimitRangeInfo_Get         = reinterpret_cast<PFN_ADL_Overdrive6_FanPWMLimitRangeInfo_Get        >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanPWMLimitRangeInfo_Get"));
      ADL2_Overdrive6_FanPWMLimitRangeInfo_Get        = reinterpret_cast<PFN_ADL2_Overdrive6_FanPWMLimitRangeInfo_Get       >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanPWMLimitRangeInfo_Get"));
      ADL_Overdrive6_FanPWMLimitData_Get              = reinterpret_cast<PFN_ADL_Overdrive6_FanPWMLimitData_Get             >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanPWMLimitData_Get"));
      ADL2_Overdrive6_FanPWMLimitData_Get             = reinterpret_cast<PFN_ADL2_Overdrive6_FanPWMLimitData_Get            >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanPWMLimitData_Get"));
      ADL_Overdrive6_FanPWMLimitData_Set              = reinterpret_cast<PFN_ADL_Overdrive6_FanPWMLimitData_Set             >(GetProcAddress(atiadl_dll, "ADL_Overdrive6_FanPWMLimitData_Set"));
      ADL2_Overdrive6_FanPWMLimitData_Set             = reinterpret_cast<PFN_ADL2_Overdrive6_FanPWMLimitData_Set            >(GetProcAddress(atiadl_dll, "ADL2_Overdrive6_FanPWMLimitData_Set"));
      m_atiadl_dll = atiadl_dll;
      m_use_adl = true;
   }
}
#endif

extern "C" static void gpa_log_callback(GPA_Logging_Type messageType, const char* message)
{
   (void)messageType;
   printf("%s\n", message);
}

static void * __stdcall adl_malloc_callback(int size)
{
   return malloc(size);
}

void amd_performance::connect_to_window(window &w)
{
   if (!m_use_gpa)
      return;
   
   GPA_Status ok;

   if (!GPA_OpenContext)
   {
      m_use_gpa = false;
      return;
   }

   ok = GPA_OpenContext(w.get_native_context());
   if (ok != GPA_STATUS_OK)
      return;

   if (!GPA_GetNumCounters)
   {
      m_use_gpa = false;
      return;
   }
   ok = GPA_GetNumCounters(&m_number_of_performance_counters);
   if (ok != GPA_STATUS_OK)
   {
      m_use_gpa = false;
      return;
   }

   //printf("GPA counters: %d\n", count);

   for (gpa_uint32 i = 0; i < m_number_of_performance_counters; ++i)
   {
      const char * name;
      if (!GPA_GetCounterName)
      {
         m_use_gpa = false;
         return;
      }

      GPA_GetCounterName(i, &name);
      //printf("%d: %s\n", i, name);
   }

   initialize_adl();

   float c = get_adapter_temperature(0);
   printf("Adapter %d overdrive5 temperature %5.1f\n", 0, c);
}

void amd_performance::initialize_gpa()
{
   load_gpa();

   if (!m_use_gpa)
      return;

   if (GPA_RegisterLoggingCallback != nullptr)
      GPA_RegisterLoggingCallback(GPA_LOGGING_ERROR_AND_MESSAGE, gpa_log_callback);

   if (GPA_Initialize == nullptr)
      m_use_gpa = false;

   m_use_gpa = (GPA_Initialize() == GPA_STATUS_OK);
}

void amd_performance::initialize_adl()
{
   load_adl();

   if (!m_use_adl)
      return;

   int res;
   if (ADL_Main_ControlX2_Create)
   {
      res = ADL_Main_ControlX2_Create(adl_malloc_callback, 1, ADL_THREADING_UNLOCKED);
      if (res != ADL_OK)
         m_use_adl = false;
   }
   else
   {
      if (!ADL_Main_Control_Create)
      {
         m_use_adl = false;
         return;
      }
      res = ADL_Main_Control_Create(adl_malloc_callback, 1);
      if (res != ADL_OK)
      {
         m_use_adl = false;
         return;
      }
   }

   if (ADL_Adapter_NumberOfAdapters_Get == nullptr)
   {
      m_use_adl = false;
      return;
   }

   res = ADL_Adapter_NumberOfAdapters_Get(&m_number_of_adapters);
   if (res != ADL_OK)
   {
      m_use_adl = false;
      return;
   }

   printf("ADL - Number of adapters: %d\n", m_number_of_adapters);
   m_adapter_enablement_status.resize(m_number_of_adapters);

   for (int i = 0; i < m_number_of_adapters; ++i)
   {
      int status = 0;

      m_adapter_enablement_status[i] = false;
      if (ADL_Adapter_Active_Get == nullptr)
      {
         m_use_adl = false;
         return;
      }

      res = ADL_Adapter_Active_Get(i, &status);
      if (res == ADL_OK)
      {
         if (status)
         {
            m_adapter_enablement_status[i] = true;
            //printf("Adapter %d / %d is enabled\n", i, number_of_adapters);
            if (!ADL_Overdrive_Caps)
            {
               m_use_adl = false;
               return;
            }

            float c = get_adapter_temperature(i);
            printf("Adapter %d overdrive5 temperature %5.1f\n", i, c);


            // int supported  = 0;
            // int enabled    = 0;
            // int version    = 0;
            // res = ADL_Overdrive_Caps(i, &supported, &enabled, &version);
            // if (res == ADL_OK)
            //    printf(
            //       "Adapter %d overdrive caps: Supported = %d, Enabled = %d, Version = %d\n",
            //       i, supported, enabled, version
            //    );
            // 
            // if (!ADL_Overdrive5_Temperature_Get)
            // {
            //    m_use_adl = false;
            //    return;
            // }
         }
      }
   }
}

int amd_performance::number_of_adapters()
{
   return m_number_of_adapters;
}

float amd_performance::get_adapter_temperature(int adapter)
{
   ADLTemperature temp5;
   temp5.iSize          = sizeof(ADLTemperature);
   temp5.iTemperature   = 0;

   if (
      (adapter < m_number_of_adapters) && 
      (m_adapter_enablement_status[adapter]) &&
      ADL_Overdrive5_Temperature_Get
   )
   {
      int res = ADL_Overdrive5_Temperature_Get(adapter, 0, &temp5);
      if (res == ADL_OK)
      {
         float c = static_cast<float>(temp5.iTemperature) * 0.001f;
         return c;
      }
   }
   return 0.0f;
}

amd_performance::amd_performance()
#if defined(_WIN32)
:  m_gpuperfapigl_dll               (nullptr)
,  m_atiadl_dll                     (nullptr)
#endif
,  m_use_gpa                        (false)
,  m_use_adl                        (false)
,  m_number_of_adapters             (0)
,  m_number_of_performance_counters (0)
{
   initialize_gpa();
}

void amd_performance::close_gpa()
{
#if defined(_WIN32)
   FreeLibrary(m_gpuperfapigl_dll);
   m_gpuperfapigl_dll = nullptr;
#endif
}

void amd_performance::close_adl()
{
   if (ADL_Main_Control_Destroy)
      (void)ADL_Main_Control_Destroy();

#if defined(_WIN32)
   FreeLibrary(m_atiadl_dll);
   m_atiadl_dll = nullptr;
#endif
}

amd_performance::~amd_performance()
{
   close_adl();
   close_gpa();
}

} }

#endif // RENDERSTACK_USE_AMD_GPU_PERF_API_AND_ADL
