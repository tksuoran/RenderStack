#ifndef gpa_hpp_renderstack_toolkit
#define gpa_hpp_renderstack_toolkit
#if defined(RENDERSTACK_USE_AMD_GPU_PERF_API_AND_ADL)

#if defined(_WIN32)
#   define _CRT_SECURE_NO_WARNINGS
#   define WIN32_LEAN_AND_MEAN
#   define NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#   define NOVIRTUALKEYCODES // VK_*
#   define NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
#   define NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#   define NOSYSMETRICS      // SM_*
#   define NOMENUS           // MF_*
#   define NOICONS           // IDI_*
#   define NOKEYSTATES       // MK_*
#   define NOSYSCOMMANDS     // SC_*
#   define NORASTEROPS       // Binary and Tertiary raster ops
#   define NOSHOWWINDOW      // SW_*
#   define OEMRESOURCE       // OEM Resource values
#   define NOATOM            // Atom Manager routines
#   define NOCLIPBOARD       // Clipboard routines
#   define NOCOLOR           // Screen colors
#   define NOCTLMGR          // Control and Dialog routines
#   define NODRAWTEXT        // DrawText() and DT_*
#   define NOGDI             // All GDI defines and routines
#   define NOKERNEL          // All KERNEL defines and routines
#   define NOUSER            // All USER defines and routines
#   define NONLS             // All NLS defines and routines
#   define NOMB              // MB_* and MessageBox()
#   define NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
#   define NOMETAFILE        // typedef METAFILEPICT
#   define NOMINMAX          // Macros min(a,b) and max(a,b)
#   define NOMSG             // typedef MSG and associated routines
#   define NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#   define NOSCROLL          // SB_* and scrolling routines
#   define NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
#   define NOSOUND           // Sound driver routines
#   define NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
#   define NOWH              // SetWindowsHook and WH_*
#   define NOWINOFFSETS      // GWL_*, GCL_*, associated routines
#   define NOCOMM            // COMM driver routines
#   define NOKANJI           // Kanji support stuff.
#   define NOHELP            // Help engine interface.
#   define NOPROFILER        // Profiler interface.
#   define NODEFERWINDOWPOS  // DeferWindowPos routines
#   define NOMCX             // Modem Configuration Extensions
#   include <windows.h>
#endif

#include "GPUPerfAPITypes.h"

#include <vector>

namespace renderstack { namespace toolkit {

class window;

class amd_performance
{
public:
   amd_performance();
   ~amd_performance();

   void connect_to_window(window &w);

   int number_of_adapters();
   float get_adapter_temperature(int adapter);

private:
   void load_gpa();
   void load_adl();

   void close_gpa();
   void close_adl();

   void initialize_gpa();
   void initialize_adl();

private:
#if defined(_WIN32)
   HMODULE           m_gpuperfapigl_dll;
   HMODULE           m_atiadl_dll;
#endif

   bool              m_use_gpa;
   bool              m_use_adl;
   gpa_uint32        m_gpa_group_count;
   int               m_number_of_adapters;
   std::vector<bool> m_adapter_enablement_status;
   gpa_uint32        m_number_of_performance_counters;
};

} }

#endif // RENDERSTACK_USE_AMD_GPU_PERF_API_AND_ADL
#endif // gpa_hpp_renderstack_toolkit



