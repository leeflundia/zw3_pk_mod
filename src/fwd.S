	.text
	.globl _CreateDirect3D11DeviceFromDXGIDevice
_CreateDirect3D11DeviceFromDXGIDevice:
	jmp *_g_tab+0
	.globl _CreateDirect3D11SurfaceFromDXGISurface
_CreateDirect3D11SurfaceFromDXGISurface:
	jmp *_g_tab+4
	.globl _D3D11CoreCreateDevice
_D3D11CoreCreateDevice:
	jmp *_g_tab+8
	.globl _D3D11CoreCreateLayeredDevice
_D3D11CoreCreateLayeredDevice:
	jmp *_g_tab+12
	.globl _D3D11CoreGetLayeredDeviceSize
_D3D11CoreGetLayeredDeviceSize:
	jmp *_g_tab+16
	.globl _D3D11CoreRegisterLayers
_D3D11CoreRegisterLayers:
	jmp *_g_tab+20
	.globl _D3D11CreateDevice
_D3D11CreateDevice:
	jmp *_g_tab+24
	.globl _D3D11CreateDeviceAndSwapChain
_D3D11CreateDeviceAndSwapChain:
	jmp *_g_tab+28
	.globl _D3D11CreateDeviceForD3D12
_D3D11CreateDeviceForD3D12:
	jmp *_g_tab+32
	.globl _D3D11On12CreateDevice
_D3D11On12CreateDevice:
	jmp *_g_tab+36
	.globl _D3DKMTCloseAdapter
_D3DKMTCloseAdapter:
	jmp *_g_tab+40
	.globl _D3DKMTCreateAllocation
_D3DKMTCreateAllocation:
	jmp *_g_tab+44
	.globl _D3DKMTCreateContext
_D3DKMTCreateContext:
	jmp *_g_tab+48
	.globl _D3DKMTCreateDevice
_D3DKMTCreateDevice:
	jmp *_g_tab+52
	.globl _D3DKMTCreateSynchronizationObject
_D3DKMTCreateSynchronizationObject:
	jmp *_g_tab+56
	.globl _D3DKMTDestroyAllocation
_D3DKMTDestroyAllocation:
	jmp *_g_tab+60
	.globl _D3DKMTDestroyContext
_D3DKMTDestroyContext:
	jmp *_g_tab+64
	.globl _D3DKMTDestroyDevice
_D3DKMTDestroyDevice:
	jmp *_g_tab+68
	.globl _D3DKMTDestroySynchronizationObject
_D3DKMTDestroySynchronizationObject:
	jmp *_g_tab+72
	.globl _D3DKMTEscape
_D3DKMTEscape:
	jmp *_g_tab+76
	.globl _D3DKMTGetContextSchedulingPriority
_D3DKMTGetContextSchedulingPriority:
	jmp *_g_tab+80
	.globl _D3DKMTGetDeviceState
_D3DKMTGetDeviceState:
	jmp *_g_tab+84
	.globl _D3DKMTGetDisplayModeList
_D3DKMTGetDisplayModeList:
	jmp *_g_tab+88
	.globl _D3DKMTGetMultisampleMethodList
_D3DKMTGetMultisampleMethodList:
	jmp *_g_tab+92
	.globl _D3DKMTGetRuntimeData
_D3DKMTGetRuntimeData:
	jmp *_g_tab+96
	.globl _D3DKMTGetSharedPrimaryHandle
_D3DKMTGetSharedPrimaryHandle:
	jmp *_g_tab+100
	.globl _D3DKMTLock
_D3DKMTLock:
	jmp *_g_tab+104
	.globl _D3DKMTOpenAdapterFromHdc
_D3DKMTOpenAdapterFromHdc:
	jmp *_g_tab+108
	.globl _D3DKMTOpenResource
_D3DKMTOpenResource:
	jmp *_g_tab+112
	.globl _D3DKMTPresent
_D3DKMTPresent:
	jmp *_g_tab+116
	.globl _D3DKMTQueryAdapterInfo
_D3DKMTQueryAdapterInfo:
	jmp *_g_tab+120
	.globl _D3DKMTQueryAllocationResidency
_D3DKMTQueryAllocationResidency:
	jmp *_g_tab+124
	.globl _D3DKMTQueryResourceInfo
_D3DKMTQueryResourceInfo:
	jmp *_g_tab+128
	.globl _D3DKMTRender
_D3DKMTRender:
	jmp *_g_tab+132
	.globl _D3DKMTSetAllocationPriority
_D3DKMTSetAllocationPriority:
	jmp *_g_tab+136
	.globl _D3DKMTSetContextSchedulingPriority
_D3DKMTSetContextSchedulingPriority:
	jmp *_g_tab+140
	.globl _D3DKMTSetDisplayMode
_D3DKMTSetDisplayMode:
	jmp *_g_tab+144
	.globl _D3DKMTSetDisplayPrivateDriverFormat
_D3DKMTSetDisplayPrivateDriverFormat:
	jmp *_g_tab+148
	.globl _D3DKMTSetGammaRamp
_D3DKMTSetGammaRamp:
	jmp *_g_tab+152
	.globl _D3DKMTSetVidPnSourceOwner
_D3DKMTSetVidPnSourceOwner:
	jmp *_g_tab+156
	.globl _D3DKMTSignalSynchronizationObject
_D3DKMTSignalSynchronizationObject:
	jmp *_g_tab+160
	.globl _D3DKMTUnlock
_D3DKMTUnlock:
	jmp *_g_tab+164
	.globl _D3DKMTWaitForSynchronizationObject
_D3DKMTWaitForSynchronizationObject:
	jmp *_g_tab+168
	.globl _D3DKMTWaitForVerticalBlankEvent
_D3DKMTWaitForVerticalBlankEvent:
	jmp *_g_tab+172
	.globl _D3DPerformance_BeginEvent
_D3DPerformance_BeginEvent:
	jmp *_g_tab+176
	.globl _D3DPerformance_EndEvent
_D3DPerformance_EndEvent:
	jmp *_g_tab+180
	.globl _D3DPerformance_GetStatus
_D3DPerformance_GetStatus:
	jmp *_g_tab+184
	.globl _D3DPerformance_SetMarker
_D3DPerformance_SetMarker:
	jmp *_g_tab+188
	.globl _EnableFeatureLevelUpgrade
_EnableFeatureLevelUpgrade:
	jmp *_g_tab+192
	.globl _OpenAdapter10
_OpenAdapter10:
	jmp *_g_tab+196
	.globl _OpenAdapter10_2
_OpenAdapter10_2:
	jmp *_g_tab+200