#if 0
extern GPA_RegisterLoggingCallbackPtrType GPA_RegisterLoggingCallback;
extern GPA_InitializePtrType              GPA_Initialize             ;
extern GPA_DestroyPtrType                 GPA_Destroy                ;
extern GPA_OpenContextPtrType             GPA_OpenContext            ;
extern GPA_CloseContextPtrType            GPA_CloseContext           ;
extern GPA_SelectContextPtrType           GPA_SelectContext          ;
extern GPA_GetNumCountersPtrType          GPA_GetNumCounters         ;
extern GPA_GetCounterNamePtrType          GPA_GetCounterName         ;
extern GPA_GetCounterDescriptionPtrType   GPA_GetCounterDescription  ;
extern GPA_GetCounterDataTypePtrType      GPA_GetCounterDataType     ;
extern GPA_GetCounterUsageTypePtrType     GPA_GetCounterUsageType    ;
extern GPA_GetDataTypeAsStrPtrType        GPA_GetDataTypeAsStr       ;
extern GPA_GetUsageTypeAsStrPtrType       GPA_GetUsageTypeAsStr      ;
extern GPA_GetStatusAsStrPtrType          GPA_GetStatusAsStr         ;
extern GPA_EnableCounterPtrType           GPA_EnableCounter          ;
extern GPA_DisableCounterPtrType          GPA_DisableCounter         ;
extern GPA_GetEnabledCountPtrType         GPA_GetEnabledCount        ;
extern GPA_GetEnabledIndexPtrType         GPA_GetEnabledIndex        ;
extern GPA_IsCounterEnabledPtrType        GPA_IsCounterEnabled       ;
extern GPA_EnableCounterStrPtrType        GPA_EnableCounterStr       ;
extern GPA_DisableCounterStrPtrType       GPA_DisableCounterStr      ;
extern GPA_EnableAllCountersPtrType       GPA_EnableAllCounters      ;
extern GPA_DisableAllCountersPtrType      GPA_DisableAllCounters     ;
extern GPA_GetCounterIndexPtrType         GPA_GetCounterIndex        ;
extern GPA_GetPassCountPtrType            GPA_GetPassCount           ;
extern GPA_BeginSessionPtrType            GPA_BeginSession           ;
extern GPA_EndSessionPtrType              GPA_EndSession             ;
extern GPA_BeginPassPtrType               GPA_BeginPass              ;
extern GPA_EndPassPtrType                 GPA_EndPass                ;
extern GPA_BeginSamplePtrType             GPA_BeginSample            ;
extern GPA_EndSamplePtrType               GPA_EndSample              ;
extern GPA_GetSampleCountPtrType          GPA_GetSampleCount         ;
extern GPA_IsSampleReadyPtrType           GPA_IsSampleReady          ;
extern GPA_IsSessionReadyPtrType          GPA_IsSessionReady         ;
extern GPA_GetSampleUInt64PtrType         GPA_GetSampleUInt64        ;
extern GPA_GetSampleUInt32PtrType         GPA_GetSampleUInt32        ;
extern GPA_GetSampleFloat32PtrType        GPA_GetSampleFloat32       ;
extern GPA_GetSampleFloat64PtrType        GPA_GetSampleFloat64       ;


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


extern PFN_ADL2_Main_ControlX2_Create                     ADL2_Main_ControlX2_Create                    ;
extern PFN_ADL_Main_ControlX2_Create                      ADL_Main_ControlX2_Create                     ;
extern PFN_ADL2_Main_Control_Create                       ADL2_Main_Control_Create                      ;
extern PFN_ADL_Main_Control_Create                        ADL_Main_Control_Create                       ;
extern PFN_ADL2_Main_Control_Refresh                      ADL2_Main_Control_Refresh                     ;
extern PFN_ADL_Main_Control_Refresh                       ADL_Main_Control_Refresh                      ;
extern PFN_ADL2_Main_Control_Destroy                      ADL2_Main_Control_Destroy                     ;
extern PFN_ADL_Main_Control_Destroy                       ADL_Main_Control_Destroy                      ;
extern PFN_ADL2_Graphics_Platform_Get                     ADL2_Graphics_Platform_Get                    ;
extern PFN_ADL_Graphics_Platform_Get                      ADL_Graphics_Platform_Get                     ;

extern PFN_ADL2_Adapter_Active_Get                        ADL2_Adapter_Active_Get                       ;
extern PFN_ADL_Adapter_Active_Get                         ADL_Adapter_Active_Get                        ;
extern PFN_ADL2_Adapter_NumberOfAdapters_Get              ADL2_Adapter_NumberOfAdapters_Get             ;
extern PFN_ADL_Adapter_NumberOfAdapters_Get               ADL_Adapter_NumberOfAdapters_Get              ;
extern PFN_ADL2_Flush_Driver_Data                         ADL2_Flush_Driver_Data                        ;
extern PFN_ADL_Flush_Driver_Data                          ADL_Flush_Driver_Data                         ;
extern PFN_ADL2_Adapter_AdapterInfo_Get                   ADL2_Adapter_AdapterInfo_Get                  ;
extern PFN_ADL_Adapter_AdapterInfo_Get                    ADL_Adapter_AdapterInfo_Get                   ;
extern PFN_ADL2_Adapter_AdapterInfoX2_Get                 ADL2_Adapter_AdapterInfoX2_Get                ;
extern PFN_ADL_Adapter_AdapterInfoX2_Get                  ADL_Adapter_AdapterInfoX2_Get                 ;
extern PFN_ADL2_Adapter_ASICFamilyType_Get                ADL2_Adapter_ASICFamilyType_Get               ;
extern PFN_ADL_Adapter_ASICFamilyType_Get                 ADL_Adapter_ASICFamilyType_Get                ;
extern PFN_ADL2_Adapter_Speed_Caps                        ADL2_Adapter_Speed_Caps                       ;
extern PFN_ADL_Adapter_Speed_Caps                         ADL_Adapter_Speed_Caps                        ;
extern PFN_ADL2_Adapter_Speed_Get                         ADL2_Adapter_Speed_Get                        ;
extern PFN_ADL_Adapter_Speed_Get                          ADL_Adapter_Speed_Get                         ;
extern PFN_ADL2_Adapter_Speed_Set                         ADL2_Adapter_Speed_Set                        ;
extern PFN_ADL_Adapter_Speed_Set                          ADL_Adapter_Speed_Set                         ;
extern PFN_ADL2_Adapter_Accessibility_Get                 ADL2_Adapter_Accessibility_Get                ;
extern PFN_ADL_Adapter_Accessibility_Get                  ADL_Adapter_Accessibility_Get                 ;
extern PFN_ADL2_Adapter_VideoBiosInfo_Get                 ADL2_Adapter_VideoBiosInfo_Get                ;
extern PFN_ADL_Adapter_VideoBiosInfo_Get                  ADL_Adapter_VideoBiosInfo_Get                 ;
extern PFN_ADL2_Adapter_ID_Get                            ADL2_Adapter_ID_Get                           ;
extern PFN_ADL_Adapter_ID_Get                             ADL_Adapter_ID_Get                            ;

extern PFN_ADL2_Overdrive5_CurrentActivity_Get            ADL2_Overdrive5_CurrentActivity_Get           ;
extern PFN_ADL_Overdrive5_CurrentActivity_Get             ADL_Overdrive5_CurrentActivity_Get            ;
extern PFN_ADL2_Overdrive5_ThermalDevices_Enum            ADL2_Overdrive5_ThermalDevices_Enum           ;
extern PFN_ADL_Overdrive5_ThermalDevices_Enum             ADL_Overdrive5_ThermalDevices_Enum            ;
extern PFN_ADL2_Overdrive5_Temperature_Get                ADL2_Overdrive5_Temperature_Get               ;
extern PFN_ADL_Overdrive5_Temperature_Get                 ADL_Overdrive5_Temperature_Get                ;
extern PFN_ADL2_Overdrive5_FanSpeedInfo_Get               ADL2_Overdrive5_FanSpeedInfo_Get              ;
extern PFN_ADL_Overdrive5_FanSpeedInfo_Get                ADL_Overdrive5_FanSpeedInfo_Get               ;
extern PFN_ADL2_Overdrive5_FanSpeed_Get                   ADL2_Overdrive5_FanSpeed_Get                  ;
extern PFN_ADL_Overdrive5_FanSpeed_Get                    ADL_Overdrive5_FanSpeed_Get                   ;
extern PFN_ADL2_Overdrive5_FanSpeed_Set                   ADL2_Overdrive5_FanSpeed_Set                  ;
extern PFN_ADL_Overdrive5_FanSpeed_Set                    ADL_Overdrive5_FanSpeed_Set                   ;
extern PFN_ADL2_Overdrive5_FanSpeedToDefault_Set          ADL2_Overdrive5_FanSpeedToDefault_Set         ;
extern PFN_ADL_Overdrive5_FanSpeedToDefault_Set           ADL_Overdrive5_FanSpeedToDefault_Set          ;
extern PFN_ADL2_Overdrive5_ODParameters_Get               ADL2_Overdrive5_ODParameters_Get              ;
extern PFN_ADL_Overdrive5_ODParameters_Get                ADL_Overdrive5_ODParameters_Get               ;
extern PFN_ADL2_Overdrive5_ODPerformanceLevels_Get        ADL2_Overdrive5_ODPerformanceLevels_Get       ;
extern PFN_ADL_Overdrive5_ODPerformanceLevels_Get         ADL_Overdrive5_ODPerformanceLevels_Get        ;
extern PFN_ADL2_Overdrive5_ODPerformanceLevels_Set        ADL2_Overdrive5_ODPerformanceLevels_Set       ;
extern PFN_ADL_Overdrive5_ODPerformanceLevels_Set         ADL_Overdrive5_ODPerformanceLevels_Set        ;
extern PFN_ADL2_Overdrive5_PowerControl_Caps              ADL2_Overdrive5_PowerControl_Caps             ;
extern PFN_ADL_Overdrive5_PowerControl_Caps               ADL_Overdrive5_PowerControl_Caps              ;
extern PFN_ADL2_Overdrive5_PowerControlInfo_Get           ADL2_Overdrive5_PowerControlInfo_Get          ;
extern PFN_ADL_Overdrive5_PowerControlInfo_Get            ADL_Overdrive5_PowerControlInfo_Get           ;
extern PFN_ADL2_Overdrive5_PowerControl_Get               ADL2_Overdrive5_PowerControl_Get              ;
extern PFN_ADL_Overdrive5_PowerControl_Get                ADL_Overdrive5_PowerControl_Get               ;
extern PFN_ADL2_Overdrive5_PowerControl_Set               ADL2_Overdrive5_PowerControl_Set              ;
extern PFN_ADL_Overdrive5_PowerControl_Set                ADL_Overdrive5_PowerControl_Set               ;
extern PFN_ADL2_Overdrive_Caps                            ADL2_Overdrive_Caps                           ;
extern PFN_ADL_Overdrive_Caps                             ADL_Overdrive_Caps                            ;

extern PFN_ADL2_Overdrive6_Capabilities_Get               ADL2_Overdrive6_Capabilities_Get              ;
extern PFN_ADL_Overdrive6_Capabilities_Get                ADL_Overdrive6_Capabilities_Get               ;
extern PFN_ADL2_Overdrive6_StateInfo_Get                  ADL2_Overdrive6_StateInfo_Get                 ;
extern PFN_ADL_Overdrive6_StateInfo_Get                   ADL_Overdrive6_StateInfo_Get                  ;
extern PFN_ADL2_Overdrive6_State_Set                      ADL2_Overdrive6_State_Set                     ;
extern PFN_ADL_Overdrive6_State_Set                       ADL_Overdrive6_State_Set                      ;
extern PFN_ADL2_Overdrive6_State_Reset                    ADL2_Overdrive6_State_Reset                   ;
extern PFN_ADL_Overdrive6_State_Reset                     ADL_Overdrive6_State_Reset                    ;
extern PFN_ADL2_Overdrive6_CurrentStatus_Get              ADL2_Overdrive6_CurrentStatus_Get             ;
extern PFN_ADL_Overdrive6_CurrentStatus_Get               ADL_Overdrive6_CurrentStatus_Get              ;
extern PFN_ADL2_Overdrive6_ThermalController_Caps         ADL2_Overdrive6_ThermalController_Caps        ;
extern PFN_ADL_Overdrive6_ThermalController_Caps          ADL_Overdrive6_ThermalController_Caps         ;
extern PFN_ADL2_Overdrive6_Temperature_Get                ADL2_Overdrive6_Temperature_Get               ;
extern PFN_ADL_Overdrive6_Temperature_Get                 ADL_Overdrive6_Temperature_Get                ;
extern PFN_ADL2_Overdrive6_FanSpeed_Get                   ADL2_Overdrive6_FanSpeed_Get                  ;
extern PFN_ADL_Overdrive6_FanSpeed_Get                    ADL_Overdrive6_FanSpeed_Get                   ;
extern PFN_ADL2_Overdrive6_FanSpeed_Set                   ADL2_Overdrive6_FanSpeed_Set                  ;
extern PFN_ADL_Overdrive6_FanSpeed_Set                    ADL_Overdrive6_FanSpeed_Set                   ;
extern PFN_ADL2_Overdrive6_FanSpeed_Reset                 ADL2_Overdrive6_FanSpeed_Reset                ;
extern PFN_ADL_Overdrive6_FanSpeed_Reset                  ADL_Overdrive6_FanSpeed_Reset                 ;
extern PFN_ADL2_Overdrive6_PowerControl_Caps              ADL2_Overdrive6_PowerControl_Caps             ;
extern PFN_ADL_Overdrive6_PowerControl_Caps               ADL_Overdrive6_PowerControl_Caps              ;
extern PFN_ADL2_Overdrive6_PowerControlInfo_Get           ADL2_Overdrive6_PowerControlInfo_Get          ;
extern PFN_ADL_Overdrive6_PowerControlInfo_Get            ADL_Overdrive6_PowerControlInfo_Get           ;
extern PFN_ADL2_Overdrive6_PowerControl_Get               ADL2_Overdrive6_PowerControl_Get              ;
extern PFN_ADL_Overdrive6_PowerControl_Get                ADL_Overdrive6_PowerControl_Get	              ;
extern PFN_ADL2_Overdrive6_PowerControl_Set               ADL2_Overdrive6_PowerControl_Set              ;
extern PFN_ADL_Overdrive6_PowerControl_Set                ADL_Overdrive6_PowerControl_Set               ;
extern PFN_ADL2_Overdrive6_VoltageControlInfo_Get         ADL2_Overdrive6_VoltageControlInfo_Get        ;
extern PFN_ADL_Overdrive6_VoltageControlInfo_Get          ADL_Overdrive6_VoltageControlInfo_Get         ;
extern PFN_ADL2_Overdrive6_VoltageControl_Get             ADL2_Overdrive6_VoltageControl_Get            ;
extern PFN_ADL_Overdrive6_VoltageControl_Get              ADL_Overdrive6_VoltageControl_Get             ;
extern PFN_ADL2_Overdrive6_VoltageControl_Set             ADL2_Overdrive6_VoltageControl_Set            ;
extern PFN_ADL_Overdrive6_VoltageControl_Set              ADL_Overdrive6_VoltageControl_Set             ;
extern PFN_ADL2_Overdrive6_CapabilitiesEx_Get             ADL2_Overdrive6_CapabilitiesEx_Get            ;
extern PFN_ADL_Overdrive6_CapabilitiesEx_Get              ADL_Overdrive6_CapabilitiesEx_Get             ;
extern PFN_ADL2_Overdrive6_StateEx_Get                    ADL2_Overdrive6_StateEx_Get                   ;
extern PFN_ADL_Overdrive6_StateEx_Get                     ADL_Overdrive6_StateEx_Get                    ;
extern PFN_ADL2_Overdrive6_StateEx_Set                    ADL2_Overdrive6_StateEx_Set                   ;
extern PFN_ADL_Overdrive6_StateEx_Set                     ADL_Overdrive6_StateEx_Set                    ;
extern PFN_ADL_Overdrive6_ThermalLimitUnlock_Set          ADL_Overdrive6_ThermalLimitUnlock_Set         ;
extern PFN_ADL2_Overdrive6_ThermalLimitUnlock_Set         ADL2_Overdrive6_ThermalLimitUnlock_Set        ;
extern PFN_ADL_Overdrive6_ThermalLimitUnlock_Get          ADL_Overdrive6_ThermalLimitUnlock_Get         ;
extern PFN_ADL2_Overdrive6_ThermalLimitUnlock_Get         ADL2_Overdrive6_ThermalLimitUnlock_Get        ;
extern PFN_ADL_Overdrive6_AdvancedFan_Caps                ADL_Overdrive6_AdvancedFan_Caps               ;
extern PFN_ADL2_Overdrive6_AdvancedFan_Caps               ADL2_Overdrive6_AdvancedFan_Caps              ;
extern PFN_ADL_Overdrive6_TargetTemperatureRangeInfo_Get  ADL_Overdrive6_TargetTemperatureRangeInfo_Get ;
extern PFN_ADL2_Overdrive6_TargetTemperatureRangeInfo_Get ADL2_Overdrive6_TargetTemperatureRangeInfo_Get;
extern PFN_ADL_Overdrive6_TargetTemperatureData_Get       ADL_Overdrive6_TargetTemperatureData_Get      ;
extern PFN_ADL2_Overdrive6_TargetTemperatureData_Get      ADL2_Overdrive6_TargetTemperatureData_Get     ;
extern PFN_ADL_Overdrive6_TargetTemperatureData_Set       ADL_Overdrive6_TargetTemperatureData_Set      ;
extern PFN_ADL2_Overdrive6_TargetTemperatureData_Set      ADL2_Overdrive6_TargetTemperatureData_Set     ;
extern PFN_ADL_Overdrive6_FanPWMLimitRangeInfo_Get        ADL_Overdrive6_FanPWMLimitRangeInfo_Get       ;
extern PFN_ADL2_Overdrive6_FanPWMLimitRangeInfo_Get       ADL2_Overdrive6_FanPWMLimitRangeInfo_Get      ;
extern PFN_ADL_Overdrive6_FanPWMLimitData_Get             ADL_Overdrive6_FanPWMLimitData_Get            ;
extern PFN_ADL2_Overdrive6_FanPWMLimitData_Get            ADL2_Overdrive6_FanPWMLimitData_Get           ;
extern PFN_ADL_Overdrive6_FanPWMLimitData_Set             ADL_Overdrive6_FanPWMLimitData_Set            ;
extern PFN_ADL2_Overdrive6_FanPWMLimitData_Set            ADL2_Overdrive6_FanPWMLimitData_Set           ;
#endif
